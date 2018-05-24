#include "tr111.h"
#include <sys/un.h>
#include "openssl/sha.h"
//#include <stddef.h>

#ifdef ANDROID
#include <jni.h>
#include <android/log.h>
#define LOGE(format, ...)  __android_log_print(ANDROID_LOG_ERROR, "(>_<)", format, ##__VA_ARGS__)
#define LOGI(format, ...)  __android_log_print(ANDROID_LOG_INFO,  "(=_=)", format, ##__VA_ARGS__)
#else
#define LOGE(format, ...)  printf("(>_<) " format "\n", ##__VA_ARGS__)
#define LOGI(format, ...)  printf("(^_^) " format "\n", ##__VA_ARGS__)
#endif

static struct sockaddr_in server;
static struct sockaddr_in binding;

#define STUN_MESSAGE_LEN 128
static unsigned char msg[STUN_MESSAGE_LEN];
static unsigned char msg_changed[STUN_MESSAGE_LEN];
static unsigned char msg_cred[STUN_MESSAGE_LEN];
static unsigned char msg_cred_changed[STUN_MESSAGE_LEN];
static unsigned char msg_resp_addr[STUN_MESSAGE_LEN];
static unsigned char msg_cred_resp_addr[STUN_MESSAGE_LEN];
//static unsigned char *sent_msg;


#define UNIT_TYPE(m)  ((m)[0] * 256 + (m)[1])
#define UNIT_LEN(m)   ((m)[2] * 256 + (m)[3])
#define MSG_LEN(m)    (UNIT_LEN(m) + 20)
#define FIRST_ATTR(m) ((m) + 20)
#define NEXT_ATTR(m)  ((m) + 4 + UNIT_LEN(m))

static char stun_username[257] = "\xff\x00"; /* Non-ascii to make it different from the real value */
static char stun_password[257] = "\xff\x00";



void trigger_6_connection()
{
	int sockfd;
	int len;
	struct sockaddr_in address;
	int result;
	char ch ='C';
	sockfd = socket(AF_INET,SOCK_STREAM,0);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = 9734;
	len = sizeof(address);

	result=connect(sockfd,(struct sockaddr *)&address,len);

	if(result == -1){
		perror("oops:client1");
		LOGE("Canot connect to local trigger_6_connection");
		//exit(1);
	}

	write(sockfd,&ch,1);
	read(sockfd,&ch,1);
	LOGE("Now trigger_6_connection recive %c\n",ch);
	close(sockfd);
}


static void init_stun_request(unsigned char *m)//the header length is 20byte and the message is variable its length was assigned by m[2]&m[3]
{

    char *t_id = "0101010101010101";//事物ID
	memset(m,0,STUN_MESSAGE_LEN);
	m[0] = 0x00;
	m[1] = 0x01;//前两个字节代表消息类型 0x0001：捆绑请求  0x0101：捆绑响应    0x0111：捆绑错误响应     0x0002：共享私密请求     0x0102：共享私密响应    0x0112：共享私密错误响应
                //the m[2]&m[3]is the length of message without the header
	memcpy(m + 4, t_id, MIN(16, strlen(t_id)));

}

static void add_attribute(unsigned char *m, uint16_t type, char *data, uint16_t len)
{
    uint16_t *d;
    uint16_t l;
    d = (uint16_t *)(m + MSG_LEN(m));
    l = len % 4 == 0 ? len : len + 4 - (len % 4);
    *d++ = htons(type);
    *d++ = htons(l);

    if (len > 0)
        memcpy(d, data, len);

    d = (uint16_t *)(m + 2);

    *d = htons(MSG_LEN(m) - 16 + l);
}

static void init_message(unsigned char * m)
{

    /* Connection-Request-Binding tr111 attribute type 0xc001*/
    char *crb = "\x64\x73\x6C\x66\x6F\x72\x75\x6D\x2E\x6F\x72\x67\x2F\x54\x52\x2D\x31\x31\x31\x20";
    init_stun_request(m);

     /* Add the Connection-Request-Binding Attribute */
     if (m != msg_resp_addr && m != msg_cred_resp_addr) {
        add_attribute(m, 0xC001, crb, 20);
    } else { /* Add the RESPONSE-ADDRESS Attribute */

    	struct {
    	            uint8_t not_used;
    	            uint8_t family;
    	            uint16_t port;
    	            uint32_t addr;
    	        } attr;

    	attr.not_used = 0;
    	attr.family = 0x01;
    	attr.port = binding.sin_port;
    	attr.addr = binding.sin_addr.s_addr;
    	add_attribute(m, 0x0002, (char *) &attr, sizeof(attr));
    }

     /* Add the Binding-Changed Attribute */
     if (m == msg_changed || m == msg_cred_changed)
         add_attribute(m, 0xC002, NULL, 0);
     if (m == msg_cred || m == msg_cred_changed || m == msg_cred_resp_addr) { /* Username is not empty */
             uint16_t len;
             uint16_t *l;
             unsigned char md[SHA_DIGEST_LENGTH] = "";
             unsigned int md_len;

             add_attribute(m, 0x0006, stun_username, strlen(stun_username));

             md_len = sizeof(md);
             len = MSG_LEN(m);

             /* This block is incorrect, but works well with stund of wks' IOP */
             l = (uint16_t *)(m + 2);
             *l = htons(len + 4);
             HMAC(EVP_sha1(), stun_password, strlen(stun_password), (const unsigned char *) m, len, md, &md_len);
             len = MSG_LEN(m);
             *l = htons(len - 44);
             add_attribute(m, 0x0008, (char *) md, 20);
     }
     /*0x0006 add username attribute*/
     //add_attribute(m,0x0006,"0047040000021010FF00900a1a000280",32);



}

static void send_request(int stunserver_port,char * stunserver_ip,unsigned char *m)
{
	int sockfd;
	int len;
	struct sockaddr_in address;
	char buffer[256];


	bzero(&address,sizeof(address));
	address.sin_family = AF_INET;//sin_family表示协议簇，一般用AF_INET表示TCP/IP协议。
	address.sin_addr.s_addr=inet_addr(stunserver_ip);//sin_addr是一个联合体，用联合体就可以使用多种方式表示IP地址
	address.sin_port=htons(stunserver_port);//give the port

	if((sockfd=socket(AF_INET,SOCK_DGRAM,0))<0)//set the socket domain tcp/ip ,type udp, one type only udp so protocol is 0
	{
	    LOGE("Bind repuest creat socket failed \n");
	}
	if((sendto(sockfd,m,MSG_LEN(m),0,(struct sockaddr *)&address,sizeof(address)))<0)
	{
		 LOGE("Send to socket failed \n");
	}

	close(sockfd);
}

static void udp_listen(int stunport,char * ip,int localport,unsigned char *m)//set up the udp connection and send the message *m
{

    int socket_descriptor;
    struct sockaddr_in stunaddr,address;//the choice of socket
    bzero(&address,sizeof(address));
    address.sin_family = AF_INET;//sin_family表示协议簇，一般用AF_INET表示TCP/IP协议。
    address.sin_addr.s_addr=INADDR_ANY;//sin_addr是一个联合体，用联合体就可以使用多种方式表示IP地址
    address.sin_port=htons(localport);//give the port

    stunaddr.sin_family = AF_INET;
    stunaddr.sin_addr.s_addr = inet_addr(ip);
    stunaddr.sin_port = htons(stunport);


    if((socket_descriptor=socket(AF_INET,SOCK_DGRAM,0))<0)//set the socket domain tcp/ip ,type udp, one type only udp so protocol is 0
    {
    	LOGE("Creat socket failed \n");
    }

    if(bind(socket_descriptor,(const struct sockaddr *)&address,sizeof(address))==-1){
    	LOGE("Bind failed \n");
    }

    struct timeval tv;
    fd_set readfds;
    int i = 0;
    unsigned int n = 0;
    char buf[1024];
    struct sockaddr addr;
    socklen_t len;
    stunstau stunstau1;

    stunstau1=STUN_SEND;

    while(1){
    	switch(stunstau1){

    		case STUN_SEND:
    			stunstau1=STUN_RECV;
    			 LOGI("Status is STUN_SEND \n");
    			if((sendto(socket_descriptor,m,MSG_LEN(m),0,(struct sockaddr *)&stunaddr,sizeof(address)))<0)
    			{
    				 LOGE("Send to socket failed \n");
    			}
    			break;
    		case STUN_RECV:
    			LOGI("Status is STUN_RECV \n");
    			FD_ZERO(&readfds);
    			FD_SET(socket_descriptor,&readfds);
       			tv.tv_sec = 60;
       			tv.tv_usec = 0;
       			select(socket_descriptor+1,&readfds,NULL,NULL,&tv);
    			if(FD_ISSET(socket_descriptor,&readfds)){
    			    if((n=recvfrom(socket_descriptor,buf,1024,0,&addr,&len))>=0){
    			    		LOGE("in time,left time %d s,%d usec\n",(int)tv.tv_sec,(int)tv.tv_usec);
    			    		if(strncmp(buf,"123456",5)==0){

    			    		}else trigger_6_connection();
    			    	}
    			    for(i=0;i<100;i++)
    			    			LOGE("buf is %x\n",buf[i]);
    			 }
    			 else{
    			    	LOGE("time out,left time %d s ,%d usec\n",(int)tv.tv_sec,(int)tv.tv_usec);
    			    	stunstau1=STUN_SEND;
    			 }
    			break;

    		default:
    			break;
    	}
    }



//    while(1){


//
//    	while(1){
//
//
//    	}
//    }



}



void test()
{
	//trigger_6_connection();
	//system("/system/bin/sh -c \"am broadcast -a android.intent.action.AT_AIRPLANE_MODE --ez state true\"");
	//system("/system/bin/sh -c \"reboot\"");
    init_message(msg);
    LOGI("To start udp_listen \n");
    udp_listen(3478,"192.168.1.2",56789,msg);
    //send_request(3478,"192.168.1.2",msg);

}






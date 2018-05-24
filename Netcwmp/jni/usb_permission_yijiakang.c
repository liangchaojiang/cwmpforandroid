#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <linux/netlink.h>


static int monitoring_usb_device( const char *arg)
{
	int iResult = 0;
	char caKernelMsgBuff[1024];
	const unsigned int uiRecvBuffSize = sizeof( caKernelMsgBuff );
	struct sockaddr_nl snl;
	int sfd = -1;
	char command[512];

	int add_flag=0;
	int rmv_flag=1;

	snl.nl_family = AF_NETLINK;
	snl.nl_pad = 0;
	snl.nl_pid = getpid();
	snl.nl_groups = 1;

	sfd = socket( PF_NETLINK,SOCK_DGRAM,NETLINK_KOBJECT_UEVENT );
	if ( -1 == sfd )
	{
		iResult = -1;
		return iResult;
	}

	setsockopt( sfd,SOL_SOCKET,SO_RCVBUF, &uiRecvBuffSize,sizeof( uiRecvBuffSize ) );

	iResult = bind( sfd,(struct sockaddr*)&snl,sizeof( snl ) );
	if ( -1 == iResult )
	{
		return -2;
	}

	memset(command, 0, sizeof(command));
	sprintf(command, "sh %s", arg);
	system(command);
	while( 1 )
	{
		//printf( "Kernel Message:\n");
		recv( sfd,&caKernelMsgBuff,sizeof( caKernelMsgBuff ),0 );

		if( 0 == memcmp( caKernelMsgBuff,"add@",4 )&& (NULL != strstr( caKernelMsgBuff,"usb" )))
		{
			if(!add_flag && rmv_flag)
			{	
				add_flag=1;
				rmv_flag=0;
				printf( "Add USB Device\n" );
				system(command);
			}
		}

		else if( 0 == memcmp( caKernelMsgBuff, "remove@",7 )&& (NULL != strstr( caKernelMsgBuff,"usb" )))
		{
			if(add_flag && (!rmv_flag))
			{
				add_flag=0;
				rmv_flag=1;
				printf( "Remove USB Device\n" );
			}
		}
		else
			printf("There have no usb divece\n");

		sleep(1);
	}


	if ( -1 != sfd )
	{
		close( sfd );
		sfd = -1;
	}
	return iResult;
}

int main(int argc, char* argv[])
{
	const char *arg=argv[1];
	if(argc<=1)
	{
		printf("Missing Parameters !\n");
		return -1;
	}
	monitoring_usb_device(arg); 
	printf( "monitoring_usb_device return \n");

	return 0;
} 

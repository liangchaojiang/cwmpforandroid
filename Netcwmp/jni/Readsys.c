#include <stdio.h>
#include <sys/system_properties.h>

void main()
{
	char osVersion[PROP_VALUE_MAX];
	int osVersionLength = __system_property_get("ro.product.manufacturer",osVersion);
	printf("the hardware id is %s\n",osVersion);


}


int echo1(int a,int b)
{
	int c = a+b;
	return c;
}
int echo2(int a,char b)
{
	return 2;
}
int echo3(char a,char b)
{
	return 3;
}


typedef struct func_list{
	const char * name;
	void * func;
}func_list1;


func_list1 new_func_list[]={
		{"echo1",echo1},
		{"echo2",echo2},
		{"echo3",echo3}
};




#include "DeviceInfo.c"
#include "ManagementServer.c"
#include "WANDevice.c"
#include "WANConnectionDevice.c"
#include "WANIPConnection.c"



int cpe_get_igd_device_summary(cwmp_t * cwmp, const char * name, char ** value, pool_t * pool)
{
	FUNCTION_TRACE();
	*value = cwmp_conf_pool_get(pool, "cwmp:cpe_summary");
	cwmp_log_debug("+++cpe_get_igd_device_summary++++ device summary is :  %s\n", *value);
    //pool_t * p = (pool_t *)pool;
    return	FAULT_CODE_OK;
}

char* cpe_get_igd_lan_device_number_of_entries(void * arg, void * pool)
{
    //pool_t * p = (pool_t *)pool;
    return NULL;
}






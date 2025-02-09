// this code is for populating configuration
// 1] loading defaults
// 2] override values with whatever is set in the config MODULE_CONFIG_FILE_NAME
// 3] override values with whatever is set in the config MODULE_LOCAL_CONFIG_FILE_NAME if present

#include "load_config.h"

char *CONFIG_INFLUXDB_ADDRESS = NULL;
char *CONFIG_INFLUXDB_NAME = NULL;
char *CONFIG_INFLUXDB_PORT = NULL;
char *CONFIG_INFLUXDB_PROTOCOL = NULL;
char *CONFIG_INFLUXDB_USER = NULL;
char *CONFIG_INFLUXDB_PWD = NULL;
int *CONFIG_INFLUXDB_SSL_INSECURE = NULL;
int *CONFIG_FORCE_MODULE_DEBUG = NULL;
int *CONFIG_ZABBIX_MAJOR_VERSION = NULL;
int *CONFIG_DATABASE_ENGINE = NULL;
char *PARSE_DATABASE_ENGINE = NULL;
int *CONFIG_REQUEST_TIMEOUT = NULL;

/*********************************************************************
 * zbx_module_load_config                                            *
 *********************************************************************/
void     zbx_module_load_config(void)
{
	char *MODULE_CONFIG_FILE = NULL;
	char *MODULE_LOCAL_CONFIG_FILE = NULL;
	// construct paths
	MODULE_CONFIG_FILE = zbx_dsprintf(MODULE_CONFIG_FILE, "%s/%s", CONFIG_LOAD_MODULE_PATH, MODULE_CONFIG_FILE_NAME);
	MODULE_LOCAL_CONFIG_FILE = zbx_dsprintf(MODULE_LOCAL_CONFIG_FILE, "%s/%s", CONFIG_LOAD_MODULE_PATH, MODULE_LOCAL_CONFIG_FILE_NAME);

	zbx_cfg_line_t  module_cfg[] =
	{
		/* PARAMETER,			VAR,				TYPE,
				MANDATORY,		MIN,		MAX */
		{"InfluxDBAddress",		&CONFIG_INFLUXDB_ADDRESS,	ZBX_CFG_TYPE_STRING,
				ZBX_CONF_PARM_OPT,   0,    0},
		{"InfluxDBName",		&CONFIG_INFLUXDB_NAME,	ZBX_CFG_TYPE_STRING,
				ZBX_CONF_PARM_MAND,  0,    0},
		{"InfluxDBUser",	&CONFIG_INFLUXDB_USER,	ZBX_CFG_TYPE_STRING,
				ZBX_CONF_PARM_OPT,   0,		0},
		{"InfluxDBPassword",	&CONFIG_INFLUXDB_PWD,	ZBX_CFG_TYPE_STRING,
				ZBX_CONF_PARM_OPT,		0,		0},
		{"InfluxDBPortNumber",	&CONFIG_INFLUXDB_PORT,	ZBX_CFG_TYPE_STRING,
				ZBX_CONF_PARM_OPT,		0,		0},
		{"InfluxDBProtocol",	&CONFIG_INFLUXDB_PROTOCOL,	ZBX_CFG_TYPE_STRING,
				ZBX_CONF_PARM_OPT,		0,		0},
		{"InfluxDBSSLInsecure",	&CONFIG_INFLUXDB_SSL_INSECURE,	ZBX_CFG_TYPE_INT,
				ZBX_CONF_PARM_OPT,		0,		1},
		{"ForceModuleDebugLogging",	&CONFIG_FORCE_MODULE_DEBUG,	ZBX_CFG_TYPE_INT,
				ZBX_CONF_PARM_OPT,		0,		1},
		{"ZabbixMajorVersion",	&CONFIG_ZABBIX_MAJOR_VERSION,	ZBX_CFG_TYPE_INT,
				ZBX_CONF_PARM_OPT,		3,		4},
		{"DatabaseEngine",	&PARSE_DATABASE_ENGINE,	ZBX_CFG_TYPE_STRING,
				ZBX_CONF_PARM_OPT,		0,		0},
        {"RequestTimeout",	&CONFIG_REQUEST_TIMEOUT, ZBX_CFG_TYPE_INT,
				ZBX_CONF_PARM_OPT,		0,		0},        
		{NULL}
	};

  // set defaults
  
	CONFIG_INFLUXDB_ADDRESS = zbx_strdup(CONFIG_INFLUXDB_ADDRESS, "localhost");
	CONFIG_INFLUXDB_PORT = zbx_strdup(CONFIG_INFLUXDB_PORT, "8086");
	CONFIG_INFLUXDB_PROTOCOL = zbx_strdup(CONFIG_INFLUXDB_PROTOCOL, "http");
	CONFIG_FORCE_MODULE_DEBUG = (int*) 0;
	CONFIG_ZABBIX_MAJOR_VERSION = (int*) 4;
	PARSE_DATABASE_ENGINE = zbx_strdup(PARSE_DATABASE_ENGINE, "mysql");
    CONFIG_REQUEST_TIMEOUT = (int*) 10;


	// load main config file
	zbx_parse_cfg_file(MODULE_CONFIG_FILE, module_cfg, ZBX_CFG_FILE_REQUIRED, ZBX_CFG_STRICT, ZBX_CFG_EXIT_FAILURE, ZBX_CFG_ENVVAR_USE);


	// load local config file if present
	zbx_parse_cfg_file(MODULE_LOCAL_CONFIG_FILE, module_cfg, ZBX_CFG_FILE_OPTIONAL, ZBX_CFG_STRICT, ZBX_CFG_EXIT_FAILURE, ZBX_CFG_ENVVAR_USE);

	// parse database engine
	if (strcmp(PARSE_DATABASE_ENGINE, "mysql") == 0) {
	    CONFIG_DATABASE_ENGINE = (int*) DATABASE_ENGINE_MYSQL;
	}
	else if (strcmp(PARSE_DATABASE_ENGINE, "postgresql") == 0) {
	    CONFIG_DATABASE_ENGINE = (int*) DATABASE_ENGINE_POSTGRESQL;
	}

	// clean up path variables
	zbx_free(MODULE_CONFIG_FILE);
	zbx_free(MODULE_LOCAL_CONFIG_FILE);

}

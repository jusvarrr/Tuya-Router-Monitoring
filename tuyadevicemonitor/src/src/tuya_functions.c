#include <argp_parse.h>
#include <daemon.h>
#include <sys_param.h>
#include <params_json.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "mqtt_client_interface.h"
#include "tuyalink_core.h"
#include "tuya_log.h"
#include "cJSON.h"
#include "tuya_functions.h"

#define OPRT_RETRYING -3

const static char tuya_cacert1[] = {\
"-----BEGIN CERTIFICATE-----\n"\
"MIIDxTCCAq2gAwIBAgIBADANBgkqhkiG9w0BAQsFADCBgzELMAkGA1UEBhMCVVMx\n"\
"EDAOBgNVBAgTB0FyaXpvbmExEzARBgNVBAcTClNjb3R0c2RhbGUxGjAYBgNVBAoT\n"\
"EUdvRGFkZHkuY29tLCBJbmMuMTEwLwYDVQQDEyhHbyBEYWRkeSBSb290IENlcnRp\n"\
"ZmljYXRlIEF1dGhvcml0eSAtIEcyMB4XDTA5MDkwMTAwMDAwMFoXDTM3MTIzMTIz\n"\
"NTk1OVowgYMxCzAJBgNVBAYTAlVTMRAwDgYDVQQIEwdBcml6b25hMRMwEQYDVQQH\n"\
"EwpTY290dHNkYWxlMRowGAYDVQQKExFHb0RhZGR5LmNvbSwgSW5jLjExMC8GA1UE\n"\
"AxMoR28gRGFkZHkgUm9vdCBDZXJ0aWZpY2F0ZSBBdXRob3JpdHkgLSBHMjCCASIw\n"\
"DQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAL9xYgjx+lk09xvJGKP3gElY6SKD\n"\
"E6bFIEMBO4Tx5oVJnyfq9oQbTqC023CYxzIBsQU+B07u9PpPL1kwIuerGVZr4oAH\n"\
"/PMWdYA5UXvl+TW2dE6pjYIT5LY/qQOD+qK+ihVqf94Lw7YZFAXK6sOoBJQ7Rnwy\n"\
"DfMAZiLIjWltNowRGLfTshxgtDj6AozO091GB94KPutdfMh8+7ArU6SSYmlRJQVh\n"\
"GkSBjCypQ5Yj36w6gZoOKcUcqeldHraenjAKOc7xiID7S13MMuyFYkMlNAJWJwGR\n"\
"tDtwKj9useiciAF9n9T521NtYJ2/LOdYq7hfRvzOxBsDPAnrSTFcaUaz4EcCAwEA\n"\
"AaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAQYwHQYDVR0OBBYE\n"\
"FDqahQcQZyi27/a9BUFuIMGU2g/eMA0GCSqGSIb3DQEBCwUAA4IBAQCZ21151fmX\n"\
"WWcDYfF+OwYxdS2hII5PZYe096acvNjpL9DbWu7PdIxztDhC2gV7+AJ1uP2lsdeu\n"\
"9tfeE8tTEH6KRtGX+rcuKxGrkLAngPnon1rpN5+r5N9ss4UXnT3ZJE95kTXWXwTr\n"\
"gIOrmgIttRD02JDHBHNA7XIloKmf7J6raBKZV8aPEjoJpL1E/QYVN8Gb5DKj7Tjo\n"\
"2GTzLH4U/ALqn83/B2gX2yKQOC16jdFU8WnjXzPKej17CuPKf1855eJ1usV2GDPO\n"\
"LPAvTK33sefOT6jEm0pUBsV/fdUID+Ic/n4XuKxe9tQWskMJDE32p2u0mYRlynqI\n"\
"4uJEvlz36hz1\n"\
"-----END CERTIFICATE-----\n"};

static const int max_attempts = 10;
static int attempt_cnt = 0;

tuya_mqtt_context_t client_instance;


void on_connected(tuya_mqtt_context_t* context, void* user_data)
{
    syslog(LOG_USER | LOG_INFO, "Connected!");
    tuyalink_thing_data_model_get(context, NULL);
}

void on_disconnect(tuya_mqtt_context_t* context, void* user_data)
{
    syslog(LOG_USER | LOG_INFO, "Disconnected!");
}

void on_messages(tuya_mqtt_context_t* context, void* user_data, const tuyalink_message_t* msg)
{
    switch (msg->type) {

        case THING_TYPE_ACTION_EXECUTE:
            if (msg->data_string == NULL) {
                syslog(LOG_USER | LOG_ERR, "Received empty string");
                return;
            }

            cJSON *action_data = cJSON_Parse(msg->data_string);
            if (action_data == NULL) {
                syslog(LOG_USER | LOG_ERR, "Error parsing JSON");
                return;
            }

            const char *actionCode = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(action_data, "actionCode"));
            if (actionCode == NULL || strcmp(actionCode, "act1") != 0) {
                syslog(LOG_USER | LOG_INFO, "Action not allowed.");
            } else {
                syslog(LOG_USER | LOG_INFO, "Parameter parsed.");
            }

            cJSON_Delete(action_data);
            break;

        default:
            break;
    }
    printf("\r\n");
}

int connect_to_tuya_cloud(char *deviceId, char *deviceSecret){
    int ret = OPRT_OK;

     tuya_mqtt_context_t* client = &client_instance;
    ret = tuya_mqtt_init(client, &(const tuya_mqtt_config_t) {
        .host = "m1.tuyacn.com",
        .port = 8883,
        .cacert = tuya_cacert1,
        .cacert_len = sizeof(tuya_cacert1),
        .device_id = deviceId,
        .device_secret = deviceSecret,
        .keepalive = 100,
        .timeout_ms = 2000,
        .on_connected = on_connected,
        .on_disconnect = on_disconnect,
        .on_messages = on_messages
    });

    if (ret != OPRT_OK) {
        syslog(LOG_USER | LOG_ERR, "Failed to initialize.\n");
        return ret;
    }
    ret = tuya_mqtt_connect(client);
    if (ret != OPRT_OK) {
        syslog(LOG_USER | LOG_ERR, "Failed to connect to cloud.");
        tuya_mqtt_deinit(client);
        return ret;
    }

    return OPRT_OK;
}

int send_data_to_cloud(){
    int ret = OPRT_OK;
    if (!tuya_mqtt_connected(&client_instance)){
        attempt_cnt++;
        tuya_mqtt_loop(&client_instance);
        if (attempt_cnt > max_attempts) {
            syslog(LOG_USER | LOG_ERR, "Max retry attempts reached, time out.");
            return OPRT_NETWORK_ERROR;
        }
        return OPRT_RETRYING;
    }
    ret = tuya_mqtt_loop(&client_instance);
     if (ret != OPRT_OK) {
        syslog(LOG_USER | LOG_ERR, "Error in MQTT loop: %d", ret);
        return ret;
    }

    attempt_cnt = 0;
    syslog(LOG_USER | LOG_INFO, "tuya daemon updated database");

    struct MemData memory = {0};
    ret = get_ubus_data(&memory);
    char json_data[256];
    create_json_data(json_data, sizeof(json_data), memory);

    ret = tuyalink_thing_property_report(&client_instance, NULL, json_data);
    if (ret == OPRT_INVALID_PARM) {
        syslog(LOG_USER | LOG_ERR, "Failed to send data. Error: %d", ret);
    } else {
        ret = OPRT_OK;
        syslog(LOG_USER | LOG_INFO, "Data sent to Tuya cloud: %s", json_data);
    }
    
    return ret;
}

int disconnect(){
    if (&client_instance == NULL)
        return 1;
    if(tuya_mqtt_disconnect(&client_instance) != OPRT_OK)
        return 1;
    if (tuya_mqtt_deinit(&client_instance) != OPRT_OK)
        return 1;
    return 0;
}

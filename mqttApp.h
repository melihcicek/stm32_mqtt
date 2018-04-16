#ifndef _MQTTAPP_H_
#define _MQTTAPP_H_

void example_publish(mqtt_client_t *client, void *arg);
void example_do_connect(mqtt_client_t *client);
static void mqtt_pub_request_cb(void *arg, err_t result);
static int inpub_id;


#endif

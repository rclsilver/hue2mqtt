#include <stdio.h>
#include <mosquitto.h>

#define SRC_CLIENT_ID "hue2mqtt"
#define SRC_EVENT_TOPIC "cmd/clip/event/publish"
#define SRC_MQTT_HOST "localhost"
#define SRC_MQTT_PORT 1883
#define SRC_MQTT_KEEPALIVE 60

#define DST_CLIENT_ID "hue2mqtt"
#define DST_EVENT_TOPIC "hue/events"

static struct mosquitto *dst_mosquitto = NULL;
static struct mosquitto *src_mosquitto = NULL;

static void on_src_connect(struct mosquitto *mosq, void *obj, int result)
{
	if (!result)
	{
		int r = mosquitto_subscribe(src_mosquitto, NULL, SRC_EVENT_TOPIC, 0);

		if (r)
		{
			fprintf(stderr, "Unable to subscribe to %s: %s\n", mosquitto_connack_string(r));
		}
		else
		{
			fprintf(stdout, "Subscribed to %s\n", SRC_EVENT_TOPIC);
		}
	}
}

static void on_src_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
	int r = mosquitto_publish(obj, NULL, DST_EVENT_TOPIC, message->payloadlen, message->payload, 0, false);

	if (r)
	{
		fprintf(stderr, "Unable to publish message: %s\n", mosquitto_connack_string(r));
	} else {
		fprintf(stdout, "Publish message to %s\n", DST_EVENT_TOPIC);
	}
}

int main(void)
{
	int r_src, r_dst;

	const char *dst_mqtt_host = "192.168.3.1";
	const int dst_mqtt_port = 1883;
	const int dst_mqtt_keepalive = 60;

	mosquitto_lib_init();

	dst_mosquitto = mosquitto_new(DST_CLIENT_ID, true, NULL);
	src_mosquitto = mosquitto_new(SRC_CLIENT_ID, true, dst_mosquitto);

	mosquitto_connect_callback_set(src_mosquitto, on_src_connect);
	mosquitto_message_callback_set(src_mosquitto, on_src_message);

	r_dst = mosquitto_connect(dst_mosquitto, dst_mqtt_host, dst_mqtt_port, dst_mqtt_keepalive);

	if (r_dst)
	{
		fprintf(stderr, "Unable to connect to destination MQTT broker: %s\n", mosquitto_connack_string(r_dst));
	}
	else
	{
		fprintf(stdout, "Connected to destination MQTT server (%s:%d)\n", dst_mqtt_host, dst_mqtt_port);
	}

	r_src = mosquitto_connect(src_mosquitto, SRC_MQTT_HOST, SRC_MQTT_PORT, SRC_MQTT_KEEPALIVE);

	if (r_src)
	{
		fprintf(stderr, "Unable to connect to source MQTT broker: %s\n", mosquitto_connack_string(r_src));
	}
	else
	{
		fprintf(stdout, "Connected to source MQTT server (%s:%d)\n", SRC_MQTT_HOST, SRC_MQTT_PORT);

		r_src = mosquitto_subscribe(src_mosquitto, NULL, SRC_EVENT_TOPIC, 0);

		if (r_src)
		{
			fprintf(stderr, "Unable to subscribe to %s: %s\n", mosquitto_connack_string(r_src));
		}
		else
		{
			fprintf(stdout, "Subscribed to %s\n", SRC_EVENT_TOPIC);
		}
	}

	if (!r_src && !r_dst)
	{
		fprintf(stdout, "Waiting for incoming messages...\n");
		mosquitto_loop_start(dst_mosquitto);
		mosquitto_loop_forever(src_mosquitto, -1, 1);
		mosquitto_loop_stop(dst_mosquitto, true);
	}

	mosquitto_disconnect(src_mosquitto);
	mosquitto_disconnect(dst_mosquitto);

	mosquitto_destroy(src_mosquitto);
	mosquitto_destroy(dst_mosquitto);

	mosquitto_lib_cleanup();

	return 0;
}

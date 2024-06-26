/*
** EPITECH PROJECT, 2024
** server
** File description:
** No file there , just an epitech header example .
** You can even have multiple lines if you want !
*/

#include <stdlib.h>
#include "server.h"
#include "server_data.h"

static json_object_t *s_message(roundtable_message_t *message)
{
    json_object_t *json = json_object_create("message");
    json_string_t *content = json_string_create("content", message->content);

    json_object_add(json, (json_t *) content);
    return json;
}

static void send_event(roundtable_server_t *server,
    const roundtable_thread_t *thread, roundtable_message_t *msg,
    roundtable_client_instance_t **instances)
{
    json_object_t *data = NULL;

    for (size_t j = 0; instances[j]; j++) {
        data = json_object_create(NULL);
        json_object_add(data, (json_t *) json_string_create("team_uuid",
            uuid_to_string(thread->channel->team->uuid)));
        json_object_add(data, (json_t *) json_string_create("channel_uuid",
            uuid_to_string(thread->channel->uuid)));
        json_object_add(data, (json_t *) json_string_create("thread_uuid",
            uuid_to_string(thread->uuid)));
        json_object_add(data, (json_t *) json_string_create("sender_uuid",
            uuid_to_string((*msg).sender_uuid)));
        json_object_add(data, (json_t *) s_message(msg));
        roundtable_server_send_event(server, instances[j],
            roundtable_server_create_event(THREAD_REPLIED, data));
    }
    free(instances);
}

void roundtable_event_thread_reply(roundtable_server_t *server,
    roundtable_thread_t *thread)
{
    roundtable_client_t *client = NULL;
    roundtable_message_t msg = thread->messages[thread->message_count - 1];
    roundtable_client_instance_t **instances = NULL;

    for (size_t i = 0; i < thread->channel->team->subscriber_count; i++) {
        client = roundtable_server_get_client_by_uuid(server,
            thread->channel->team->subscribers[i]);
        instances = roundtable_client_instances_find(server, client);
        send_event(server, thread, &msg, instances);
    }
}

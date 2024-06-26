/*
** EPITECH PROJECT, 2024
** server
** File description:
** No file there , just an epitech header example .
** You can even have multiple lines if you want !
*/

#include "server.h"

void roundtable_event_team_created(roundtable_server_t *server,
    roundtable_team_t *team)
{
    json_object_t *data = NULL;

    for (size_t i = 0; i < server->instance_count; i++) {
        data = json_object_create(NULL);
        json_object_add(data, (json_t *) json_string_create("team_uuid",
            uuid_to_string(team->uuid)));
        json_object_add(data, (json_t *) json_string_create("name",
            team->name));
        json_object_add(data, (json_t *) json_string_create("description",
            team->description));
        roundtable_server_send_event(server, server->instances[i],
            roundtable_server_create_event(TEAM_CREATED, data));
    }
}

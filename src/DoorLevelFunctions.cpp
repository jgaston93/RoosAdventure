#include "DoorLevelFunctions.hpp"

void init_door(Level* level, void* data, Entity* player)
{
    DoorLevelData* door_level_data = (DoorLevelData*)data;

    door_level_data->counter = 0;
    door_level_data->background_1_active = true;
    level->background_texture = door_level_data->background_1;
    level->num_exits = 0;
    
}

void update_door(Level* level, void* data, Entity* player, std::map<SDL_Scancode, bool> key_map, float delta_time)
{
    DoorLevelData* door_level_data = (DoorLevelData*)data;

    player->x = 800;
    player->y = 600;
    if(key_map[SDL_SCANCODE_DOWN])
    {
        level->num_exits = 1;
    }

    if((door_level_data->counter++ % 60) == 0)
    {
        if(door_level_data->background_1_active)
        {
            level->background_texture = door_level_data->background_2;
            door_level_data->background_1_active = false;
        }
        else
        {
            level->background_texture = door_level_data->background_1;
            door_level_data->background_1_active = true;
        }
    }
}
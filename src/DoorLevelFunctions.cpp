#include "DoorLevelFunctions.hpp"

char Door_1_Texture_Filename[] = "assets/roo_door_1.png";
char Door_2_Texture_Filename[] = "assets/roo_door_2.png";

void setup_door(Level* level, SDL_Renderer* renderer)
{
    level->background_texture = loadTexture(Door_1_Texture_Filename, renderer);
    level->num_exits = 0;
    level->exits[0] = { 800, 600, 100, 100, HALLWAY_LEVEL_INDEX, 800, 600 };
    level->x_init = 800;
    level->y_init = 600;    
    level->init_level = &init_door;
    level->update_level = &update_door;
    level->pre_character_draw_level = NULL;
    level->post_character_draw_level = NULL;
    DoorLevelData door_level_data;
    door_level_data.background_1 = loadTexture(Door_1_Texture_Filename, renderer);
    door_level_data.background_2 = loadTexture(Door_2_Texture_Filename, renderer);
    door_level_data.background_1_active = true;
    door_level_data.counter = 0;
    level->level_data = new DoorLevelData();
    memcpy(level->level_data, &door_level_data, sizeof(DoorLevelData));
}

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
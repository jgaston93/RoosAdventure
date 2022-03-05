#include "BedroomLevelFunctions.hpp"

void init_bedroom(Level* level, void* data, Entity* player)
{
    BedroomLevelData* bedroom_level_data = (BedroomLevelData*)data;
    if(player->collected[2])
    {
        level->post_character_draw_obstacles[0].texture = bedroom_level_data->bed_maze_entrance_closed_texture;
        level->post_character_draw_obstacles[0].width = 400;
        level->post_character_draw_obstacles[0].height = 200;
        if(level->y_init < 500)
        {
            player->x = 95;
            player->y = 205;
        }
    }    
}
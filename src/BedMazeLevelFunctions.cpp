#include "BedMazeLevelFunctions.hpp"
#include "GameUtilityFunctions.hpp"

void update_bed_maze(Level* level, void* data, Entity* player, std::map<SDL_Scancode, bool> key_map, float delta_time)
{
    BedroomMazeLevelData* bed_maze_level_data = (BedroomMazeLevelData*)data;
    bed_maze_level_data->spotlight_pos_x = player->x;
    bed_maze_level_data->spotlight_pos_y = player->y;

    if(player->collected[2])
    {
        level->num_pre_character_draw_obstacles = 13;
        level->num_exits = 2;
        level->pre_character_draw_obstacles[0].height = 380;
    }
}

void draw_bed_maze(Level* level, void* data, SDL_Renderer* renderer)
{
    BedroomMazeLevelData* bed_maze_level_data = (BedroomMazeLevelData*)data;
    blit(bed_maze_level_data->spolight_texture, bed_maze_level_data->spotlight_pos_x, bed_maze_level_data->spotlight_pos_y, -750, -550, renderer);
}
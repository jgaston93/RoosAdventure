#include "BedroomLevelFunctions.hpp"
#include "GameUtilityFunctions.hpp"

char Bedroom_Level_Background_Texture_Filename[] = "assets/bedroom_background.png";
char Bed_Maze_Entrance_Texture_Filename[] = "assets/bed_maze_entrance.png";
char Bed_Maze_Entrance_Closed_Texture_Filename[] = "assets/bed_maze_entrance_closed.png";

void setup_bedroom(Level* level, SDL_Renderer* renderer)
{
    level->background_texture = loadTexture(Bedroom_Level_Background_Texture_Filename, renderer);
    level->num_pre_character_draw_obstacles = 5;
    level->num_post_character_draw_obstacles = 1;
    level->pre_character_draw_obstacles[0] = {-100, 0.0, 100, 600, 0, 0, NULL };
    level->pre_character_draw_obstacles[1] = {0.0, 0.0, 800, 200, 0, 0, NULL };
    level->pre_character_draw_obstacles[2] = {800.0, 0.0, 100, 600, 0, 0, NULL };
    level->pre_character_draw_obstacles[3] = {200.0, 200.0, 140, 200, 0, 0, NULL };
    level->pre_character_draw_obstacles[4] = {460.0, 200.0, 140, 200, 0, 0, NULL };
    level->post_character_draw_obstacles[0] = {200.0, 201.0, 0, 0, 0, 0, loadTexture(Bed_Maze_Entrance_Texture_Filename, renderer) };
    level->x_init = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2;
    level->y_init = SCREEN_HEIGHT - PLAYER_HEIGHT;
    level->num_exits = 2;
    level->exits[0] = { 0, SCREEN_HEIGHT + (float)PLAYER_HEIGHT / 2, 800, 100, HALLWAY_LEVEL_INDEX, SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2, SCREEN_HEIGHT - PLAYER_HEIGHT};
    level->exits[1] = { SCREEN_WIDTH / 2 - (float)PLAYER_WIDTH / 2, 200, 100, 100, BEDROOM_MAZE_1_LEVEL_INDEX, SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2, 400};
    level->init_level = &init_bedroom;
    level->update_level = NULL;
    level->pre_character_draw_level = NULL;
    level->post_character_draw_level = NULL;
    BedroomLevelData bedroom_level_data;
    bedroom_level_data.bed_maze_entrance_closed_texture = loadTexture(Bed_Maze_Entrance_Closed_Texture_Filename, renderer);
    level->level_data = new BedroomLevelData();
    memcpy(level->level_data, &bedroom_level_data, sizeof(BedroomLevelData));
}

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
#include "BedMazeLevelFunctions.hpp"
#include "GameUtilityFunctions.hpp"

char Bed_Maze_Spotlight_Texture_Fileanme[] = "assets/bed_maze_spotlight.png";
char Bedroom_Level_Maze_1_Background_Texture_Filename[] = "assets/bedroom_maze_1_background.png";
char Bedroom_Level_Maze_2_Background_Texture_Filename[] = "assets/bedroom_maze_2_background.png";
char Bed_Maze_Block_Texture_Filename[] = "assets/bed_maze_block.png";
char Yellow_Lamb_Texture_Filename[] = "assets/yellow_lamb.png";

void setup_bed_maze_1(Level* level, SDL_Renderer* renderer)
{
    level->background_texture = loadTexture(Bedroom_Level_Maze_1_Background_Texture_Filename, renderer);
    level->num_pre_character_draw_obstacles = 12;
    level->pre_character_draw_obstacles[0] = {-100, 0.0, 100, 600, 0, 0, NULL };
    level->pre_character_draw_obstacles[1] = {0.0, -100.0, 340, 100, 0, 0, NULL };
    level->pre_character_draw_obstacles[2] = {460.0, -100.0, 340, 100, 0, 0, NULL };
    level->pre_character_draw_obstacles[3] = {800.0, 0.0, 100, 600, 0, 0, NULL };
    level->pre_character_draw_obstacles[4] = {0.0, 600.0, 340, 100, 0, 0, NULL };
    level->pre_character_draw_obstacles[5] = {460.0, 600.0, 340, 100, 0, 0, NULL };
    level->pre_character_draw_obstacles[6] = {0.0, 0.0, 100, 330, 0, 0, NULL };
    level->pre_character_draw_obstacles[7] = {0.0, 450.0, 340, 150, 0, 0, NULL };
    level->pre_character_draw_obstacles[8] = {220.0, 0.0, 120, 330, 0, 0, NULL };
    level->pre_character_draw_obstacles[9] = {340.0, 170.0, 330, 110, 0, 0, NULL };
    level->pre_character_draw_obstacles[10] = {460.0, 0.0, 210, 50, 0, 0, NULL };
    level->pre_character_draw_obstacles[11] = {460.0, 400.0, 210, 200, 0, 0, NULL };
    level->x_init = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2;
    level->y_init = SCREEN_HEIGHT - PLAYER_HEIGHT;
    level->num_exits = 2;
    level->exits[0] = { 0, SCREEN_HEIGHT + (float)PLAYER_HEIGHT / 2, 800, 100, BEDROOM_LEVEL_INDEX, SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2, SCREEN_HEIGHT - PLAYER_HEIGHT};
    level->exits[1] = { 0, -100, 800, 100, BEDROOM_MAZE_2_LEVEL_INDEX, SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2, 0 };
    level->init_level = NULL;
    level->update_level = &update_bed_maze;
    level->pre_character_draw_level = NULL;
    level->post_character_draw_level = &draw_bed_maze;
    BedroomMazeLevelData bedroom_maze_level_data;
    bedroom_maze_level_data.spolight_texture = loadTexture(Bed_Maze_Spotlight_Texture_Fileanme, renderer);
    bedroom_maze_level_data.spotlight_pos_x = 0;
    bedroom_maze_level_data.spotlight_pos_y = 0;
    level->level_data = new BedroomMazeLevelData();
    memcpy(level->level_data, &bedroom_maze_level_data, sizeof(BedroomMazeLevelData));
}

void setup_bed_maze_2(Level* level, SDL_Renderer* renderer)
{
    level->background_texture = loadTexture(Bedroom_Level_Maze_2_Background_Texture_Filename, renderer);
    level->num_collectibles = 1;
    level->collectibles[0] = { 170.0, 380.0, 100, 100, false, loadTexture(Yellow_Lamb_Texture_Filename, renderer), -50, 0};
    level->num_pre_character_draw_obstacles = 12;
    level->pre_character_draw_obstacles[0] = { -100, 0.0, 100, 600, 0, 0, NULL };
    level->pre_character_draw_obstacles[1] = { 0.0, -100.0, 800, 100, 0, 0, NULL };
    level->pre_character_draw_obstacles[2] = { 800.0, 0.0, 100, 600, 0, 0, NULL };
    level->pre_character_draw_obstacles[3] = { 0.0, 600.0, 340, 100, 0, 0, NULL };
    level->pre_character_draw_obstacles[4] = { 460.0, 600.0, 340, 100, 0, 0, NULL };
    level->pre_character_draw_obstacles[5] = { 120.0, 120.0, 100, 40, 0, 0, NULL };
    level->pre_character_draw_obstacles[6] = { 120.0, 280.0, 100, 100, 0, 0, NULL };
    level->pre_character_draw_obstacles[7] = { 0.0, 500.0, 220, 100, 0, 0, NULL };
    level->pre_character_draw_obstacles[8] = { 220.0, 330.0, 120, 270, 0, 0, NULL };
    level->pre_character_draw_obstacles[9] = { 340.0, 120.0, 100, 370, 0, 0, NULL };
    level->pre_character_draw_obstacles[10] = { 440.0, 120.0, 240, 120, 0, 0, NULL };
    level->pre_character_draw_obstacles[11] = { 440.0, 360.0, 240, 130, 0, 0, NULL };
    level->pre_character_draw_obstacles[12] = { 10.0, 280.0, 120, 100, 0, 0, loadTexture(Bed_Maze_Block_Texture_Filename, renderer) };
    level->x_init = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2;
    level->y_init = SCREEN_HEIGHT - PLAYER_HEIGHT;
    level->num_exits = 1;
    level->exits[0] = { 0, SCREEN_HEIGHT + (float)PLAYER_HEIGHT / 2, 800, 100, BEDROOM_MAZE_1_LEVEL_INDEX, SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2, SCREEN_HEIGHT - PLAYER_HEIGHT};
    level->exits[1] = { -150, 380, 100, 100, BEDROOM_LEVEL_INDEX, 0, 0 };
    level->init_level = NULL;
    level->update_level = &update_bed_maze;
    level->pre_character_draw_level = NULL;
    level->post_character_draw_level = &draw_bed_maze;
    BedroomMazeLevelData bedroom_maze_level_data;
    bedroom_maze_level_data.spolight_texture = loadTexture(Bed_Maze_Spotlight_Texture_Fileanme, renderer);
    bedroom_maze_level_data.spotlight_pos_x = 0;
    bedroom_maze_level_data.spotlight_pos_y = 0;
    level->level_data = new BedroomMazeLevelData();
    memcpy(level->level_data, &bedroom_maze_level_data, sizeof(BedroomMazeLevelData));
}

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
    blit(bed_maze_level_data->spolight_texture, bed_maze_level_data->spotlight_pos_x, bed_maze_level_data->spotlight_pos_y, -760, -567, renderer);
}
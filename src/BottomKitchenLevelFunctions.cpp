#include "BottomKitchenLevelFunctions.hpp"
#include "GameUtilityFunctions.hpp"

char Bottom_Kitchen_Level_Background_Texture_Filename[] = "assets/bottom_kitchen_background.png";
char Stairs_Texture_Filename[] = "assets/stairs.png";

void setup_bottom_kitchen_level(Level* level, SDL_Renderer* renderer)
{
    level->background_texture = loadTexture(Bottom_Kitchen_Level_Background_Texture_Filename, renderer);
    level->num_pre_character_draw_obstacles = 5;
    level->num_post_character_draw_obstacles = 1;
    level->pre_character_draw_obstacles[0] = { -100.0, 0.0, 100, 600, 0, 0, NULL };
    level->pre_character_draw_obstacles[1] = { 0.0, 0.0, 90, 100, 0, 0, NULL };
    level->pre_character_draw_obstacles[2] = { 210.0, 0.0, 280, 100, 0, 0, NULL };
    level->pre_character_draw_obstacles[3] = { 610.0, 0.0, 190, 100, 0, 0, NULL };
    level->pre_character_draw_obstacles[4] = { 800.0, 300.0, 100, 300, 0, 0, NULL };
    level->post_character_draw_obstacles[0] = { 600.0, 210.0, 200, 90, 0, -210, loadTexture(Stairs_Texture_Filename, renderer) };
    level->x_init = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2;
    level->y_init = SCREEN_HEIGHT - PLAYER_HEIGHT;
    level->num_exits = 4;
    level->exits[0] = { 0.0, SCREEN_HEIGHT + (float)PLAYER_HEIGHT / 2, 800, 100, KITCHEN_LEVEL_INDEX, SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2, SCREEN_HEIGHT - PLAYER_HEIGHT };
    level->exits[1] = { SCREEN_WIDTH + (float)PLAYER_WIDTH / 2, 100, 100, 110, HALLWAY_LEVEL_INDEX, SCREEN_WIDTH - PLAYER_WIDTH, 100 };
    level->exits[2] = { 90.0, -((float)PLAYER_HEIGHT + (float)PLAYER_HEIGHT / 2), 120, PLAYER_HEIGHT , OUTSIDE_LEVEL_INDEX, 100, 0 };
    level->exits[3] = { 490.0, -((float)PLAYER_HEIGHT + (float)PLAYER_HEIGHT / 2), 120, PLAYER_HEIGHT, OFFICE_LEVEL_INDEX, 500, 0 };
    level->init_level = NULL;
    level->update_level = NULL;
    level->pre_character_draw_level = NULL;
    level->post_character_draw_level = NULL;
    level->level_data = NULL;
}
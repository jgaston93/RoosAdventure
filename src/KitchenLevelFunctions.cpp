#include "KitchenLevelFunctions.hpp"
#include "GameUtilityFunctions.hpp"

char Middle_Counter_Texture_Filename[] = "assets/middle_counter.png";
char Kitchen_Level_Background_Texture_Filename[] = "assets/kitchen_background.png";

void setup_kitchen_level(Level* level, SDL_Renderer* renderer)
{
    level->background_texture = loadTexture(Kitchen_Level_Background_Texture_Filename, renderer);
    level->num_pre_character_draw_obstacles = 3;
    level->num_post_character_draw_obstacles = 1;
    level->pre_character_draw_obstacles[0] = { 0.0, 0.0, 100, 350, 0, 0, NULL };
    level->pre_character_draw_obstacles[1] = { 100.0, 0.0, 700, 200, 0, 0, NULL };
    level->pre_character_draw_obstacles[2] = { 800.0, 0.0, 100, 600, 0, 0, NULL };
    level->post_character_draw_obstacles[0] = { 300.0, 310.0, 300, 200, 0, -100, loadTexture(Middle_Counter_Texture_Filename, renderer) };
    level->x_init = 0;
    level->y_init = 400;
    level->num_exits = 2;
    level->exits[0] = { -(100 + (float)PLAYER_WIDTH / 2), 400, 100, 200, LIVING_ROOM_LEVEL_INDEX , 0, 400};
    level->exits[1] = { 0.0, (float)SCREEN_HEIGHT + PLAYER_HEIGHT / 2, 800, 100, BOTTOM_KITCHEN_LEVEL_INDEX, SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2, SCREEN_HEIGHT - PLAYER_HEIGHT };
    level->init_level = NULL;
    level->update_level = NULL;
    level->pre_character_draw_level = NULL;
    level->post_character_draw_level = NULL;
    level->level_data = NULL;
}
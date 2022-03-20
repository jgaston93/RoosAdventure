#include "HallwayLevelFunctions.hpp"
#include "GameUtilityFunctions.hpp"

char Hallway_Level_Background_Texture_Filename[] = "assets/hallway_background.png";
char Hallway_Stairs_Texture_Filename[] = "assets/hallway_stairs.png";

void setup_hallway_level(Level* level, SDL_Renderer* renderer)
{
    level->background_texture = loadTexture(Hallway_Level_Background_Texture_Filename, renderer);
    level->num_pre_character_draw_obstacles = 4;
    level->num_post_character_draw_obstacles = 1;
    level->pre_character_draw_obstacles[0] = {0.0, 0.0, 590, 125, 0, 0, NULL };
    level->pre_character_draw_obstacles[1] = {710.0, 0.0, 90, 125, 0, 0, NULL };
    level->pre_character_draw_obstacles[2] = {800, 100.0, 100, 500, 0, 0, NULL };
    level->pre_character_draw_obstacles[3] = {0.0, 600.0, 800, 100, 0, 0, NULL };
    level->post_character_draw_obstacles[0] = {0.0, 300.0, 450, 100, 0, -200, loadTexture(Hallway_Stairs_Texture_Filename, renderer) };
    level->x_init = 450;
    level->y_init = 125;
    level->num_exits = 3;
    level->exits[0] = { 300, 125, 100, 175, BOTTOM_KITCHEN_LEVEL_INDEX, 450, 125};
    level->exits[1] = { 590, -150, 120, 100, BEDROOM_LEVEL_INDEX , 600, 0};
    level->exits[2] = { -150, 400, 100, 200, DOOR_LEVEL_INDEX, 0, 450 };
    level->init_level = NULL;
    level->update_level = NULL;
    level->pre_character_draw_level = NULL;
    level->post_character_draw_level = NULL;
    level->level_data = NULL;
}
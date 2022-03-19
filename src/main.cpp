#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <map>
#include <limits>

#include "CollisionCheckingFunctions.hpp"
#include "GameConstants.hpp"
#include "GameDataStructures.hpp"
#include "GameUtilityFunctions.hpp"

#include "BedMazeLevelFunctions.hpp"
#include "ComputerLevelFunctions.hpp"
#include "LivingRoomLevelFunctions.hpp"
#include "OfficeLevelFunctions.hpp"
#include "OutsideLevelFunctions.hpp"
#include "DoorLevelFunctions.hpp"
#include "BedroomLevelFunctions.hpp"

std::string Window_Title = "Roo's Adventure";

char Standing_Texture_Filename[] = "assets/roo_standing.png";
char Walking_Texture_Filename_1[] = "assets/roo_walking_1.png";
char Walking_Texture_Filename_2[] = "assets/roo_walking_2.png";
char Peeing_Texture_Filename_1[] = "assets/roo_peeing_1.png";
char Peeing_Texture_Filename_2[] = "assets/roo_peeing_2.png";
char Idle_Texture_Filename[] = "assets/roo_sitting.png";
char Kitchen_Level_Background_Texture_Filename[] = "assets/kitchen_background.png";
char Bottom_Kitchen_Level_Background_Texture_Filename[] = "assets/bottom_kitchen_background.png";
char Stairs_Texture_Filename[] = "assets/stairs.png";
char Middle_Counter_Texture_Filename[] = "assets/middle_counter.png";
char White_Lamb_Texture_Filename[] = "assets/white_lamb.png";
char Purple_Lamb_Texture_Filename[] = "assets/purple_lamb.png";
char Living_Room_Level_Background_Texture_Filename[] = "assets/living_room_level_background.png";
char Box_Obstacle_Texture_Filename[] = "assets/box_obstacle.png";
char Hallway_Level_Background_Texture_Filename[] = "assets/hallway_background.png";
char Hallway_Stairs_Texture_Filename[] = "assets/hallway_stairs.png";
char Outside_Level_Background_Texture_Filename[] = "assets/outside_background.png";
char Office_Level_Background_Texture_Filename[] = "assets/office_background.png";
char Electric_Cage_1_Texture_Filename[] = "assets/electric_cage_1.png";
char Electric_Cage_2_Texture_Filename[] = "assets/electric_cage_2.png";
char Computer_Face_1_Texture_Filename[] = "assets/computer_face_1.png";
char Computer_Face_2_Texture_Filename[] = "assets/computer_face_2.png";
char Computer_Background_1_Texture_Filename[] = "assets/computer_background_1.png";
char Computer_Background_2_Texture_Filename[] = "assets/computer_background_2.png";
char Computer_Background_3_Texture_Filename[] = "assets/computer_background_3.png";
char Paddle_Texture_Filename[] = "assets/paddle.png";
char Ball_Texture_Filename[] = "assets/ball.png";
char Ball_2_Texture_Filename[] = "assets/ball_2.png";
char Bone_Texture_Filename[] = "assets/bone.png";
char Bedroom_Level_Background_Texture_Filename[] = "assets/bedroom_background.png";
char Bed_Maze_Entrance_Texture_Filename[] = "assets/bed_maze_entrance.png";
char Bed_Maze_Entrance_Closed_Texture_Filename[] = "assets/bed_maze_entrance_closed.png";
char Bed_Maze_Spotlight_Texture_Fileanme[] = "assets/bed_maze_spotlight.png";
char Bedroom_Level_Maze_1_Background_Texture_Filename[] = "assets/bedroom_maze_1_background.png";
char Bedroom_Level_Maze_2_Background_Texture_Filename[] = "assets/bedroom_maze_2_background.png";
char Bed_Maze_Block_Texture_Filename[] = "assets/bed_maze_block.png";
char Loft_Level_Background_Texture_Filename[] = "assets/loft_background.png";
char Bush_Texture_Filename[] = "assets/bush.png";
char Angry_Bush_1_Texture_Filename[] = "assets/angry_bush_1.png";
char Angry_Bush_2_Texture_Filename[] = "assets/angry_bush_2.png";
char Dead_Bush_1_Texture_Filename[] = "assets/dead_bush_1.png";
char Dead_Bush_2_Texture_Filename[] = "assets/dead_bush_2.png";
char Dead_Bush_3_Texture_Filename[] = "assets/dead_bush_3.png";
char Dead_Bush_4_Texture_Filename[] = "assets/dead_bush_4.png";
char Dead_Bush_5_Texture_Filename[] = "assets/dead_bush_5.png";
char Vaccuum_1_Texture_Filename[] = "assets/vaccuum_1.png";
char Vaccuum_2_Texture_Filename[] = "assets/vaccuum_2.png";
char Vaccuum_3_Texture_Filename[] = "assets/vaccuum_3.png";
char Vaccuum_4_Texture_Filename[] = "assets/vaccuum_4.png";
char Vaccuum_5_Texture_Filename[] = "assets/vaccuum_5.png";
char Vaccuum_6_Texture_Filename[] = "assets/vaccuum_6.png";
char Vaccuum_7_Texture_Filename[] = "assets/vaccuum_7.png";
char Vaccuum_8_Texture_Filename[] = "assets/vaccuum_8.png";
char Outlet_Spark_1_Texture_Filename[] = "assets/outlet_spark_1.png";
char Outlet_Spark_2_Texture_Filename[] = "assets/outlet_spark_2.png";
char Door_1_Texture_Filename[] = "assets/roo_door_1.png";
char Door_2_Texture_Filename[] = "assets/roo_door_2.png";


SDL_Renderer *renderer;
SDL_Window* window;

std::map<SDL_Scancode, bool> key_map;
int level_transition_counter;

bool isRunning = true;

SDL_Texture* loadTexture(char* filename);
void handleInput();
void updatePlayer(Entity& p);

int main(int argv, char** args)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow(Window_Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);

    IMG_Init(IMG_INIT_PNG);

    Level kitchen_level;
    Level bottom_kitchen_level;
    Level living_room_level;
    Level hallway_level;
    Level outside_level;
    Level office_level;
    Level computer_level;
    Level bedroom_level;
    Level bedroom_maze_1_level;
    Level bedroom_maze_2_level;
    Level loft_level;
    Level door_level;
    Entity player;
    memset(&kitchen_level, 0, sizeof(Level));
    memset(&bottom_kitchen_level, 0, sizeof(Level));
    memset(&living_room_level, 0, sizeof(Level));
    memset(&hallway_level, 0, sizeof(Level));
    memset(&outside_level, 0, sizeof(Level));
    memset(&office_level, 0, sizeof(Level));
    memset(&computer_level, 0, sizeof(Level));
    memset(&bedroom_level, 0, sizeof(Level));
    memset(&bedroom_maze_1_level, 0, sizeof(Level));
    memset(&bedroom_maze_2_level, 0, sizeof(Level));
    memset(&loft_level, 0, sizeof(Level));
    memset(&door_level, 0, sizeof(Level));
    memset(&player, 0, sizeof(Entity));

    player.width = 80;
    player.height = 65;
    player.speed = 200;
    player.idle_threshold = 200;
    player.collected[0] = false;
    player.collected[1] = false;
    player.collected[2] = false;
    player.collected[3] = false;

    // Kitchen level initialization
    kitchen_level.background_texture = loadTexture(Kitchen_Level_Background_Texture_Filename);
    kitchen_level.num_pre_character_draw_obstacles = 3;
    kitchen_level.num_post_character_draw_obstacles = 1;
    kitchen_level.pre_character_draw_obstacles[0] = { 0.0, 0.0, 100, 350, 0, 0, NULL };
    kitchen_level.pre_character_draw_obstacles[1] = { 100.0, 0.0, 700, 200, 0, 0, NULL };
    kitchen_level.pre_character_draw_obstacles[2] = { 800.0, 0.0, 100, 600, 0, 0, NULL };
    kitchen_level.post_character_draw_obstacles[0] = { 300.0, 310.0, 300, 200, 0, -100, loadTexture(Middle_Counter_Texture_Filename) };
    kitchen_level.x_init = 0;
    kitchen_level.y_init = 400;
    kitchen_level.num_collectibles = 0;
    kitchen_level.collectibles[0] = { 400, SCREEN_HEIGHT - 100, 100, 100, false, loadTexture(White_Lamb_Texture_Filename), 0, 0};
    kitchen_level.num_exits = 2;
    kitchen_level.exits[0] = { -(100 + (float)player.width / 2), 400, 100, 200, LIVING_ROOM_LEVEL_INDEX , 0, 400};
    kitchen_level.exits[1] = { 0.0, (float)SCREEN_HEIGHT + player.height / 2, 800, 100, BOTTOM_KITCHEN_LEVEL_INDEX, SCREEN_WIDTH / 2 - player.width / 2, SCREEN_HEIGHT - player.height };
    kitchen_level.init_level = NULL;
    kitchen_level.update_level = NULL;
    kitchen_level.pre_character_draw_level = NULL;
    kitchen_level.post_character_draw_level = NULL;
    kitchen_level.level_data = NULL;

    // Bottom level initialization
    bottom_kitchen_level.background_texture = loadTexture(Bottom_Kitchen_Level_Background_Texture_Filename);
    bottom_kitchen_level.num_pre_character_draw_obstacles = 5;
    bottom_kitchen_level.num_post_character_draw_obstacles = 1;
    bottom_kitchen_level.pre_character_draw_obstacles[0] = { -100.0, 0.0, 100, 600, 0, 0, NULL };
    bottom_kitchen_level.pre_character_draw_obstacles[1] = { 0.0, 0.0, 90, 100, 0, 0, NULL };
    bottom_kitchen_level.pre_character_draw_obstacles[2] = { 210.0, 0.0, 280, 100, 0, 0, NULL };
    bottom_kitchen_level.pre_character_draw_obstacles[3] = { 610.0, 0.0, 190, 100, 0, 0, NULL };
    bottom_kitchen_level.pre_character_draw_obstacles[4] = { 800.0, 300.0, 100, 300, 0, 0, NULL };
    bottom_kitchen_level.post_character_draw_obstacles[0] = { 600.0, 210.0, 200, 90, 0, -210, loadTexture(Stairs_Texture_Filename) };
    bottom_kitchen_level.x_init = SCREEN_WIDTH / 2 - player.width / 2;
    bottom_kitchen_level.y_init = SCREEN_HEIGHT - player.height;
    bottom_kitchen_level.num_exits = 4;
    bottom_kitchen_level.exits[0] = { 0.0, SCREEN_HEIGHT + (float)player.height / 2, 800, 100, KITCHEN_LEVEL_INDEX, SCREEN_WIDTH / 2 - player.width / 2, SCREEN_HEIGHT - player.height };
    bottom_kitchen_level.exits[1] = { SCREEN_WIDTH + (float)player.width / 2, 100, 100, 110, HALLWAY_LEVEL_INDEX, SCREEN_WIDTH - player.width, 100 };
    bottom_kitchen_level.exits[2] = { 90.0, -((float)player.height + (float)player.height / 2), 120, player.height , OUTSIDE_LEVEL_INDEX, 100, 0 };
    bottom_kitchen_level.exits[3] = { 490.0, -((float)player.height + (float)player.height / 2), 120, player.height, OFFICE_LEVEL_INDEX, 500, 0 };
    bottom_kitchen_level.init_level = NULL;
    bottom_kitchen_level.update_level = NULL;
    bottom_kitchen_level.pre_character_draw_level = NULL;
    bottom_kitchen_level.post_character_draw_level = NULL;
    bottom_kitchen_level.level_data = NULL;

    // Living room level initialization
    living_room_level.background_texture = loadTexture(Living_Room_Level_Background_Texture_Filename);
    living_room_level.num_pre_character_draw_obstacles = 9;
    living_room_level.num_post_character_draw_obstacles = 1;
    living_room_level.pre_character_draw_obstacles[0] = { 0.0, 0.0, 260, 255, 0, 0, NULL };
    living_room_level.pre_character_draw_obstacles[1] = { 260.0, 0.0, 300, 210, 0, 0, NULL };
    living_room_level.pre_character_draw_obstacles[2] = { 560.0, 0.0, 50, 255, 0, 0, NULL };
    living_room_level.pre_character_draw_obstacles[3] = { 615.0, 0.0, 185, 70, 0, 0, NULL };
    living_room_level.pre_character_draw_obstacles[4] = { 0.0, 600.0, 800, 100, 0, 0, NULL };
    living_room_level.pre_character_draw_obstacles[5] = { 800.0, 0.0, 100, 600, 0, 0, NULL };
    living_room_level.pre_character_draw_obstacles[6] = { 135.0, 335.0, 90, 90, -5, -5, loadTexture(Box_Obstacle_Texture_Filename) };
    living_room_level.pre_character_draw_obstacles[7] = { 355.0, 385.0, 90, 90, -5, -5, loadTexture(Box_Obstacle_Texture_Filename) };
    living_room_level.pre_character_draw_obstacles[8] = { 575.0, 335.0, 90, 90, -5, -5, loadTexture(Box_Obstacle_Texture_Filename) };
    living_room_level.post_character_draw_obstacles[0] = {620.0, 115.0, 100, 20, 0, -80, NULL };
    living_room_level.x_init = 0;
    living_room_level.y_init = 400;
    living_room_level.num_collectibles = 0;
    living_room_level.collectibles[0] = { -100, -100, 10, 10, false, loadTexture(White_Lamb_Texture_Filename), -45, -45};
    living_room_level.num_exits = 1;
    living_room_level.exits[0] = { -(100 + (float)player.width / 2), 210, 100, 390, KITCHEN_LEVEL_INDEX , 0, 400};
    living_room_level.init_level = &init_living_room;
    living_room_level.update_level = &update_living_room;
    living_room_level.pre_character_draw_level = &draw_living_room;
    living_room_level.post_character_draw_level = &draw_vaccuum;
    LivingRoomLevelData living_room_level_data;
    living_room_level_data.counter = 0;
    living_room_level_data.complete = false;
    living_room_level_data.num_vaccuum_cord_points = 0;
    Animation vaccuum_animation;
    vaccuum_animation.animation_counter = 0;
    vaccuum_animation.animation_speed = 15;
    vaccuum_animation.current_texture_index = 0;
    vaccuum_animation.num_textures = 2;
    vaccuum_animation.textures[0] = loadTexture(Vaccuum_1_Texture_Filename);
    vaccuum_animation.textures[1] = loadTexture(Vaccuum_2_Texture_Filename);
    living_room_level_data.vaccuum_animation = vaccuum_animation;
    Animation vaccuum_short_circuit_animation;
    vaccuum_short_circuit_animation.animation_counter = 0;
    vaccuum_short_circuit_animation.animation_speed = 15;
    vaccuum_short_circuit_animation.current_texture_index = 0;
    vaccuum_short_circuit_animation.num_textures = 2;
    vaccuum_short_circuit_animation.textures[0] = loadTexture(Vaccuum_3_Texture_Filename);
    vaccuum_short_circuit_animation.textures[1] = loadTexture(Vaccuum_4_Texture_Filename);
    living_room_level_data.vaccuum_short_circuit_animation = vaccuum_short_circuit_animation;
    Animation vaccuum_smoking_animation;
    vaccuum_smoking_animation.animation_counter = 0;
    vaccuum_smoking_animation.animation_speed = 30;
    vaccuum_smoking_animation.current_texture_index = 0;
    vaccuum_smoking_animation.num_textures = 2;
    vaccuum_smoking_animation.textures[0] = loadTexture(Vaccuum_5_Texture_Filename);
    vaccuum_smoking_animation.textures[1] = loadTexture(Vaccuum_6_Texture_Filename);
    living_room_level_data.vaccuum_smoking_animation = vaccuum_smoking_animation;
    Animation vaccuum_explosion_animation;
    vaccuum_explosion_animation.animation_counter = 0;
    vaccuum_explosion_animation.animation_speed = 10;
    vaccuum_explosion_animation.current_texture_index = 0;
    vaccuum_explosion_animation.num_textures = 2;
    vaccuum_explosion_animation.textures[0] = loadTexture(Vaccuum_7_Texture_Filename);
    vaccuum_explosion_animation.textures[1] = loadTexture(Vaccuum_8_Texture_Filename);
    living_room_level_data.vaccuum_explosion_animation = vaccuum_explosion_animation;
    Animation outlet_spark_animation;
    outlet_spark_animation.animation_counter = 0;
    outlet_spark_animation.animation_speed = 10;
    outlet_spark_animation.current_texture_index = 0;
    outlet_spark_animation.num_textures = 2;
    outlet_spark_animation.textures[0] = loadTexture(Outlet_Spark_1_Texture_Filename);
    outlet_spark_animation.textures[1] = loadTexture(Outlet_Spark_2_Texture_Filename);
    living_room_level_data.outlet_spark_animation = outlet_spark_animation;
    living_room_level.level_data = &living_room_level_data;

    // Hallway level initialization
    hallway_level.background_texture = loadTexture(Hallway_Level_Background_Texture_Filename);
    hallway_level.num_pre_character_draw_obstacles = 4;
    hallway_level.num_post_character_draw_obstacles = 1;
    hallway_level.pre_character_draw_obstacles[0] = {0.0, 0.0, 590, 125, 0, 0, NULL };
    hallway_level.pre_character_draw_obstacles[1] = {710.0, 0.0, 90, 125, 0, 0, NULL };
    hallway_level.pre_character_draw_obstacles[2] = {800, 100.0, 100, 500, 0, 0, NULL };
    hallway_level.pre_character_draw_obstacles[3] = {0.0, 600.0, 800, 100, 0, 0, NULL };
    hallway_level.post_character_draw_obstacles[0] = {0.0, 300.0, 450, 100, 0, -200, loadTexture(Hallway_Stairs_Texture_Filename) };
    hallway_level.x_init = 450;
    hallway_level.y_init = 125;
    hallway_level.num_exits = 3;
    hallway_level.exits[0] = { 300, 125, 100, 175, BOTTOM_KITCHEN_LEVEL_INDEX, 450, 125};
    hallway_level.exits[1] = { 590, -150, 120, 100, BEDROOM_LEVEL_INDEX , 600, 0};
    hallway_level.exits[2] = { -150, 400, 100, 200, DOOR_LEVEL_INDEX, 0, 450 };
    hallway_level.init_level = NULL;
    hallway_level.update_level = NULL;
    hallway_level.pre_character_draw_level = NULL;
    hallway_level.post_character_draw_level = NULL;
    hallway_level.level_data = NULL;

    // Outside level initialization
    outside_level.background_texture = loadTexture(Outside_Level_Background_Texture_Filename);
    outside_level.num_pre_character_draw_obstacles = 8;
    outside_level.num_post_character_draw_obstacles = 0;
    outside_level.num_collectibles = 0;
    outside_level.collectibles[0] = { 355.0, 325.0, 80, 2, false, loadTexture(Purple_Lamb_Texture_Filename), 0, 0};
    outside_level.pre_character_draw_obstacles[0] = {-100, 0.0, 100, 600, 0, 0, NULL };
    outside_level.pre_character_draw_obstacles[1] = {0.0, 0.0, 800, 100, 0, 0, NULL };
    outside_level.pre_character_draw_obstacles[2] = {800.0, 0.0, 100, 600, 0, 0, NULL };
    SDL_Texture* bush_texture = loadTexture(Bush_Texture_Filename);
    outside_level.pre_character_draw_obstacles[3] = {105.0, 225.0, 80, 22, 0, -20, bush_texture };
    outside_level.pre_character_draw_obstacles[4] = {605.0, 225.0, 80, 22, 0, -20, bush_texture };
    outside_level.pre_character_draw_obstacles[5] = {355.0, 325.0, 80, 22, 0, -20, bush_texture };
    outside_level.pre_character_draw_obstacles[6] = {105.0, 425.0, 80, 22, 0, -20, bush_texture };
    outside_level.pre_character_draw_obstacles[7] = {605.0, 425.0, 80, 22, 0, -20, bush_texture };
    outside_level.x_init = SCREEN_WIDTH / 2 - player.width / 2;
    outside_level.y_init = SCREEN_HEIGHT - player.height;
    outside_level.num_exits = 1;
    outside_level.exits[0] = { 0, SCREEN_HEIGHT + (float)player.height / 2, 800, 100, BOTTOM_KITCHEN_LEVEL_INDEX, SCREEN_WIDTH / 2 - player.width / 2, SCREEN_HEIGHT - player.height};

    outside_level.init_level = &init_outside;
    outside_level.update_level = &update_outside;
    outside_level.pre_character_draw_level = NULL;
    outside_level.post_character_draw_level = NULL;
    OutsideLevelData outside_level_data;
    outside_level_data.num_bushes = 5;
    outside_level_data.bush_order[0] = 0;
    outside_level_data.bush_order[1] = 1;
    outside_level_data.bush_order[2] = 2;
    outside_level_data.bush_order[3] = 3;
    outside_level_data.bush_order[4] = 4;
    outside_level_data.bush_index = 0;
    outside_level_data.counter = 0;
    outside_level_data.selected_bush = 0;
    outside_level_data.complete = false;
    Animation bush_animation;
    bush_animation.animation_counter = 0;
    bush_animation.animation_speed = 15;
    bush_animation.current_texture_index = 0;
    bush_animation.num_textures = 2;
    bush_animation.textures[0] = loadTexture(Angry_Bush_1_Texture_Filename);
    bush_animation.textures[1] = loadTexture(Angry_Bush_2_Texture_Filename);
    outside_level_data.bush_animation = bush_animation;
    Animation bush_death_animation;
    bush_death_animation.animation_counter = 0;
    bush_death_animation.animation_speed = 15;
    bush_death_animation.current_texture_index = 0;
    bush_death_animation.num_textures = 4;
    bush_death_animation.textures[0] = loadTexture(Dead_Bush_1_Texture_Filename);
    bush_death_animation.textures[1] = loadTexture(Dead_Bush_2_Texture_Filename);
    bush_death_animation.textures[2] = loadTexture(Dead_Bush_3_Texture_Filename);
    bush_death_animation.textures[3] = loadTexture(Dead_Bush_4_Texture_Filename);
    bush_death_animation.textures[4] = loadTexture(Dead_Bush_5_Texture_Filename);
    outside_level_data.bush_death_animation = bush_death_animation;
    outside_level_data.bush_texture = bush_texture;
    outside_level_data.dead_bush_texture_1 = loadTexture(Dead_Bush_1_Texture_Filename);
    outside_level_data.dead_bush_texture_2 = loadTexture(Dead_Bush_5_Texture_Filename);
    outside_level.level_data = &outside_level_data;

    // Office level initialization
    office_level.background_texture = loadTexture(Office_Level_Background_Texture_Filename);
    office_level.num_pre_character_draw_obstacles = 4;
    office_level.num_post_character_draw_obstacles = 1;
    office_level.pre_character_draw_obstacles[0] = { -100, 0.0, 100, 600, 0, 0, NULL };
    office_level.pre_character_draw_obstacles[1] = { 0.0, 0.0, 800, 100, 0, 0, NULL };
    office_level.pre_character_draw_obstacles[2] = { 800.0, 0.0, 100, 600, 0, 0, NULL };
    office_level.pre_character_draw_obstacles[3] = { 300.0, 100.0, 200, 50, 0, 0, NULL };
    office_level.post_character_draw_obstacles[0] = { 200.0, 100.0, 100, 100, 0, 0, loadTexture(Electric_Cage_1_Texture_Filename) };
    office_level.num_collectibles = 1;
    office_level.collectibles[0] = { 200.0, 100.0, 100, 100, false, loadTexture(Purple_Lamb_Texture_Filename), 0, 0 };
    office_level.x_init = SCREEN_WIDTH / 2 - player.width / 2;
    office_level.y_init = SCREEN_HEIGHT - player.height;
    office_level.num_exits = 2;
    office_level.exits[0] = { 0, SCREEN_HEIGHT + (float)player.height / 2, 800, 100, BOTTOM_KITCHEN_LEVEL_INDEX, SCREEN_WIDTH / 2 - player.width / 2, SCREEN_HEIGHT - player.height};
    office_level.exits[1] = { 300.0, 150.0, 200, 10, COMPUTER_LEVEL_INDEX, SCREEN_WIDTH / 2 - player.width / 2, 200 };
    office_level.init_level = &init_office;
    office_level.update_level = &update_office;
    office_level.pre_character_draw_level = &draw_office;
    office_level.post_character_draw_level = NULL;
    OfficeLevelData office_level_data;
    office_level_data.computer_face_1_texture = loadTexture(Computer_Face_1_Texture_Filename);
    office_level_data.computer_face_2_texture = loadTexture(Computer_Face_2_Texture_Filename);
    office_level_data.face_1_active = true;
    Animation electric_cage_animation;
    electric_cage_animation.animation_counter = 0;
    electric_cage_animation.animation_speed = 15;
    electric_cage_animation.current_texture_index = 0;
    electric_cage_animation.num_textures = 2;
    electric_cage_animation.textures[0] = loadTexture(Electric_Cage_1_Texture_Filename);
    electric_cage_animation.textures[1] = loadTexture(Electric_Cage_2_Texture_Filename);
    office_level_data.electric_cage_animation = electric_cage_animation;
    office_level_data.complete = false;
    office_level.level_data = &office_level_data;
    
    // Computer level initialization
    computer_level.background_texture = loadTexture(Computer_Background_1_Texture_Filename);
    computer_level.num_pre_character_draw_obstacles = 7;
    computer_level.num_post_character_draw_obstacles = 2;
    computer_level.pre_character_draw_obstacles[0] = {50.0, 50.0, 100, 50, 0, 0, loadTexture(Bone_Texture_Filename) };
    computer_level.pre_character_draw_obstacles[1] = {150.0, 50.0, 100, 50, 0, 0, loadTexture(Bone_Texture_Filename) };
    computer_level.pre_character_draw_obstacles[2] = {250.0, 50.0, 100, 50, 0, 0, loadTexture(Bone_Texture_Filename) };
    computer_level.pre_character_draw_obstacles[3] = {350.0, 50.0, 100, 50, 0, 0, loadTexture(Bone_Texture_Filename) };
    computer_level.pre_character_draw_obstacles[4] = {450.0, 50.0, 100, 50, 0, 0, loadTexture(Bone_Texture_Filename) };
    computer_level.pre_character_draw_obstacles[5] = {550.0, 50.0, 100, 50, 0, 0, loadTexture(Bone_Texture_Filename) };
    computer_level.pre_character_draw_obstacles[6] = {650.0, 50.0, 100, 50, 0, 0, loadTexture(Bone_Texture_Filename) };
    computer_level.post_character_draw_obstacles[0] = { SCREEN_WIDTH / 2 - 150 / 2, SCREEN_HEIGHT - 25 * 2, 150, 25, 0, 0, loadTexture(Paddle_Texture_Filename) };
    computer_level.post_character_draw_obstacles[1] = { SCREEN_WIDTH / 2 - 25 / 2, SCREEN_HEIGHT - 25 * 2 - 25, 25, 25, 0, 0, loadTexture(Ball_Texture_Filename) };
    computer_level.x_init = SCREEN_WIDTH / 2 - player.width / 2;
    computer_level.y_init = SCREEN_HEIGHT - player.height;
    computer_level.num_exits = 0;
    computer_level.exits[0] = { -100, -100, 100, 100, OFFICE_LEVEL_INDEX, SCREEN_WIDTH / 2 - player.width / 2, 400 };
    computer_level.init_level = &init_computer;
    computer_level.update_level = &update_computer;
    computer_level.pre_character_draw_level = NULL;
    computer_level.post_character_draw_level = NULL;
    ComputerLevelData computer_level_data;
    computer_level_data.counter = 0;    
    computer_level_data.ball_1_texture = loadTexture(Ball_Texture_Filename);
    computer_level_data.ball_2_texture = loadTexture(Ball_2_Texture_Filename);
    computer_level_data.ball_1_texture_active = true;
    computer_level_data.computer_face_1_texture = loadTexture(Computer_Background_1_Texture_Filename);
    computer_level_data.computer_face_2_texture = loadTexture(Computer_Background_2_Texture_Filename);
    computer_level_data.computer_face_3_texture = loadTexture(Computer_Background_3_Texture_Filename);
    computer_level_data.face_1_active = true;
    computer_level_data.ball_velocity_x = 0;
    computer_level_data.ball_velocity_y = 0;
    computer_level_data.bricks_remaining = 7;
    computer_level_data.computer_pain_counter = 0;
    computer_level_data.complete = false;
    computer_level.level_data = &computer_level_data;

    // Bedroom level initialization
    bedroom_level.background_texture = loadTexture(Bedroom_Level_Background_Texture_Filename);
    bedroom_level.num_pre_character_draw_obstacles = 5;
    bedroom_level.num_post_character_draw_obstacles = 1;
    bedroom_level.pre_character_draw_obstacles[0] = {-100, 0.0, 100, 600, 0, 0, NULL };
    bedroom_level.pre_character_draw_obstacles[1] = {0.0, 0.0, 800, 200, 0, 0, NULL };
    bedroom_level.pre_character_draw_obstacles[2] = {800.0, 0.0, 100, 600, 0, 0, NULL };
    bedroom_level.pre_character_draw_obstacles[3] = {200.0, 200.0, 140, 200, 0, 0, NULL };
    bedroom_level.pre_character_draw_obstacles[4] = {460.0, 200.0, 140, 200, 0, 0, NULL };
    bedroom_level.post_character_draw_obstacles[0] = {200.0, 201.0, 0, 0, 0, 0, loadTexture(Bed_Maze_Entrance_Texture_Filename) };
    bedroom_level.x_init = SCREEN_WIDTH / 2 - player.width / 2;
    bedroom_level.y_init = SCREEN_HEIGHT - player.height;
    bedroom_level.num_exits = 2;
    bedroom_level.exits[0] = { 0, SCREEN_HEIGHT + (float)player.height / 2, 800, 100, HALLWAY_LEVEL_INDEX, SCREEN_WIDTH / 2 - player.width / 2, SCREEN_HEIGHT - player.height};
    bedroom_level.exits[1] = { SCREEN_WIDTH / 2 - (float)player.width / 2, 200, 100, 100, BEDROOM_MAZE_1_LEVEL_INDEX, SCREEN_WIDTH / 2 - player.width / 2, 400};
    bedroom_level.init_level = &init_bedroom;
    bedroom_level.update_level = NULL;
    bedroom_level.pre_character_draw_level = NULL;
    bedroom_level.post_character_draw_level = NULL;
    BedroomLevelData bedroom_level_data;
    bedroom_level_data.bed_maze_entrance_closed_texture = loadTexture(Bed_Maze_Entrance_Closed_Texture_Filename);
    bedroom_level.level_data = &bedroom_level_data;
    
    // Bedroom Maze 1 initialization
    bedroom_maze_1_level.background_texture = loadTexture(Bedroom_Level_Maze_1_Background_Texture_Filename);
    bedroom_maze_1_level.num_pre_character_draw_obstacles = 12;
    bedroom_maze_1_level.pre_character_draw_obstacles[0] = {-100, 0.0, 100, 600, 0, 0, NULL };
    bedroom_maze_1_level.pre_character_draw_obstacles[1] = {0.0, -100.0, 340, 100, 0, 0, NULL };
    bedroom_maze_1_level.pre_character_draw_obstacles[2] = {460.0, -100.0, 340, 100, 0, 0, NULL };
    bedroom_maze_1_level.pre_character_draw_obstacles[3] = {800.0, 0.0, 100, 600, 0, 0, NULL };
    bedroom_maze_1_level.pre_character_draw_obstacles[4] = {0.0, 600.0, 340, 100, 0, 0, NULL };
    bedroom_maze_1_level.pre_character_draw_obstacles[5] = {460.0, 600.0, 340, 100, 0, 0, NULL };
    bedroom_maze_1_level.pre_character_draw_obstacles[6] = {0.0, 0.0, 100, 330, 0, 0, NULL };
    bedroom_maze_1_level.pre_character_draw_obstacles[7] = {0.0, 450.0, 340, 150, 0, 0, NULL };
    bedroom_maze_1_level.pre_character_draw_obstacles[8] = {220.0, 0.0, 120, 330, 0, 0, NULL };
    bedroom_maze_1_level.pre_character_draw_obstacles[9] = {340.0, 170.0, 330, 110, 0, 0, NULL };
    bedroom_maze_1_level.pre_character_draw_obstacles[10] = {460.0, 0.0, 210, 50, 0, 0, NULL };
    bedroom_maze_1_level.pre_character_draw_obstacles[11] = {460.0, 400.0, 210, 200, 0, 0, NULL };
    bedroom_maze_1_level.x_init = SCREEN_WIDTH / 2 - player.width / 2;
    bedroom_maze_1_level.y_init = SCREEN_HEIGHT - player.height;
    bedroom_maze_1_level.num_exits = 2;
    bedroom_maze_1_level.exits[0] = { 0, SCREEN_HEIGHT + (float)player.height / 2, 800, 100, BEDROOM_LEVEL_INDEX, SCREEN_WIDTH / 2 - player.width / 2, SCREEN_HEIGHT - player.height};
    bedroom_maze_1_level.exits[1] = { 0, -100, 800, 100, BEDROOM_MAZE_2_LEVEL_INDEX, SCREEN_WIDTH / 2 - player.width / 2, 0 };
    bedroom_maze_1_level.init_level = NULL;
    bedroom_maze_1_level.update_level = &update_bed_maze;
    bedroom_maze_1_level.pre_character_draw_level = NULL;
    bedroom_maze_1_level.post_character_draw_level = &draw_bed_maze;
    BedroomMazeLevelData bedroom_maze_level_data;
    bedroom_maze_level_data.spolight_texture = loadTexture(Bed_Maze_Spotlight_Texture_Fileanme);
    bedroom_maze_level_data.spotlight_pos_x = 0;
    bedroom_maze_level_data.spotlight_pos_y = 0;
    bedroom_maze_1_level.level_data = &bedroom_maze_level_data;

    // Bedroom Maze 2 initialization
    bedroom_maze_2_level.background_texture = loadTexture(Bedroom_Level_Maze_2_Background_Texture_Filename);
    bedroom_maze_2_level.num_collectibles = 1;
    bedroom_maze_2_level.collectibles[0] = { 170.0, 380.0, 100, 100, false, loadTexture(Purple_Lamb_Texture_Filename), -50, 0};
    bedroom_maze_2_level.num_pre_character_draw_obstacles = 12;
    bedroom_maze_2_level.pre_character_draw_obstacles[0] = { -100, 0.0, 100, 600, 0, 0, NULL };
    bedroom_maze_2_level.pre_character_draw_obstacles[1] = { 0.0, -100.0, 800, 100, 0, 0, NULL };
    bedroom_maze_2_level.pre_character_draw_obstacles[2] = { 800.0, 0.0, 100, 600, 0, 0, NULL };
    bedroom_maze_2_level.pre_character_draw_obstacles[3] = { 0.0, 600.0, 340, 100, 0, 0, NULL };
    bedroom_maze_2_level.pre_character_draw_obstacles[4] = { 460.0, 600.0, 340, 100, 0, 0, NULL };
    bedroom_maze_2_level.pre_character_draw_obstacles[5] = { 120.0, 120.0, 100, 40, 0, 0, NULL };
    bedroom_maze_2_level.pre_character_draw_obstacles[6] = { 120.0, 280.0, 100, 100, 0, 0, NULL };
    bedroom_maze_2_level.pre_character_draw_obstacles[7] = { 0.0, 500.0, 220, 100, 0, 0, NULL };
    bedroom_maze_2_level.pre_character_draw_obstacles[8] = { 220.0, 330.0, 120, 270, 0, 0, NULL };
    bedroom_maze_2_level.pre_character_draw_obstacles[9] = { 340.0, 120.0, 100, 370, 0, 0, NULL };
    bedroom_maze_2_level.pre_character_draw_obstacles[10] = { 440.0, 120.0, 240, 120, 0, 0, NULL };
    bedroom_maze_2_level.pre_character_draw_obstacles[11] = { 440.0, 360.0, 240, 130, 0, 0, NULL };
    bedroom_maze_2_level.pre_character_draw_obstacles[12] = { 10.0, 280.0, 120, 100, 0, 0, loadTexture(Bed_Maze_Block_Texture_Filename) };
    bedroom_maze_2_level.x_init = SCREEN_WIDTH / 2 - player.width / 2;
    bedroom_maze_2_level.y_init = SCREEN_HEIGHT - player.height;
    bedroom_maze_2_level.num_exits = 1;
    bedroom_maze_2_level.exits[0] = { 0, SCREEN_HEIGHT + (float)player.height / 2, 800, 100, BEDROOM_MAZE_1_LEVEL_INDEX, SCREEN_WIDTH / 2 - player.width / 2, SCREEN_HEIGHT - player.height};
    bedroom_maze_2_level.exits[1] = { -150, 380, 100, 100, BEDROOM_LEVEL_INDEX, 0, 0 };
    bedroom_maze_2_level.init_level = NULL;
    bedroom_maze_2_level.update_level = &update_bed_maze;
    bedroom_maze_2_level.pre_character_draw_level = NULL;
    bedroom_maze_2_level.post_character_draw_level = &draw_bed_maze;
    bedroom_maze_2_level.level_data = &bedroom_maze_level_data;

    // Door Level 
    door_level.background_texture = loadTexture(Door_1_Texture_Filename);
    door_level.num_exits = 0;
    door_level.exits[0] = { 800, 600, 100, 100, HALLWAY_LEVEL_INDEX, 800, 600 };
    door_level.x_init = 800;
    door_level.y_init = 600;    
    door_level.init_level = &init_door;
    door_level.update_level = &update_door;
    door_level.pre_character_draw_level = NULL;
    door_level.post_character_draw_level = NULL;
    DoorLevelData door_level_data;
    door_level_data.background_1 = loadTexture(Door_1_Texture_Filename);
    door_level_data.background_2 = loadTexture(Door_2_Texture_Filename);
    door_level_data.background_1_active = true;
    door_level_data.counter = 0;
    door_level.level_data = &door_level_data;

    int current_level_index = 0;
    Level levels[MAX_NUM_LEVELS];
    levels[0] = kitchen_level;
    levels[1] = bottom_kitchen_level;
    levels[2] = living_room_level;
    levels[3] = hallway_level;
    levels[4] = outside_level;
    levels[5] = office_level;
    levels[6] = bedroom_level;
    levels[7] = bedroom_maze_1_level;
    levels[8] = bedroom_maze_2_level;
    levels[9] = computer_level;
    levels[10] = door_level;

    player.x = levels[current_level_index].x_init;
    player.y = levels[current_level_index].y_init;

    player.animations[0].num_textures = 1;
    player.animations[0].textures[0] = loadTexture(Standing_Texture_Filename);

    player.animations[1].num_textures = 2;
    player.animations[1].animation_speed = 10;
    player.animations[1].textures[0] = loadTexture(Walking_Texture_Filename_1);
    player.animations[1].textures[1] = loadTexture(Walking_Texture_Filename_2);

    player.animations[2].num_textures = 1;
    player.animations[2].textures[0] = loadTexture(Idle_Texture_Filename);

    player.animations[3].num_textures = 2;
    player.animations[3].animation_speed = 10;
    player.animations[3].textures[0] = loadTexture(Peeing_Texture_Filename_1);
    player.animations[3].textures[1] = loadTexture(Peeing_Texture_Filename_2);

    player.animation_counter = 0;

    player.facing_right = true;

    key_map[SDL_SCANCODE_UP] = false;
    key_map[SDL_SCANCODE_DOWN] = false;
    key_map[SDL_SCANCODE_LEFT] = false;
    key_map[SDL_SCANCODE_RIGHT] = false;
    key_map[SDL_SCANCODE_SPACE] = false;

    int num_frames = 0;
    int prev_time = 0;
    level_transition_counter = 0;

    while (isRunning)
    {
        unsigned int current_time = SDL_GetTicks();
        float delta_time = (current_time - prev_time) * 1e-3;
        prev_time = current_time;

        if(level_transition_counter > 0)
        {
            // Clear scene
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            level_transition_counter--;
        }
        else
        {
            handleInput();
            if(player.animation_counter == 0)
            {
                updatePlayer(player);
            }
            else
            {
                player.animation_counter--;
            }

            Animation* animation = &player.animations[player.current_animation_index];
            player.current_texture = updateAnimation(animation);

            Level& level = levels[current_level_index];
            if(level.update_level != NULL)
            {
                level.update_level(&level, level.level_data, &player, key_map, delta_time);
            }

            // Check collision
            float x_min_time = 1;
            float y_min_time = 1;

            // X Axis
            if(player.x_vel != 0)
            {
                for(int i = 0; i < level.num_pre_character_draw_obstacles; i++)
                {
                    bool x_collision = checkXCollision(player, level.pre_character_draw_obstacles[i], delta_time);
                    if(x_collision)
                    {
                        float dx = calculateXDistance(player, level.pre_character_draw_obstacles[i]);
                        float x_time = abs(dx / player.x_vel);
                        if(x_time < x_min_time)
                        {
                            x_min_time = x_time;
                        }
                    }
                }
                for(int i = 0; i < level.num_post_character_draw_obstacles; i++)
                {
                    bool x_collision = checkXCollision(player, level.post_character_draw_obstacles[i], delta_time);
                    if(x_collision)
                    {
                        float dx = calculateXDistance(player, level.post_character_draw_obstacles[i]);
                        float x_time = abs(dx / player.x_vel);
                        if(x_time < x_min_time)
                        {
                            x_min_time = x_time;
                        }
                    }
                }

                player.x += player.x_vel * delta_time * x_min_time;
            }
            if(player.y_vel != 0)
            {
                for(int i = 0; i < level.num_pre_character_draw_obstacles; i++)
                {
                    bool y_collision = checkYCollision(player, level.pre_character_draw_obstacles[i], delta_time);
                    if(y_collision)
                    {
                        float dy = calculateYDistance(player, level.pre_character_draw_obstacles[i]);
                        float y_time = abs(dy / player.y_vel);
                        if(y_time < y_min_time)
                        {
                            y_min_time = y_time;
                        }
                    }
                }
                for(int i = 0; i < level.num_post_character_draw_obstacles; i++)
                {
                    bool y_collision = checkYCollision(player, level.post_character_draw_obstacles[i], delta_time);
                    if(y_collision)
                    {
                        float dy = calculateYDistance(player, level.post_character_draw_obstacles[i]);
                        float y_time = abs(dy / player.y_vel);
                        if(y_time < y_min_time)
                        {
                            y_min_time = y_time;
                        }
                    }
                }

                player.y += player.y_vel * delta_time * y_min_time;
            }

            // Check collectible collision
            for(int i = 0; i < level.num_collectibles; i++)
            {
                if(!level.collectibles[i].collected)
                {
                    bool collision = checkCollision(player, level.collectibles[i]);
                    if(collision)
                    {
                        level.collectibles[i].collected = true;
                        level.num_collectibles -= 1;
                        switch(current_level_index)
                        {
                            case OUTSIDE_LEVEL_INDEX:
                                player.collected[0] = true;
                                break;
                            case OFFICE_LEVEL_INDEX:
                                player.collected[1] = true;
                                break;
                            case BEDROOM_MAZE_2_LEVEL_INDEX:
                                player.collected[2] = true;
                                break;
                            case LIVING_ROOM_LEVEL_INDEX:
                                player.collected[3] = true;
                                break;
                        }
                    }
                }
            }

            // Check for collision with exits
            for(int i = 0; i < level.num_exits; i++)
            {
                bool collision = checkCollision(player, level.exits[i]);
                if(collision)
                {
                    current_level_index = level.exits[i].next_level_index;
                    level.x_init = level.exits[i].x_reentry;
                    level.y_init = level.exits[i].y_reentry;
                    player.x = levels[level.exits[i].next_level_index].x_init;
                    player.y = levels[level.exits[i].next_level_index].y_init;
                    if(player.x < 0)
                    {
                        player.x = 0;
                    }
                    else if(player.x + player.width > SCREEN_WIDTH)
                    {
                        player.x = SCREEN_WIDTH - player.width;
                    }
                    if(player.y < 0)
                    {
                        player.y = 0;
                    }
                    else if(player.y + player.height > SCREEN_HEIGHT)
                    {
                        player.y = SCREEN_HEIGHT - player.height;
                    }

                    level_transition_counter = MAX_NUM_LEVEL_TRANSITION_FRAMES;

                    if(levels[level.exits[i].next_level_index].init_level != NULL)
                    {
                        levels[level.exits[i].next_level_index].init_level(&levels[level.exits[i].next_level_index], levels[level.exits[i].next_level_index].level_data, &player);
                    }
                    break;
                }
            }

            if(level_transition_counter > 0)
            {
                // Clear scene
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
            }
            else
            {
                // Clear scene
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderClear(renderer);

                // Draw level background if available
                if(level.background_texture != NULL)
                {
                    blit(level.background_texture, 0, 0, 0, 0, renderer);
                }

                if(level.pre_character_draw_level != NULL)
                {
                    level.pre_character_draw_level(&level, level.level_data, renderer);
                }

                // Draw obstacles before character
                for(int i = 0; i < level.num_pre_character_draw_obstacles; i++)
                {
                    Obstacle o = level.pre_character_draw_obstacles[i];
                    if(o.texture != NULL)
                    {
                        blit(o.texture, o.x, o.y, o.texture_x_offset, o.texture_y_offset, renderer);
                    }
                }

                // Draw collectibles before character
                for(int i = 0; i < level.num_collectibles; i++)
                {
                    if(!level.collectibles[i].collected)
                    {
                        blit(level.collectibles[i].texture, level.collectibles[i].x, level.collectibles[i].y, level.collectibles[i].texture_x_offset, level.collectibles[i].texture_y_offset, renderer);
                    }
                }

                // Draw character
                blit(player.current_texture, player.x, player.y, player.facing_right, renderer);

                
                if(level.post_character_draw_level != NULL)
                {
                    level.post_character_draw_level(&level, level.level_data, renderer);
                }

                // Draw obstacles after character
                for(int i = 0; i < level.num_post_character_draw_obstacles; i++)
                {
                    Obstacle o = level.post_character_draw_obstacles[i];
                    if(o.texture != NULL)
                    {
                        blit(o.texture, o.x, o.y, o.texture_x_offset, o.texture_y_offset, renderer);
                    }
                }
            }
        }

        // Present renderer
        SDL_RenderPresent(renderer);

        // Calculate time to sleep and sleep if necessary
        int next_frame_time = num_frames++ * MS_PER_FRAME;
        current_time = SDL_GetTicks();
        if(current_time < next_frame_time)
        {
            int ms_to_sleep = next_frame_time - current_time;
            SDL_Delay(ms_to_sleep);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

SDL_Texture *loadTexture(char *filename)
{
    SDL_Texture *texture;

    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

    texture = IMG_LoadTexture(renderer, filename);

    return texture;
}

void handleInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                isRunning = false;
            }
            else if (!event.key.repeat)
            {
                key_map[event.key.keysym.scancode] = true;
            }
            break;

        case SDL_KEYUP:
            if (!event.key.repeat)
            {
                key_map[event.key.keysym.scancode] = false;
            }
            break;
        }
    }
}

void updatePlayer(Entity& p)
{
    if((!key_map[SDL_SCANCODE_UP] && !key_map[SDL_SCANCODE_DOWN]) || (key_map[SDL_SCANCODE_UP] && key_map[SDL_SCANCODE_DOWN]))
    {
        p.y_vel = 0;
    }
    else if(key_map[SDL_SCANCODE_UP])
    {
        p.y_vel = -p.speed;
    }
    else if(key_map[SDL_SCANCODE_DOWN])
    {
        p.y_vel = p.speed;
    }
    if((!key_map[SDL_SCANCODE_LEFT] && !key_map[SDL_SCANCODE_RIGHT]) || (key_map[SDL_SCANCODE_LEFT] && key_map[SDL_SCANCODE_RIGHT]))
    {
        p.x_vel = 0;
    }
    else if(key_map[SDL_SCANCODE_LEFT])
    {
        p.x_vel = -p.speed;
        p.facing_right = false;
    }
    else if(key_map[SDL_SCANCODE_RIGHT])
    {
        p.x_vel = p.speed;
        p.facing_right = true;
    }

    if(p.x_vel != 0 || p.y_vel != 0)
    {
        p.current_animation_index = 1;
        p.idle_counter = 0;
    }
    else if(p.idle_counter < p.idle_threshold)
    {
        p.current_animation_index = 0;
        p.idle_counter++;
    }
    else
    {
        p.current_animation_index = 2;
    }
}
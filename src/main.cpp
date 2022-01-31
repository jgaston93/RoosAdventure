#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <map>
#include <limits>

#include "CollisionCheckingFunctions.hpp"
#include "GameConstants.hpp"
#include "GameDataStructures.hpp"

std::string Window_Title = "Roo's Adventure";

char Standing_Texture_Filename[] = "assets/roo_standing.png\0";
char Walking_Texture_Filename_1[] = "assets/roo_walking_1.png\0";
char Walking_Texture_Filename_2[] = "assets/roo_walking_2.png\0";
char Peeing_Texture_Filename_1[] = "assets/roo_peeing_1.png\0";
char Peeing_Texture_Filename_2[] = "assets/roo_peeing_2.png\0";
char Idle_Texture_Filename[] = "assets/roo_sitting.png\0";
char Kitchen_Level_Background_Texture_Filename[] = "assets/kitchen_background.png\0";
char Bottom_Kitchen_Level_Background_Texture_Filename[] = "assets/bottom_kitchen_background.png\0";
char Stairs_Texture_Filename[] = "assets/stairs.png\0";
char Middle_Counter_Texture_Filename[] = "assets/middle_counter.png\0";
char White_Lamb_Texture_Filename[] = "assets/white_lamb.png\0";
char Purple_Lamb_Texture_Filename[] = "assets/purple_lamb.png\0";
char Living_Room_Level_Background_Texture_Filename[] = "assets/living_room_level_background.png\0";
char Coffee_Table_Texture_Filename[] = "assets/coffee_table.png\0";
char Hallway_Level_Background_Texture_Filename[] = "assets/hallway_background.png\0";
char Hallway_Stairs_Texture_Filename[] = "assets/hallway_stairs.png\0";
char Outside_Level_Background_Texture_Filename[] = "assets/outside_background.png\0";
char Office_Level_Background_Texture_Filename[] = "assets/office_background.png\0";
char Bedroom_Level_Background_Texture_Filename[] = "assets/bedroom_background.png\0";
char Loft_Level_Background_Texture_Filename[] = "assets/loft_background.png\0";
char Bush_Texture_Filename[] = "assets/bush.png\0";
char Angry_Bush_1_Texture_Filename[] = "assets/angry_bush_1.png\0";
char Angry_Bush_2_Texture_Filename[] = "assets/angry_bush_2.png\0";
char Dead_Bush_1_Texture_Filename[] = "assets/dead_bush_1.png\0";
char Dead_Bush_2_Texture_Filename[] = "assets/dead_bush_2.png\0";
char Vaccuum_1_Texture_Filename[] = "assets/vaccuum_1.png\0";
char Vaccuum_2_Texture_Filename[] = "assets/vaccuum_2.png\0";
char Vaccuum_3_Texture_Filename[] = "assets/vaccuum_3.png\0";
char Vaccuum_4_Texture_Filename[] = "assets/vaccuum_4.png\0";
char Vaccuum_5_Texture_Filename[] = "assets/vaccuum_5.png\0";
char Vaccuum_6_Texture_Filename[] = "assets/vaccuum_6.png\0";

SDL_Renderer *renderer;
SDL_Window* window;

std::map<SDL_Scancode, bool> key_map;
int level_transition_counter;

bool isRunning = true;

SDL_Texture* loadTexture(char* filename);
void handleInput();
void updatePlayer(Entity& p);
void blit(SDL_Texture* texture, int x, int y, int x_offset, int y_offset);
void blit(SDL_Texture* texture, int x, int y, bool facing_right);
SDL_Texture* updateAnimation(Animation* animation);

// Level update functions
void init_outside(Level* level, void* data);
void update_outside(Level* level, void* data, Entity* player, std::map<SDL_Scancode, bool> key_map, float delta_time);

void init_living_room(Level* level, void* data);
void update_living_room(Level* level, void* data, Entity* player, std::map<SDL_Scancode, bool> key_map, float delta_time);
void draw_living_room(Level* level, void* data, SDL_Renderer* renderer);

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
    Level bedroom_level;
    Level loft_level;
    Entity player;
    memset(&kitchen_level, 0, sizeof(Level));
    memset(&bottom_kitchen_level, 0, sizeof(Level));
    memset(&living_room_level, 0, sizeof(Level));
    memset(&hallway_level, 0, sizeof(Level));
    memset(&outside_level, 0, sizeof(Level));
    memset(&office_level, 0, sizeof(Level));
    memset(&bedroom_level, 0, sizeof(Level));
    memset(&loft_level, 0, sizeof(Level));
    memset(&player, 0, sizeof(Entity));

    player.width = 100;
    player.height = 100;
    player.speed = 200;
    player.idle_threshold = 200;

    // Kitchen level initialization
    kitchen_level.background_texture = loadTexture(Kitchen_Level_Background_Texture_Filename);
    kitchen_level.num_pre_character_draw_obstacles = 3;
    kitchen_level.num_post_character_draw_obstacles = 1;
    kitchen_level.pre_character_draw_obstacles[0] = { 0.0, 0.0, 100, 350, 0, 0, NULL };
    kitchen_level.pre_character_draw_obstacles[1] = { 100.0, 0.0, 700, 200, 0, 0, NULL };
    kitchen_level.pre_character_draw_obstacles[2] = { 800.0, 0.0, 100, 600, 0, 0, NULL };
    kitchen_level.post_character_draw_obstacles[0] = { 300.0, 310.0, 300, 180, 0, -100, loadTexture(Middle_Counter_Texture_Filename) };
    kitchen_level.x_init = 0;
    kitchen_level.y_init = 400;
    kitchen_level.num_collectibles = 0;
    kitchen_level.collectibles[0] = { 400, SCREEN_HEIGHT - 100, 100, 100, false, loadTexture(White_Lamb_Texture_Filename), 0, 0};
    kitchen_level.num_exits = 2;
    kitchen_level.exits[0] = { -(100 + (float)player.width / 2), 400, 100, 200, 2 , 0, 400};
    kitchen_level.exits[1] = { 0.0, (float)SCREEN_HEIGHT + player.height / 2, 800, 100, 1, SCREEN_WIDTH / 2 - player.width / 2, SCREEN_HEIGHT - player.height };
    kitchen_level.init_level = NULL;
    kitchen_level.update_level = NULL;
    kitchen_level.draw_level = NULL;
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
    bottom_kitchen_level.exits[0] = { 0.0, SCREEN_HEIGHT + (float)player.height / 2, 800, 100, 0, SCREEN_WIDTH / 2 - player.width / 2, SCREEN_HEIGHT - player.height };
    bottom_kitchen_level.exits[1] = { SCREEN_WIDTH + (float)player.width / 2, 100, 100, 110, 3, SCREEN_WIDTH - player.width, 100 };
    bottom_kitchen_level.exits[2] = { 90.0, -((float)player.height + (float)player.height / 2), 120, 100, 4, 100, 0 };
    bottom_kitchen_level.exits[3] = { 490.0, -((float)player.height + (float)player.height / 2), 120, 100, 5, 500, 0 };
    bottom_kitchen_level.init_level = NULL;
    bottom_kitchen_level.update_level = NULL;
    bottom_kitchen_level.draw_level = NULL;
    bottom_kitchen_level.level_data = NULL;

    // Living room level initialization
    living_room_level.background_texture = loadTexture(Living_Room_Level_Background_Texture_Filename);
    living_room_level.num_pre_character_draw_obstacles = 5;
    living_room_level.num_post_character_draw_obstacles = 2;
    living_room_level.pre_character_draw_obstacles[0] = {0.0, 0.0, 615, 335, 0, 0, NULL };
    living_room_level.pre_character_draw_obstacles[1] = {615.0, 0.0, 185, 50, 0, 0, NULL };
    living_room_level.pre_character_draw_obstacles[2] = {0.0, 600.0, 800, 100, 0, 0, NULL };
    living_room_level.pre_character_draw_obstacles[3] = {800.0, 0.0, 100, 600, 0, 0, NULL };
    living_room_level.post_character_draw_obstacles[0] = {620.0, 115.0, 100, 20, 0, -80, loadTexture(Vaccuum_1_Texture_Filename) };
    living_room_level.post_character_draw_obstacles[1] = {175, 440, 400, 55, 0, -55, loadTexture(Coffee_Table_Texture_Filename) };
    living_room_level.x_init = 0;
    living_room_level.y_init = 400;
    living_room_level.num_exits = 1;
    living_room_level.exits[0] = { -(100 + (float)player.width / 2), 335, 100, 265, 0 , 0, 400};
    living_room_level.init_level = &init_living_room;
    living_room_level.update_level = &update_living_room;
    living_room_level.draw_level = &draw_living_room;
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
    hallway_level.num_exits = 2;
    hallway_level.exits[0] = { 300, 125, 100, 175, 1 , 450, 125};
    hallway_level.exits[1] = { 590, -150, 120, 100, 6 , 600, 0};
    hallway_level.init_level = NULL;
    hallway_level.update_level = NULL;
    hallway_level.draw_level = NULL;
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
    outside_level.pre_character_draw_obstacles[3] = {105.0, 225.0, 80, 2, 0, -20, bush_texture };
    outside_level.pre_character_draw_obstacles[4] = {605.0, 225.0, 80, 2, 0, -20, bush_texture };
    outside_level.pre_character_draw_obstacles[5] = {355.0, 325.0, 80, 2, 0, -20, bush_texture };
    outside_level.pre_character_draw_obstacles[6] = {105.0, 425.0, 80, 2, 0, -20, bush_texture };
    outside_level.pre_character_draw_obstacles[7] = {605.0, 425.0, 80, 2, 0, -20, bush_texture };
    outside_level.x_init = SCREEN_WIDTH / 2 - player.width / 2;
    outside_level.y_init = SCREEN_HEIGHT - player.height;
    outside_level.num_exits = 1;
    outside_level.exits[0] = { 0, SCREEN_HEIGHT + (float)player.height / 2, 800, 100, 1 , SCREEN_WIDTH / 2 - player.width / 2, SCREEN_HEIGHT - player.height};

    outside_level.init_level = &init_outside;
    outside_level.update_level = &update_outside;
    outside_level.draw_level = NULL;
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
    outside_level_data.bush_texture = bush_texture;
    outside_level_data.dead_bush_texture_1 = loadTexture(Dead_Bush_1_Texture_Filename);
    outside_level_data.dead_bush_texture_2 = loadTexture(Dead_Bush_2_Texture_Filename);
    outside_level.level_data = &outside_level_data;

    // Office level initialization
    office_level.background_texture = loadTexture(Office_Level_Background_Texture_Filename);
    office_level.num_pre_character_draw_obstacles = 4;
    office_level.pre_character_draw_obstacles[0] = {-100, 0.0, 100, 600, 0, 0, NULL };
    office_level.pre_character_draw_obstacles[1] = {0.0, 0.0, 800, 100, 0, 0, NULL };
    office_level.pre_character_draw_obstacles[2] = {800.0, 0.0, 100, 600, 0, 0, NULL };
    office_level.pre_character_draw_obstacles[3] = {300.0, 100.0, 200, 100, 0, 0, NULL };
    office_level.x_init = SCREEN_WIDTH / 2 - player.width / 2;
    office_level.y_init = SCREEN_HEIGHT - player.height;
    office_level.num_exits = 1;
    office_level.exits[0] = { 0, SCREEN_HEIGHT + (float)player.height / 2, 800, 100, 1 , SCREEN_WIDTH / 2 - player.width / 2, SCREEN_HEIGHT - player.height};
    office_level.init_level = NULL;
    office_level.update_level = NULL;
    office_level.draw_level = NULL;
    office_level.level_data = NULL;

    // Bedroom level initialization
    bedroom_level.background_texture = loadTexture(Bedroom_Level_Background_Texture_Filename);
    bedroom_level.num_pre_character_draw_obstacles = 4;
    bedroom_level.pre_character_draw_obstacles[0] = {-100, 0.0, 100, 600, 0, 0, NULL };
    bedroom_level.pre_character_draw_obstacles[1] = {0.0, 0.0, 800, 200, 0, 0, NULL };
    bedroom_level.pre_character_draw_obstacles[2] = {800.0, 0.0, 100, 600, 0, 0, NULL };
    bedroom_level.pre_character_draw_obstacles[3] = {200.0, 200.0, 400, 200, 0, 0, NULL };
    bedroom_level.x_init = SCREEN_WIDTH / 2 - player.width / 2;
    bedroom_level.y_init = SCREEN_HEIGHT - player.height;
    bedroom_level.num_exits = 1;
    bedroom_level.exits[0] = { 0, SCREEN_HEIGHT + (float)player.height / 2, 800, 100, 3, SCREEN_WIDTH / 2 - player.width / 2, SCREEN_HEIGHT - player.height};
    bedroom_level.init_level = NULL;
    bedroom_level.update_level = NULL;
    bedroom_level.draw_level = NULL;
    bedroom_level.level_data = NULL;

    int current_level_index = 0;
    Level levels[MAX_NUM_LEVELS];
    levels[0] = kitchen_level;
    levels[1] = bottom_kitchen_level;
    levels[2] = living_room_level;
    levels[3] = hallway_level;
    levels[4] = outside_level;
    levels[5] = office_level;
    levels[6] = bedroom_level;

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
                        levels[level.exits[i].next_level_index].init_level(&levels[level.exits[i].next_level_index], levels[level.exits[i].next_level_index].level_data);
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
                    blit(level.background_texture, 0, 0, 0, 0);
                }

                if(level.draw_level != NULL)
                {
                    level.draw_level(&level, level.level_data, renderer);
                }

                // Draw obstacles before character
                for(int i = 0; i < level.num_pre_character_draw_obstacles; i++)
                {
                    Obstacle o = level.pre_character_draw_obstacles[i];
                    if(o.texture != NULL)
                    {
                        blit(o.texture, o.x, o.y, o.texture_x_offset, o.texture_y_offset);
                    }
                }

                // Draw collectibles before character
                for(int i = 0; i < level.num_collectibles; i++)
                {
                    if(!level.collectibles[i].collected)
                    {
                        blit(level.collectibles[i].texture, level.collectibles[i].x, level.collectibles[i].y, level.collectibles[i].texture_x_offset, level.collectibles[i].texture_y_offset);
                    }
                }

                // Draw character
                blit(player.current_texture, player.x, player.y, player.facing_right);

                // Draw obstacles after character
                for(int i = 0; i < level.num_post_character_draw_obstacles; i++)
                {
                    Obstacle o = level.post_character_draw_obstacles[i];
                    if(o.texture != NULL)
                    {
                        blit(o.texture, o.x, o.y, o.texture_x_offset, o.texture_y_offset);
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

void init_outside(Level* level, void* data)
{
    OutsideLevelData* outside_level_data = (OutsideLevelData*)data;

    // level isn't complete so reset
    if(!outside_level_data->complete)
    {
        outside_level_data->counter = 0;
        outside_level_data->bush_index = 0;
    }
    // level is complete
    else
    {
        outside_level_data->counter = 60;
        for(int i = 0; i < outside_level_data->num_bushes; i++)
        {
            if(i != 2) // Bush number 2 has the lamb under it
            {
                level->pre_character_draw_obstacles[i + 3].texture = outside_level_data->dead_bush_texture_2;
            }
        }
    }
}

void update_outside(Level* level, void* data, Entity* player, std::map<SDL_Scancode, bool> key_map, float delta_time)
{
    OutsideLevelData* outside_level_data = (OutsideLevelData*)data;

    // Bushes bounce when entering level
    if(outside_level_data->counter < 60)
    {
        // At last timestep set all bush textures to ordinary bush texture
        if(outside_level_data->counter == 59)
        {
            for(int i = 0; i < outside_level_data->num_bushes; i++)
            {
                level->pre_character_draw_obstacles[i + 3].texture = outside_level_data->bush_texture;
            }
        }
        // Bounce animation
        else
        {
            Animation* animation = &outside_level_data->bush_animation;

            for(int i = 0; i < outside_level_data->num_bushes; i++)
            {
                level->pre_character_draw_obstacles[i + 3].texture = animation->textures[animation->current_texture_index];
            }

            if((animation->animation_counter++ % animation->animation_speed) == 0)
            {
                animation->current_texture_index = (animation->current_texture_index + 1) % animation->num_textures;
            }
        }
    }
    // After level initialization perform level logic if level is not complete
    else if(!outside_level_data->complete)
    {
        // Player has selected bush
        if(player->current_animation_index == 3)
        {
            // Logic while player animation is happening
            if(player->animation_counter > 1)
            {
                if(outside_level_data->selected_bush == outside_level_data->bush_order[outside_level_data->bush_index])
                {
                    level->pre_character_draw_obstacles[outside_level_data->bush_order[outside_level_data->bush_index] + 3].texture = outside_level_data->dead_bush_texture_1;
                }
                else
                {
                    Animation* animation = &outside_level_data->bush_animation;

                    level->pre_character_draw_obstacles[outside_level_data->bush_order[outside_level_data->bush_index] + 3].texture = animation->textures[animation->current_texture_index];

                    if((animation->animation_counter++ % animation->animation_speed) == 0)
                    {
                        animation->current_texture_index = (animation->current_texture_index + 1) % animation->num_textures;
                    }
                }
            }
            // Player animation has finished so check if selected bush was correct
            else
            {
                // Selected bush is correct
                if(outside_level_data->selected_bush == outside_level_data->bush_order[outside_level_data->bush_index])
                {
                    outside_level_data->bush_index++;
                    // Player has selected bushes in correct order
                    if(outside_level_data->bush_index == outside_level_data->num_bushes)
                    {
                        outside_level_data->complete = true;
                        level->pre_character_draw_obstacles[5] = { 0, 0, 0, 0, 0, 0, NULL };
                        level->num_collectibles = 1;
                    }
                }
                // Selected bush is incorrect
                else
                {
                    outside_level_data->bush_index = 0;
                    for(int i = 0; i < outside_level_data->num_bushes; i++)
                    {
                        level->pre_character_draw_obstacles[i + 3].texture = outside_level_data->bush_texture;
                    }
                }
                player->current_animation_index = 0;
            }
        }
        // Handle input
        else if(key_map[SDL_SCANCODE_SPACE])
        {
            bool in_range = false;
            for(int i = 0 ; i < outside_level_data->num_bushes; i++)
            {
                // Check if bush has already been selected
                bool check_for_collision = true;
                if(outside_level_data->bush_index > 0)
                {
                    for(int j = 0; j < outside_level_data->bush_index; j++)
                    {
                        if(i == outside_level_data->bush_order[j])
                        {
                            check_for_collision = false;
                            break;
                        }
                    }
                }
                // If bush has not been selected already then check if player is selecting current bush
                if(check_for_collision)
                {
                    Obstacle o = level->pre_character_draw_obstacles[i + 3];
                    o.y += 25;
                    bool collision = checkXYCollision(*player, o, 0.0);
                    if(collision)
                    {
                        player->current_animation_index = 3;
                        player->animation_counter = 60;
                        player->x_vel = 0;
                        player->y_vel = 0;
                        outside_level_data->selected_bush = i;
                        outside_level_data->bush_animation.animation_counter = 0;
                    }
                }
            }
        }
    }
    outside_level_data->counter++;

}


void init_living_room(Level* level, void* data)
{
    LivingRoomLevelData* living_room_level_data = (LivingRoomLevelData*)data;

    if(!living_room_level_data->complete)
    {
        living_room_level_data->counter = 0;
        living_room_level_data->num_vaccuum_cord_points = 0;

        level->post_character_draw_obstacles[0].x = 620.0;
        level->post_character_draw_obstacles[0].y = 115.0;
    }
}

void update_living_room(Level* level, void* data, Entity* player, std::map<SDL_Scancode, bool> key_map, float delta_time)
{
    LivingRoomLevelData* living_room_level_data = (LivingRoomLevelData*)data;

    Obstacle* vaccuum = &level->post_character_draw_obstacles[0];

    if(player->current_animation_index == 3)
    {
            // Logic while player animation is happening
            if(player->animation_counter > 1)
            {
                Animation* animation = &living_room_level_data->vaccuum_short_circuit_animation;
                level->post_character_draw_obstacles[0].texture = updateAnimation(animation);
            }
            // Player animation has finished so check if selected bush was correct
            else
            {
                living_room_level_data->complete = true;
                player->current_animation_index = 0;
                living_room_level_data->vaccuum_final_x_position = vaccuum->x;
                living_room_level_data->vaccuum_final_y_position = vaccuum->y;
            }
    }
    else if(living_room_level_data->complete)
    {
        Animation* animation = &living_room_level_data->vaccuum_smoking_animation;
        level->post_character_draw_obstacles[0].texture = updateAnimation(animation);
    }
    else if(living_room_level_data->num_vaccuum_cord_points < MAX_NUM_VACCUUM_CORD_POINTS)
    {
        float vaccuum_speed = 75;
        float x_vel = 0;
        float y_vel = 0;
        if(vaccuum->x > player->x)
        {
            x_vel = -vaccuum_speed;
        }
        else
        {
            x_vel = vaccuum_speed;
        }
        if(vaccuum->y > player->y)
        {
            y_vel = -vaccuum_speed;
        }
        else
        {
            y_vel = vaccuum_speed;
        }

        // Check collision
        float x_min_time = 1;
        float y_min_time = 1;

        // X Axis
        if(x_vel != 0)
        {
            bool player_collision = checkXYCollision(*player, level->post_character_draw_obstacles[0], delta_time);
            if(player_collision)
            {
                level->init_level(level, living_room_level_data);
                player->x = 0;
                player->y = 400;
                level_transition_counter = MAX_NUM_LEVEL_TRANSITION_FRAMES;
            }

            for(int i = 0; i < level->num_pre_character_draw_obstacles; i++)
            {
                Obstacle* o = &level->pre_character_draw_obstacles[i];
                bool x_collision = checkXCollision(vaccuum->x, vaccuum->y, vaccuum->width, vaccuum->height, o->x, o->y, o->width, o->height, delta_time, x_vel);
                if(x_collision)
                {
                    float dx = calculateXDistance(*vaccuum, *o);
                    float x_time = abs(dx / x_vel);
                    if(x_time < x_min_time)
                    {
                        x_min_time = x_time;
                    }
                }
            }

            Obstacle* o = &level->post_character_draw_obstacles[1];
            bool x_collision = checkXCollision(vaccuum->x, vaccuum->y, vaccuum->width, vaccuum->height, o->x, o->y, o->width, o->height, delta_time, x_vel);
            if(x_collision)
            {
                float dx = calculateXDistance(*vaccuum, *o);
                float x_time = abs(dx / x_vel);
                if(x_time < x_min_time)
                {
                    x_min_time = x_time;
                }
            }

            vaccuum->x += x_vel * delta_time * x_min_time;
        }
        if(y_vel != 0)
        {
            bool player_collision = checkXYCollision(*player, level->post_character_draw_obstacles[0], delta_time);
            if(player_collision)
            {
                level->init_level(level, living_room_level_data);
                player->x = 0;
                player->y = 400;
                level_transition_counter = MAX_NUM_LEVEL_TRANSITION_FRAMES;
            }
            
            for(int i = 0; i < level->num_pre_character_draw_obstacles; i++)
            {
                Obstacle* o = &level->pre_character_draw_obstacles[i];
                bool y_collision = checkYCollision(vaccuum->x, vaccuum->y, vaccuum->width, vaccuum->height, o->x, o->y, o->width, o->height, delta_time, y_vel);
                if(y_collision)
                {
                    float dy = calculateYDistance(*vaccuum, *o);
                    float y_time = abs(dy / y_vel);
                    if(y_time < y_min_time)
                    {
                        y_min_time = y_time;
                    }
                }
            }
            
            Obstacle* o = &level->post_character_draw_obstacles[1];
            bool y_collision = checkYCollision(vaccuum->x, vaccuum->y, vaccuum->width, vaccuum->height, o->x, o->y, o->width, o->height, delta_time, y_vel);
            if(y_collision)
            {
                float dy = calculateYDistance(*vaccuum, *o);
                float y_time = abs(dy / y_vel);
                if(y_time < y_min_time)
                {
                    y_min_time = y_time;
                }
            }

            vaccuum->y += y_vel * delta_time * y_min_time;
        }

        Animation* animation = &living_room_level_data->vaccuum_animation;

        level->post_character_draw_obstacles[0].texture = updateAnimation(animation);

        if((living_room_level_data->counter % 20) == 0)
        {
            living_room_level_data->vaccuum_cord_points[living_room_level_data->num_vaccuum_cord_points * 2] = vaccuum->x + 50;
            living_room_level_data->vaccuum_cord_points[living_room_level_data->num_vaccuum_cord_points * 2 + 1] = vaccuum->y + 10;
            living_room_level_data->num_vaccuum_cord_points++;
        }
    }

    if(key_map[SDL_SCANCODE_SPACE] && !living_room_level_data->complete)
    {
        player->current_animation_index = 3;
        player->animation_counter = 60;
        player->x_vel = 0;
        player->y_vel = 0;
    }

    living_room_level_data->counter++;
}


void draw_living_room(Level* level, void* data, SDL_Renderer* renderer)
{
    LivingRoomLevelData* living_room_level_data = (LivingRoomLevelData*)data;

    if(living_room_level_data->num_vaccuum_cord_points > 0)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        for(int i = 0; i < living_room_level_data->num_vaccuum_cord_points; i++)
        {
            if(i == living_room_level_data->num_vaccuum_cord_points - 1)
            {
                SDL_RenderDrawLineF(renderer, living_room_level_data->vaccuum_cord_points[i * 2], living_room_level_data->vaccuum_cord_points[i * 2 + 1],
                                                level->post_character_draw_obstacles[0].x + 50, level->post_character_draw_obstacles[0].y + 10);
            }
            else
            {
                SDL_RenderDrawLineF(renderer, living_room_level_data->vaccuum_cord_points[i * 2], living_room_level_data->vaccuum_cord_points[i * 2 + 1],
                                                living_room_level_data->vaccuum_cord_points[(i + 1) * 2], living_room_level_data->vaccuum_cord_points[(i + 1) * 2 + 1]);
            }
        }
    }

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

void blit(SDL_Texture *texture, int x, int y, int x_offset, int y_offset)
{
    SDL_Rect dest;

    dest.x = x + x_offset;
    dest.y = y + y_offset;

    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

    SDL_RenderCopy(renderer, texture, NULL, &dest);
}

void blit(SDL_Texture *texture, int x, int y, bool facing_right)
{
    SDL_Rect dest;

    dest.x = x;
    dest.y = y;

    SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
    if(facing_right)
    {
        flip = SDL_FLIP_NONE;
    }

    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

    SDL_RenderCopyEx(renderer, texture, NULL, &dest, 0, NULL, flip);
}

SDL_Texture* updateAnimation(Animation* animation)
{        
    SDL_Texture* texture = animation->textures[animation->current_texture_index];

    if(animation->num_textures > 1)
    {
        if((animation->animation_counter++ % animation->animation_speed) == 0)
        {
            animation->current_texture_index = (animation->current_texture_index + 1) % animation->num_textures;
        }
    }

    return texture;
}
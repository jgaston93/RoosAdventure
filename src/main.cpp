#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <map>
#include <limits>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int MAX_NUM_OBSTACLES = 20;
const int MAX_NUM_COLLECTIBLES = 20;
const int MAX_NUM_LEVELS = 20;
const int MAX_NUM_EXITS = 10;
const int MS_PER_FRAME = 16;
const int MAX_NUM_LEVEL_TRANSITION_FRAMES = 30;

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
char Living_Room_Level_Background_Texture_Filename[] = "assets/living_room_level_background.png\0";
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

SDL_Renderer *renderer;
SDL_Window* window;

const int MAX_NUM_ANIMATIONS = 10;
const int MAX_NUM_FRAMES_PER_ANIMATION = 10;

struct Animation
{
    int num_textures;
    int current_texture_index;
    int animation_speed;
    int animation_counter;
    SDL_Texture* textures[MAX_NUM_FRAMES_PER_ANIMATION];
};

struct Obstacle
{
    float x;
    float y;
    int width;
    int height;
    float texture_x_offset;
    float texture_y_offset;
    SDL_Texture* texture;
};

struct Entity
{
    float x;
    float y;
    int width;
    int height;
    float speed;
    float x_vel;
    float y_vel;
    int current_animation_index;
    Animation animations[10];
    SDL_Texture* current_texture; 
    bool facing_right;
    int idle_counter;
    int idle_threshold;
    int animation_counter;
};

struct Collectible
{
    float x;
    float y;
    int width;
    int height;
    bool collected;
    SDL_Texture* texture;
    float texture_x_offset;
    float texture_y_offset;
};

struct Exit
{
    float x;
    float y;
    int width;
    int height;
    int next_level_index;
    int x_reentry;
    int y_reentry;
};

struct Level
{
    SDL_Texture* background_texture;
    int num_pre_character_draw_obstacles;
    int num_post_character_draw_obstacles;
    Obstacle pre_character_draw_obstacles[MAX_NUM_OBSTACLES];
    Obstacle post_character_draw_obstacles[MAX_NUM_OBSTACLES];
    int num_collectibles;
    Collectible collectibles[MAX_NUM_COLLECTIBLES];
    int num_exits;
    Exit exits[MAX_NUM_EXITS];
    float x_init;
    float y_init;
    void (*init_level)(Level*, void*);
    void (*update_level)(Level*, void*, Entity*, std::map<SDL_Scancode, bool>);
    void* level_data;
};

struct OutsideLevelData
{
    int counter;
    int num_bushes;
    int bush_order[5];
    int bush_index;
    int selected_bush;
    Animation bush_animation;
    SDL_Texture* bush_texture;
    SDL_Texture* dead_bush_texture_1;
    SDL_Texture* dead_bush_texture_2;
    bool complete;
};

typedef struct Animation Animation;
typedef struct Obstacle Obstacle;
typedef struct Entity Entity;
typedef struct Collectible Collectible;
typedef struct Level Level;
typedef struct OutsideLevelData OutsideLevelData;

std::map<SDL_Scancode, bool> key_map;

bool isRunning = true;

SDL_Texture* loadTexture(char* filename);
void handleInput();
void updatePlayer(Entity& p);
void blit(SDL_Texture* texture, int x, int y, int x_offset, int y_offset);
void blit(SDL_Texture* texture, int x, int y, bool facing_right);
bool checkXYCollision(Entity e, Obstacle o, float delta_time);
bool checkXCollision(Entity e, Obstacle o, float delta_time);
bool checkYCollision(Entity e, Obstacle o, float delta_time);
void calculateDistance(Entity e, Obstacle o, float& dx, float& dy);
float calculateXDistance(Entity e, Obstacle o);
float calculateYDistance(Entity e, Obstacle o);
void calculateTimeToCollide(float x_vel, float y_vel, float dx, float dy, float& x_time, float& y_time);
bool checkCollision(Entity e, Collectible c);
bool checkCollision(Entity en, Exit ex);

// Level update functions
void init_outside(Level* level, void* data);
void update_outside(Level* level, void* data, Entity* player, std::map<SDL_Scancode, bool> key_map);

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
    kitchen_level.num_collectibles = 1;
    kitchen_level.collectibles[0] = { 400, SCREEN_HEIGHT - 100, 100, 100, false, loadTexture(White_Lamb_Texture_Filename), 0, 0};
    kitchen_level.num_exits = 2;
    kitchen_level.exits[0] = { -(100 + (float)player.width / 2), 400, 100, 200, 2 , 0, 400};
    kitchen_level.exits[1] = { 0.0, (float)SCREEN_HEIGHT + player.height / 2, 800, 100, 1, SCREEN_WIDTH / 2 - player.width / 2, SCREEN_HEIGHT - player.height };
    kitchen_level.init_level = NULL;
    kitchen_level.update_level = NULL;
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
    bottom_kitchen_level.level_data = NULL;

    // Living room level initialization
    living_room_level.background_texture = loadTexture(Living_Room_Level_Background_Texture_Filename);
    living_room_level.num_pre_character_draw_obstacles = 4;
    living_room_level.pre_character_draw_obstacles[0] = {0.0, 0.0, 615, 335, 0, 0, NULL };
    living_room_level.pre_character_draw_obstacles[1] = {615.0, 0.0, 185, 113, 0, 0, NULL };
    living_room_level.pre_character_draw_obstacles[2] = {0.0, 600.0, 800, 100, 0, 0, NULL };
    living_room_level.pre_character_draw_obstacles[3] = {800.0, 0.0, 100, 600, 0, 0, NULL };
    living_room_level.x_init = 0;
    living_room_level.y_init = 400;
    living_room_level.num_exits = 1;
    living_room_level.exits[0] = { -(100 + (float)player.width / 2), 335, 100, 265, 0 , 0, 400};
    living_room_level.init_level = NULL;
    living_room_level.update_level = NULL;
    living_room_level.level_data = NULL;
    
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
    hallway_level.level_data = NULL;
    
    // Outside level initialization
    outside_level.background_texture = loadTexture(Outside_Level_Background_Texture_Filename);
    outside_level.num_pre_character_draw_obstacles = 8;
    outside_level.num_post_character_draw_obstacles = 0;
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
    int level_transition_counter = 0;

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

            if(animation->num_textures > 1)
            {
                player.current_texture = animation->textures[animation->current_texture_index];
                if((animation->animation_counter++ % animation->animation_speed) == 0)
                {
                    animation->current_texture_index = (animation->current_texture_index + 1) % animation->num_textures;
                }
            }
            else
            {
                player.current_texture = animation->textures[animation->current_texture_index];
            }
            
            Level& level = levels[current_level_index];

            if(level.update_level != NULL)
            {
                level.update_level(&level, level.level_data, &player, key_map);
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

bool checkCollision(Entity e, Collectible c)
{
    bool x_collision = e.x + e.width >= c.x && c.x + c.width > e.x;
    bool y_collision = e.y + e.height >= c.y && c.y + c.height > e.y;

    return x_collision && y_collision;
}

bool checkCollision(Entity en, Exit ex)
{
    bool x_collision = en.x + en.width >= ex.x && ex.x + ex.width > en.x;
    bool y_collision = en.y + en.height >= ex.y && ex.y + ex.height > en.y;

    return x_collision && y_collision;
}

bool checkXYCollision(Entity e, Obstacle o, float delta_time)
{
    float x_intended = e.x + e.x_vel * delta_time;
    float y_intended = e.y + e.y_vel * delta_time;
    return x_intended < o.x + o.width && x_intended + e.width > o.x && y_intended < o.y + o.height && y_intended + e.height > o.y;
}

bool checkXCollision(Entity e, Obstacle o, float delta_time)
{
    float x_intended = e.x + e.x_vel * delta_time;
    return x_intended < o.x + o.width && x_intended + e.width > o.x && e.y < o.y + o.height && e.y + e.height > o.y;
}

bool checkYCollision(Entity e, Obstacle o, float delta_time)
{
    float y_intended = e.y + e.y_vel * delta_time;
    return e.x < o.x + o.width && e.x + e.width > o.x && y_intended < o.y + o.height && y_intended + e.height > o.y;
}

void calculateDistance(Entity e, Obstacle o, float& dx, float& dy)
{
    if(e.x < o.x)
    {
        dx = o.x - (e.x + e.width);
    }
    else if(e.x > o.x)
    {
        dx = e.x - (o.x + o.width);
    }

    if(e.y < o.y)
    {
        dy = o.y - (e.y + e.height);
    }
    else if(e.y > o.y)
    {
        dy = e.y - (o.y + o.height);
    }
}

float calculateXDistance(Entity e, Obstacle o)
{
    float dx = 0;
    if(e.x < o.x)
    {
        dx = o.x - (e.x + e.width);
    }
    else if(e.x > o.x)
    {
        dx = e.x - (o.x + o.width);
    }
    return dx;
}

float calculateYDistance(Entity e, Obstacle o)
{
    float dy = 0;
    if(e.y < o.y)
    {
        dy = o.y - (e.y + e.height);
    }
    else if(e.y > o.y)
    {
        dy = e.y - (o.y + o.height);
    }
    return dy;
}

void calculateTimeToCollide(float x_vel, float y_vel, float dx, float dy, float& x_time, float& y_time)
{
    x_time = x_vel != 0 ? abs(dx / x_vel) : 0;
    y_time = y_vel != 0 ? abs(dy / y_vel) : 0;
}

void init_outside(Level* level, void* data)
{
    OutsideLevelData* outside_level_data = (OutsideLevelData*)data;

    if(!outside_level_data->complete)
    {
        outside_level_data->counter = 0;
        outside_level_data->bush_index = 0;
        // unsigned int ticks = SDL_GetTicks();
        // outside_level_data->active_bush = ticks % outside_level_data->num_bushes;
        // outside_level_data->active_bush_increment = ticks % outside_level_data->num_bushes;
    }
    else
    {
        outside_level_data->counter = 60;
        for(int i = 0; i < outside_level_data->num_bushes; i++)
        {
            level->pre_character_draw_obstacles[i + 3].texture = outside_level_data->dead_bush_texture_2;
        }
    }
}

void update_outside(Level* level, void* data, Entity* player, std::map<SDL_Scancode, bool> key_map)
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

#ifndef GAME_DATA_STRUCTURES_HPP
#define GAME_DATA_STRUCTURES_HPP

#include <map>
#include <SDL2/SDL.h>
#include "GameConstants.hpp"

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
    void (*update_level)(Level*, void*, Entity*, std::map<SDL_Scancode, bool>, float);
    void (*draw_level)(Level*, void*, SDL_Renderer*);
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

struct LivingRoomLevelData
{
    int counter;
    float vaccuum_final_x_position;
    float vaccuum_final_y_position;
    Animation vaccuum_animation;
    Animation vaccuum_short_circuit_animation;
    Animation vaccuum_smoking_animation;
    int num_vaccuum_cord_points;
    int vaccuum_cord_points[MAX_NUM_VACCUUM_CORD_POINTS * 2];
    bool complete;
};

typedef struct Animation Animation;
typedef struct Obstacle Obstacle;
typedef struct Entity Entity;
typedef struct Collectible Collectible;
typedef struct Level Level;
typedef struct OutsideLevelData OutsideLevelData;
typedef struct LivingRoomLevelData LivingRoomLevelData;

#endif
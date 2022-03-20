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
#include "KitchenLevelFunctions.hpp"
#include "BottomKitchenLevelFunctions.hpp"
#include "HallwayLevelFunctions.hpp"

std::string Window_Title = "Roo's Adventure";

char Standing_Texture_Filename[] = "assets/roo_standing.png";
char Walking_Texture_Filename_1[] = "assets/roo_walking_1.png";
char Walking_Texture_Filename_2[] = "assets/roo_walking_2.png";
char Peeing_Texture_Filename_1[] = "assets/roo_peeing_1.png";
char Peeing_Texture_Filename_2[] = "assets/roo_peeing_2.png";
char Idle_Texture_Filename[] = "assets/roo_sitting.png";

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

    player.width = PLAYER_WIDTH;
    player.height = PLAYER_HEIGHT;
    player.speed = 200;
    player.idle_threshold = 200;
    player.collected[0] = false;
    player.collected[1] = false;
    player.collected[2] = false;
    player.collected[3] = false;

    setup_kitchen_level(&kitchen_level, renderer);
    setup_bottom_kitchen_level(&bottom_kitchen_level, renderer);
    setup_living_room(&living_room_level, renderer);
    setup_hallway_level(&hallway_level, renderer);
    setup_outside(&outside_level, renderer);
    setup_office(&office_level, renderer);
    setup_computer(&computer_level, renderer);
    setup_bedroom(&bedroom_level, renderer);
    setup_bed_maze_1(&bedroom_maze_1_level, renderer);
    setup_bed_maze_2(&bedroom_maze_2_level, renderer);
    setup_door(&door_level, renderer);

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
    player.animations[0].textures[0] = loadTexture(Standing_Texture_Filename, renderer);

    player.animations[1].num_textures = 2;
    player.animations[1].animation_speed = 10;
    player.animations[1].textures[0] = loadTexture(Walking_Texture_Filename_1, renderer);
    player.animations[1].textures[1] = loadTexture(Walking_Texture_Filename_2, renderer);

    player.animations[2].num_textures = 1;
    player.animations[2].textures[0] = loadTexture(Idle_Texture_Filename, renderer);

    player.animations[3].num_textures = 2;
    player.animations[3].animation_speed = 10;
    player.animations[3].textures[0] = loadTexture(Peeing_Texture_Filename_1, renderer);
    player.animations[3].textures[1] = loadTexture(Peeing_Texture_Filename_2, renderer);

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
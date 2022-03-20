#include "OutsideLevelFunctions.hpp"
#include "GameUtilityFunctions.hpp"
#include "CollisionCheckingFunctions.hpp"

char Outside_Level_Background_Texture_Filename[] = "assets/outside_background.png";
char Bush_Texture_Filename[] = "assets/bush.png";
char Angry_Bush_1_Texture_Filename[] = "assets/angry_bush_1.png";
char Angry_Bush_2_Texture_Filename[] = "assets/angry_bush_2.png";
char Dead_Bush_1_Texture_Filename[] = "assets/dead_bush_1.png";
char Dead_Bush_2_Texture_Filename[] = "assets/dead_bush_2.png";
char Dead_Bush_3_Texture_Filename[] = "assets/dead_bush_3.png";
char Dead_Bush_4_Texture_Filename[] = "assets/dead_bush_4.png";
char Dead_Bush_5_Texture_Filename[] = "assets/dead_bush_5.png";
char Purple_Lamb_Texture_Filename[] = "assets/purple_lamb.png";

void setup_outside(Level* level, SDL_Renderer* renderer)
{
    level->background_texture = loadTexture(Outside_Level_Background_Texture_Filename, renderer);
    level->num_pre_character_draw_obstacles = 8;
    level->num_post_character_draw_obstacles = 0;
    level->num_collectibles = 0;
    level->collectibles[0] = { 355.0, 325.0, 80, 2, false, loadTexture(Purple_Lamb_Texture_Filename, renderer), 0, 0};
    level->pre_character_draw_obstacles[0] = {-100, 0.0, 100, 600, 0, 0, NULL };
    level->pre_character_draw_obstacles[1] = {0.0, 0.0, 800, 100, 0, 0, NULL };
    level->pre_character_draw_obstacles[2] = {800.0, 0.0, 100, 600, 0, 0, NULL };
    SDL_Texture* bush_texture = loadTexture(Bush_Texture_Filename, renderer);
    level->pre_character_draw_obstacles[3] = {105.0, 225.0, 80, 22, 0, -20, bush_texture };
    level->pre_character_draw_obstacles[4] = {605.0, 225.0, 80, 22, 0, -20, bush_texture };
    level->pre_character_draw_obstacles[5] = {355.0, 325.0, 80, 22, 0, -20, bush_texture };
    level->pre_character_draw_obstacles[6] = {105.0, 425.0, 80, 22, 0, -20, bush_texture };
    level->pre_character_draw_obstacles[7] = {605.0, 425.0, 80, 22, 0, -20, bush_texture };
    level->x_init = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2;
    level->y_init = SCREEN_HEIGHT - PLAYER_HEIGHT;
    level->num_exits = 1;
    level->exits[0] = { 0, SCREEN_HEIGHT + (float)PLAYER_HEIGHT / 2, 800, 100, BOTTOM_KITCHEN_LEVEL_INDEX, SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2, SCREEN_HEIGHT - PLAYER_HEIGHT};
    level->init_level = &init_outside;
    level->update_level = &update_outside;
    level->pre_character_draw_level = NULL;
    level->post_character_draw_level = NULL;
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
    bush_animation.textures[0] = loadTexture(Angry_Bush_1_Texture_Filename, renderer);
    bush_animation.textures[1] = loadTexture(Angry_Bush_2_Texture_Filename, renderer);
    outside_level_data.bush_animation = bush_animation;
    Animation bush_death_animation;
    bush_death_animation.animation_counter = 0;
    bush_death_animation.animation_speed = 15;
    bush_death_animation.current_texture_index = 0;
    bush_death_animation.num_textures = 4;
    bush_death_animation.textures[0] = loadTexture(Dead_Bush_1_Texture_Filename, renderer);
    bush_death_animation.textures[1] = loadTexture(Dead_Bush_2_Texture_Filename, renderer);
    bush_death_animation.textures[2] = loadTexture(Dead_Bush_3_Texture_Filename, renderer);
    bush_death_animation.textures[3] = loadTexture(Dead_Bush_4_Texture_Filename, renderer);
    bush_death_animation.textures[4] = loadTexture(Dead_Bush_5_Texture_Filename, renderer);
    outside_level_data.bush_death_animation = bush_death_animation;
    outside_level_data.bush_texture = bush_texture;
    outside_level_data.dead_bush_texture_1 = loadTexture(Dead_Bush_1_Texture_Filename, renderer);
    outside_level_data.dead_bush_texture_2 = loadTexture(Dead_Bush_5_Texture_Filename, renderer);
    level->level_data = new OutsideLevelData();
    memcpy(level->level_data, &outside_level_data, sizeof(OutsideLevelData));
}

void init_outside(Level* level, void* data, Entity* player)
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
                    if(outside_level_data->bush_index == outside_level_data->num_bushes - 1)
                    {
                        Animation* bush_death_animation = &outside_level_data->bush_death_animation;
                        SDL_Texture* current_texture = updateAnimation(bush_death_animation);
                        for(int i = 0; i < outside_level_data->num_bushes; i++)
                        {
                            level->pre_character_draw_obstacles[i + 3].texture = current_texture;
                        }
                    }
                    else
                    {
                        level->pre_character_draw_obstacles[outside_level_data->bush_order[outside_level_data->bush_index] + 3].texture = outside_level_data->dead_bush_texture_1;
                    }
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
                        for(int i = 0; i < outside_level_data->num_bushes; i++)
                        {
                            level->pre_character_draw_obstacles[i + 3].texture = outside_level_data->dead_bush_texture_2;
                        }
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
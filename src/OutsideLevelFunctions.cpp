#include "OutsideLevelFunctions.hpp"
#include "GameUtilityFunctions.hpp"
#include "CollisionCheckingFunctions.hpp"

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
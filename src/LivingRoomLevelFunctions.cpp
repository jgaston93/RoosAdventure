#include "LivingRoomLevelFunctions.hpp"
#include "CollisionCheckingFunctions.hpp"
#include "GameUtilityFunctions.hpp"

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
        if(vaccuum->x + 10 > player->x + 50)
        {
            x_vel = -vaccuum_speed;
        }
        else
        {
            x_vel = vaccuum_speed;
        }
        if(vaccuum->y + -30 > player->y + 50)
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
                // level_transition_counter = MAX_NUM_LEVEL_TRANSITION_FRAMES;
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

            // Obstacle* o = &level->post_character_draw_obstacles[1];
            // bool x_collision = checkXCollision(vaccuum->x, vaccuum->y, vaccuum->width, vaccuum->height, o->x, o->y, o->width, o->height, delta_time, x_vel);
            // if(x_collision)
            // {
            //     float dx = calculateXDistance(*vaccuum, *o);
            //     float x_time = abs(dx / x_vel);
            //     if(x_time < x_min_time)
            //     {
            //         x_min_time = x_time;
            //     }
            // }

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
                // level_transition_counter = MAX_NUM_LEVEL_TRANSITION_FRAMES;
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
            
            // Obstacle* o = &level->post_character_draw_obstacles[1];
            // bool y_collision = checkYCollision(vaccuum->x, vaccuum->y, vaccuum->width, vaccuum->height, o->x, o->y, o->width, o->height, delta_time, y_vel);
            // if(y_collision)
            // {
            //     float dy = calculateYDistance(*vaccuum, *o);
            //     float y_time = abs(dy / y_vel);
            //     if(y_time < y_min_time)
            //     {
            //         y_min_time = y_time;
            //     }
            // }

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

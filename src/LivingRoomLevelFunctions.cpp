#include "LivingRoomLevelFunctions.hpp"
#include "CollisionCheckingFunctions.hpp"
#include "GameUtilityFunctions.hpp"

char Living_Room_Level_Background_Texture_Filename[] = "assets/living_room_level_background.png";
char Box_Obstacle_Texture_Filename[] = "assets/box_obstacle.png";
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
char White_Lamb_Texture_Filename[] = "assets/white_lamb.png";

void setup_living_room(Level* level, SDL_Renderer* renderer)
{
    level->background_texture = loadTexture(Living_Room_Level_Background_Texture_Filename, renderer);
    level->num_pre_character_draw_obstacles = 9;
    level->num_post_character_draw_obstacles = 1;
    level->pre_character_draw_obstacles[0] = { 0.0, 0.0, 260, 255, 0, 0, NULL };
    level->pre_character_draw_obstacles[1] = { 260.0, 0.0, 300, 210, 0, 0, NULL };
    level->pre_character_draw_obstacles[2] = { 560.0, 0.0, 50, 255, 0, 0, NULL };
    level->pre_character_draw_obstacles[3] = { 615.0, 0.0, 185, 70, 0, 0, NULL };
    level->pre_character_draw_obstacles[4] = { 0.0, 600.0, 800, 100, 0, 0, NULL };
    level->pre_character_draw_obstacles[5] = { 800.0, 0.0, 100, 600, 0, 0, NULL };
    level->pre_character_draw_obstacles[6] = { 135.0, 335.0, 90, 90, -5, -5, loadTexture(Box_Obstacle_Texture_Filename, renderer) };
    level->pre_character_draw_obstacles[7] = { 355.0, 385.0, 90, 90, -5, -5, loadTexture(Box_Obstacle_Texture_Filename, renderer) };
    level->pre_character_draw_obstacles[8] = { 575.0, 335.0, 90, 90, -5, -5, loadTexture(Box_Obstacle_Texture_Filename, renderer) };
    level->post_character_draw_obstacles[0] = {620.0, 115.0, 100, 20, 0, -80, NULL };
    level->x_init = 0;
    level->y_init = 400;
    level->num_collectibles = 0;
    level->collectibles[0] = { -100, -100, 10, 10, false, loadTexture(White_Lamb_Texture_Filename, renderer), -45, -45};
    level->num_exits = 1;
    level->exits[0] = { -(100 + (float)PLAYER_WIDTH / 2), 210, 100, 390, KITCHEN_LEVEL_INDEX , 0, 400};
    level->init_level = &init_living_room;
    level->update_level = &update_living_room;
    level->pre_character_draw_level = &draw_living_room;
    level->post_character_draw_level = &draw_vaccuum;
    LivingRoomLevelData living_room_level_data;
    living_room_level_data.counter = 0;
    living_room_level_data.complete = false;
    living_room_level_data.num_vaccuum_cord_points = 0;
    Animation vaccuum_animation;
    vaccuum_animation.animation_counter = 0;
    vaccuum_animation.animation_speed = 15;
    vaccuum_animation.current_texture_index = 0;
    vaccuum_animation.num_textures = 2;
    vaccuum_animation.textures[0] = loadTexture(Vaccuum_1_Texture_Filename, renderer);
    vaccuum_animation.textures[1] = loadTexture(Vaccuum_2_Texture_Filename, renderer);
    living_room_level_data.vaccuum_animation = vaccuum_animation;
    Animation vaccuum_short_circuit_animation;
    vaccuum_short_circuit_animation.animation_counter = 0;
    vaccuum_short_circuit_animation.animation_speed = 15;
    vaccuum_short_circuit_animation.current_texture_index = 0;
    vaccuum_short_circuit_animation.num_textures = 2;
    vaccuum_short_circuit_animation.textures[0] = loadTexture(Vaccuum_3_Texture_Filename, renderer);
    vaccuum_short_circuit_animation.textures[1] = loadTexture(Vaccuum_4_Texture_Filename, renderer);
    living_room_level_data.vaccuum_short_circuit_animation = vaccuum_short_circuit_animation;
    Animation vaccuum_smoking_animation;
    vaccuum_smoking_animation.animation_counter = 0;
    vaccuum_smoking_animation.animation_speed = 30;
    vaccuum_smoking_animation.current_texture_index = 0;
    vaccuum_smoking_animation.num_textures = 2;
    vaccuum_smoking_animation.textures[0] = loadTexture(Vaccuum_5_Texture_Filename, renderer);
    vaccuum_smoking_animation.textures[1] = loadTexture(Vaccuum_6_Texture_Filename, renderer);
    living_room_level_data.vaccuum_smoking_animation = vaccuum_smoking_animation;
    Animation vaccuum_explosion_animation;
    vaccuum_explosion_animation.animation_counter = 0;
    vaccuum_explosion_animation.animation_speed = 10;
    vaccuum_explosion_animation.current_texture_index = 0;
    vaccuum_explosion_animation.num_textures = 2;
    vaccuum_explosion_animation.textures[0] = loadTexture(Vaccuum_7_Texture_Filename, renderer);
    vaccuum_explosion_animation.textures[1] = loadTexture(Vaccuum_8_Texture_Filename, renderer);
    living_room_level_data.vaccuum_explosion_animation = vaccuum_explosion_animation;
    Animation outlet_spark_animation;
    outlet_spark_animation.animation_counter = 0;
    outlet_spark_animation.animation_speed = 10;
    outlet_spark_animation.current_texture_index = 0;
    outlet_spark_animation.num_textures = 2;
    outlet_spark_animation.textures[0] = loadTexture(Outlet_Spark_1_Texture_Filename, renderer);
    outlet_spark_animation.textures[1] = loadTexture(Outlet_Spark_2_Texture_Filename, renderer);
    living_room_level_data.outlet_spark_animation = outlet_spark_animation;
    level->level_data = new LivingRoomLevelData();
    memcpy(level->level_data, &living_room_level_data, sizeof(LivingRoomLevelData));
}

void init_living_room(Level* level, void* data, Entity* player)
{
    LivingRoomLevelData* living_room_level_data = (LivingRoomLevelData*)data;

    if(!living_room_level_data->complete)
    {
        level->post_character_draw_obstacles[0].x = 620.0;
        level->post_character_draw_obstacles[0].y = 115.0;
        living_room_level_data->prev_position_x = level->post_character_draw_obstacles[0].x;
        living_room_level_data->prev_position_y = level->post_character_draw_obstacles[0].y;
        
        living_room_level_data->counter = 0;
        living_room_level_data->num_vaccuum_cord_points = 0;
        living_room_level_data->vaccuum_cord_points[living_room_level_data->num_vaccuum_cord_points * 2] = level->post_character_draw_obstacles[0].x + 50;
        living_room_level_data->vaccuum_cord_points[living_room_level_data->num_vaccuum_cord_points * 2 + 1] = level->post_character_draw_obstacles[0].y + 10;
        living_room_level_data->num_vaccuum_cord_points++;
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
            living_room_level_data->vaccuum_texture = updateAnimation(animation);
        }
        else
        {
            living_room_level_data->complete = true;
            player->current_animation_index = 0;
            living_room_level_data->vaccuum_final_x_position = vaccuum->x;
            living_room_level_data->vaccuum_final_y_position = vaccuum->y;
            living_room_level_data->explosion_counter = 30;
        }
    }
    else if(living_room_level_data->complete)
    {
        if(living_room_level_data->explosion_counter > 1)
        {
            Animation* animation = &living_room_level_data->vaccuum_explosion_animation;
            living_room_level_data->vaccuum_texture = updateAnimation(animation);
        }
        else if(living_room_level_data->explosion_counter == 1)
        {
            level->num_collectibles = 1;
            level->collectibles->x = level->post_character_draw_obstacles[0].x + 45;
            level->collectibles->y = level->post_character_draw_obstacles[0].y;
            level->post_character_draw_obstacles[0].x = -100;
            level->post_character_draw_obstacles[0].y = -100;
        }
        if(living_room_level_data->explosion_counter > 0)
        {
            living_room_level_data->explosion_counter--;
        }
    }
    else if(living_room_level_data->num_vaccuum_cord_points < MAX_NUM_VACCUUM_CORD_POINTS)
    {
        float vaccuum_speed = 75;
        float x_vel = 0;
        float y_vel = 0;
        if(vaccuum->x + 50 > player->x + (player->width / 2) + 2)
        {
            x_vel = -vaccuum_speed;
            living_room_level_data->facing_right = true;
        }
        else if (vaccuum->x + 50 < player->x + (player->width / 2) - 2)
        {
            x_vel = vaccuum_speed;
            living_room_level_data->facing_right = false;
        }
        if(vaccuum->y + 10 > player->y + (player->height / 2) + 2)
        {
            y_vel = -vaccuum_speed;
        }
        else if(vaccuum->y + 10 < player->y + (player->height / 2) - 2)
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
                level->init_level(level, living_room_level_data, player);
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

            vaccuum->x += x_vel * delta_time * x_min_time;
        }
        if(y_vel != 0)
        {
            bool player_collision = checkXYCollision(*player, level->post_character_draw_obstacles[0], delta_time);
            if(player_collision)
            {
                level->init_level(level, living_room_level_data, player);
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

            vaccuum->y += y_vel * delta_time * y_min_time;
        }

        Animation* animation = &living_room_level_data->vaccuum_animation;

        living_room_level_data->vaccuum_texture = updateAnimation(animation);

        if((living_room_level_data->counter % 20) == 0 && 
                (vaccuum->x < living_room_level_data->prev_position_x - 20 || vaccuum->x > living_room_level_data->prev_position_x + 20) ||
                (vaccuum->y < living_room_level_data->prev_position_y - 20 || vaccuum->y > living_room_level_data->prev_position_y + 20))
        {
            living_room_level_data->vaccuum_cord_points[living_room_level_data->num_vaccuum_cord_points * 2] = vaccuum->x + 50;
            living_room_level_data->vaccuum_cord_points[living_room_level_data->num_vaccuum_cord_points * 2 + 1] = vaccuum->y + 10;
            living_room_level_data->num_vaccuum_cord_points++;
            living_room_level_data->prev_position_x = vaccuum->x;
            living_room_level_data->prev_position_y = vaccuum->y;
        }
    }
    else
    {
        living_room_level_data->vaccuum_texture = updateAnimation(&living_room_level_data->vaccuum_smoking_animation);
    }

    if(key_map[SDL_SCANCODE_SPACE] && !living_room_level_data->complete && 
            player->y < 85 && (player->x > 615 && player->x < 650))
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
                if(!living_room_level_data->complete)
                {
                    SDL_RenderDrawLineF(renderer, living_room_level_data->vaccuum_cord_points[i * 2], living_room_level_data->vaccuum_cord_points[i * 2 + 1],
                                                    level->post_character_draw_obstacles[0].x + 50, level->post_character_draw_obstacles[0].y + 10);
                }
            }
            else
            {
                SDL_RenderDrawLineF(renderer, living_room_level_data->vaccuum_cord_points[i * 2], living_room_level_data->vaccuum_cord_points[i * 2 + 1],
                                                living_room_level_data->vaccuum_cord_points[(i + 1) * 2], living_room_level_data->vaccuum_cord_points[(i + 1) * 2 + 1]);
            }
        }
    }
    if(living_room_level_data->num_vaccuum_cord_points == MAX_NUM_VACCUUM_CORD_POINTS && !living_room_level_data->complete)
    {
        SDL_Texture* spark_texture = updateAnimation(&living_room_level_data->outlet_spark_animation);
        blit(spark_texture, 624, 33, 0, 0, renderer);
    }

}

void draw_vaccuum(Level* level, void* data, SDL_Renderer* renderer)
{
    LivingRoomLevelData* living_room_level_data = (LivingRoomLevelData*)data;
    Obstacle* vaccuum = &level->post_character_draw_obstacles[0];

    blit(living_room_level_data->vaccuum_texture, vaccuum->x, vaccuum->y, living_room_level_data->facing_right, vaccuum->texture_x_offset, vaccuum->texture_y_offset, renderer);
}

#include "ComputerLevelFunctions.hpp"
#include "CollisionCheckingFunctions.hpp"

void init_computer(Level* level, void* data, Entity* player)
{
    ComputerLevelData* computer_level_data = (ComputerLevelData*)data;
    
    computer_level_data->counter = 0;

    computer_level_data->face_1_active = true;
    computer_level_data->computer_pain_counter = 0;

    computer_level_data->ball_velocity_x = 0;
    computer_level_data->ball_velocity_y = 0;

    level->post_character_draw_obstacles[0].x = SCREEN_WIDTH / 2 - 150 / 2;
    level->post_character_draw_obstacles[0].y = SCREEN_HEIGHT - 25 * 2;
    level->post_character_draw_obstacles[1].x = SCREEN_WIDTH / 2 - 25 / 2;
    level->post_character_draw_obstacles[1].y = SCREEN_HEIGHT - 25 * 2 - 25;

    computer_level_data->bricks_remaining = 7;
    level->pre_character_draw_obstacles[0].x = 50;
    level->pre_character_draw_obstacles[0].y = 50;
    level->pre_character_draw_obstacles[1].x = 150;
    level->pre_character_draw_obstacles[1].y = 50;
    level->pre_character_draw_obstacles[2].x = 250;
    level->pre_character_draw_obstacles[2].y = 50;
    level->pre_character_draw_obstacles[3].x = 350;
    level->pre_character_draw_obstacles[3].y = 50;
    level->pre_character_draw_obstacles[4].x = 450;
    level->pre_character_draw_obstacles[4].y = 50;
    level->pre_character_draw_obstacles[5].x = 550;
    level->pre_character_draw_obstacles[5].y = 50;
    level->pre_character_draw_obstacles[6].x = 650;
    level->pre_character_draw_obstacles[6].y = 50;
}

void update_computer(Level* level, void* data, Entity* player, std::map<SDL_Scancode, bool> key_map, float delta_time)
{
    ComputerLevelData* computer_level_data = (ComputerLevelData*)data;

    player->x = -100;
    player->y = -100;

    if(computer_level_data->computer_pain_counter > 0)
    {
        if(computer_level_data->computer_pain_counter == 1)
        {
            if(computer_level_data->bricks_remaining == 0)
            {
                level->num_exits = 1;
                computer_level_data->complete = true;
            }
            else
            {
                level->background_texture = computer_level_data->computer_face_1_texture;
            }
        }
        else
        {
            level->background_texture = computer_level_data->computer_face_3_texture;
        }
        computer_level_data->computer_pain_counter--;
    }
    else
    {
        if(computer_level_data->counter % 60 == 0)
        {
            if(computer_level_data->face_1_active)
            {
                level->background_texture = computer_level_data->computer_face_2_texture;
            }
            else
            {
                level->background_texture = computer_level_data->computer_face_1_texture;
            }
            computer_level_data->face_1_active = !computer_level_data->face_1_active;
        }
    }
    computer_level_data->counter++;

    float paddle_velocity = 0;
    float paddle_speed = 100;
    float ball_speed = 100;
    if((!key_map[SDL_SCANCODE_LEFT] && !key_map[SDL_SCANCODE_RIGHT]) || (key_map[SDL_SCANCODE_LEFT] && key_map[SDL_SCANCODE_RIGHT]))
    {
        paddle_velocity = 0;
    }
    else if(key_map[SDL_SCANCODE_LEFT])
    {
        paddle_velocity = -paddle_speed;
        if(computer_level_data->ball_velocity_x == 0 && computer_level_data->ball_velocity_y == 0)
        {
            computer_level_data->ball_velocity_y = -ball_speed;
            computer_level_data->ball_velocity_x = -ball_speed;
        }
    }
    else if(key_map[SDL_SCANCODE_RIGHT])
    {
        paddle_velocity = paddle_speed;
        if(computer_level_data->ball_velocity_x == 0 && computer_level_data->ball_velocity_y == 0)
        {
            computer_level_data->ball_velocity_y = -ball_speed;
            computer_level_data->ball_velocity_x = ball_speed;
        }
    }

    Obstacle* paddle = &level->post_character_draw_obstacles[0];
    paddle->x += paddle_velocity * delta_time;
    if(paddle->x < 0)
    {
        paddle->x = 0;
    }
    else if(paddle->x + paddle->width > 800)
    {
        paddle->x = SCREEN_WIDTH - paddle->width;
    }

    float x_min_time = 1;
    float y_min_time = 1;
    bool x_bounce = false;
    bool y_bounce = false;

    bool brick_collision = false;
    int brick_index = 0;

    Obstacle* ball = &level->post_character_draw_obstacles[1];

    // Move ball in X axis
    for(int i = 0; i < level->num_pre_character_draw_obstacles; i++)
    {
        Obstacle* o = &level->pre_character_draw_obstacles[i];
        bool x_collision = checkXCollision(ball->x, ball->y, ball->width, ball->height, o->x, o->y, o->width, o->height, delta_time, computer_level_data->ball_velocity_x);
        if(x_collision)
        {
            float dx = calculateXDistance(*ball, *o);
            float x_time = abs(dx / computer_level_data->ball_velocity_x);
            if(x_time < x_min_time)
            {
                x_min_time = x_time;
            }
            x_bounce = true;
            brick_collision = true;
            brick_index = i;
        }
    }

    bool x_collision = checkXCollision(ball->x, ball->y, ball->width, ball->height, paddle->x, paddle->y, paddle->width, paddle->height, delta_time, computer_level_data->ball_velocity_x);
    if(x_collision)
    {
        float dx = calculateXDistance(*ball, *paddle);
        float x_time = abs(dx / computer_level_data->ball_velocity_x);
        if(x_time < x_min_time)
        {
            x_min_time = x_time;
        }
        x_bounce = true;
    }

    ball->x += computer_level_data->ball_velocity_x * delta_time * x_min_time;
    if(ball->x < 0)
    {
        ball->x = 0;
        x_bounce = true;
    }
    else if(ball->x + ball->width > SCREEN_WIDTH)
    {
        ball->x = SCREEN_WIDTH - ball->width;
        x_bounce = true;
    }

    if(x_bounce)
    {
        if(computer_level_data->ball_velocity_x > 0)
        {
            computer_level_data->ball_velocity_x = -ball_speed;
        }
        else
        {
            computer_level_data->ball_velocity_x = ball_speed;
        }
    }

    // Move ball in Y axis
    for(int i = 0; i < level->num_pre_character_draw_obstacles; i++)
    {
        Obstacle* o = &level->pre_character_draw_obstacles[i];
        bool y_collision = checkYCollision(ball->x, ball->y, ball->width, ball->height, o->x, o->y, o->width, o->height, delta_time, computer_level_data->ball_velocity_y);
        if(y_collision)
        {
            float dy = calculateYDistance(*ball, *o);
            float y_time = abs(dy / computer_level_data->ball_velocity_y);
            if(y_time < y_min_time)
            {
                y_min_time = y_time;
            }
            y_bounce = true;
            brick_collision = true;
            brick_index = i;
        }
    }

    bool y_collision = checkYCollision(ball->x, ball->y, ball->width, ball->height, paddle->x, paddle->y, paddle->width, paddle->height, delta_time, computer_level_data->ball_velocity_y);
    if(y_collision)
    {
        float dy = calculateYDistance(*ball, *paddle);
        float y_time = abs(dy / computer_level_data->ball_velocity_y);
        if(y_time < y_min_time)
        {
            y_min_time = y_time;
        }
        y_bounce = true;
    }

    ball->y += computer_level_data->ball_velocity_y * delta_time * y_min_time;
    if(ball->y < 0)
    {
        ball->y = 0;
        y_bounce = true;
    }
    else if(ball->y + ball->height > SCREEN_HEIGHT)
    {
        init_computer(level, data, player);
    }

    if(y_bounce)
    {
        if(computer_level_data->ball_velocity_y > 0)
        {
            computer_level_data->ball_velocity_y = -ball_speed;
        }
        else
        {
            computer_level_data->ball_velocity_y = ball_speed;
        }
    }

    if(brick_collision)
    {
        level->pre_character_draw_obstacles[brick_index].x = -100;
        level->pre_character_draw_obstacles[brick_index].y = -25;
        computer_level_data->computer_pain_counter = 60;
        brick_collision = false;
        computer_level_data->bricks_remaining--;
    }
}
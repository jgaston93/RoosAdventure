#include "OfficeLevelFunctions.hpp"
#include "GameUtilityFunctions.hpp"

void init_office(Level* level, void* data, Entity* player)
{
    OfficeLevelData* office_level_data = (OfficeLevelData*)data;

    if(level->y_init < 300)
    {
        office_level_data->complete = true;
        level->num_exits = 1;
        level->num_post_character_draw_obstacles = 0;
    }

    if(!office_level_data->complete)
    {
        office_level_data->counter = 0;
        office_level_data->face_1_active = true;
    }
}

void update_office(Level* level, void* data, Entity* player, std::map<SDL_Scancode, bool> key_map, float delta_time)
{
    OfficeLevelData* office_level_data = (OfficeLevelData*)data;

    level->post_character_draw_obstacles[0].texture = updateAnimation(&office_level_data->electric_cage_animation);

    if(office_level_data->counter % 15 == 0)
    {
        if(office_level_data->face_1_active)
        {
            office_level_data->current_texture = office_level_data->computer_face_2_texture;
        }
        else
        {
            office_level_data->current_texture = office_level_data->computer_face_1_texture;
        }
        office_level_data->face_1_active = !office_level_data->face_1_active;
    }
    office_level_data->counter++;
}

void draw_office(Level* level, void* data, SDL_Renderer* renderer)
{
    OfficeLevelData* office_level_data = (OfficeLevelData*)data;
    if(!office_level_data->complete)
    {
        blit(office_level_data->current_texture, 356, 55, 0, 0, renderer);
    }
}
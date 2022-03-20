#include "OfficeLevelFunctions.hpp"
#include "GameUtilityFunctions.hpp"

char Office_Level_Background_Texture_Filename[] = "assets/office_background.png";
char Electric_Cage_1_Texture_Filename[] = "assets/electric_cage_1.png";
char Electric_Cage_2_Texture_Filename[] = "assets/electric_cage_2.png";
char Computer_Face_1_Texture_Filename[] = "assets/computer_face_1.png";
char Computer_Face_2_Texture_Filename[] = "assets/computer_face_2.png";
char Green_Lamb_Texture_Filename[] = "assets/green_lamb.png";

void setup_office(Level* level, SDL_Renderer* renderer)
{
    level->background_texture = loadTexture(Office_Level_Background_Texture_Filename, renderer);
    level->num_pre_character_draw_obstacles = 4;
    level->num_post_character_draw_obstacles = 1;
    level->pre_character_draw_obstacles[0] = { -100, 0.0, 100, 600, 0, 0, NULL };
    level->pre_character_draw_obstacles[1] = { 0.0, 0.0, 800, 100, 0, 0, NULL };
    level->pre_character_draw_obstacles[2] = { 800.0, 0.0, 100, 600, 0, 0, NULL };
    level->pre_character_draw_obstacles[3] = { 300.0, 100.0, 200, 50, 0, 0, NULL };
    level->post_character_draw_obstacles[0] = { 200.0, 100.0, 100, 100, 0, 0, loadTexture(Electric_Cage_1_Texture_Filename, renderer) };
    level->num_collectibles = 1;
    level->collectibles[0] = { 200.0, 100.0, 100, 100, false, loadTexture(Green_Lamb_Texture_Filename, renderer), 0, 0 };
    level->x_init = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2;
    level->y_init = SCREEN_HEIGHT - PLAYER_HEIGHT;
    level->num_exits = 2;
    level->exits[0] = { 0, SCREEN_HEIGHT + (float)PLAYER_HEIGHT / 2, 800, 100, BOTTOM_KITCHEN_LEVEL_INDEX, SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2, SCREEN_HEIGHT - PLAYER_HEIGHT};
    level->exits[1] = { 300.0, 150.0, 200, 10, COMPUTER_LEVEL_INDEX, SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2, 200 };
    level->init_level = &init_office;
    level->update_level = &update_office;
    level->pre_character_draw_level = &draw_office;
    level->post_character_draw_level = NULL;
    OfficeLevelData office_level_data;
    office_level_data.computer_face_1_texture = loadTexture(Computer_Face_1_Texture_Filename, renderer);
    office_level_data.computer_face_2_texture = loadTexture(Computer_Face_2_Texture_Filename, renderer);
    office_level_data.face_1_active = true;
    Animation electric_cage_animation;
    electric_cage_animation.animation_counter = 0;
    electric_cage_animation.animation_speed = 15;
    electric_cage_animation.current_texture_index = 0;
    electric_cage_animation.num_textures = 2;
    electric_cage_animation.textures[0] = loadTexture(Electric_Cage_1_Texture_Filename, renderer);
    electric_cage_animation.textures[1] = loadTexture(Electric_Cage_2_Texture_Filename, renderer);
    office_level_data.electric_cage_animation = electric_cage_animation;
    office_level_data.complete = false;
    level->level_data = new OfficeLevelData();
    memcpy(level->level_data, &office_level_data, sizeof(OfficeLevelData));
}

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
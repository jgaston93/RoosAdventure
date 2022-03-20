#ifndef OFFICE_LEVEL_FUNCTIONS_HPP
#define OFFICE_LEVEL_FUNCTIONS_HPP

#include "GameDataStructures.hpp"

void setup_office(Level* level, SDL_Renderer* renderer);
void init_office(Level* level, void* data, Entity* player);
void update_office(Level* level, void* data, Entity* player, std::map<SDL_Scancode, bool> key_map, float delta_time);
void draw_office(Level* level, void* data, SDL_Renderer* renderer);

#endif
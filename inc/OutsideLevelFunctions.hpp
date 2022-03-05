#ifndef OUTSIDE_LEVEL_FUNCTIONS_HPP
#define OUTSIDE_LEVEL_FUNCTIONS_HPP

#include "GameDataStructures.hpp"

void init_outside(Level* level, void* data, Entity* player);
void update_outside(Level* level, void* data, Entity* player, std::map<SDL_Scancode, bool> key_map, float delta_time);

#endif
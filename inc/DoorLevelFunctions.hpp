#ifndef DOOR_LEVEL_FUNCTIONS_HPP
#define DOOR_LEVEL_FUNCTIONS_HPP

#include "GameDataStructures.hpp"

void init_door(Level* level, void* data, Entity* player);
void update_door(Level* level, void* data, Entity* player, std::map<SDL_Scancode, bool> key_map, float delta_time);

#endif
#ifndef COMPUTER_LEVEL_FUNCTIONS_HPP
#define COMPUTER_LEVEL_FUNCTIONS_HPP

#include "GameDataStructures.hpp"

void init_computer(Level* level, void* data);
void update_computer(Level* level, void* data, Entity* player, std::map<SDL_Scancode, bool> key_map, float delta_time);

#endif
#ifndef LIVING_ROOM_LEVEL_FUNCTIONS_HPP
#define LIVING_ROOM_LEVEL_FUNCTIONS_HPP

#include "GameDataStructures.hpp"

void setup_living_room(Level* level, SDL_Renderer* renderer);
void init_living_room(Level* level, void* data, Entity* player);
void update_living_room(Level* level, void* data, Entity* player, std::map<SDL_Scancode, bool> key_map, float delta_time);
void draw_living_room(Level* level, void* data, SDL_Renderer* renderer);
void draw_vaccuum(Level* level, void* data, SDL_Renderer* renderer);

#endif
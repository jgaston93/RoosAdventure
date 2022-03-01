#ifndef BED_MAZE_LEVEL_FUNCTIONS_HPP
#define BED_MAZE_LEVEL_FUNCTIONS_HPP

#include "GameDataStructures.hpp"

void update_bed_maze(Level* level, void* data, Entity* player, std::map<SDL_Scancode, bool> key_map, float delta_time);
void draw_bed_maze(Level* level, void* data, SDL_Renderer* renderer);

#endif
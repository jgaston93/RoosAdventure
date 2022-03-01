#ifndef GAME_UTILITY_FUNCTIONS_HPP
#define GAME_UTILITY_FUNCTIONS_HPP

#include <SDL2/SDL.h>
#include "GameDataStructures.hpp"

void blit(SDL_Texture* texture, int x, int y, int x_offset, int y_offset, SDL_Renderer* renderer);
void blit(SDL_Texture* texture, int x, int y, bool facing_right, SDL_Renderer* renderer);
SDL_Texture* updateAnimation(Animation* animation);

#endif
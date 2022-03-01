#include "GameUtilityFunctions.hpp"

void blit(SDL_Texture *texture, int x, int y, int x_offset, int y_offset, SDL_Renderer* renderer)
{
    SDL_Rect dest;

    dest.x = x + x_offset;
    dest.y = y + y_offset;

    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

    SDL_RenderCopy(renderer, texture, NULL, &dest);
}

void blit(SDL_Texture *texture, int x, int y, bool facing_right, SDL_Renderer* renderer)
{
    SDL_Rect dest;

    dest.x = x;
    dest.y = y;

    SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
    if(facing_right)
    {
        flip = SDL_FLIP_NONE;
    }

    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

    SDL_RenderCopyEx(renderer, texture, NULL, &dest, 0, NULL, flip);
}

SDL_Texture* updateAnimation(Animation* animation)
{        
    SDL_Texture* texture = animation->textures[animation->current_texture_index];

    if(animation->num_textures > 1)
    {
        if((animation->animation_counter % animation->animation_speed) == 0 && animation->animation_counter > 0)
        {
            animation->current_texture_index = (animation->current_texture_index + 1) % animation->num_textures;
        }
        animation->animation_counter++;
    }

    return texture;
}
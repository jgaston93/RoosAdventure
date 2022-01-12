#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <map>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int MAX_NUM_OBSTACLES = 20;
const int MS_PER_FRAME = 16;

std::string Window_Title = "Hello SDL";

char Standing_Texture_Filename[] = "assets/roo_standing.png\0";
char Walking_Texture_Filename_1[] = "assets/roo_walking_1.png\0";
char Walking_Texture_Filename_2[] = "assets/roo_walking_2.png\0";
char Idle_Texture_Filename[] = "assets/roo_sitting.png\0";
char Level_Background_Texture_Filename[] = "assets/background.png\0";
char Left_Counter_Texture_Filename[] = "assets/left_counter.png\0";
char Back_Counter_Texture_Filename[] = "assets/back_counter.png\0";
char Middle_Counter_Texture_Filename[] = "assets/middle_counter.png\0";

SDL_Renderer *renderer;
SDL_Window* window;

struct Animation
{
    int num_textures;
    int current_texture_index;
    int animation_speed;
    int animation_counter;
    SDL_Texture* textures[10];
};

struct Obstacle
{
    SDL_Texture* texture;
    float x;
    float y;
    int width;
    int height;
    float texture_x_offset;
    float texture_y_offset;
};

struct Entity
{
    float x;
    float y;
    int width;
    int height;
    float speed;
    float dx;
    float dy;
    int current_animation_index;
    Animation animations[10];
    SDL_Texture* current_texture; 
    bool facing_right;
    int idle_counter;
    int idle_threshold;
};

struct Level
{
    SDL_Texture* background_texture;
    int num_pre_character_draw_obstacles;
    int num_post_character_draw_obstacles;
    Obstacle pre_character_draw_obstacles[MAX_NUM_OBSTACLES];
    Obstacle post_character_draw_obstacles[MAX_NUM_OBSTACLES];
    float x_init;
    float y_init;
};

typedef struct Animation Animation;
typedef struct Obstacle Obstacle;
typedef struct Entity Entity;
typedef struct Level Level;

std::map<SDL_Scancode, bool> key_map;

SDL_Texture* loadTexture(char* filename);
void blit(SDL_Texture* texture, int x, int y, int x_offset, int y_offset);
void blit(SDL_Texture* texture, int x, int y, bool facing_right);
bool checkXCollision(Entity e, Obstacle o);
bool checkYCollision(Entity e, Obstacle o);

int main(int argv, char** args)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow(Window_Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	renderer = SDL_CreateRenderer(window, -1, 0);

    IMG_Init(IMG_INIT_PNG);

    Level level;
    Entity player;
    memset(&level, 0, sizeof(Level));
    memset(&player, 0, sizeof(Entity));

    player.width = 100;
    player.height = 100;
    player.speed = 100;
    player.idle_threshold = 200;

    level.background_texture = loadTexture(Level_Background_Texture_Filename);
    level.num_pre_character_draw_obstacles = 2;
    level.num_post_character_draw_obstacles = 1;
    level.pre_character_draw_obstacles[0] = { loadTexture(Left_Counter_Texture_Filename), 0.0, 0.0, 100, 400, 0, 0};
    level.pre_character_draw_obstacles[1] = { loadTexture(Back_Counter_Texture_Filename), 100, 0.0, 700, 200, 0, 0};
    level.post_character_draw_obstacles[0] = { loadTexture(Middle_Counter_Texture_Filename), 300.0, 310.0, 300, 190, 0, -100};
    level.x_init = 0; // SCREEN_WIDTH / 2 - player.width / 2;
    level.y_init = SCREEN_HEIGHT - player.height;// SCREEN_HEIGHT / 2 - player.height / 2;

    player.x = level.x_init;
    player.y = level.y_init;

    player.animations[0].num_textures = 1;
    player.animations[0].textures[0] = loadTexture(Standing_Texture_Filename);

    player.animations[1].num_textures = 2;
    player.animations[1].animation_speed = 10;
    player.animations[1].textures[0] = loadTexture(Walking_Texture_Filename_1);
    player.animations[1].textures[1] = loadTexture(Walking_Texture_Filename_2);

    player.animations[2].num_textures = 1;
    player.animations[2].textures[0] = loadTexture(Idle_Texture_Filename);

    player.facing_right = true;

    key_map[SDL_SCANCODE_UP] = false;
    key_map[SDL_SCANCODE_DOWN] = false;
    key_map[SDL_SCANCODE_LEFT] = false;
    key_map[SDL_SCANCODE_RIGHT] = false;

	bool isRunning = true;
	SDL_Event event;
    int num_frames = 0;
    int prev_time = 0;

	while (isRunning)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				isRunning = false;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					isRunning = false;
				}
                else if (!event.key.repeat && event.key.keysym.scancode == SDL_SCANCODE_UP)
                {
                    key_map[event.key.keysym.scancode] = true;
                }
                else if (!event.key.repeat && event.key.keysym.scancode == SDL_SCANCODE_DOWN)
                {
                    key_map[event.key.keysym.scancode] = true;
                }
                else if (!event.key.repeat && event.key.keysym.scancode == SDL_SCANCODE_LEFT)
                {
                    key_map[event.key.keysym.scancode] = true;
                }
                else if (!event.key.repeat && event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
                {
                    key_map[event.key.keysym.scancode] = true;                    
                }
                break;
            
            case SDL_KEYUP:
                if (!event.key.repeat && event.key.keysym.scancode == SDL_SCANCODE_UP)
                {
                    key_map[event.key.keysym.scancode] = false;
                }
                else if (!event.key.repeat && event.key.keysym.scancode == SDL_SCANCODE_DOWN)
                {
                    key_map[event.key.keysym.scancode] = false;                    
                }
                else if (!event.key.repeat && event.key.keysym.scancode == SDL_SCANCODE_LEFT)
                {
                    key_map[event.key.keysym.scancode] = false;                    
                }
                else if (!event.key.repeat && event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
                {
                    key_map[event.key.keysym.scancode] = false;                    
                }
                break;
			}
		}

        if((!key_map[SDL_SCANCODE_UP] && !key_map[SDL_SCANCODE_DOWN]) || (key_map[SDL_SCANCODE_UP] && key_map[SDL_SCANCODE_DOWN]))
        {
            player.dy = 0;
        }
        else if(key_map[SDL_SCANCODE_UP])
        {
            player.dy = -player.speed;
        }
        else if(key_map[SDL_SCANCODE_DOWN])
        {
            player.dy = player.speed;
        }
        if((!key_map[SDL_SCANCODE_LEFT] && !key_map[SDL_SCANCODE_RIGHT]) || (key_map[SDL_SCANCODE_LEFT] && key_map[SDL_SCANCODE_RIGHT]))
        {
            player.dx = 0;
        }
        else if(key_map[SDL_SCANCODE_LEFT])
        {
            player.dx = -player.speed;
            player.facing_right = false;
        }
        else if(key_map[SDL_SCANCODE_RIGHT])
        {
            player.dx = player.speed;
            player.facing_right = true;
        }

        if(player.dx != 0 || player.dy != 0)
        {
            player.current_animation_index = 1;
            player.idle_counter = 0;
        }
        else if(player.idle_counter < player.idle_threshold)
        {
            player.current_animation_index = 0;
            player.idle_counter++;
        }
        else
        {
            player.current_animation_index = 2;
        }

        Animation* animation = &player.animations[player.current_animation_index];

        if(animation->num_textures > 1)
        {
            player.current_texture = animation->textures[animation->current_texture_index];
            if((animation->animation_counter++ % animation->animation_speed) == 0)
            {
                animation->current_texture_index = (animation->current_texture_index + 1) % animation->num_textures;
            }
        }
        else
        {
            player.current_texture = animation->textures[animation->current_texture_index];
        }        

        int current_time = SDL_GetTicks();
        float delta_time = (current_time - prev_time) * 1e-3;
        prev_time = current_time;

        player.x += player.dx * delta_time;
        player.y += player.dy * delta_time;

        bool x_collision = false;
        bool y_collision = false;
        for(int i = 0; i < level.num_pre_character_draw_obstacles; i++)
        {
            x_collision = checkXCollision(player, level.pre_character_draw_obstacles[i]);
            y_collision = checkYCollision(player, level.pre_character_draw_obstacles[i]);
            if(x_collision && y_collision)
            {
                break;
            }
        }
        if(!(x_collision && y_collision))
        {
            for(int i = 0; i < level.num_post_character_draw_obstacles; i++)
            {
                x_collision = checkXCollision(player, level.post_character_draw_obstacles[i]);
                y_collision = checkYCollision(player, level.post_character_draw_obstacles[i]);
                if(x_collision && y_collision)
                {
                    break;
                }
            }
        }

        if(x_collision && y_collision)
        {
            player.x -= player.dx * delta_time;
            player.y -= player.dy * delta_time;
        }

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

        for(int i = 0; i < level.num_pre_character_draw_obstacles; i++)
        {
            Obstacle o = level.pre_character_draw_obstacles[i];
            blit(o.texture, o.x, o.y, o.texture_x_offset, o.texture_y_offset);
        }
        blit(player.current_texture, player.x, player.y, player.facing_right);
        for(int i = 0; i < level.num_post_character_draw_obstacles; i++)
        {
            Obstacle o = level.post_character_draw_obstacles[i];
            blit(o.texture, o.x, o.y, o.texture_x_offset, o.texture_y_offset);
        }
        
		SDL_RenderPresent(renderer);

        int next_frame_time = num_frames++ * MS_PER_FRAME;
        current_time = SDL_GetTicks();
        if(current_time < next_frame_time)
        {
            int ms_to_sleep = next_frame_time - current_time;
            SDL_Delay(ms_to_sleep);
        }
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

SDL_Texture *loadTexture(char *filename)
{
	SDL_Texture *texture;

	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

	texture = IMG_LoadTexture(renderer, filename);

	return texture;
}

void blit(SDL_Texture *texture, int x, int y, int x_offset, int y_offset)
{
	SDL_Rect dest;

	dest.x = x + x_offset;
	dest.y = y + y_offset;

	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

	SDL_RenderCopy(renderer, texture, NULL, &dest);
}

void blit(SDL_Texture *texture, int x, int y, bool facing_right)
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

bool checkXCollision(Entity e, Obstacle o)
{
    bool x_collision = e.x + e.width >= o.x && o.x + o.width > e.x;
    return x_collision;
}

bool checkYCollision(Entity e, Obstacle o)
{
    bool y_collision = e.y + e.height >= o.y && o.y + o.height > e.y;
    return y_collision;
}
#include "CollisionCheckingFunctions.hpp"

bool checkCollision(Entity e, Collectible c)
{
    bool x_collision = e.x + e.width >= c.x && c.x + c.width > e.x;
    bool y_collision = e.y + e.height >= c.y && c.y + c.height > e.y;

    return x_collision && y_collision;
}

bool checkCollision(Entity en, Exit ex)
{
    bool x_collision = en.x + en.width >= ex.x && ex.x + ex.width > en.x;
    bool y_collision = en.y + en.height >= ex.y && ex.y + ex.height > en.y;

    return x_collision && y_collision;
}

bool checkCollision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2)
{
    return x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2;
}

bool checkXYCollision(Entity e, Obstacle o, float delta_time)
{
    float x_intended = e.x + e.x_vel * delta_time;
    float y_intended = e.y + e.y_vel * delta_time;
    return x_intended < o.x + o.width && x_intended + e.width > o.x && y_intended < o.y + o.height && y_intended + e.height > o.y;
}

bool checkXCollision(Entity e, Obstacle o, float delta_time)
{
    float x_intended = e.x + e.x_vel * delta_time;
    return x_intended < o.x + o.width && x_intended + e.width > o.x && e.y < o.y + o.height && e.y + e.height > o.y;
}

bool checkXCollision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2, float delta_time, float vel)
{
    float x_intended = x1 + vel * delta_time;
    return checkCollision(x_intended, y1, w1, h1, x2, y2, w2, h2);
}

bool checkYCollision(Entity e, Obstacle o, float delta_time)
{
    float y_intended = e.y + e.y_vel * delta_time;
    return e.x < o.x + o.width && e.x + e.width > o.x && y_intended < o.y + o.height && y_intended + e.height > o.y;
}

bool checkYCollision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2, float delta_time, float vel)
{
    float y_intended = y1 + vel * delta_time;
    return checkCollision(x1, y_intended, w1, h1, x2, y2, w2, h2);
}

float calculateXDistance(Entity e, Obstacle o)
{
    float dx = 0;
    if(e.x < o.x)
    {
        dx = o.x - (e.x + e.width);
    }
    else if(e.x > o.x)
    {
        dx = e.x - (o.x + o.width);
    }
    return dx;
}

float CalculateXDistance(float x1, int w1, float x2, int w2)
{
    float dx = 0;
    if(x1 < x2)
    {
        dx = x2 - (x1 + w2);
    }
    else if(x1 > x2)
    {
        dx = x1 - (x2 + w2);
    }
    return dx;
}

float calculateXDistance(Obstacle e, Obstacle o)
{
    float dx = 0;
    if(e.x < o.x)
    {
        dx = o.x - (e.x + e.width);
    }
    else if(e.x > o.x)
    {
        dx = e.x - (o.x + o.width);
    }
    return dx;
}

float calculateYDistance(Entity e, Obstacle o)
{
    float dy = 0;
    if(e.y < o.y)
    {
        dy = o.y - (e.y + e.height);
    }
    else if(e.y > o.y)
    {
        dy = e.y - (o.y + o.height);
    }
    return dy;
}

float CalculateYDistance(float y1, int h1, float y2, int h2)
{
    float dy = 0;
    if(y1 < y2)
    {
        dy = y2 - (y1 + h1);
    }
    else if(y1 > y2)
    {
        dy = y1 - (y2 + h2);
    }
    return dy;
}

float calculateYDistance(Obstacle e, Obstacle o)
{
    float dy = 0;
    if(e.y < o.y)
    {
        dy = o.y - (e.y + e.height);
    }
    else if(e.y > o.y)
    {
        dy = e.y - (o.y + o.height);
    }
    return dy;
}
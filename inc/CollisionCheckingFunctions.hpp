#ifndef COLLISION_CHECKING_FUNCTIONS_HPP
#define COLLISION_CHECKING_FUNCTIONS_HPP

#include "GameDataStructures.hpp"

bool checkXYCollision(Entity e, Obstacle o, float delta_time);
bool checkCollision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);
bool checkXCollision(Entity e, Obstacle o, float delta_time);
bool checkXCollision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2, float delta_time, float vel);
bool checkYCollision(Entity e, Obstacle o, float delta_time);
bool checkYCollision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2, float delta_time, float vel);
float CalculateXDistance(float x1, int w1, float x2, int w2);
float calculateXDistance(Entity e, Obstacle o);
float calculateXDistance(Obstacle e, Obstacle o);
float calculateYDistance(Entity e, Obstacle o);
float calculateYDistance(Obstacle e, Obstacle o);
void calculateTimeToCollide(float x_vel, float y_vel, float dx, float dy, float& x_time, float& y_time);
bool checkCollision(Entity e, Collectible c);
bool checkCollision(Entity en, Exit ex);

#endif
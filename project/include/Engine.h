#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>
#include <cmath>
#include <vector>
#include <string>

#include "defines.h"

using namespace std;

SDL_Texture* LoadTexture(string file, SDL_Renderer* renderer);
SDL_Point* LoadPoint(coordinates coor);
float triangleArea (int x1, int y1, int x2, int y2, int x3, int y3);
bool isInsideATriangle (SDL_Point* triangle1, SDL_Point* triangle2, SDL_Point* triangle3, SDL_Point* mousePoint);
bool isInsideAHexagon (vector<SDL_Point*> points, SDL_Point* mousePoint);


void CoordinatesToRect(coordinates coor, SDL_Rect& rect);
#endif // ENGINE_H

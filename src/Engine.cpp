#include "Engine.h"

SDL_Texture* LoadTexture(string file, SDL_Renderer* renderer)
{
    SDL_Texture* objectTexture;
    file = "img\\" + file;

    SDL_Surface* loadingSurface = SDL_LoadBMP(file.c_str());
    objectTexture = SDL_CreateTextureFromSurface(renderer, loadingSurface);
    SDL_FreeSurface(loadingSurface);

    return objectTexture;
}

SDL_Point* LoadPoint(coordinates coor)
{
    SDL_Point* point = new SDL_Point;
    point->x = coor.x;
    point->y = coor.y;

    return point;
}

void CoordinatesToRect(coordinates coor, SDL_Rect& rect)
{
    rect.x = coor.x;
    rect.y = coor.y;
}

float triangleArea(SDL_Point* point1, SDL_Point* point2, SDL_Point* point3)
{
   return abs((point1->x * (point2->y - point3->y) + point2->x * (point3 -> y - point1->y) + point3 -> x*(point1->y - point2->y))/2.0);
}

bool isInsideATriangle(SDL_Point* triangle1, SDL_Point* triangle2, SDL_Point* triangle3, SDL_Point* mousePoint)
{
   float A = triangleArea (triangle1, triangle2, triangle3);

   float A1 = triangleArea (mousePoint, triangle2, triangle3);

   float A2 = triangleArea (triangle1, mousePoint, triangle3);

   float A3 = triangleArea (triangle1, triangle2, mousePoint);

   return (A == A1 + A2 + A3);
}

bool isInsideAHexagon(vector<SDL_Point*> points, SDL_Point* mousePoint)
{
    for(int i = 2; i < points.size(); i++)
    {
        if(isInsideATriangle(points[0], points[1], points[i], mousePoint) == true)
        {
            return true;
        }
    }
    return false;
}

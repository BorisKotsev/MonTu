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

int hexDistance(coordinates a, coordinates b)
{
    return (abs(a.x - b.x) + abs(a.x + a.y - b.x - b.y) + abs(a.y - b.y)) / 2;
}

float triangleArea(SDL_Point* point1, SDL_Point* point2, SDL_Point* point3)
{
   return abs((point1->x * (point2->y - point3->y) + point2->x * (point3 -> y - point1->y) + point3 -> x*(point1->y - point2->y))/2.0);
}

float returnAngleByCoordinates(coordinates direction)
{

    return atan2(direction.x, -1*direction.y) * 180 / PI;
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
    for(int i = 0; i < points.size(); i++)
    {
        for(int j = i + 1; j < points.size(); j++)
        {
            for(int m = j + 1; m < points.size(); m++)
            {
                if(isInsideATriangle(points[i], points[j], points[m], mousePoint) == true)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool checkForMouseCollision(int mouseX, int mouseY, SDL_Rect object)
{
    if(mouseX > object.x && mouseX < object.x + object.w && mouseY > object.y && mouseY < object.y + object.h)
    {
        return true;
    }
    return false;
}

bool checkForPossibleMove(int distance, int &movement)
{
    if(distance <= movement)
    {
        return true;
        /// movement -= distance;
    }
    return false;
}

void write(string text, coordinates coor, SDL_Renderer* renderer, int FONT_SIZE)
{
    SDL_Texture* texture;
    SDL_Surface* surface;
    SDL_Rect rect;
    SDL_Color fcolor;
    TTF_Font* font;

    string str = "ttf/Roboto-Regular.ttf";
    font = TTF_OpenFont(str.c_str(), FONT_SIZE);

    if (font == NULL)
    {
        fprintf(stderr, "error: font not found\n");
        exit(EXIT_FAILURE);
    }

    fcolor.r = 255;
    fcolor.g = 255;
    fcolor.b = 255;
    const char* t = text.c_str();
    surface = TTF_RenderText_Solid(font, t, fcolor);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    rect.w = surface->w;
    rect.h = surface->h;
    rect.x = coor.x;
    rect.y = coor.y;
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}


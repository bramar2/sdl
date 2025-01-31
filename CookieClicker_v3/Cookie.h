#pragma once
#include <SDL3/SDL.h>

extern SDL_Surface *CookieSurface;

void Cookie_Init(SDL_Renderer *renderer);
void Cookie_Render(SDL_Renderer *renderer);
bool Cookie_Contains(SDL_Renderer *renderer, int x, int y);
bool Cookie_ContainsCursor(SDL_Renderer *renderer);
void Cookie_Destroy();
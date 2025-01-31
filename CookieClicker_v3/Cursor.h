#pragma once
#include <SDL3/SDL_render.h>

void Cursor_Init();
void Cursor_Loop(SDL_Renderer *renderer);
void Cursor_SetDefault();
void Cursor_SetPointer();
void Cursor_Destroy();
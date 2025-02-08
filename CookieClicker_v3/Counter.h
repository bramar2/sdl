#pragma once
#include <SDL3/SDL.h>


extern const int Counter_Digits;
extern const int Counter_MaxCount;
extern const int Counter_DigitWidth;
extern const int Counter_DigitHeight;
extern const int Counter_DigitThickness;
extern const int Counter_DigitDistance;
extern const int Counter_DigitColor[4];
extern const int Counter_Width;

void Counter_Init();
void Counter_AnimateChange(int currentValue, int targetValue);
void Counter_RenderStatic(SDL_Renderer *renderer, int clickCount, int x, int y);
void Counter_RenderAnimation(SDL_Renderer *renderer, const int *backgroundColor);
void Counter_Loop(SDL_Renderer *renderer);
int Counter_PosY();
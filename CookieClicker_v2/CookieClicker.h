#include <SDL3/SDL_render.h>
#include <stdio.h>


extern const char *APP_TITLE;
extern int ClickCount;

void App_Initialize();
void App_Render();
void App_EventLoop();
void App_OnClick();
void App_Quit();
void App_UpdateCursor();
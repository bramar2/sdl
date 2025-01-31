#include "Cookie.h"
#include "CookieClicker.h"
#include "Counter.h"
#include "Cursor.h"
#include "Screen.h"
#include "Utils.h"

#include <assert.h>
#include <time.h>
#include <stdlib.h>

int ScreenWidth = 640;
int ScreenHeight = 640;

const char *APP_TITLE = "Cookie Clicker";
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int ClickCount = 0;
const int BackgroundColor[4] = {0, 0, 0, 255};

void App_Initialize() {
	if(!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO)) {
		SDL_Log("error initializing SDL: %s\n", SDL_GetError());
		return;
	}
	if(!SDL_CreateWindowAndRenderer(APP_TITLE, ScreenWidth, ScreenHeight, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return;
    }
    
    Cursor_Init();
    Counter_Init();
    Cookie_Init(renderer);
    SDL_SetWindowIcon(window, CookieSurface);

    App_Render();
}

void App_Render() {
	SDL_RenderClear(renderer);


	SDL_FRect rect = {0, 0, ScreenWidth, ScreenHeight};
	SDL_SetRenderDrawColor(renderer, BackgroundColor[0], BackgroundColor[1], BackgroundColor[2], BackgroundColor[3]);
    SDL_RenderFillRect(renderer, &rect);

    Counter_RenderStatic(renderer, ClickCount, Utils_MiddleWidth(Counter_Width), Counter_PosY);
    Counter_RenderAnimation(renderer, BackgroundColor);
    Cookie_Render(renderer);

    
    SDL_RenderPresent(renderer);
}

void App_OnClick() {
	if(Cookie_ContainsCursor(renderer)) {
		Counter_AnimateChange(ClickCount, ClickCount + 1);
		ClickCount++;
		App_Render();
	}
}

void App_EventLoop() {
	SDL_Event event;
    bool running = 1;
    while(running) {
    	while(SDL_PollEvent(&event)) {
	    	switch(event.type) {
	    	case SDL_EVENT_QUIT:
	    		running = 0;
	    		break;
	    	case SDL_EVENT_MOUSE_BUTTON_UP:
	    		App_OnClick();
	    		break;
	    	default:
	    		break;
	    	}
	    }
	    SDL_Delay(2);
	    Counter_Loop(renderer);
	    Cursor_Loop(renderer);
	    App_Render();
    }
    App_Quit();
}

void App_Quit() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	Cookie_Destroy();
	Cursor_Destroy();

	SDL_Quit();
}

int main(int argc, char **argv) {
	App_Initialize();
	App_Render();
	App_EventLoop();
	return 0;
}
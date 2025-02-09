#include "Cookie.h"
#include "Counter.h"
#include "Cursor.h"
#include "SDL3/SDL_events.h"
#include "Screen.h"
#include "Utils.h"

#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int ScreenWidth = 640;
int ScreenHeight = 640;

const char *APP_TITLE = "Cookie Clicker";
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int ClickCount = 0;
const int BackgroundColor[4] = {0, 0, 0, 255};

const int FRAME_TIMES_SZ = 10000;
Uint32 frame_times[10000];
size_t frameStart = 0, frameSz = 0;

const char *DBG_EXTRA = " FPS | github.com/bramar2/sdl";

void App_Render();

void App_Initialize() {
	if(!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO)) {
		SDL_Log("error initializing SDL: %s\n", SDL_GetError());
		return;
	}
	if(!SDL_CreateWindowAndRenderer(APP_TITLE, ScreenWidth, ScreenHeight, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return;
    }
    SDL_SetRenderVSync(renderer, 1);
    
    Cursor_Init();
    Counter_Init();
    Cookie_Init(renderer);
    SDL_SetWindowIcon(window, CookieSurface);

    frame_times[0] = 0;
    frameStart = frameSz = 0;
    App_Render();
}

void App_Render() {

	SDL_RenderClear(renderer);


	SDL_FRect rect = {0, 0, ScreenWidth, ScreenHeight};
	SDL_SetRenderDrawColor(renderer, BackgroundColor[0], BackgroundColor[1], BackgroundColor[2], BackgroundColor[3]);
    SDL_RenderFillRect(renderer, &rect);

    Counter_RenderStatic(renderer, ClickCount, Utils_MiddleWidth(Counter_Width), Counter_PosY());
    Counter_RenderAnimation(renderer, BackgroundColor);
    Cookie_Render(renderer);


    Uint32 curr = SDL_GetTicks();
    while(frameSz > 0) {
    	if(frame_times[frameStart] < curr - 1000) {
    		frameStart = (frameStart + 1) % FRAME_TIMES_SZ;
    		frameSz--;
    	}else {
    		break;
    	}
    }
    
    char dbg_text[40];
    sprintf(dbg_text, "%i", (int) frameSz);
    uint64_t len = strlen(dbg_text);
    uint64_t extra_len = strlen(DBG_EXTRA);
    for(size_t i = 0; i < extra_len; i++) {
    	dbg_text[len + i] = DBG_EXTRA[i];
    }
    dbg_text[len + extra_len] = '\0';
    SDL_RenderDebugText(renderer, 0, 0, dbg_text);

    SDL_RenderPresent(renderer);

    frame_times[(frameStart + frameSz) % FRAME_TIMES_SZ] = curr;
    frameSz++;
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
	    	case SDL_EVENT_WINDOW_RESIZED:
	    		ScreenWidth = event.window.data1;
	    		ScreenHeight = event.window.data2;
	    		break;
	    	default:
	    		break;
	    	}
	    }
	    Counter_Loop(renderer);
	    Cursor_Loop(renderer);
	    App_Render();
    }
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
	App_Quit();
	return 0;
}
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_oldnames.h"
#include "SDL3/SDL_timer.h"
#include "SDL3/SDL_video.h"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_events.h>
#include <stdio.h>

const unsigned SDL_INIT_EVERYTHING = SDL_INIT_AUDIO | SDL_INIT_CAMERA | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD | SDL_INIT_HAPTIC | SDL_INIT_JOYSTICK | SDL_INIT_SENSOR | SDL_INIT_VIDEO;


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;
int main() {
	SDL_Window *window;
	SDL_Renderer *renderer;
	if(!SDL_Init(SDL_INIT_EVERYTHING)) {
		SDL_Log("error initializing SDL: %s\n", SDL_GetError());
		return 1;
	}
	if(!SDL_CreateWindowAndRenderer("Test", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return 1;
    }
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_FRect rect;
    rect.x = 0, rect.y = 0, rect.w = 250, rect.h = 250;
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);
    SDL_Event event;
    bool running = 1;
    while(running) {
    	while(SDL_PollEvent(&event)) {
	    	switch(event.type) {
	    	case SDL_EVENT_QUIT:
	    		running = 0;
	    		break;
	    	default:
	    		SDL_Log("Received event: %d", event.type);
	    	}
	    }
	    SDL_Delay(8);
    }

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
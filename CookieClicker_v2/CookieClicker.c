#include "CookieBmp.h"
#include "CookieClicker.h"
#include "SDL3/SDL_iostream.h"
#include <assert.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <SDL3/SDL.h>


typedef struct {
	int X, Y;
	int From, To;
	int Progress;
} AnimationData;

const char *APP_TITLE = "Cookie Clicker";
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
int ClickCount = 0;
const int CookieBopTime = 2500;
const float CookieBopRoom = 0.125;
const float CookieBaseSize = 1.2;
const int CounterDigits = 9;
const int MaxClickCount = 999999999;
const int DigitWidth = 32;
const int DigitHeight = 64;
const int DigitThickness = 4;
const int DigitDistance = DigitWidth + DigitThickness + 8;
const int DigitColor[4] = {255, 255, 255, 255};
const int BackgroundColor[4] = {0, 0, 0, 255};
int ScreenWidth = 640;
int ScreenHeight = 640;
SDL_Surface* CookieSurface;
SDL_Texture* CookieTexture;

SDL_Cursor* CursorDefault;
SDL_Cursor* CursorPointer;

int CounterY;
int CookieY;

const bool AnimationSpeedUpMultiDigit = true;
const int AnimationPixelBuffer = 24;
const int AnimationProgressEnd = 50;
const Uint64 AnimateTick = 3;
Uint64 LastAnimate = 0;
AnimationData RunningAnimation[32];
size_t AnimationSize = 0;

const Uint64 CursorUpdateLoop = 20;
Uint64 LastCursorUpdate = 0;


void UseColor(const int *color) {
	SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], color[3]);
}

void RenderRect(int x, int y, int w, int h) {
	static SDL_FRect tmp;
	tmp.x = x, tmp.y = y, tmp.w = w, tmp.h = h;
	SDL_RenderFillRect(renderer, &tmp);
}

void RenderDigit(int x, int y, int digit) {
	assert(0 <= digit && digit <= 9);
	UseColor(DigitColor);
	int mid_y = (y + (y + DigitHeight - DigitThickness)) / 2;
	switch(digit) {
	case 0:
		RenderRect(x, y, DigitWidth, DigitThickness);
		RenderRect(x, y, DigitThickness, DigitHeight);
		RenderRect(x + DigitWidth - DigitThickness, y, DigitThickness, DigitHeight);
		RenderRect(x, y + DigitHeight - DigitThickness, DigitWidth, DigitThickness);
		break;
	case 1:
		RenderRect(x + DigitWidth - DigitThickness, y, DigitThickness, DigitHeight);
		break;
	case 2:
		RenderRect(x, y, DigitWidth, DigitThickness);
		RenderRect(x, y + DigitHeight - DigitThickness, DigitWidth, DigitThickness);
		RenderRect(x, mid_y, DigitWidth, DigitThickness);

		RenderRect(x + DigitWidth - DigitThickness, y, DigitThickness, DigitHeight / 2);
		RenderRect(x, mid_y, DigitThickness, DigitHeight / 2);
		break;
	case 3:
		RenderRect(x, y, DigitWidth, DigitThickness);
		RenderRect(x, y + DigitHeight - DigitThickness, DigitWidth, DigitThickness);
		RenderRect(x, mid_y, DigitWidth, DigitThickness);

		RenderRect(x + DigitWidth - DigitThickness, y, DigitThickness, DigitHeight);
		break;
	case 4:
		RenderRect(x, y, DigitThickness, DigitHeight / 2);
		RenderRect(x, mid_y, DigitWidth, DigitThickness);
		RenderRect(x + DigitWidth - DigitThickness, y, DigitThickness, DigitHeight);
		break;
	case 5:
		RenderRect(x, y, DigitWidth, DigitThickness);
		RenderRect(x, y, DigitThickness, DigitHeight / 2);
		RenderRect(x, mid_y, DigitWidth, DigitThickness);
		RenderRect(x + DigitWidth - DigitThickness, mid_y, DigitThickness, DigitHeight / 2);
		RenderRect(x, y + DigitHeight - DigitThickness, DigitWidth, DigitThickness);
		break;
	case 6:
		RenderRect(x, y, DigitWidth, DigitThickness);
		RenderRect(x, y, DigitThickness, DigitHeight / 2);
		RenderRect(x, mid_y, DigitWidth, DigitThickness);
		RenderRect(x, mid_y, DigitThickness, DigitHeight / 2);
		RenderRect(x + DigitWidth - DigitThickness, mid_y, DigitThickness, DigitHeight / 2);
		RenderRect(x, y + DigitHeight - DigitThickness, DigitWidth, DigitThickness);
		break;
	case 7:
		RenderRect(x, y, DigitWidth, DigitThickness);
		RenderRect(x + DigitWidth - DigitThickness, y, DigitThickness, DigitHeight);
		break;
	case 8:
		RenderRect(x, y, DigitWidth, DigitThickness);
		RenderRect(x, y + DigitHeight - DigitThickness, DigitWidth, DigitThickness);
		RenderRect(x, mid_y, DigitWidth, DigitThickness);

		RenderRect(x, y, DigitThickness, DigitHeight);
		RenderRect(x + DigitWidth - DigitThickness, y, DigitThickness, DigitHeight);
		break;
	case 9:
		RenderRect(x, y, DigitWidth, DigitThickness);
		RenderRect(x, y + DigitHeight - DigitThickness, DigitWidth, DigitThickness);
		RenderRect(x, mid_y, DigitWidth, DigitThickness);
		
		RenderRect(x, y, DigitThickness, DigitHeight / 2);
		RenderRect(x + DigitWidth - DigitThickness, y, DigitThickness, DigitHeight);
		break;
	default:
		break;
	}
}

void RenderDigitAnimation(AnimationData *data) {
	if(data->Progress > AnimationProgressEnd) data->Progress = AnimationProgressEnd;
	int nextDigit = (data->From + 1) % 10;
	UseColor(BackgroundColor);
	RenderRect(data->X, data->Y, DigitWidth, DigitHeight);
	if(data->Progress == 0) {
		RenderDigit(data->X, data->Y, data->From);
	}else if(data->Progress == AnimationProgressEnd) {
		RenderDigit(data->X, data->Y, nextDigit);
	}else {
		RenderDigit(data->X, data->Y - (DigitHeight + AnimationPixelBuffer) * data->Progress / AnimationProgressEnd, data->From);
		RenderDigit(data->X, data->Y + DigitHeight - DigitHeight * data->Progress / AnimationProgressEnd, nextDigit);

		int top_y = data->Y - DigitHeight - AnimationPixelBuffer;
		int bottom_y = data->Y + DigitHeight;
		if(top_y < 0) top_y = 0;

		UseColor(BackgroundColor);
		RenderRect(data->X, top_y, DigitWidth, DigitHeight + AnimationPixelBuffer);
		RenderRect(data->X, bottom_y, DigitWidth, DigitHeight);
	}
}

void RenderCount(int x, int y) {
	if(ClickCount < 0) {
		ClickCount = 0;
	}
	if(ClickCount >= MaxClickCount) {
		ClickCount = MaxClickCount;
	}

	if(ClickCount == 0) {
		for(int i = 0; i < CounterDigits; i++) {
			RenderDigit(x + i * DigitDistance, y, 0);
		}
	}else {
		int position = x + (CounterDigits - 1) * DigitDistance;

		for(int cnt = ClickCount; cnt > 0; cnt /= 10) {
			int digit = cnt % 10;
			RenderDigit(position, y, digit);
			position -= DigitDistance;
		}

		int digitCnt = 1 + log10(ClickCount + 0.1);
		while(digitCnt < CounterDigits) {
			RenderDigit(position, y, 0);
			position -= DigitDistance;
			digitCnt++;
		}
	}
}

void ProgressAnimation() {
	size_t newSize = AnimationSize;
	for(size_t i = 0, moveIdx = 0; i < AnimationSize; i++, moveIdx++) {
		if(RunningAnimation[i].Progress >= AnimationProgressEnd) {
			RunningAnimation[i].From++;
			RunningAnimation[i].From %= 10;
			if(RunningAnimation[i].From == RunningAnimation[i].To) {
				moveIdx--;
				newSize--;
				continue;
			}
			RunningAnimation[i].Progress = 0;
		}else {
			if(AnimationSpeedUpMultiDigit) {
				int digitsCount = (RunningAnimation[i].To - RunningAnimation[i].From + 10) % 10;
				RunningAnimation[i].Progress += digitsCount;
			}else {
				RunningAnimation[i].Progress++;
			}
		}
		if(i != moveIdx) RunningAnimation[moveIdx] = RunningAnimation[i];
	}
	AnimationSize = newSize;
}

void AddAnimation(int x, int y, int from, int to) {
	for(size_t i = 0; i < AnimationSize; i++) {
		if(RunningAnimation[i].X == x && RunningAnimation[i].Y == y) {
			RunningAnimation[i].To = to;
			return;
		}
	}
	RunningAnimation[AnimationSize++] = (AnimationData) {x, y, from, to, 0};
}

int GetCounterWidth() {  
	return (CounterDigits - 1) * DigitDistance + DigitWidth;
}

int MiddleWidth(int width) {
	return (ScreenWidth - width) / 2;
}

void GetPixelColor(int x, int y, Uint8 *r, Uint8 *g, Uint8 *b) {
	SDL_Rect rect = {x, y, 1, 1};
	SDL_Surface *surface = SDL_RenderReadPixels(renderer, &rect);
	const SDL_PixelFormatDetails *formatDetail = SDL_GetPixelFormatDetails(surface->format);
	SDL_GetRGB(((Uint32*) surface->pixels)[0], formatDetail, NULL, r, g, b);
	SDL_DestroySurface(surface);
}

void App_Initialize() {
	if(!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO)) {
		SDL_Log("error initializing SDL: %s\n", SDL_GetError());
		return;
	}
	if(!SDL_CreateWindowAndRenderer(APP_TITLE, ScreenWidth, ScreenHeight, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return;
    }
    SDL_IOStream *stream = SDL_IOFromConstMem(CookieBmp_mem, CookieBmp_len);
    CookieSurface = SDL_LoadBMP_IO(stream, true);
    CookieTexture = SDL_CreateTextureFromSurface(renderer, CookieSurface);


    SDL_SetWindowIcon(window, CookieSurface);

    CounterY = ScreenHeight * 15 / 100;
    CookieY = (CounterY + DigitHeight) + ScreenHeight / 10;

    CursorDefault = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
    CursorPointer = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER);

    App_Render();
}

void App_Render() {
	SDL_RenderClear(renderer);

    UseColor(BackgroundColor);
    RenderRect(0, 0, ScreenWidth, ScreenHeight);

    // cw + 2*side = screen width
    // side = (screen width - cw) / 2
    // pos = side
    RenderCount(MiddleWidth(GetCounterWidth()), CounterY);

    // SDL_SetRenderDrawColor(renderer, 15, 100, 15, 100);
    // RenderRect(100, 200, 100, 100);

    float mult = CookieBaseSize;
    int cookieState = SDL_GetTicks() % CookieBopTime;
    int halfCookieBop = CookieBopTime / 2;
    if(cookieState < halfCookieBop) {
    	mult += CookieBopRoom * cookieState / halfCookieBop;
    }else {
    	mult += CookieBopRoom;
    	mult += CookieBopRoom * (halfCookieBop - cookieState) / halfCookieBop;
    }
    SDL_FRect destrect = {
    	MiddleWidth(CookieTexture->w) - (mult - 1) * CookieTexture->w / 2,
    	CookieY - (mult - 1) * CookieTexture->h / 2,
    	mult * CookieTexture->w,
    	mult * CookieTexture->h};
    SDL_RenderTexture(renderer, CookieTexture, NULL, &destrect);

    // animations

    for(size_t i = 0; i < AnimationSize; i++) {
    	RenderDigitAnimation(&RunningAnimation[i]);
    }

    //
    SDL_RenderPresent(renderer);
}

bool MouseOnCookie() {
	float x, y;
	SDL_GetMouseState(&x, &y);
	if(y <= (CounterY + DigitHeight + 5)) return false;
	Uint8 r, g, b;
	GetPixelColor(x, y, &r, &g, &b);
	if(r == 0 && g == 0 && b == 0) return false;
	return true;
}

void App_UpdateCursor() {
	if(MouseOnCookie()) {
		SDL_SetCursor(CursorPointer);
	}else {
		SDL_SetCursor(CursorDefault);
	}
}

void App_OnClick() {
	if(MouseOnCookie()) {
		char before[10], after[10];
		itoa(ClickCount, before, 10);
		ClickCount++;
		itoa(ClickCount, after, 10);

		int beforeLength = strlen(before);
		int afterLength = strlen(after);
		int position = MiddleWidth(GetCounterWidth()) + (CounterDigits - 1) * DigitDistance;
		while(beforeLength > 0 && afterLength > 0) {
			if(before[beforeLength - 1] != after[afterLength - 1]) {
				AddAnimation(position, CounterY, before[beforeLength - 1] - '0', after[afterLength - 1] - '0');
			}

			position -= DigitDistance;
			beforeLength--, afterLength--;
		}
		while(beforeLength > 0) {
			if(before[beforeLength - 1] != '0') {
				AddAnimation(position, CounterY, before[beforeLength - 1] - '0', 0);
			}

			position -= DigitDistance;
			beforeLength--;
		}
		while(afterLength > 0) {
			if(after[afterLength - 1] != '0') {
				AddAnimation(position, CounterY, 0, after[afterLength - 1] - '0');
			}

			position -= DigitDistance;
			afterLength--;
		}
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
	    SDL_Delay(1);
	    if(SDL_GetTicks() - LastAnimate >= AnimateTick) {
	    	LastAnimate = SDL_GetTicks();
	    	ProgressAnimation();
	    	App_Render();
	    }
	    if(SDL_GetTicks() - LastCursorUpdate >= CursorUpdateLoop) {
	    	LastCursorUpdate = SDL_GetTicks();
	    	App_UpdateCursor();
	    }
    }
    App_Quit();
}

void App_Quit() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_DestroySurface(CookieSurface);
	SDL_DestroyTexture(CookieTexture);
	SDL_DestroyCursor(CursorDefault);
	SDL_DestroyCursor(CursorPointer);
	SDL_Quit();
}

int main(int argc, char **argv) {
	App_Initialize();
	App_Render();
	App_EventLoop();
	return 0;
}
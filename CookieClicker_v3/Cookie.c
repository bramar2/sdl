#include "Cookie.h"
#include "CookieBmp.h"
#include "Counter.h"
#include "Screen.h"
#include "Utils.h"

const int CookieBopTime = 2500;
const float CookieBopRoom = 0.125;
const float CookieBaseSize = 1.2;
int CookieY;

SDL_Surface *CookieSurface;
SDL_Texture *CookieTexture;

void Cookie_Init(SDL_Renderer *renderer) {
	CookieY = (Counter_PosY + Counter_DigitHeight) + ScreenHeight / 10;
	SDL_IOStream *stream = SDL_IOFromConstMem(CookieBmp_mem, CookieBmp_len);
    CookieSurface = SDL_LoadBMP_IO(stream, true);
    CookieTexture = SDL_CreateTextureFromSurface(renderer, CookieSurface);
}

void Cookie_Render(SDL_Renderer *renderer) {
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
    	Utils_MiddleWidth(CookieTexture->w) - (mult - 1) * CookieTexture->w / 2,
    	CookieY - (mult - 1) * CookieTexture->h / 2,
    	mult * CookieTexture->w,
    	mult * CookieTexture->h};
    SDL_RenderTexture(renderer, CookieTexture, NULL, &destrect);
}

bool Cookie_Contains(SDL_Renderer *renderer, int x, int y) {
	if(y <= (Counter_PosY + Counter_DigitHeight + 5)) return false;
	Uint8 r, g, b;

	SDL_Rect rect = {x, y, 1, 1};
	SDL_Surface *surface = SDL_RenderReadPixels(renderer, &rect);
	const SDL_PixelFormatDetails *formatDetail = SDL_GetPixelFormatDetails(surface->format);
	SDL_GetRGB(((Uint32*) surface->pixels)[0], formatDetail, NULL, &r, &g, &b);
	SDL_DestroySurface(surface);

	if(r == 0 && g == 0 && b == 0) return false;
	return true;
}

bool Cookie_ContainsCursor(SDL_Renderer *renderer) {
	float x, y;
	SDL_GetMouseState(&x, &y);
	return Cookie_Contains(renderer, (int) x, (int) y);
}

void Cookie_Destroy() {
	SDL_DestroySurface(CookieSurface);
	SDL_DestroyTexture(CookieTexture);
}
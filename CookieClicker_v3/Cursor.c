#include "Cursor.h"
#include "Cookie.h"

SDL_Cursor *CursorDefault;
SDL_Cursor *CursorPointer;

const Uint64 CursorUpdateLoop = 20;
Uint64 LastCursorUpdate = 0;

void Cursor_Init() {
	CursorDefault = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
    CursorPointer = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER);
}
void Cursor_Loop(SDL_Renderer *renderer) {
	int ticks = SDL_GetTicks();
	if(ticks - LastCursorUpdate >= CursorUpdateLoop) {
    	LastCursorUpdate = ticks;
    	if(Cookie_ContainsCursor(renderer)) {
    		Cursor_SetPointer();
    	}else {
    		Cursor_SetDefault();
    	}
    }
}
void Cursor_SetDefault() {
	SDL_SetCursor(CursorDefault);
}
void Cursor_SetPointer() {
	SDL_SetCursor(CursorPointer);
}
void Cursor_Destroy() {
	SDL_DestroyCursor(CursorDefault);
	SDL_DestroyCursor(CursorPointer);
}
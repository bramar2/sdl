#include "Counter.h"
#include "Screen.h"
#include "Utils.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>

typedef struct {
	int X, Y;
	int From, To;
	int Progress;
} AnimationData;

const int Counter_Digits = 9;
const int Counter_MaxCount = 999999999;
const int Counter_DigitWidth = 32;
const int Counter_DigitHeight = 64;
const int Counter_DigitThickness = 4;
const int Counter_DigitDistance = Counter_DigitWidth + Counter_DigitThickness + 8;
const int Counter_DigitColor[4] = {255, 255, 255, 255};

const int Counter_Width = (Counter_Digits - 1) * Counter_DigitDistance + Counter_DigitWidth;
const bool AnimationSpeedUpMultiDigit = true;
const int AnimationPixelBuffer = 24;

AnimationData RunningAnimation[32];
size_t AnimationSize = 0;

const Uint64 AnimateProgressMs = 4;
const int AnimationProgressEnd = 66;
Uint64 LastAnimateTick = 0;

int Counter_PosY;

void RenderRect(SDL_Renderer *renderer, int x, int y, int w, int h) {
	SDL_FRect tmp = {x, y, w, h};
	SDL_RenderFillRect(renderer, &tmp);
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

void RenderDigit(SDL_Renderer *renderer, int x, int y, int digit) {
	assert(0 <= digit && digit <= 9);
	SDL_SetRenderDrawColor(renderer, Counter_DigitColor[0], Counter_DigitColor[1], Counter_DigitColor[2], Counter_DigitColor[3]);
	int mid_y = (y + (y + Counter_DigitHeight - Counter_DigitThickness)) / 2;
	switch(digit) {
	case 0:
		RenderRect(renderer, x, y, Counter_DigitWidth, Counter_DigitThickness);
		RenderRect(renderer, x, y, Counter_DigitThickness, Counter_DigitHeight);
		RenderRect(renderer, x + Counter_DigitWidth - Counter_DigitThickness, y, Counter_DigitThickness, Counter_DigitHeight);
		RenderRect(renderer, x, y + Counter_DigitHeight - Counter_DigitThickness, Counter_DigitWidth, Counter_DigitThickness);
		break;
	case 1:
		RenderRect(renderer, x + Counter_DigitWidth - Counter_DigitThickness, y, Counter_DigitThickness, Counter_DigitHeight);
		break;
	case 2:
		RenderRect(renderer, x, y, Counter_DigitWidth, Counter_DigitThickness);
		RenderRect(renderer, x, y + Counter_DigitHeight - Counter_DigitThickness, Counter_DigitWidth, Counter_DigitThickness);
		RenderRect(renderer, x, mid_y, Counter_DigitWidth, Counter_DigitThickness);

		RenderRect(renderer, x + Counter_DigitWidth - Counter_DigitThickness, y, Counter_DigitThickness, Counter_DigitHeight / 2);
		RenderRect(renderer, x, mid_y, Counter_DigitThickness, Counter_DigitHeight / 2);
		break;
	case 3:
		RenderRect(renderer, x, y, Counter_DigitWidth, Counter_DigitThickness);
		RenderRect(renderer, x, y + Counter_DigitHeight - Counter_DigitThickness, Counter_DigitWidth, Counter_DigitThickness);
		RenderRect(renderer, x, mid_y, Counter_DigitWidth, Counter_DigitThickness);

		RenderRect(renderer, x + Counter_DigitWidth - Counter_DigitThickness, y, Counter_DigitThickness, Counter_DigitHeight);
		break;
	case 4:
		RenderRect(renderer, x, y, Counter_DigitThickness, Counter_DigitHeight / 2);
		RenderRect(renderer, x, mid_y, Counter_DigitWidth, Counter_DigitThickness);
		RenderRect(renderer, x + Counter_DigitWidth - Counter_DigitThickness, y, Counter_DigitThickness, Counter_DigitHeight);
		break;
	case 5:
		RenderRect(renderer, x, y, Counter_DigitWidth, Counter_DigitThickness);
		RenderRect(renderer, x, y, Counter_DigitThickness, Counter_DigitHeight / 2);
		RenderRect(renderer, x, mid_y, Counter_DigitWidth, Counter_DigitThickness);
		RenderRect(renderer, x + Counter_DigitWidth - Counter_DigitThickness, mid_y, Counter_DigitThickness, Counter_DigitHeight / 2);
		RenderRect(renderer, x, y + Counter_DigitHeight - Counter_DigitThickness, Counter_DigitWidth, Counter_DigitThickness);
		break;
	case 6:
		RenderRect(renderer, x, y, Counter_DigitWidth, Counter_DigitThickness);
		RenderRect(renderer, x, y, Counter_DigitThickness, Counter_DigitHeight / 2);
		RenderRect(renderer, x, mid_y, Counter_DigitWidth, Counter_DigitThickness);
		RenderRect(renderer, x, mid_y, Counter_DigitThickness, Counter_DigitHeight / 2);
		RenderRect(renderer, x + Counter_DigitWidth - Counter_DigitThickness, mid_y, Counter_DigitThickness, Counter_DigitHeight / 2);
		RenderRect(renderer, x, y + Counter_DigitHeight - Counter_DigitThickness, Counter_DigitWidth, Counter_DigitThickness);
		break;
	case 7:
		RenderRect(renderer, x, y, Counter_DigitWidth, Counter_DigitThickness);
		RenderRect(renderer, x + Counter_DigitWidth - Counter_DigitThickness, y, Counter_DigitThickness, Counter_DigitHeight);
		break;
	case 8:
		RenderRect(renderer, x, y, Counter_DigitWidth, Counter_DigitThickness);
		RenderRect(renderer, x, y + Counter_DigitHeight - Counter_DigitThickness, Counter_DigitWidth, Counter_DigitThickness);
		RenderRect(renderer, x, mid_y, Counter_DigitWidth, Counter_DigitThickness);

		RenderRect(renderer, x, y, Counter_DigitThickness, Counter_DigitHeight);
		RenderRect(renderer, x + Counter_DigitWidth - Counter_DigitThickness, y, Counter_DigitThickness, Counter_DigitHeight);
		break;
	case 9:
		RenderRect(renderer, x, y, Counter_DigitWidth, Counter_DigitThickness);
		RenderRect(renderer, x, y + Counter_DigitHeight - Counter_DigitThickness, Counter_DigitWidth, Counter_DigitThickness);
		RenderRect(renderer, x, mid_y, Counter_DigitWidth, Counter_DigitThickness);
		
		RenderRect(renderer, x, y, Counter_DigitThickness, Counter_DigitHeight / 2);
		RenderRect(renderer, x + Counter_DigitWidth - Counter_DigitThickness, y, Counter_DigitThickness, Counter_DigitHeight);
		break;
	default:
		break;
	}
}

void RenderDigitAnimation(SDL_Renderer *renderer, const int *backgroundColor, AnimationData *data) {
	if(data->Progress > AnimationProgressEnd) data->Progress = AnimationProgressEnd;
	int nextDigit = (data->From + 1) % 10;
	SDL_SetRenderDrawColor(renderer, backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);
	RenderRect(renderer, data->X, data->Y, Counter_DigitWidth, Counter_DigitHeight);
	if(data->Progress == 0) {
		RenderDigit(renderer, data->X, data->Y, data->From);
	}else if(data->Progress == AnimationProgressEnd) {
		RenderDigit(renderer, data->X, data->Y, nextDigit);
	}else {
		RenderDigit(renderer, data->X, data->Y - (Counter_DigitHeight + AnimationPixelBuffer) * data->Progress / AnimationProgressEnd, data->From);
		RenderDigit(renderer, data->X, data->Y + Counter_DigitHeight - Counter_DigitHeight * data->Progress / AnimationProgressEnd, nextDigit);

		int top_y = data->Y - Counter_DigitHeight - AnimationPixelBuffer;
		int bottom_y = data->Y + Counter_DigitHeight;
		if(top_y < 0) top_y = 0;

		SDL_SetRenderDrawColor(renderer, backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);
		RenderRect(renderer, data->X, top_y, Counter_DigitWidth, Counter_DigitHeight + AnimationPixelBuffer);
		RenderRect(renderer, data->X, bottom_y, Counter_DigitWidth, Counter_DigitHeight);
	}
}

void Counter_Init() {
	Counter_PosY = ScreenWidth * 15 / 100;
}

void Counter_AnimateChange(int currentValue, int targetValue) {
	char current[10], target[10];
	itoa(currentValue, current, 10);
	itoa(targetValue, target, 10);
	int beforeLength = strlen(current);
	int afterLength = strlen(target);
	int position = Utils_MiddleWidth(Counter_Width) + (Counter_Digits - 1) * Counter_DigitDistance;
	while(beforeLength > 0 && afterLength > 0) {
		if(current[beforeLength - 1] != target[afterLength - 1]) {
			AddAnimation(position, Counter_PosY, current[beforeLength - 1] - '0', target[afterLength - 1] - '0');
		}

		position -= Counter_DigitDistance;
		beforeLength--, afterLength--;
	}
	while(beforeLength > 0) {
		if(current[beforeLength - 1] != '0') {
			AddAnimation(position, Counter_PosY, current[beforeLength - 1] - '0', 0);
		}

		position -= Counter_DigitDistance;
		beforeLength--;
	}
	while(afterLength > 0) {
		if(target[afterLength - 1] != '0') {
			AddAnimation(position, Counter_PosY, 0, target[afterLength - 1] - '0');
		}

		position -= Counter_DigitDistance;
		afterLength--;
	}
}


void Counter_RenderStatic(SDL_Renderer *renderer, int clickCount, int x, int y) {
	assert(0 <= clickCount && clickCount <= Counter_MaxCount);
	if(clickCount == 0) {
		for(int i = 0; i < Counter_Digits; i++) {
			RenderDigit(renderer, x + i * Counter_DigitDistance, y, 0);
		}
	}else {
		int position = x + (Counter_Digits - 1) * Counter_DigitDistance;

		for(int cnt = clickCount; cnt > 0; cnt /= 10) {
			int digit = cnt % 10;
			RenderDigit(renderer, position, y, digit);
			position -= Counter_DigitDistance;
		}

		int digitCnt = 1 + log10(clickCount + 0.1);
		while(digitCnt < Counter_Digits) {
			RenderDigit(renderer, position, y, 0);
			position -= Counter_DigitDistance;
			digitCnt++;
		}
	}
}

void Counter_RenderAnimation(SDL_Renderer *renderer, const int *backgroundColor) {
	for(size_t i = 0; i < AnimationSize; i++) {
		RenderDigitAnimation(renderer, backgroundColor, &RunningAnimation[i]);
	}
}

void Counter_Loop(SDL_Renderer *renderer) {
	Uint64 ticks = SDL_GetTicks();
	if(ticks - LastAnimateTick >= AnimateProgressMs) {
    	LastAnimateTick = ticks;
    	ProgressAnimation();
    }
}
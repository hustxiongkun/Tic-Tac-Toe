//Tic-Tac-Toe
//Version: 1.0
//Author: XiongKun
//Date: 2017-10-10

#include <SDL.h>
#include <stdio.h>
#include <string>

//Width and height for window
const int SCREEN_WIDTH = 630;
const int SCREEN_HEIGHT = 280;

//Initialize the SDL
bool init();
//Load media
bool loadMedia();
//Free dynamically memory
void close();
//Load image by path
SDL_Surface* loadSurface(std::string path);
//Start or restart the game
void restart();
//Judge that a point (x,y) is in a SDL_Rect or not
bool contain(SDL_Rect &r, int x, int y);
//When a player draw X or O, update the screen
void update(int x, int y);
//Judge that where there is a player win or not, after playing a step
bool isWin(int pos);

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
//Start screen
SDL_Surface* gStart = NULL;
//Players' images which they can draw on start screen
SDL_Surface* gPlayers[2] = { NULL };
//Note for whose turn to play
int gCurrentPlayer = 0;
SDL_Surface* gCurrentSurface = NULL;


//If the number of boxes if too big, could use 2d arrays and binary search
SDL_Rect gBox[10] = { {270,240,90,20},
	{ 210,10,70,70 },{ 280,10,70,70 },{ 350,10,70,70 },
	{ 210,80,70,70 },{ 280,80,70,70 },{ 350,80,70,70 },
	{ 210,150,70,70 },{ 280,150,70,70 },{ 350,150,70,70 }, };

//0 means this box could be used;
//1 means this box used by player 1;
//2 means this box used by player 2
int used[10] = { 0 }; 

bool init() {
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not be initialized! SDL_Error: %s\n", SDL_GetError());
		success = false;
	} else {
		gWindow = SDL_CreateWindow("Tic-Tac-Toe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		} else {
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}
	return success;
}

bool loadMedia() {
	bool success = true;
	gStart = loadSurface("../resource/images/start.bmp");
	if (gStart == NULL) {
		printf("Unable to load image start.bmp! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}

	gPlayers[0] = loadSurface("../resource/images/X.bmp");
	if (gStart == NULL) {
		printf("Unable to load image X.bmp! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}

	gPlayers[1] = loadSurface("../resource/images/O.bmp");
	if (gStart == NULL) {
		printf("Unable to load image O.bmp! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	return success;
}

SDL_Surface *loadSurface(std::string path) {
	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_Error: %s\n", path, SDL_GetError());
	}
	return loadedSurface;
}

void close() {
	SDL_FreeSurface(gStart);
	gStart = NULL;
	SDL_FreeSurface(gPlayers[0]);
	gPlayers[0] = NULL;
	SDL_FreeSurface(gPlayers[1]);
	gPlayers[1] = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
}

void restart() {
	SDL_BlitSurface(gStart, NULL, gScreenSurface, NULL);
	SDL_UpdateWindowSurface(gWindow);
	gCurrentPlayer = 0;
	gCurrentSurface = gPlayers[gCurrentPlayer];
	for (int i = 1; i < 10; i++) {
		used[i] = 0;
	}
}

bool contain(SDL_Rect &r, int x, int y) {
	bool in = false;
	if (x > r.x && x < r.x + r.w && y > r.y && y < r.y + r.h) {
		in = true;
	}
	return in;
}

void update(int x, int y) {
	for (int i = 1; i < 10; i++) {
		if (!used[i] && contain(gBox[i], x, y)) {
			SDL_Rect r = gBox[i];
			r.x = r.x + 3;
			r.y = r.y + 5;

			//Draw X or O
			SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, &r);
			SDL_UpdateWindowSurface(gWindow);

			//This box could not be used next time
			used[i] = gCurrentPlayer + 1;

			if (isWin(i)) {
				for (int i = 1; i < 10; i++) {
					if (!used[i]) {
						used[i] = -1;
					}
				}
				printf("Player %d win!\n", used[i]);
				//for (int j = 1; j < 10; j++) {
				//	printf("%d ", used[j]);
				//}
				//printf("\n");
			}

			//Turn to another player
			gCurrentPlayer = (gCurrentPlayer + 1) % 2;
			gCurrentSurface = gPlayers[gCurrentPlayer];

			return;
		}
	}
}
bool isWin(int pos) {
	bool win = false;
	//printf("--- box = %d, player = %d\n", pos, used[pos]);

	int x1, x2, x3;
	int temp;

	//Horizontal
	temp = (pos - 1) / 3;
	x1 = 1 + 3 * temp;
	x2 = 2 + 3 * temp;
	x3 = 3 + 3 * temp;
	if ((used[x1] == used[x2]) && (used[x2] == used[x3])) {
		win = true;
		return win;
	}
	//Vertical
	temp = (pos - 1) % 3;
	x1 = 1 + temp;
	x2 = 4 + temp;
	x3 = 7 + temp;
	if ((used[x1] == used[x2]) && (used[x2] == used[x3])) {
		win = true;
		return win;
	}
	//Diagonal
	if (pos % 4 == 1 && (used[1] == used[5]) && (used[5] == used[9])) {
		win = true;
		return win;
	}
	if ((pos % 4 == 3 || pos == 5) && (used[3] == used[5]) && (used[5] == used[7])) {
		win = true;
		return win;
	}

	return win;
}

int main(int argv, char* args[]) {
	if (!init()) {
		printf("Failed to initialize!\n");
		return -1;
	} else {
		if (!loadMedia()) {
			printf("Failed to load media!\n");
			return -1;
		} else {
			bool quit = false;
			SDL_Event e;

			restart();
			while (!quit) {
				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT) {
						quit = true;
					} else if (e.type == SDL_MOUSEBUTTONDOWN) {
						//printf("(%d, %d)\n", e.button.x, e.button.y);
						int x = e.button.x;
						int y = e.button.y;
						if (contain(gBox[0], x, y)) {
							restart();
						} else {
							update(x, y);
						}
					}
				}
			}
		}
	}
	close();

	return 0;
}
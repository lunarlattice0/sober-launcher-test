#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "main_bg.h"
#include "font.h"
#include <utility>
#include "logo.h"

#define WINDOW_TITLE "test"
#define WINDOW_HEIGHT 480
#define WINDOW_LENGTH 640

SDL_Surface *image;
SDL_Surface *screen;

int main() {
    bool quit = false;
	// Initialize window
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Init(SDL_INIT_TIMER);
    SDL_Init(SDL_INIT_EVENTS);
    SDL_WINDOWPOS_CENTERED_DISPLAY(0);
    SDL_Window *window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_LENGTH, WINDOW_HEIGHT,
	SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI
    );

	// Initialize Renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	// Load BMP and write to a surface and then texture
	SDL_RWops *RWop = SDL_RWFromConstMem(main_bg_bmp, main_bg_bmp_len);
    SDL_Surface *background_image = SDL_LoadBMP_RW(RWop, 1);
    SDL_Texture *bg_texture = SDL_CreateTextureFromSurface(renderer, background_image);

    SDL_RWops *logoRWop = SDL_RWFromConstMem(logo_bmp, logo_bmp_len);
    SDL_Surface *logo = SDL_LoadBMP_RW(logoRWop, 1);
    SDL_Texture *logo_texture = SDL_CreateTextureFromSurface(renderer, logo);

	// Set up textbox
	TTF_Init();
	SDL_RWops *fontRWop = SDL_RWFromConstMem(font_ttf, font_ttf_len);
	TTF_Font *font = TTF_OpenFontRW(fontRWop, 1, 12);
	SDL_Color color = {255,255,255,255};
	SDL_Surface *copyrightNotice = TTF_RenderText_Solid(font, "Copyright 2024 Sober V1.0.0", color);
	SDL_Surface *mainText = TTF_RenderText_Solid(font, "Please wait, loading...", color);
	SDL_Texture *textureCN = SDL_CreateTextureFromSurface(renderer, copyrightNotice);
	SDL_Texture *textureMT = SDL_CreateTextureFromSurface(renderer, mainText);

	// horiz positive = moving right
	// vert positive = moving down
	int horiz_motion = 1;
	int vert_motion = 1;
	SDL_Rect logoPositionRect = {(WINDOW_LENGTH - 200) / 2, (WINDOW_HEIGHT - 100) / 2, 200, 100};

	// mainloop
	while (!quit) {
    	SDL_Event event;

		while (SDL_PollEvent(&event)) {
		    switch (event.type) {
		    	// quit upon event
		        case SDL_QUIT:
		            quit = true;
		            break;
		    }
		}


		    // draw BG and borders
			// decrease scale by 10 for every 5 X/Y
			SDL_Rect bg_rect = {20, 40, WINDOW_LENGTH - 40, WINDOW_HEIGHT - 80};
			SDL_Rect bg_outline_rect = {19, 39, WINDOW_LENGTH - 38, WINDOW_HEIGHT - 78};
			SDL_SetRenderDrawColor(renderer, 255,255,255,255);
			SDL_RenderDrawRect(renderer, &bg_outline_rect);
			SDL_RenderCopy(renderer, bg_texture, NULL, &bg_rect);

			// Display Text
			// Display Copyright Text
			SDL_Rect copyright_rect = {470, 20, 150, 15};
			SDL_RenderCopy(renderer, textureCN, NULL, &copyright_rect);
			// Display Main Text
			SDL_Rect main_text_rect = {(WINDOW_LENGTH - 140) / 2, 450, 140, 20};
			SDL_RenderCopy(renderer, textureMT, NULL, &main_text_rect);

			// draw logo and add bounce effect
			// edges are Left = 20, Right = 620, Top = 20, Bottom =  460
			// adjusted for logo sizing, Left = 20, Top = 40, Right = 420, Bottom = 340

			if (logoPositionRect.x <= 20) {
				horiz_motion = 1;
			} else if (logoPositionRect.x >= 420) {
				horiz_motion = -1;
			}

			if (logoPositionRect.y <= 40) {
				vert_motion = 1;
			} else if (logoPositionRect.y >= 340) {
				vert_motion = -1;
			}

			logoPositionRect.x += horiz_motion;
			logoPositionRect.y += vert_motion;

			SDL_RenderCopy(renderer, logo_texture, NULL, &logoPositionRect);

			SDL_RenderPresent(renderer);
        }

        // Clean up
		SDL_DestroyTexture(bg_texture);
		SDL_DestroyTexture(textureCN);
		SDL_FreeSurface(image);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		TTF_Quit();
        SDL_Quit();

        return 0;

}

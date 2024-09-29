#include <SDL.h>

int main(int argc, char** argv)
{
	SDL_DisplayMode mode;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Event evt;
	SDL_bool keep_going = SDL_TRUE;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		return 1;
	}
	else if (SDL_GetCurrentDisplayMode(0, &mode) != 0) {
		return 1;
	}
	else if (SDL_CreateWindowAndRenderer(mode.w, mode.h, SDL_WINDOW_FULLSCREEN, &window, &renderer) != 0) {
		return 1;
	}

	while (keep_going) {
		while (SDL_PollEvent(&evt)) {
			if ((evt.type == SDL_KEYDOWN) && (evt.key.keysym.sym == SDLK_ESCAPE)) {
				keep_going = SDL_FALSE;
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
	}

	SDL_Quit();
	return 0;
}
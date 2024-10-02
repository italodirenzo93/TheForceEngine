#include <SDL.h>
#include <SDL_opengl.h>

int main(int argc, char** argv)
{
	SDL_DisplayMode mode;
	SDL_Window* window = NULL;
	SDL_Event evt;
	SDL_bool keep_going = SDL_TRUE;

	SDL_GLContext gl_context;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		return 1;
	}
	else if (SDL_GetCurrentDisplayMode(0, &mode) != 0) {
		return 1;
	}
	
	window = SDL_CreateWindow("The Force Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mode.w, mode.h, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
	if (!window) return 1;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	gl_context = SDL_GL_CreateContext(window);
	if (!gl_context) {
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	while (keep_going) {
		while (SDL_PollEvent(&evt)) {
			if ((evt.type == SDL_KEYDOWN) && (evt.key.keysym.sym == SDLK_ESCAPE)) {
				keep_going = SDL_FALSE;
			}
		}

		glClearColor(0.0f, 255.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		SDL_GL_SwapWindow(window);
	}

	SDL_Quit();
	return 0;
}
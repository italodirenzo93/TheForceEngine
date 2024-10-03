#include <SDL.h>
#include "glad.h"

static GLuint vboID, vaoID, vsID, fsID, program;

static const auto vs_src = R"(#version 330 core
layout(location=0) in vec3 pos;

void main() {
	gl_Position = vec4(pos, 1.0);
})";

static const auto fs_src = R"(#version 330 core
out vec4 outColor;

void main() {
	outColor = vec4(0.0, 0.0, 1.0, 1.0);
})";

static const float s_vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

static void initGLResources()
{
	// shaders
	GLint status;
	GLchar info_log[512];

	vsID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vsID, 1, &vs_src, nullptr);
	glCompileShader(vsID);

	glGetShaderiv(vsID, GL_COMPILE_STATUS, &status);
	if (!status) {
		glGetShaderInfoLog(vsID, 512, nullptr, info_log);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "vs error", info_log, nullptr);
	}


	fsID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsID, 1, &fs_src, nullptr);
	glCompileShader(fsID);

	glGetShaderiv(fsID, GL_COMPILE_STATUS, &status);
	if (!status) {
		glGetShaderInfoLog(fsID, 512, nullptr, info_log);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "fs error", info_log, nullptr);
	}

	program = glCreateProgram();
	glAttachShader(program, vsID);
	glAttachShader(program, fsID);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (!status) {
		glGetProgramInfoLog(program, 512, nullptr, info_log);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "prog error", info_log, nullptr);
	}

	glDetachShader(program, vsID);
	glDetachShader(program, fsID);

	glDeleteShader(vsID);
	glDeleteShader(fsID);

	// buffers
	glGenBuffers(1, &vboID);
	glGenVertexArrays(1, &vaoID);

	glBindVertexArray(vaoID);

	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertices), s_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glEnableVertexAttribArray(0);

	glUseProgram(program);
}

static void cleanupGLResources()
{
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDeleteProgram(program);
	glDeleteBuffers(1, &vboID);
	glDeleteVertexArrays(1, &vaoID);
}

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

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "glad error", "could not register glad loader", nullptr);
		return 1;
	}

	initGLResources();

	while (keep_going) {
		while (SDL_PollEvent(&evt)) {
			if ((evt.type == SDL_KEYDOWN) && (evt.key.keysym.sym == SDLK_ESCAPE)) {
				keep_going = SDL_FALSE;
			}
		}

		glClearColor(0.0f, 255.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);
		glBindVertexArray(vaoID);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		SDL_GL_SwapWindow(window);
	}

	cleanupGLResources();

	SDL_Quit();
	return 0;
}
#define GLEW_STATIC

#include <glew.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL.h>
#include <iostream>
#include <ostream>
#include <string>
#include "Mesh.h"

using namespace std;

void openGL()
{
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32); // 32 bit inc RGBA
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16); // depth is not 32 bit
}

void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const string &errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		cerr << errorMessage << ": '" << error << "'" << endl;		
	}
}

int main(int arcg, char *argv[])
{

	SDL_Init(SDL_INIT_EVERYTHING);

	openGL();

	SDL_Window* window = SDL_CreateWindow("My window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SDL_GLContext GLContext = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	GLenum status = glewInit();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	if (status != GLEW_OK)
	{
		cout << "GLEW failed to initialize!" << endl;
	}

	// triangle
	float Vertices[]
	{
		/*-1.0f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,

		1.0f, 0.25f, 0.0f,
		0.25f, 0.25f, 0.0f,
		-0.25f, 0.25f, 0.0f*/

		-1.0f, 1.0f,0.0f,
		0.0f,0.0f,0.0f,
		-1.0, 0.0f,0.0f,

		1,1,0,
		1,0,0,
		0,0,0
	};

	Mesh Tri1(Vertices, 3);

	GLuint VertexBufferObject = 0;
	glGenBuffers(1, &VertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), Vertices, GL_STATIC_DRAW);

	GLuint VertexArrayObject = 0;
	glGenVertexArrays(1, &VertexArrayObject);
	glBindVertexArray(VertexArrayObject);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(0);

	const char* VertexShaderCode =
		"#version 450\n"
		"in vec3 vp;"
		"uniform mat4 model;"
		"void main() {"
		"  gl_Position = model * vec4(vp, 1.0);"
		"}";

	const char* FragmentShaderCode =
		"#version 450\n"
		"out vec4 frag_colour;"
		"void main() {"
		"  frag_colour = vec4(0.0, 0.95, 0.0, 1.0);"
		"}";

	GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, &VertexShaderCode, NULL);
	glCompileShader(VertexShader);
	GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &FragmentShaderCode, NULL);
	glCompileShader(FragmentShader);

	GLuint ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, VertexShader);
	glAttachShader(ShaderProgram, FragmentShader);

	glLinkProgram(ShaderProgram);
	CheckShaderError(ShaderProgram, GL_LINK_STATUS, true, "Error: program linking failed ");
	glValidateProgram(ShaderProgram);
	CheckShaderError(ShaderProgram, GL_VALIDATE_STATUS, true, "Error: program linking failed ");

	glClearColor(0.0f, 0.15f, 0.3f, 1.0f);
	glViewport(0, 0, 800, 600);

	while (true)
	{

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(ShaderProgram);
		
		GLint modelLoc = glGetUniformLocation(ShaderProgram, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &Tri1.m_transform.GetModel()[0][0]);

		Tri1.m_transform.setPos(vec3(0, 0, 0));
		Tri1.m_transform.setRot(Tri1.m_transform.getRot() + vec3(0, 0, 5));
		Tri1.m_transform.setScale(vec3(0.15, 0.7, 0.7));
		Tri1.Draw();

		SDL_Delay(16);

		SDL_GL_SwapWindow(window);

	}

	SDL_GL_DeleteContext(GLContext);
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
	return 0;
}


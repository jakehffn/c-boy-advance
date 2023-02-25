#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef __EMSCRIPTEN__

#include <emscripten.h>
#include <GLES3/gl3.h>
#include <GLES3/gl2ext.h>

#else 
// GLEW must come before OpenGL
#include <GL\glew.h>

#endif

#include <SDL.h>
#include <SDL_opengl.h>

typedef struct Shader* Shader;

bool initShader(Shader* shader, const char* vertex_source, const char* fragment_source);
void destroyShader(Shader shader);
void useShader(Shader shader);
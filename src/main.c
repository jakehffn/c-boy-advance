#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __EMSCRIPTEN__

#include <emscripten.h>
#include <emscripten/html5.h>
#include <GLES3/gl3.h>
#include <GLES3/gl2ext.h>

#else 

// GLEW must come before OpenGL
#include <GL\glew.h>

#endif

#include <SDL.h>
#include <SDL_opengl.h>

#define WIDTH 160
#define HEIGHT 144

bool init();
void close();

SDL_Window* window = NULL;
SDL_GLContext context;

// Initializes SDL, GLEW, then OpenGL
bool init() {
    bool success = true;

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;

	} else {

        #ifndef __EMSCRIPTEN__

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

        #endif

		window = SDL_CreateWindow(
            "AGE", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
			WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
		);

		if(window == NULL) {

			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;

		} else {

            context = SDL_GL_CreateContext(window);

			if(context == NULL) {
				printf("SDL: OpenGL context could not be created!\nSDL Error: %s\n", SDL_GetError());
				success = false;
			} else {

                #ifndef __EMSCRIPTEN__

                    glewExperimental = GL_TRUE; 
                    GLenum glewError = glewInit();
                    if( glewError != GLEW_OK ) {
                        printf("GLEW: Error initializing! %s\n", glewGetErrorString(glewError));
                    }

                #endif

				// Use Vsync
				if( SDL_GL_SetSwapInterval( 1 ) < 0 ) {
					printf("SDL: Warning: Unable to set VSync!\nSDL Error: %s\n", SDL_GetError());
				}
			}
		}
	}

	return success;
}

void close() {

	// Destroy window	
	SDL_DestroyWindow(window);
	window = NULL;

	// Quit SDL subsystems
	SDL_Quit();
}

const char* vertex_shader_source = "#version 300 es\n"
    "precision highp float;\n"
    "layout (location = 0) in vec4 vertex;\n"  // <vec2 position, vec2 texCoords>
    "out vec2 TexCoords;\n"
    "void main() {\n"
        "TexCoords = vertex.zw;\n"
        "gl_Position = vec4(2.0f*vertex.x - 1.0f, 1.0f - 2.0f*vertex.y, 0.0, 1.0);\n"
    "}\0";

const char* fragment_shader_source = "#version 300 es\n"
    "precision highp float;\n"
    "in vec2 TexCoords;\n"
    "out vec4 color;\n"
    "uniform sampler2D screenTexture;\n"
    "void main() {\n"
        "color = vec4(texture(screenTexture, vec2(TexCoords.x, TexCoords.y)).rgb, 1.0f);\n"
        // "color = vec4(1.0f*TexCoords.x, 0.6f*TexCoords.y, 1.0f, 1.0f);\n"
    "}\0";

const float quadVertexData[] = { 
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 
    
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
};

char texture_data[WIDTH*HEIGHT*3];

void drawToCanvas() {

    unsigned int color = 0x00FFFF;

    for (int xx = 0; xx < WIDTH/2; xx++) {

        for (int yy = 0; yy < HEIGHT/2; yy++) {

            int cell_start = (yy*WIDTH + xx)*3;

            texture_data[cell_start] = 0x00;
            texture_data[cell_start + 1] = 0x00;
            texture_data[cell_start + 2] = 0x00;

            if (xx/4 % 3 == 0) {
                texture_data[cell_start] = 0xFF;
            }

            if (xx/4 % 3 == 1) {
                texture_data[cell_start+1] = 0xFF;
            }

            if (xx/4 % 3 == 2) {
                texture_data[cell_start+2] = 0xFF;
            }
        }
    }

    GLuint VAO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    // The verticies will never change so the buffer ID is not saved
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertexData), quadVertexData, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    // Free bound buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  

    GLuint screen_texture;

    glGenTextures(1, &screen_texture);
    glBindTexture(GL_TEXTURE_2D, screen_texture);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    int success;
    char info_log[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        printf("ERROR compiling shader: %s", info_log);
    }

    unsigned int fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        printf("ERROR compiling shader: %s", info_log);
    }

    unsigned int shader_program;
    shader_program = glCreateProgram();

    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        printf("ERROR compiling shader: %s", info_log);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // glBindVertexArray(0);

    glClearColor(0.5, 0.2, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);
    glUseProgram(shader_program);
    glBindTexture(GL_TEXTURE_2D, screen_texture);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glUseProgram(0);

    SDL_GL_SwapWindow(window);
}

int main(int argv, char** args) {

    if (!init()) {

        printf("Failed to initialize SDL\n");

    } else {

        printf("SDL initialized\n");

        #ifdef __EMSCRIPTEN__

            emscripten_set_main_loop(drawToCanvas, 0, 1);

        #else

            bool quit = false;

            while (!quit) {

                SDL_Event event;
                while (SDL_PollEvent(&event)) {

                    if (event.type == SDL_QUIT) {
                        quit = true;      
                    }
                }

                drawToCanvas();
            }

        #endif
    }

    return 0;
}
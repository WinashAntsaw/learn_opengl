#ifndef _render_h_
#define _render_h_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <camera.h>


// Struct to act as the main window of the game
typedef struct _game_window_ {
    GLFWwindow* glfw_window;
    GLuint shader_program;
    double update_time;
    int fps;
    float aspect;
    Camera *camera;
} GameWindow;


// create and return a pointer to a GameWindow with a specified width and height, returns NULL if there was a failure
GameWindow *init_GameWindow(int width, int height, double update_time, int fps,
    char *v_path, char *f_path);

// begin the render loop for the GameWindow
void init_render_loop(GameWindow *window, void (*update)(void *input));

#endif
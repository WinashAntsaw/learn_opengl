#ifndef _render_h_
#define _render_h_

#include <glad/glad.h>
#include <GLFW/glfw3.h>


// Struct to act as the main window of the game
typedef struct _game_window_ {
    GLFWwindow* glfw_window;
    double update_time;
    int fps;
} GameWindow;


// create and return a pointer to a GameWindow with a specified width and height, returns NULL if there was a failure
GameWindow *init_GameWindow(int width, int height, double update_time, int fps);

// begin the render loop for the GameWindow
void init_render_loop(GameWindow *window, void (*update)(void *input));

#endif
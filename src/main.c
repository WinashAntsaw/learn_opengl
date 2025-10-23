// opengl related includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// system includes
#include <stdio.h>
#include <stdbool.h>


// user defined includes
#include <render.h>
#include <shader_utils.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow *window);


int main() {

    GameWindow *game_window = init_GameWindow(800, 600, 0.01, 60,
        "src/shaders/vertex.glsl", "src/shaders/fragment.glsl");
    
    
    

    // main render loop
    init_render_loop(game_window, NULL);
    
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


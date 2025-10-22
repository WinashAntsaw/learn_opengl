#include <render.h>
#include <stdlib.h>
#include <time.h>

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


GameWindow *init_GameWindow(int width, int height, double update_time, int fps) {

    // init and setup glfw window and glad
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);

    // init and setup glad 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return NULL;
    }

    // set the initial viewport and configure it to resize
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // malloc a struct for a GameWindow, and initialize the window field with the GLFWwindow
    GameWindow *output = malloc(sizeof(GameWindow));
    if (output == NULL) {
        printf("Failed to allocate GameWindow struct\n");
        return NULL;
    }
    output->glfw_window = window;
    output->fps = fps;
    output->update_time = update_time;

    return output;

}


void init_render_loop(GameWindow *window, void (*update)(void *input)) {
    double start_time = glfwGetTime();
    
    while (!glfwWindowShouldClose(window->glfw_window)) {
        glfwSwapBuffers(window->glfw_window);
        glfwPollEvents();
    }
}
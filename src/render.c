#include <render.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <shader_utils.h>
#include <stdbool.h>
#include <camera.h>

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    GameWindow *parent = glfwGetWindowUserPointer(window);

    parent->width = width;
    parent->height = height;
    camera_resize_perspective(parent->camera, (float) width / (float) height);
}

void process_input(GameWindow *game_window) {

    GLFWwindow *window = game_window->glfw_window;
    float theta = 0.01;
    float delta = 0.01;

    bool should_update_view = true;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        //glfwSetWindowShouldClose(window, true);

        if (game_window->cursor_disabled == true) {
            game_window->cursor_disabled = false;
            glfwSetCursorPos(window, game_window->width / 2, game_window->height / 2);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPos(window, 0, 0);
            game_window->cursor_disabled = true;
        }
        
    }

    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera_move(game_window->camera, Z_AXIS, delta);
        should_update_view = true;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera_move(game_window->camera, Z_AXIS, -delta);
        should_update_view = true;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera_move(game_window->camera, X_AXIS, delta);
        should_update_view = true;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera_move(game_window->camera, X_AXIS, -delta);
        should_update_view = true;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera_move(game_window->camera, Y_AXIS, delta);
        should_update_view = true;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera_move(game_window->camera, Y_AXIS, -delta);
        should_update_view = true;
    }
    

    if (should_update_view) {
        camera_update_view(game_window->camera);
        camera_update_transform(game_window->camera);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
    }
    
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {

    GameWindow *parent = glfwGetWindowUserPointer(window);

    if (parent->cursor_disabled) {
        Camera *camera = parent->camera;

        float global_sens = 0.01;

        float yaw_delta = global_sens * xpos;
        float pitch_delta = global_sens * ypos;

        //printf("\rPitch Delta: %5.2f, Yaw Delta: %5.2f", pitch_delta, yaw_delta);

        camera_rotate(camera, X_AXIS, -pitch_delta);
        camera_rotate(camera, Y_AXIS, -yaw_delta);

        camera_update_view(camera);
        camera_update_transform(camera);

        glfwSetCursorPos(window, 0.0f, 0.0f);
    }
    

}

GameWindow *init_GameWindow(int width, int height, double update_time, int fps, char *v_path, char *f_path) {

    // init and setup glfw window and glad
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);
    


    // init and setup glad 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");

        glfwTerminate();
        return NULL;
    }

    // set the initial viewport and configure it to resize
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // malloc a struct for a GameWindow, and initialize the window field with the GLFWwindow
    GameWindow *output = malloc(sizeof(GameWindow));
    if (output == NULL) {
        fprintf(stderr, "Failed to allocate GameWindow struct\n");

        glfwTerminate();
        return NULL;
    }
    output->glfw_window = window;
    output->fps = fps;
    output->update_time = update_time;
    output->shader_program = init_shader_program(v_path, f_path);
    output->cursor_disabled = true;
    output->width = width;
    output->height = height;

    if (output->shader_program == 0) {
        fprintf(stderr, "Error with initializing shaders");
        free(output);
        glfwTerminate();
        return NULL;
    }

    // Now that we've guaranteed a working shader program, initialize and bind the camera to it
    output->camera = init_camera(output->shader_program, "transform", (float) width / (float) height);
    if (output->camera == NULL) {
        fprintf(stderr, "Error initializing camera");
        free(output);
        glfwTerminate();
        return NULL;
    }


    // set the glfw window to contain a pointer to its parent Game Window
    glfwSetWindowUserPointer(output->glfw_window, output);

    // set up the cursor modes and callback
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    glfwSetCursorPos(window, 0, 0);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    return output;

}


void init_render_loop(GameWindow *window, void (*update)(void *input)) {

    // setup vertex data and buffers
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 
    
    while (!glfwWindowShouldClose(window->glfw_window)) {
        // input
        process_input(window);

        //rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // drawing our first triangle
        glUseProgram(window->shader_program);
        //camera_update_transform(window->camera);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window->glfw_window);
        glfwPollEvents();
    }

    glfwTerminate();
}
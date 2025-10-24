#include <camera.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <cglm/cglm.h>
#include <stdio.h>

// prints a mat4 matrix
static void print_mat4(mat4 input) {
    int i, j;

    // mat4s are represented in column major order
    for (i = 0; i < 4; i++) {
        printf("[");
        for (j = 0; j < 4; j++) {
            if (j > 0) {
                printf("\t");
            }
            printf("%5.2f", input[j][i]);
        }
        printf("]\n");
    }
}

// prints a vec3 vector
static void print_vec3(vec3 input) {
    printf("[%5.2f\t%5.2f\t%5.2f]\n", input[0], input[1], input[2]);
}

// mallocs and initializes values for the camera, and binds the camera's view matrix to the view matrix uniform in the parameter shader program
Camera *init_camera(GLuint shader_program, const char const *transform_var_name, float aspect) {

    Camera *output = calloc(1, sizeof(Camera));

    if (output == NULL) {
        fprintf(stderr, "Error initializing camera: Could not calloc space for Camera struct");
        return NULL;
    }

    output->pitch = output->yaw = output->roll = 0.0f;
    output->fov = 90.0f, output->aspect = aspect;
    output->near = 0.1f, output->far = 100.0f;

    output->pos[2] = -5.0;

    
    GLint loc;
    
    if (transform_var_name == NULL || (loc = glGetUniformLocation(shader_program, transform_var_name) == -1)) {
        fprintf(stderr, "Error initializing camera: Could not get transform uniform location");
        free(output);
        return NULL;
    } 

    output->transform_loc = loc;

    // initialize the view and projection matrices and send the resulting transform
    camera_update_view(output);
    camera_update_projection(output);
    camera_update_transform(output);

    //glUniformMatrix4fv(loc, 1, GL_FALSE, (float *) output->transform);

    return output;
}

void camera_update_view(Camera *camera) {
    vec3 forward;
    forward[0] = cos(glm_rad(camera->pitch)) * sin(glm_rad(camera->yaw));
    forward[1] = -sin(glm_rad(camera->pitch));
    forward[2] = cos(glm_rad(camera->pitch)) * cos(glm_rad(camera->yaw));

    glm_normalize(forward);

    
    vec3 up = {0.0f, 1.0f, 0.0f};

    glm_look(camera->pos, forward, up, camera->view);

}

void camera_update_projection(Camera *camera) {
    glm_perspective(glm_rad(camera->fov), camera->aspect, camera->near, camera->far, camera->proj);

}

void camera_update_transform(Camera *camera) {
    mat4 transform;

    glm_mat4_mul(camera->proj, camera->view, transform);

    glUniformMatrix4fv(camera->transform_loc, 1, GL_FALSE, (float *) transform);
}

void camera_resize_perspective(Camera *camera, float aspect) {
    camera->aspect = aspect;
    glm_perspective_resize(aspect, camera->proj);
}

void camera_rotate(Camera *camera, Axis axis, float theta) {

    if (axis == X_AXIS) {
        camera->pitch += theta;
    } else if (axis == Y_AXIS) {
        camera->yaw += theta;
    } else if (axis == Z_AXIS) {
        camera->roll += theta;
    }

}

void camera_move(Camera *camera, Axis axis, float delta) {

    // currently just going to manually define whether it moves along a global or local axis
    double cos_yaw = cos(glm_rad(camera->yaw));
    double sin_yaw = sin(glm_rad(camera->yaw));
    
    if (axis == X_AXIS) {
        camera->pos[0] += cos_yaw * delta;
        camera->pos[2] += sin_yaw * delta;
    } else if (axis == Y_AXIS) {
        camera->pos[1] += delta;
    } else if (axis == Z_AXIS) {
        camera->pos[0] += -sin_yaw * delta;
        camera->pos[2] += cos_yaw * delta;
    }

    printf("\rPos: [%5.2f, %5.2f, %5.2f], Yaw: %5.2f", camera->pos[0], camera->pos[1], camera->pos[2], camera->yaw);
}
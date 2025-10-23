#include <camera.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <cglm/cglm.h>

// mallocs and initializes values for the camera, and binds the camera's view matrix to the view matrix uniform in the parameter shader program
Camera *init_camera(GLuint shader_program) {
    Camera *output = calloc(1, sizeof(Camera));

    output->pitch = output->yaw = output->roll = 0.0f;
    output->fov = 45.0f;
    output->near = 0.1f, output->far = 100.0f;

    GLint loc = glGetUniformLocation(shader_program, "transform");
    output->transform_loc = loc;

    glUniformMatrix4fv(loc, 1, GL_FALSE, output->transform);

    return output;
}

void update_transform(Camera *camera, float aspect) {
    vec3 forward;
    forward[0] = cos(glm_rad(camera->pitch)) * sin(glm_rad(camera->yaw));
    forward[1] = sin(glm_rad(camera->pitch));
    forward[2] = cos(glm_rad(camera->pitch)) * cos(glm_rad(camera->yaw));

    glm_normalize(forward);

    vec3 target;
    glm_vec3_add(camera->pos, forward, target);

    vec3 up = {0.0f, 1.0f, 0.0f};

    mat4 view;
    glm_lookat(camera->pos, target, up, view);

    mat4 proj;
    

    glUniformMatrix4fv(camera->transform_loc, 1, GL_FALSE, camera->transform);
}

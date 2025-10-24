#ifndef _camera_h_
#define _camera_h_
#include <glad/glad.h>
#include <cglm/cglm.h>

typedef struct _camera_ {
    vec3 pos;
    float pitch;
    float yaw;
    float roll;
    float fov, near, far, aspect;
    mat4 view;
    mat4 proj;
    GLint transform_loc;
} Camera;

typedef enum {
    X_AXIS,
    Y_AXIS,
    Z_AXIS
} Axis;

Camera *init_camera(GLuint shader_program, const char const *transform_var_name, float aspect);

void camera_update_view(Camera *camera);

void camera_update_projection(Camera * camera);

void camera_resize_perspective(Camera *camera, float aspect);

void camera_update_transform(Camera *camera);

void camera_rotate(Camera *camera, Axis axis, float theta);

void camera_move(Camera *camera, Axis axis, float delta);

#endif
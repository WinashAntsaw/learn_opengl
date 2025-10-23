#ifndef _camera_h_
#define _camera_h_
#include <glad/glad.h>
#include <cglm/cglm.h>

typedef struct _camera_ {
    vec3 pos;
    float pitch;
    float yaw;
    float roll;
    float fov, near, far;
    mat4 transform;
    GLint transform_loc;
} Camera;

typedef enum {
    X_AXIS,
    Y_AXIS,
    Z_AXIS
} Axis;

Camera *init_camera(GLuint shader_program);

void global_rotate(Camera *camera, Axis axis, float theta);

void global_translate(Camera *camera, Axis axis, float delta);

void local_rotate(Camera *camera, Axis axis, float theta);

void local_translate(Camera *camera, Axis axis, float theta);

void update_view(Camera *camera);

#endif
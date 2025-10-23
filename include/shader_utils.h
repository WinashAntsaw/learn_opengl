#include <glad/glad.h>

int compile_shader_object(GLuint shader_obj, const char *path);

GLuint init_shader_program(const char *vertex_path, const char *fragment_path);
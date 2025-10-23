#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

// read a text file into a dynamically allocated char pointer
static char *read_shader_file(const char *path) {

    // open file and check for error
    FILE *shader_file = fopen(path, "rb");
    if (shader_file == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", path);
        return NULL;
    }

    // get the length of the file
    fseek(shader_file, 0, SEEK_END);
    long file_length = ftell(shader_file);
    fseek(shader_file, 0, SEEK_SET);

    // allocate space for the file soruce in memory
    char *loaded_src = malloc(file_length + 1);
    if (loaded_src == NULL) {
        fprintf(stderr, 
            "Could not load space for file %s with length %ld in memory\n", 
            path, file_length);
        fclose(shader_file);
        return NULL;
    }

    // read the file into memory
    size_t result = fread(loaded_src, sizeof(char), file_length, shader_file);
    if (result != file_length) {
        fprintf(stderr, "Error reading file into memory, %d bytes reac\n", result);
        free(loaded_src);
        return NULL;
    }

    // nul terminate the end of the string and return
    loaded_src[file_length] = '\0';
    
    fclose(shader_file);
    return loaded_src;
}


// compile a shader object given a path to a soure file, assumes shader_obj is valid, returns -1 if failure, 0 otherwise
int compile_shader_object(GLuint shader_obj, const char *path) {
    
    char *shader_source = read_shader_file(path);

    if (shader_source == NULL) {
        fprintf(stderr, "Error from read_shader_file, called by compile_shader_object on file: %s\n", path);
        return -1;
    }

    // add source to shader object and compile
    glShaderSource(shader_obj, 1, &shader_source, NULL);
    glCompileShader(shader_obj);

    // verify compilation success
    int success;
    char info_log[512];
    glGetShaderiv(shader_obj, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader_obj, 512, NULL, info_log);
        fprintf(stderr, "Error compiling shader with source file: %s\nInfo Log: %s\n",
                path, info_log);
        
        free(shader_source);
        return -1;
    }

    free(shader_source);
    return 0;
}


GLuint init_shader_program(const char *vertex_path, const char *fragment_path) {

    // create the shader objects, and return with 0 if there was an error
    unsigned int vertex_shader, fragment_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    if (vertex_shader == 0 || fragment_shader == 0) {
        fprintf(stderr, "Error with creating shader object(s)\n");
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        return 0;
    }

    // compile each shader object with the corresponding source file
    int v_result = compile_shader_object(vertex_shader, vertex_path);
    int f_result = compile_shader_object(fragment_shader, fragment_path);

    if (v_result == -1 || f_result == -1) {
        fprintf(stderr, "Error with compiling shader object(s)\n");
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        return 0;
    }

    // create shader program
    GLuint shader_program = glCreateProgram();
    if (shader_program == 0) {
        fprintf(stderr, "Error with creating shader program\n");
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        glDeleteProgram(shader_program);
        return 0;
    }

    // attach and shader objects to shader program and link
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    int success;
    char info_log[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        fprintf(stderr, 
                "Error linking shader with source files:\n%s\n%s\nInfo Log: %s\n",
                vertex_path, fragment_path, info_log);
        
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        glDeleteProgram(shader_program);

        return 0;
    }

    // use shader program
    glUseProgram(shader_program);

    // delete attached shader objects
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // return shader program
    return shader_program;

}
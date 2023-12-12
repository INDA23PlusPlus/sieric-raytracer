#include <shaders.h>
#include <fileio.h>

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

static GLuint shaders_load_file(const char *path, GLenum type) {
    GLuint shader;
    GLint result, log_sz;
    char *code, *log;

    if(data_file_to_buf(&code, path) < 0) {
        perror("[Shader ERR] load file");
        return -1;
    }

    shader = glCreateShader(type);
    glShaderSource(shader, 1, (const char *const *)&code, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_sz);
    if(!result) {
        log = malloc(log_sz);
        glGetShaderInfoLog(shader, log_sz, NULL, log);
        fprintf(stderr, "[Shader INFO] Compilation Log: %s\n", log);
        fprintf(stderr, "[Shader INFO] Code: %s\n", code);
        free(log);
    }

    free(code);
    return shader;
}

GLuint shaders_program_load_file(const char *vertex_path,
                                 const char *fragment_path) {
    GLuint vertex, fragment, program;
    GLint result, log_sz;
    char *log;

    vertex = shaders_load_file(vertex_path, GL_VERTEX_SHADER);
    if(vertex == (GLuint)-1) return -1;
    fragment = shaders_load_file(fragment_path, GL_FRAGMENT_SHADER);
    if(fragment == (GLuint)-1) {
        glDeleteShader(vertex);
        return -1;
    }

    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_sz);
    if(!result) {
        log = malloc(log_sz);
        glGetProgramInfoLog(program, log_sz, NULL, log);
        fprintf(stderr, "[Shader INFO] Program Log: %s\n", log);
        free(log);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}

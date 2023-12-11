#include <renderer.h>
#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include "linmath.h"

typedef struct vertex {
    float x, y, r, g, b;
} vertex_t;

vertex_t verts[3] = {
{-1.f, -.866f, 1.f, 0.f, 0.f},
{0.f, .866f, 0.f, 1.f, 0.f},
{1.f, -.866f, 0.f, 0.f, 1.f},
};

GLuint vbo, program;
GLint mvp_loc, vpos_loc, vcol_loc;
mat4x4 m, p, mvp;

GLuint shaders_load_file(const char *vertex_path,
                         const char *fragment_path) {
    /* size_t fs; */
    char *vertex_text, *fragment_text; //, filename_buf[256];
    GLuint vertex_shader, fragment_shader, program;

    /* logging */
    GLint result;
    int log_size, log_size_max;
    char *log;

    /* compile vertex shader */
    /* snprintf(filename_buf, 256, DATADIR "/shaders/%s", vertex_path); */
    /* fs = file_read_to_buffer(&vertex_text, filename_buf); */
    /* if(fs == -1) { */
    /*     // TODO: Logging */
    /*     fprintf(stderr, "[Shader ERR] %s\n", "No such vertex shader file"); */
    /*     return -1; */
    /* } */
    vertex_text = "\
#version 460\n\
\n\
uniform mat4 MVP;\n\
layout(location = 0) in vec3 vCol;\n\
layout(location = 1) in vec2 vPos;\n\
out vec3 color;\n\
\n\
void main() {\n\
    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n\
    color = vCol;\n\
}";
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, (const char *const *)&vertex_text, NULL);
    glCompileShader(vertex_shader);

    /* free(vertex_text); */

    /* check vertex shader */
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &log_size);
    log_size_max = log_size;
    log = (char *)malloc(((log_size > 1) ? log_size : 1) * sizeof(char));
    if(!result) {
        glGetShaderInfoLog(vertex_shader, log_size, NULL, log);
        fprintf(stderr, "[Shader INFO] Vertex Log: %s", log);
    }

    /* compile fragment shader */
    /* snprintf(filename_buf, 256, DATADIR "/shaders/%s", fragment_path); */
    /* fs = file_read_to_buffer(&fragment_text, filename_buf); */
    /* if(fs == -1) { */
    /*     fprintf(stderr, "[Shader ERR] %s\n", "No such fragment shader file"); */
    /*     return -1; */
    /* } */
    fragment_text = "\
#version 460\n\
\n\
in vec3 color;\n\
out vec4 outputColor;\n\
\n\
void main() {\n\
    outputColor = vec4(color, 1.0);\n\
}";
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, (const char *const *)&fragment_text, NULL);
    glCompileShader(fragment_shader);

    /* free(fragment_text); */

    /* check fragment shader */
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &log_size);
    if(log_size > log_size_max) {
        log_size_max = log_size;
        log = realloc(log, log_size*sizeof(char));
    }
    if(!result) {
        glGetShaderInfoLog(fragment_shader, log_size, NULL, log);
        fprintf(stderr, "[Shader INFO] Fragment Log: %s", log);
    }

    /* create program */
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    /* check program */
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_size);
    if(log_size > log_size_max) {
        log_size_max = log_size;
        log = realloc(log, log_size);
    }
    if(!result) {
        glGetProgramInfoLog(program, log_size, NULL, log);
        fprintf(stderr, "[Shader INFO] Program Log: %s", log);
    }
    free(log);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}

void renderer_init(state_t *s) {
    mat4x4_identity(m);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof verts, verts, GL_STATIC_DRAW);

    program = shaders_load_file("triangle.vert", "triangle.frag");

    mvp_loc = glGetUniformLocation(program, "MVP");
    vpos_loc = glGetAttribLocation(program, "vPos");
    vcol_loc = glGetAttribLocation(program, "vCol");

    glEnableVertexAttribArray(vpos_loc);
    glVertexAttribPointer(vpos_loc, 2, GL_FLOAT, GL_FALSE,
                          sizeof(vertex_t), (void *)0);
    glEnableVertexAttribArray(vcol_loc);
    glVertexAttribPointer(vcol_loc, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertex_t), (void *)(2*sizeof(float)));
}

void renderer_fixed_update(state_t *s, double dt) {
    return;
}

void renderer_update(state_t *s, double dt) {
    mat4x4_rotate_Z(m, m, dt);
    mat4x4_ortho(p, -s->ratio, s->ratio, -1.f, 1.f, 1.f, -1.f);
    mat4x4_mul(mvp, p, m);

    glUseProgram(program);
    glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, (const GLfloat *)mvp);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void renderer_stop(state_t *s) {
    return;
}

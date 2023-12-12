#include <renderer.h>
#include <shaders.h>
#include <feat.h>

#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include "linmath.h"

typedef struct vertex {
    float x, y;
} vertex_t;

vertex_t verts[] = {
{-1.f, -1.f},
{1.f, -1.f},
{-1.f, 1.f},
{1.f, 1.f},
};

GLubyte vert_indices[6] = {
0, 1, 2,
1, 2, 3,
};

GLuint vbo, ibo, program;
GLint pos_loc, ratio_loc;

void renderer_init(__unused state_t *s) {
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof verts, verts, GL_STATIC_DRAW);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof vert_indices, vert_indices,
                 GL_STATIC_DRAW);

    program = shaders_program_load_file("shaders/raymarch.vert",
                                        "shaders/raymarch.frag");

    pos_loc = glGetAttribLocation(program, "pos");
    ratio_loc = glGetUniformLocation(program, "ratio");

    glEnableVertexAttribArray(pos_loc);
    glVertexAttribPointer(pos_loc, 2, GL_FLOAT, GL_FALSE,
                          sizeof(vertex_t), (void *)0);
}

void renderer_fixed_update(__unused state_t *s, __unused double dt) {
    return;
}

void renderer_update(state_t *s, __unused double dt) {
    glUseProgram(program);
    glUniform1f(ratio_loc, s->ratio);
    glDrawElements(GL_TRIANGLES, sizeof vert_indices, GL_UNSIGNED_BYTE,
                   (void *)0);
}

void renderer_stop(__unused state_t *s) {
    return;
}

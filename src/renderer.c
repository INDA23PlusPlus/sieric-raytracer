#include <renderer.h>
#include <shaders.h>
#include <feat.h>

#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include "linmath.h"

typedef struct vertex {
    float x, y, r, g, b;
} vertex_t;

vertex_t verts[3] = {
{-1.f, -.866f, 1.f, 1.f, 0.f},
{0.f, .866f, 1.f, 0.f, 1.f},
{1.f, -.866f, 0.f, 1.f, 1.f},
};

GLuint vbo, program;
GLint mvp_loc, vpos_loc, vcol_loc;
mat4x4 m, p, mvp;

void renderer_init(__unused state_t *s) {
    mat4x4_identity(m);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof verts, verts, GL_STATIC_DRAW);

    program = shaders_program_load_file("shaders/triangle.vert",
                                        "shaders/triangle.frag");

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

void renderer_fixed_update(__unused state_t *s, __unused double dt) {
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

void renderer_stop(__unused state_t *s) {
    return;
}

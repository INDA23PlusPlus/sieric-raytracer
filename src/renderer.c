#include <renderer.h>
#include <shaders.h>
#include <feat.h>

#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include "linmath.h"

#define CLAMP(x, m, M) ((x)<(m)?(m):(x)>(M)?(M):(x))

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
GLint pos_loc, ratio_loc, affine_loc;

mat4x4 trans, affine;
float x, y, z, y_angle, x_angle;

static uint8_t key_pressed(state_t *s, int key, uint8_t *mod) {
    if(key > GLFW_KEY_LAST || key < 0) return 0;
    uint8_t *k = s->input.key_press+key;
    if(mod) *mod = *k & ~0x80;
    return *k;
}

void renderer_init(__unused state_t *s) {
    x = y = z = y_angle = x_angle = 0.f;

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
    affine_loc = glGetUniformLocation(program, "affine");

    glEnableVertexAttribArray(pos_loc);
    glVertexAttribPointer(pos_loc, 2, GL_FLOAT, GL_FALSE,
                          sizeof(vertex_t), (void *)0);
}

static void move_camera(float *x, float *z, float y_angle, float delta) {
    *x += delta * cosf(y_angle);
    *z += delta * -sinf(y_angle);
}

void renderer_fixed_update(__unused state_t *s, __unused double dt) {
    const float adelta = 0.01, adelta_s = 0.03,
                delta = 10, delta_s = 50;
    uint8_t mod;
    if(key_pressed(s, GLFW_KEY_W, &mod))
        move_camera(&x, &z, y_angle - M_PI_2,
                    mod & GLFW_MOD_SHIFT ? delta_s : delta);
    if(key_pressed(s, GLFW_KEY_A, &mod))
        move_camera(&x, &z, y_angle + M_PI,
                    mod & GLFW_MOD_SHIFT ? delta_s : delta);
    if(key_pressed(s, GLFW_KEY_S, &mod))
        move_camera(&x, &z, y_angle + M_PI_2,
                    mod & GLFW_MOD_SHIFT ? delta_s : delta);
    if(key_pressed(s, GLFW_KEY_D, &mod))
        move_camera(&x, &z, y_angle,
                    mod & GLFW_MOD_SHIFT ? delta_s : delta);
    if(key_pressed(s, GLFW_KEY_SPACE, &mod))
        y += mod & GLFW_MOD_SHIFT ? delta_s : delta;
    if(key_pressed(s, GLFW_KEY_C, &mod))
        y -= mod & GLFW_MOD_SHIFT ? delta_s : delta;
    if(key_pressed(s, GLFW_KEY_RIGHT, &mod))
        y_angle += mod & GLFW_MOD_SHIFT ? adelta_s : adelta;
    if(key_pressed(s, GLFW_KEY_LEFT, &mod))
        y_angle -= mod & GLFW_MOD_SHIFT ? adelta_s : adelta;
    if(key_pressed(s, GLFW_KEY_UP, &mod))
        x_angle -= mod & GLFW_MOD_SHIFT ? adelta_s : adelta;
    if(key_pressed(s, GLFW_KEY_DOWN, &mod))
        x_angle += mod & GLFW_MOD_SHIFT ? adelta_s : adelta;

    x_angle = CLAMP(x_angle, -M_PI_2, M_PI_2);
}

void renderer_update(state_t *s, __unused double dt) {
    mat4x4_translate(trans, x, y, z);
    mat4x4_rotate_Y(affine, trans, y_angle);
    mat4x4_rotate_X(affine, affine, x_angle);

    glUseProgram(program);
    glUniform1f(ratio_loc, s->ratio);
    glUniformMatrix4fv(affine_loc, 1, GL_FALSE, (const GLfloat *)affine);
    glDrawElements(GL_TRIANGLES, sizeof vert_indices, GL_UNSIGNED_BYTE,
                   (void *)0);
}

void renderer_stop(__unused state_t *s) {
    return;
}

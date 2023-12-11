#ifndef RENDERER_H_
#define RENDERER_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <state.h>

void renderer_init(state_t *);
void renderer_fixed_update(state_t *, double);
void renderer_update(state_t *, double);
void renderer_stop(state_t *);

#endif /* RENDERER_H_ */

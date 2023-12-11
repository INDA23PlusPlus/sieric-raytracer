#ifndef STATE_H_
#define STATE_H_

#include <GLFW/glfw3.h>

typedef struct state {
    GLFWwindow *win;
    int width, height;
    float ratio;
} state_t;

#endif /* STATE_H_ */

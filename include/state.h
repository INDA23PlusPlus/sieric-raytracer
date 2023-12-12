#ifndef STATE_H_
#define STATE_H_

#include <GLFW/glfw3.h>

typedef struct state {
    GLFWwindow *win;
    int width, height;
    float ratio;

    struct {
        uint8_t key_press[GLFW_KEY_LAST+1];
    } input;
} state_t;

#endif /* STATE_H_ */

#include <stdio.h>

#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <pthread.h>

#include <state.h>
#include <renderer.h>

#define TICK_TIME (1.0/60.0)

void glfw_error_callback(int err, const char *desc) {
    fprintf(stderr, "[GLFW Error] %d: %s\n", err, desc);
}

uint8_t render_loop_on = 1;
void *render_loop(void *arg) {
    state_t *s = arg;

    const double fps_freq = .5;
    uint32_t fps_frames = 0;
    double dt, dt_prev, fps_prev;
    dt_prev = fps_prev = glfwGetTime();
    while(render_loop_on) {
        dt = glfwGetTime() - dt_prev;
        dt_prev = glfwGetTime();

        ++fps_frames;
        if(glfwGetTime() - fps_prev > fps_freq) {
            printf("----------------------\nframetime: %.6fms\nfps: %.1f\n----------------------\n",
                   fps_freq*1000.0/fps_frames, fps_frames/fps_freq);
            fps_frames = 0, fps_prev += fps_freq;
        }

        glfwGetFramebufferSize(s->win, &s->width, &s->height);
        glViewport(0, 0, s->width, s->height);
        s->ratio = s->width / (float)s->height;

        glClear(GL_COLOR_BUFFER_BIT);
        glfwPollEvents();

        renderer_update(s, dt);

        glfwSwapBuffers(s->win);
    }

    return NULL;
}

int main(void) {
    pthread_t render_thread;
    state_t s;

    glfwSetErrorCallback(&glfw_error_callback);
    if(!glfwInit())
        return 1;

    s.win = glfwCreateWindow(1920, 1080, "tayracer", NULL, NULL);
    if(!s.win) {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(s.win);
    glfwSwapInterval(0);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(err != GLEW_OK) {
        fprintf(stderr, "[GLEW Error] glewInit failed: %s\n",
                glewGetErrorString(err));
        return 1;
    }

    renderer_init(&s);

    /* pthread_create(&render_thread, NULL, render_loop, &s.win); */

    const double fps_freq = .5;
    uint32_t fps_frames = 0;
    double dt, dt_prev, fps_prev;
    dt_prev = fps_prev = glfwGetTime();
    while(!glfwWindowShouldClose(s.win)) {
        dt = glfwGetTime() - dt_prev;
        dt_prev = glfwGetTime();

        ++fps_frames;
        if(glfwGetTime() - fps_prev > fps_freq) {
            printf("----------------------\nframetime: %.6fms\nfps: %.1f\n----------------------\n",
                   fps_freq*1000.0/fps_frames, fps_frames/fps_freq);
            fps_frames = 0, fps_prev += fps_freq;
        }

        glfwGetFramebufferSize(s.win, &s.width, &s.height);
        glViewport(0, 0, s.width, s.height);
        s.ratio = s.width / (float)s.height;

        glClear(GL_COLOR_BUFFER_BIT);
        glfwPollEvents();

        renderer_update(&s, dt);

        glfwSwapBuffers(s.win);
    }

    /* double dt_prev, dt; */
    /* dt_prev = glfwGetTime(); */
    /* while(!glfwWindowShouldClose(s.win)) { */
    /*     if(glfwGetTime() - dt_prev >= TICK_TIME) { */
    /*         dt = glfwGetTime() - dt_prev; */
    /*         dt_prev += TICK_TIME; */

    /*         renderer_fixed_update(&s, dt); */
    /*     } */
    /* } */

    renderer_stop(&s);

    render_loop_on = 0;
    pthread_join(render_thread, NULL);

    glfwDestroyWindow(s.win);
    glfwTerminate();
    return 0;
}

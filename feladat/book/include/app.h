#ifndef APP_H
#define APP_H

#include "camera.h"
#include "scene.h"

#include <SDL2/SDL.h>

#include <stdbool.h>

#define VIEWPORT_RATIO (4.0 / 3.0)
#define VIEWPORT_ASPECT 50.0

typedef struct App {
    SDL_Window* window;
    int window_width;
    int window_height;
    SDL_GLContext gl_context;
    bool is_running;
    double uptime;
    Camera camera;
    bool mouse_is_free;
    Scene scene;
    bool is_drawing;
} App;

/**
 * Initialize the application.
 */
void init_app(App* app, int width, int height);

/**
 * Initialize the OpenGL context.
 */
void init_opengl();

/**
 * Reshape the window.
 */
void reshape(GLsizei width, GLsizei height);

/**
 * Set projection matrix to central projection.
 */
void central_projection();

/**
 * Set projection matrix to parallel projection.
 */
void parallel_projection();

/**
 * Activate drawing mode in the application.
 */
void enter_drawing(App* app);

/**
 * Deactivate drawing mode in the application.
 */
void exit_drawing(App* app);

/**
 * Draw on the top page of the book.
 */
void draw_on_page(Scene* scene, int mouse_x, int mouse_y);

/**
 * Convert mouse coordinates to offsets on the page texture.
 */
void mouse_pos_to_page_offset(int mouse_x,
                              int mouse_y,
                              int* x_offset,
                              int* y_offset);

/**
 * Handle the events of the application.
 */
void handle_app_events(App* app);

/**
 * Update the application.
 */
void update_app(App* app);

/**
 * Render the application.
 */
void render_app(App* app);

/**
 * Destroy the application.
 */
void destroy_app(App* app);

#endif /* APP_H */

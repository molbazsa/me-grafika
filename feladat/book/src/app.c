#include "app.h"

#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

void init_app(App* app, int width, int height) {
    int error_code;
    int inited_loaders;

    app->is_running = false;

    error_code = SDL_Init(SDL_INIT_EVERYTHING);
    if (error_code != 0) {
        printf("[ERROR] SDL initialization error: %s\n", SDL_GetError());
        return;
    }

    app->window = SDL_CreateWindow("Cube!",
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   width,
                                   height,
                                   SDL_WINDOW_OPENGL);
    if (app->window == NULL) {
        printf("[ERROR] Unable to create the application window!\n");
        return;
    }

    app->window_width = width;
    app->window_height = height;

    SDL_WarpMouseInWindow(
        app->window, app->window_width / 2, app->window_height / 2);

    inited_loaders = IMG_Init(IMG_INIT_PNG);
    if (inited_loaders == 0) {
        printf("[ERROR] IMG initialization error: %s\n", IMG_GetError());
        return;
    }

    app->gl_context = SDL_GL_CreateContext(app->window);
    if (app->gl_context == NULL) {
        printf("[ERROR] Unable to create the OpenGL context!\n");
        return;
    }

    init_opengl();
    reshape(width, height);
    central_projection();

    init_camera(&(app->camera));
    app->mouse_is_free = false;
    SDL_ShowCursor(SDL_DISABLE);

    init_scene(&(app->scene));
    app->is_drawing = false;

    app->is_running = true;
}

void init_opengl() {
    glShadeModel(GL_SMOOTH);

    glEnable(GL_NORMALIZE);
    glEnable(GL_AUTO_NORMAL);

    glClearColor(0.1, 0.1, 0.1, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);

    glClearDepth(1.0);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void reshape(GLsizei width, GLsizei height) {
    int x, y, w, h;
    double ratio;

    ratio = (double) width / height;
    if (ratio > VIEWPORT_RATIO) {
        w = (int) ((double) height * VIEWPORT_RATIO);
        h = height;
        x = (width - w) / 2;
        y = 0;
    } else {
        w = width;
        h = (int) ((double) width / VIEWPORT_RATIO);
        x = 0;
        y = (height - h) / 2;
    }

    glViewport(x, y, w, h);
}

void central_projection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-.08, .08, -.06, .06, .1, 100);
}

void parallel_projection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-.6, .6, -.45, .45, .1, 10);
}

void enter_drawing(App* app) {
    app->is_drawing = true;
    parallel_projection();
    app->mouse_is_free = true;
    SDL_ShowCursor(SDL_ENABLE);
}

void exit_drawing(App* app) {
    app->is_drawing = false;
    central_projection();
    app->mouse_is_free = false;
    SDL_ShowCursor(SDL_DISABLE);
    SDL_WarpMouseInWindow(
        app->window, app->window_width / 2, app->window_height / 2);
}

void draw_on_page(Scene* scene, int mouse_x, int mouse_y) {
    int x_offset;
    int y_offset;

    mouse_pos_to_page_offset(mouse_x, mouse_y, &x_offset, &y_offset);

    printf("mx = %d my = %d | ox = %d oy = %d\n",
           mouse_x,
           mouse_y,
           x_offset,
           y_offset);

    glBindTexture(GL_TEXTURE_2D, scene->right_page.object->texture_id);
    glTexSubImage2D(GL_TEXTURE_2D,
                    0,
                    x_offset,
                    y_offset,
                    1,
                    1,
                    GL_RGB,
                    GL_UNSIGNED_BYTE,
                    scene->dot->pixels);
}

void mouse_pos_to_page_offset(int mouse_x,
                              int mouse_y,
                              int* x_offset,
                              int* y_offset) {
    int mouse_top_x = 400;
    int mouse_top_y = 99;
    int mouse_bottom_x = 715;
    int mouse_bottom_y = 555;
    int mouse_range_x = mouse_bottom_x - mouse_top_x;
    int mouse_range_y = mouse_bottom_y - mouse_top_y;

    int page_top_x = 18;
    int page_top_y = 104;
    int page_bottom_x = 73;
    int page_bottom_y = 127;
    int page_range_x = page_bottom_x - page_top_x;
    int page_range_y = page_bottom_y - page_top_y;

    double normalized_x = (mouse_x - mouse_top_x) / (double) mouse_range_x;
    double normalized_y = (mouse_y - mouse_top_y) / (double) mouse_range_y;

    *x_offset = page_top_x + normalized_y * page_range_x;

    if (*x_offset < page_top_x || *x_offset > page_bottom_x) {
        *x_offset = page_top_x - 1;
    }

    *y_offset = page_top_y + normalized_x * page_range_y + 1;

    if (*y_offset < page_top_y || *y_offset > page_bottom_y) {
        *y_offset = page_top_y - 1;
    }
}

void handle_app_events(App* app) {
    SDL_Event event;
    static bool is_mouse_down = false;
    static int mouse_x = 0;
    static int mouse_y = 0;
    double const mouse_sensitivity = 0.5;
    assert(mouse_sensitivity <= 1);
    int x;
    int y;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_ESCAPE:
                        app->is_running = false;
                        break;
                    case SDL_SCANCODE_W:
                        set_camera_speed(&(app->camera), 1);
                        break;
                    case SDL_SCANCODE_S:
                        set_camera_speed(&(app->camera), -1);
                        break;
                    case SDL_SCANCODE_A:
                        set_camera_side_speed(&(app->camera), 1);
                        break;
                    case SDL_SCANCODE_D:
                        set_camera_side_speed(&(app->camera), -1);
                        break;
                    case SDL_SCANCODE_SPACE:
                        set_camera_vertical_speed(&(app->camera), 1);
                        break;
                    case SDL_SCANCODE_LSHIFT:
                        set_camera_vertical_speed(&(app->camera), -1);
                        break;
                    case SDL_SCANCODE_T:
                        turn_page(&(app->scene));
                        break;
                    case SDL_SCANCODE_B:
                        turn_page_back(&(app->scene));
                        break;
                    case SDL_SCANCODE_P:
                        if (!app->is_drawing) {
                            enter_drawing(app);
                        } else {
                            exit_drawing(app);
                        }
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_W:
                    case SDL_SCANCODE_S:
                        set_camera_speed(&(app->camera), 0);
                        break;
                    case SDL_SCANCODE_A:
                    case SDL_SCANCODE_D:
                        set_camera_side_speed(&(app->camera), 0);
                        break;
                    case SDL_SCANCODE_SPACE:
                    case SDL_SCANCODE_LSHIFT:
                        set_camera_vertical_speed(&(app->camera), 0);
                        break;
                    default:
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (app->mouse_is_free) {
                    draw_on_page(&(app->scene), mouse_x, mouse_y);
                    is_mouse_down = true;
                }
                break;
            case SDL_MOUSEMOTION:
                SDL_GetMouseState(&x, &y);
                if (app->mouse_is_free) {
                    if (is_mouse_down) {
                        draw_on_page(&(app->scene), x, y);
                    }

                    mouse_x = x;
                    mouse_y = y;
                } else if (x != app->window_width / 2 ||
                           y != app->window_height / 2) {
                    rotate_camera(&(app->camera),
                                  app->window_width / 2 - x,
                                  app->window_height / 2 - y,
                                  mouse_sensitivity);
                    SDL_WarpMouseInWindow(app->window,
                                          app->window_width / 2,
                                          app->window_height / 2);
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (app->mouse_is_free) {
                    is_mouse_down = false;
                }
                break;
            case SDL_QUIT:
                app->is_running = false;
                break;
            default:
                break;
        }
    }
}

void update_app(App* app) {
    double current_time;
    double elapsed_time;

    current_time = (double) SDL_GetTicks() / 1000;
    elapsed_time = current_time - app->uptime;
    app->uptime = current_time;

    if (!app->is_drawing) {
        update_camera(&(app->camera),
                      elapsed_time,
                      app->scene.bounding_boxes,
                      app->scene.n_bounding_boxes);
    }
    update_scene(&(app->scene), elapsed_time);
}

void render_app(App* app) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();

    if (app->is_drawing) {
        set_parallel_view();
    } else {
        set_view(&(app->camera));
    }

    render_scene(&(app->scene));
    glPopMatrix();

    // if (app->camera.is_preview_visible) {
    //   show_texture_preview();
    // }

    SDL_GL_SwapWindow(app->window);
}

void destroy_app(App* app) {
    if (app->gl_context != NULL) {
        SDL_GL_DeleteContext(app->gl_context);
    }

    if (app->window != NULL) {
        SDL_DestroyWindow(app->window);
    }

    destroy_scene(&(app->scene));

    SDL_Quit();
}

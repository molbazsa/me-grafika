#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texture.h"

#include <SDL2/SDL.h>
#include <obj/model.h>

#include <stdlib.h>
#include <stdio.h>

typedef struct Page {
    WorldObject* object;
    int turn_direction;
} Page;

typedef struct Scene {
    BoundingBox* bounding_boxes;
    size_t n_bounding_boxes;
    WorldObject* objects;
    char** object_ids;
    size_t n_objects;
    size_t n_static_objects;
    float light_intensity[4];
    float light_intensity_delta;
    bool desk_lamp_on;
    Page left_page;
    Page turning_page;
    Page right_page;
    size_t n_pages;
    size_t pages_turned;
    SDL_Surface* dot;
    GLuint* drawing_textures;
    GLuint grass_texture;
    GLuint skybox_texture;
} Scene;

/**
 * Initialize the scene by loading models.
 */
void init_scene(Scene* scene);

bool load_objects(Scene* scene, char const* scene_file);
size_t obj_index(const Scene* scene, char const* object_id);

/**
 * Set the lighting of the scene.
 */
void set_lighting(const Scene* scene);

/**
 * Set the current material.
 */
void set_material(const Material* material);

/**
 * Update the scene.
 */
void update_scene(Scene* scene, double time);

/**
 * Render the scene.
 */
void render_scene(const Scene* scene);

/**
 * Render every static world object.
 */
void render_static_objects(const Scene* scene);

/**
 * Render a world object.
 */
void render_object(const WorldObject* object);

/**
 * Render the grass below the desk.
 */
void render_grass(const Scene* scene);

/**
 * Render the grass below the desk.
 */
void render_skybox(const Scene* scene);

/**
 * Render the top pages for the book.
 */
void render_pages(const Scene* scene);

/**
 * Turn the page for the book.
 */
void turn_page(Scene* scene);

/**
 * Turn back the page for the book.
 */
void turn_page_back(Scene* scene);

/**
 * Draw the axes of the world coordinate system.
 */
void draw_axes();

/**
 * Destroy the scene.
 */
void destroy_scene(Scene* scene);

#endif /* SCENE_H */

#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texture.h"

#include <obj/model.h>

#include <stdlib.h>

typedef struct WorldObject {
  Model model;
  vec3 position;
  Material material;
  GLuint texture_id;
} WorldObject;

typedef struct Page {
  vec3 position;
  float height;
  float flatness;
  float rotation;
  bool is_turning;
} Page;

typedef struct Scene {
  WorldObject book;
  Page page;
  size_t n_pages;
  size_t pages_turned;
  WorldObject cube;
} Scene;

/**
 * Initialize the scene by loading models.
 */
void init_scene(Scene* scene);

/**
 * Set the lighting of the scene.
 */
void set_lighting();

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
 * Render a world object.
 */
void render_object(const WorldObject* object);

/**
 * Render the paper for the book.
 */
void render_paper(const Page* page);

/**
 * Turn the page for the book.
 */
void turn_page(Scene* scene);

/**
 * Turn back the page for the book.
 */
void turn_page_back(Scene* scene);

/**
 * Calculate the coordinates of the paper's curve.
 */
vec3 paper_curve(float t);

/**
 * Draw the axes of the world coordinate system.
 */
void draw_axes();

#endif /* SCENE_H */

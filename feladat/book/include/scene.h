#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texture.h"

#include <obj/model.h>

typedef struct WorldObject {
  Model model;
  vec3 position;
  Material material;
  GLuint texture_id;
} WorldObject;

typedef struct Scene {
  WorldObject book;
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
void update_scene(Scene* scene);

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
void render_paper();

/**
 * Calculate the coordinates of the paper's curve.
 */
vec3 paper_curve(float t);

/**
 * Draw the axes of the world coordinate system.
 */
void draw_axes();

#endif /* SCENE_H */

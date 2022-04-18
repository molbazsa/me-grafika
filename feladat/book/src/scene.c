#include "scene.h"

#include <obj/load.h>
#include <obj/draw.h>

#define _USE_MATH_DEFINES
#include <math.h>

void init_scene(Scene* scene) {
  load_model(&(scene->book.model), "assets/models/cube.obj");
  scene->book.texture_id = load_texture("assets/textures/dino.jpg");

  scene->book.material.ambient.red = 1.0;
  scene->book.material.ambient.green = 1.0;
  scene->book.material.ambient.blue = 1.0;

  scene->book.material.diffuse.red = 1.0;
  scene->book.material.diffuse.green = 1.0;
  scene->book.material.diffuse.blue = 1.0;

  scene->book.material.specular.red = 0.0;
  scene->book.material.specular.green = 0.0;
  scene->book.material.specular.blue = 0.0;

  scene->book.material.shininess = 0.0;

  scene->book.position.x = -2;
  scene->book.position.y = 0;
  scene->book.position.z = 0;

  load_model(&(scene->cube.model), "assets/models/cube.obj");
  scene->cube.texture_id = load_texture("assets/textures/dino_inv.jpg");

  scene->cube.material.ambient.red = 1.0;
  scene->cube.material.ambient.green = 1.0;
  scene->cube.material.ambient.blue = 1.0;

  scene->cube.material.diffuse.red = 1.0;
  scene->cube.material.diffuse.green = 1.0;
  scene->cube.material.diffuse.blue = 1.0;

  scene->cube.material.specular.red = 0.0;
  scene->cube.material.specular.green = 0.0;
  scene->cube.material.specular.blue = 0.0;

  scene->cube.material.shininess = 0.0;

  scene->cube.position.x = 1;
  scene->cube.position.y = 0;
  scene->cube.position.z = 0;
}

void set_lighting() {
  float ambient_light[] = {0.2f, 0.2f, 0.2f, 1.0f};
  float diffuse_light[] = {1.0f, 1.0f, 1.0, 1.0f};
  float specular_light[] = {0.0f, 0.0f, 0.0f, 1.0f};
  float position[] = {0.0f, 0.0f, 10.0f, 1.0f};

  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
  glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void set_material(const Material* material) {
  float ambient_material_color[] = {
    material->ambient.red, material->ambient.green, material->ambient.blue};

  float diffuse_material_color[] = {
    material->diffuse.red, material->diffuse.green, material->diffuse.blue};

  float specular_material_color[] = {
    material->specular.red, material->specular.green, material->specular.blue};

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);

  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}

void update_scene(Scene* scene) {
  (void) scene;
}

void render_scene(const Scene* scene) {
  set_lighting();

  draw_axes();

  render_object(&(scene->book));
  // render_object(&(scene->cube));

  render_paper();
}

void render_object(const WorldObject* object) {
  glPushMatrix();

  glTranslatef(object->position.x, object->position.y, object->position.z);
  glBindTexture(GL_TEXTURE_2D, object->texture_id);
  set_material(&(object->material));
  draw_model(&(object->model));

  glPopMatrix();
}

void render_paper() {
  float t;
  float delta;

  float bottom = -2.5f;
  float top = 2.5f;

  glDisable(GL_TEXTURE_2D);

  glBegin(GL_QUAD_STRIP);

  glColor3f(1, 1, 1);

  t = 0.04f;
  vec3 curve = paper_curve(t);
  glVertex3f(curve.x, bottom, curve.z);
  glVertex3f(curve.x, top, curve.z);

  delta = 0.001f;

  while (t < 0.2) {
    t += delta;
    vec3 curve = paper_curve(t);
    glVertex3f(curve.x, bottom, curve.z);
    glVertex3f(curve.x, top, curve.z);
  }

  delta = 0.01f;

  while (t < 1.5) {
    t += delta;
    vec3 curve = paper_curve(t);
    glVertex3f(curve.x, bottom, curve.z);
    glVertex3f(curve.x, top, curve.z);
  }

  delta = 0.04f;

  while (t < M_PI) {
    t += delta;
    vec3 curve = paper_curve(t);
    glVertex3f(curve.x, bottom, curve.z);
    glVertex3f(curve.x, top, curve.z);
  }

  glEnd();

  glEnable(GL_TEXTURE_2D);
}

vec3 paper_curve(float t) {
  vec3 curve = {
    .x = sqrt(1.0 / t) * cos(t),
    .y = 0,
    .z = sqrt(1.0 / t) * sin(t),
  };
  return curve;
}

void draw_axes() {
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);

  glBegin(GL_LINES);

  glColor3f(1, 0, 0);
  glVertex3f(0, 0, 0);
  glVertex3f(1, 0, 0);

  glColor3f(0, 1, 0);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 1, 0);

  glColor3f(0, 0, 1);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, 1);

  glEnd();

  glEnable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
}

#include "scene.h"

#include <obj/load.h>
#include <obj/draw.h>

#define _USE_MATH_DEFINES
#include <math.h>

void init_scene(Scene* scene) {
  load_model(&(scene->book.model), "assets/models/book.obj");
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

  scene->book.position.x = 0;
  scene->book.position.y = 0;
  scene->book.position.z = 0;

  scene->book.rotation.x = -90;
  scene->book.rotation.y = 0;
  scene->book.rotation.z = 0;

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

  scene->cube.rotation.x = 0;
  scene->cube.rotation.y = 0;
  scene->cube.rotation.z = 0;

  scene->page.position.x = 0;
  scene->page.position.y = 0;
  scene->page.position.z = 0;

  scene->page.height = 0.7;
  scene->page.width = 0.47;
  scene->page.flatness = 1;
  scene->page.rotation = 0;

  scene->page.is_turning = false;

  scene->n_pages = 1;
  scene->pages_turned = 0;
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

void update_scene(Scene* scene, double time) {
  double const TURN_SPEED = 2;

  if (scene->page.is_turning) {
    if (scene->page.rotation >= 180) {
      scene->page.rotation = 180;
      scene->page.flatness = -1;
      scene->page.is_turning = false;
      scene->pages_turned++;
    } else {
      scene->page.rotation += 180 * TURN_SPEED * time;
      scene->page.flatness -= 2 * TURN_SPEED * time;
    }
  }
}

void render_scene(const Scene* scene) {
  set_lighting();

  draw_axes();

  render_object(&(scene->book));
  // render_object(&(scene->cube));

  render_paper(&(scene->page));
}

void render_object(const WorldObject* object) {
  glPushMatrix();

  glTranslatef(object->position.x, object->position.y, object->position.z);
  glRotatef(object->rotation.x, -1, 0, 0);
  glRotatef(object->rotation.y, 0, -1, 0);
  glRotatef(object->rotation.z, 0, 0, -1);
  glBindTexture(GL_TEXTURE_2D, object->texture_id);
  set_material(&(object->material));
  draw_model(&(object->model));

  glPopMatrix();
}

void render_paper(const Page* page) {
  float t;
  float delta;

  float bottom = -page->height / 2;
  float top = page->height / 2;

  vec3 curve;
  vec3 curve_start;
  vec3 curve_end;

  glDisable(GL_TEXTURE_2D);

  t = 0.04f;
  curve_start = paper_curve(t);
  curve_end = paper_curve(M_PI);
  glPushMatrix();
  glTranslatef(page->position.x, page->position.y, page->position.z);
  glRotatef(page->rotation, 0, -1, 0);
  glScalef(page->width / (curve_start.x - curve_end.x),
           1,
           page->width / (curve_start.x - curve_end.x));
  glTranslatef(-curve_end.x + 0.009, 0, 0.01);

  glBegin(GL_QUAD_STRIP);

  glColor3f(1, 1, 1);

  t = 0.04f;
  curve = paper_curve(t);
  glVertex3f(curve.x, bottom, page->flatness * curve.z);
  glVertex3f(curve.x, top, page->flatness * curve.z);

  delta = 0.001f;

  while (t < 0.2) {
    t += delta;
    curve = paper_curve(t);
    glVertex3f(curve.x, bottom, page->flatness * curve.z);
    glVertex3f(curve.x, top, page->flatness * curve.z);
  }

  delta = 0.01f;

  while (t < 1.5) {
    t += delta;
    curve = paper_curve(t);
    glVertex3f(curve.x, bottom, page->flatness * curve.z);
    glVertex3f(curve.x, top, page->flatness * curve.z);
  }

  delta = 0.04f;

  while (t < M_PI) {
    t += delta;
    curve = paper_curve(t);
    glVertex3f(curve.x, bottom, page->flatness * curve.z);
    glVertex3f(curve.x, top, page->flatness * curve.z);
  }

  glEnd();

  glPopMatrix();

  glEnable(GL_TEXTURE_2D);
}

void turn_page(Scene* scene) {
  if (!(scene->page.is_turning) && scene->pages_turned < scene->n_pages) {
    scene->page.is_turning = true;
  }
}

void turn_page_back(Scene* scene) {
  if (!(scene->page.is_turning) && scene->pages_turned > 0) {
    scene->page.rotation = 0;
    scene->page.flatness = 1;
    scene->pages_turned--;
  }
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

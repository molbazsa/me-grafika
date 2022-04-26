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

  scene->book.orientation.x = 0;
  scene->book.orientation.y = 0;
  scene->book.orientation.z = 0;

  scene->book.position.x = 0;
  scene->book.position.y = 0;
  scene->book.position.z = 0;

  scene->book.rotation.x = -90;
  scene->book.rotation.y = 0;
  scene->book.rotation.z = 0;

  scene->book.scale.x = 1;
  scene->book.scale.y = 1;
  scene->book.scale.z = 1;

  load_model(&(scene->page.object.model), "assets/models/page.obj");
  scene->page.object.texture_id = load_texture("assets/textures/dino.jpg");

  scene->page.object.material.ambient.red = 1.0;
  scene->page.object.material.ambient.green = 1.0;
  scene->page.object.material.ambient.blue = 1.0;

  scene->page.object.material.diffuse.red = 1.0;
  scene->page.object.material.diffuse.green = 1.0;
  scene->page.object.material.diffuse.blue = 1.0;

  scene->page.object.material.specular.red = 0.0;
  scene->page.object.material.specular.green = 0.0;
  scene->page.object.material.specular.blue = 0.0;

  scene->page.object.material.shininess = 0.0;

  scene->page.object.position.x = 0;
  scene->page.object.position.y = 0;
  scene->page.object.position.z = 0;

  scene->page.object.orientation.x = -90;
  scene->page.object.orientation.y = 0;
  scene->page.object.orientation.z = 0;

  scene->page.object.rotation.x = 0;
  scene->page.object.rotation.y = 0;
  scene->page.object.rotation.z = 0;

  scene->page.object.scale.x = 1;
  scene->page.object.scale.y = 1;
  scene->page.object.scale.z = 1;

  scene->page.turn_direction = 0;

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

  if (scene->page.turn_direction == 1) {
    if (scene->page.object.rotation.y >= 180) {
      scene->page.object.rotation.y = 180;
      scene->page.object.scale.z = -1;
      scene->page.turn_direction = 0;
      scene->pages_turned++;
    } else {
      scene->page.object.rotation.y += 180 * TURN_SPEED * time;
      scene->page.object.scale.z -= 2 * TURN_SPEED * time;
    }
  } else if (scene->page.turn_direction == -1) {
    if (scene->page.object.rotation.y <= 0) {
      scene->page.object.rotation.y = 0;
      scene->page.object.scale.z = 1;
      scene->page.turn_direction = 0;
      scene->pages_turned--;
    } else {
      scene->page.object.rotation.y -= 180 * TURN_SPEED * time;
      scene->page.object.scale.z += 2 * TURN_SPEED * time;
    }
  }
}

void render_scene(const Scene* scene) {
  set_lighting();

  draw_axes();

  render_object(&(scene->book));
  render_object(&(scene->page.object));
}

void render_object(const WorldObject* object) {
  glPushMatrix();

  glTranslatef(object->position.x, object->position.y, object->position.z);
  glRotatef(object->rotation.z, 0, 0, -1);
  glRotatef(object->rotation.y, 0, -1, 0);
  glRotatef(object->rotation.x, -1, 0, 0);
  glScalef(object->scale.x, object->scale.y, object->scale.z);
  glRotatef(object->orientation.z, 0, 0, -1);
  glRotatef(object->orientation.y, 0, -1, 0);
  glRotatef(object->orientation.x, -1, 0, 0);
  glBindTexture(GL_TEXTURE_2D, object->texture_id);
  set_material(&(object->material));
  draw_model(&(object->model));

  glPopMatrix();
}

void turn_page(Scene* scene) {
  if (scene->page.turn_direction == 0 && scene->pages_turned < scene->n_pages) {
    scene->page.turn_direction = 1;
  }
}

void turn_page_back(Scene* scene) {
  if (scene->page.turn_direction == 0 && scene->pages_turned > 0) {
    scene->page.turn_direction = -1;
  }
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

#include "scene.h"

#include <SDL2/SDL_image.h>
#include <obj/load.h>
#include <obj/draw.h>

#define _USE_MATH_DEFINES
#include <math.h>

void init_scene(Scene* scene) {
    load_model(&(scene->desk.model), "assets/models/desk.obj");
    scene->desk.texture_id = load_texture("assets/textures/desk.jpg");

    scene->desk.material.ambient.red = 1.0;
    scene->desk.material.ambient.green = 1.0;
    scene->desk.material.ambient.blue = 1.0;

    scene->desk.material.diffuse.red = 1.0;
    scene->desk.material.diffuse.green = 1.0;
    scene->desk.material.diffuse.blue = 1.0;

    scene->desk.material.specular.red = 0.0;
    scene->desk.material.specular.green = 0.0;
    scene->desk.material.specular.blue = 0.0;

    scene->desk.material.shininess = 0.0;

    scene->desk.orientation.x = 0;
    scene->desk.orientation.y = 0;
    scene->desk.orientation.z = 0;

    scene->desk.position.x = 0;
    scene->desk.position.y = 0;
    scene->desk.position.z = -0.055;

    scene->desk.rotation.x = 180;
    scene->desk.rotation.y = 0;
    scene->desk.rotation.z = 0;

    scene->desk.scale.x = 0.4;
    scene->desk.scale.y = 0.4;
    scene->desk.scale.z = 0.4;

    load_model(&(scene->cover.model), "assets/models/cover.obj");
    scene->cover.texture_id = load_texture("assets/textures/leather2.jpg");

    scene->cover.material.ambient.red = 1.0;
    scene->cover.material.ambient.green = 1.0;
    scene->cover.material.ambient.blue = 1.0;

    scene->cover.material.diffuse.red = 1.0;
    scene->cover.material.diffuse.green = 1.0;
    scene->cover.material.diffuse.blue = 1.0;

    scene->cover.material.specular.red = 0.0;
    scene->cover.material.specular.green = 0.0;
    scene->cover.material.specular.blue = 0.0;

    scene->cover.material.shininess = 0.0;

    scene->cover.orientation.x = 0;
    scene->cover.orientation.y = 0;
    scene->cover.orientation.z = 0;

    scene->cover.position.x = 0;
    scene->cover.position.y = 0;
    scene->cover.position.z = 0;

    scene->cover.rotation.x = -90;
    scene->cover.rotation.y = 0;
    scene->cover.rotation.z = 0;

    scene->cover.scale.x = 1;
    scene->cover.scale.y = 1;
    scene->cover.scale.z = 1;

    load_model(&(scene->pages.model), "assets/models/pages.obj");
    scene->pages.texture_id = load_texture("assets/textures/page.jpg");

    scene->pages.material.ambient.red = 1.0;
    scene->pages.material.ambient.green = 1.0;
    scene->pages.material.ambient.blue = 1.0;

    scene->pages.material.diffuse.red = 1.0;
    scene->pages.material.diffuse.green = 1.0;
    scene->pages.material.diffuse.blue = 1.0;

    scene->pages.material.specular.red = 0.0;
    scene->pages.material.specular.green = 0.0;
    scene->pages.material.specular.blue = 0.0;

    scene->pages.material.shininess = 0.0;

    scene->pages.orientation.x = 0;
    scene->pages.orientation.y = 0;
    scene->pages.orientation.z = 0;

    scene->pages.position.x = 0;
    scene->pages.position.y = 0;
    scene->pages.position.z = 0;

    scene->pages.rotation.x = -90;
    scene->pages.rotation.y = 0;
    scene->pages.rotation.z = 0;

    scene->pages.scale.x = 1;
    scene->pages.scale.y = 1;
    scene->pages.scale.z = 1;

    scene->n_pages = 3;
    scene->pages_turned = 0;

    scene->drawing_textures = malloc((scene->n_pages + 2) * sizeof(GLuint));
    for (size_t i = 0; i < scene->n_pages + 2; i++) {
        scene->drawing_textures[i] = load_texture("assets/textures/page.jpg");
    }

    load_model(&(scene->left_page.object.model), "assets/models/page.obj");

    scene->left_page.object.material.ambient.red = 1.0;
    scene->left_page.object.material.ambient.green = 1.0;
    scene->left_page.object.material.ambient.blue = 1.0;

    scene->left_page.object.material.diffuse.red = 1.0;
    scene->left_page.object.material.diffuse.green = 1.0;
    scene->left_page.object.material.diffuse.blue = 1.0;

    scene->left_page.object.material.specular.red = 0.0;
    scene->left_page.object.material.specular.green = 0.0;
    scene->left_page.object.material.specular.blue = 0.0;

    scene->left_page.object.material.shininess = 0.0;

    scene->left_page.object.position.x = 0;
    scene->left_page.object.position.y = 0;
    scene->left_page.object.position.z = 0;

    scene->left_page.object.orientation.x = -90;
    scene->left_page.object.orientation.y = 0;
    scene->left_page.object.orientation.z = 0;

    scene->left_page.object.rotation.x = 0;
    scene->left_page.object.rotation.y = 180;
    scene->left_page.object.rotation.z = 0;

    scene->left_page.object.scale.x = 1;
    scene->left_page.object.scale.y = 1;
    scene->left_page.object.scale.z = -1;

    scene->left_page.turn_direction = 0;

    load_model(&(scene->turning_page.object.model), "assets/models/page.obj");

    scene->turning_page.object.material.ambient.red = 1.0;
    scene->turning_page.object.material.ambient.green = 1.0;
    scene->turning_page.object.material.ambient.blue = 1.0;

    scene->turning_page.object.material.diffuse.red = 1.0;
    scene->turning_page.object.material.diffuse.green = 1.0;
    scene->turning_page.object.material.diffuse.blue = 1.0;

    scene->turning_page.object.material.specular.red = 0.0;
    scene->turning_page.object.material.specular.green = 0.0;
    scene->turning_page.object.material.specular.blue = 0.0;

    scene->turning_page.object.material.shininess = 0.0;

    scene->turning_page.object.position.x = 0;
    scene->turning_page.object.position.y = 0;
    scene->turning_page.object.position.z = 0;

    scene->turning_page.object.orientation.x = -90;
    scene->turning_page.object.orientation.y = 0;
    scene->turning_page.object.orientation.z = 0;

    scene->turning_page.object.rotation.x = 0;
    scene->turning_page.object.rotation.y = 0;
    scene->turning_page.object.rotation.z = 0;

    scene->turning_page.object.scale.x = 1;
    scene->turning_page.object.scale.y = 1;
    scene->turning_page.object.scale.z = 1;

    scene->turning_page.turn_direction = 0;

    load_model(&(scene->right_page.object.model), "assets/models/page.obj");

    scene->right_page.object.material.ambient.red = 1.0;
    scene->right_page.object.material.ambient.green = 1.0;
    scene->right_page.object.material.ambient.blue = 1.0;

    scene->right_page.object.material.diffuse.red = 1.0;
    scene->right_page.object.material.diffuse.green = 1.0;
    scene->right_page.object.material.diffuse.blue = 1.0;

    scene->right_page.object.material.specular.red = 0.0;
    scene->right_page.object.material.specular.green = 0.0;
    scene->right_page.object.material.specular.blue = 0.0;

    scene->right_page.object.material.shininess = 0.0;

    scene->right_page.object.position.x = 0;
    scene->right_page.object.position.y = 0;
    scene->right_page.object.position.z = 0;

    scene->right_page.object.orientation.x = -90;
    scene->right_page.object.orientation.y = 0;
    scene->right_page.object.orientation.z = 0;

    scene->right_page.object.rotation.x = 0;
    scene->right_page.object.rotation.y = 0;
    scene->right_page.object.rotation.z = 0;

    scene->right_page.object.scale.x = 1;
    scene->right_page.object.scale.y = 1;
    scene->right_page.object.scale.z = 1;

    scene->right_page.turn_direction = 0;

    scene->left_page.object.texture_id =
        scene->drawing_textures[scene->pages_turned];

    scene->turning_page.object.texture_id =
        scene->drawing_textures[scene->pages_turned + 1];

    scene->right_page.object.texture_id =
        scene->drawing_textures[scene->pages_turned + 1];

    scene->dot = IMG_Load("assets/textures/dot.jpg");
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

    float specular_material_color[] = {material->specular.red,
                                       material->specular.green,
                                       material->specular.blue};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}

void update_scene(Scene* scene, double time) {
    double const TURN_SPEED = 2;

    if (scene->turning_page.turn_direction == 1) {
        scene->turning_page.object.rotation.y += 180 * TURN_SPEED * time;
        scene->turning_page.object.scale.z -= 2 * TURN_SPEED * time;
        if (scene->turning_page.object.rotation.y >= 180) {
            scene->turning_page.turn_direction = 0;
            scene->pages_turned++;
            scene->left_page.object.texture_id =
                scene->drawing_textures[scene->pages_turned];
        }
    } else if (scene->turning_page.turn_direction == -1) {
        scene->turning_page.object.rotation.y -= 180 * TURN_SPEED * time;
        scene->turning_page.object.scale.z += 2 * TURN_SPEED * time;
        if (scene->turning_page.object.rotation.y <= 0) {
            scene->turning_page.turn_direction = 0;
            scene->pages_turned--;
            scene->right_page.object.texture_id =
                scene->drawing_textures[scene->pages_turned + 1];
        }
    }
}

void render_scene(const Scene* scene) {
    set_lighting();

    // draw_axes();

    render_object(&(scene->desk));
    render_object(&(scene->cover));
    render_object(&(scene->pages));
    render_pages(scene);
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

void render_pages(const Scene* scene) {
    if (scene->turning_page.turn_direction != 0) {
        render_object(&(scene->turning_page.object));
    }
    render_object(&(scene->left_page.object));
    render_object(&(scene->right_page.object));
}

void turn_page(Scene* scene) {
    if (scene->turning_page.turn_direction == 0 &&
        scene->pages_turned < scene->n_pages) {
        scene->turning_page.object.rotation.y = 0;
        scene->turning_page.object.scale.z = 1;
        scene->turning_page.turn_direction = 1;
        scene->turning_page.object.texture_id =
            scene->drawing_textures[scene->pages_turned + 1];
        scene->right_page.object.texture_id =
            scene->drawing_textures[scene->pages_turned + 2];
    }
}

void turn_page_back(Scene* scene) {
    if (scene->turning_page.turn_direction == 0 && scene->pages_turned > 0) {
        scene->turning_page.object.rotation.y = 180;
        scene->turning_page.object.scale.z = -1;
        scene->turning_page.turn_direction = -1;
        scene->left_page.object.texture_id =
            scene->drawing_textures[scene->pages_turned - 1];
        scene->turning_page.object.texture_id =
            scene->drawing_textures[scene->pages_turned];
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

void destroy_scene(Scene* scene) {
    free(scene->drawing_textures);
    SDL_FreeSurface(scene->dot);
}

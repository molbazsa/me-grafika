#include "scene.h"

#include <SDL2/SDL_image.h>
#include <obj/load.h>
#include <obj/draw.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

void init_scene(Scene* scene) {
    char* scene_file = "src/desk.scene";
    bool success = load_objects(scene, scene_file);
    if (!success) {
        fprintf(stderr, "Syntax error in %s\n", scene_file);
        exit(EXIT_FAILURE);
    }

    printf("Loaded objects:\n");
    size_t i = 0;
    printf("- Static:\n");
    while (i < scene->n_static_objects) {
        printf("    %s\n", scene->object_ids[i]);
        i++;
    }
    printf("- Normal:\n");
    while (i < scene->n_objects) {
        printf("    %s\n", scene->object_ids[i]);
        i++;
    }

    scene->grass_texture = load_texture("assets/textures/grass.jpg");
    scene->skybox_texture = load_texture("assets/textures/skybox.jpg");

    scene->n_pages = 3;
    scene->pages_turned = 0;

    scene->left_page.object = scene->objects + obj_index(scene, "left_page");
    scene->turning_page.object =
        scene->objects + obj_index(scene, "turning_page");
    scene->right_page.object = scene->objects + obj_index(scene, "right_page");

    scene->left_page.turn_direction = 0;
    scene->turning_page.turn_direction = 0;
    scene->right_page.turn_direction = 0;

    scene->drawing_textures = malloc((scene->n_pages + 2) * sizeof(GLuint));
    for (size_t i = 0; i < scene->n_pages + 2; i++) {
        scene->drawing_textures[i] = load_texture("assets/textures/page.jpg");
    }

    scene->left_page.object->texture_id =
        scene->drawing_textures[scene->pages_turned];
    scene->turning_page.object->texture_id =
        scene->drawing_textures[scene->pages_turned + 1];
    scene->right_page.object->texture_id =
        scene->drawing_textures[scene->pages_turned + 1];

    scene->dot = IMG_Load("assets/textures/dot.jpg");
}

void set_object_props(Scene* scene,
                      size_t current_obj,
                      SceneFileCommand command,
                      SceneFileParam param) {
    switch (command) {
        case model_command:
            load_model(&(scene->objects[current_obj].model),
                        param.filename);
            break;
        case texture_command:
            scene->objects[current_obj].texture_id =
                load_texture(param.filename);
            break;

        case mat_ambient_command:
            scene->objects[current_obj].material.ambient.red =
                param.vector.x;
            scene->objects[current_obj].material.ambient.green =
                param.vector.y;
            scene->objects[current_obj].material.ambient.blue =
                param.vector.z;
            break;
        case mat_diffuse_command:
            scene->objects[current_obj].material.diffuse.red =
                param.vector.x;
            scene->objects[current_obj].material.diffuse.green =
                param.vector.y;
            scene->objects[current_obj].material.diffuse.blue =
                param.vector.z;
            break;
        case mat_specular_command:
            scene->objects[current_obj].material.specular.red =
                param.vector.x;
            scene->objects[current_obj].material.specular.green =
                param.vector.y;
            scene->objects[current_obj].material.specular.blue =
                param.vector.z;
            break;
        case mat_shininess_command:
            scene->objects[current_obj].material.shininess =
                param.float_val;
            break;

        case orientation_command:
            scene->objects[current_obj].orientation = param.vector;
            break;
        case position_command:
            scene->objects[current_obj].position = param.vector;
            break;
        case rotation_command:
            scene->objects[current_obj].rotation = param.vector;
            break;
        case scale_command:
            scene->objects[current_obj].scale = param.vector;
            break;

        default:
            break;
    }
}

bool load_objects(Scene* scene, char const* scene_file) {
    char line[BUFSIZ];
    char* success;
    SceneFileParam param;
    bool skip;

    FILE* scenef = fopen(scene_file, "r");
    if (!scenef) {
        return false;
    }

    size_t n_objects = 0;
    size_t n_bounding_boxes = 0;

    success = fgets(line, BUFSIZ, scenef);
    while (success) {
        SceneFileCommand command = parse_scene_file_command(line, &param);
        switch (command) {
            case syntax_error:
                fclose(scenef);
                return false;
            case static_object_command:
            case object_command:
                n_objects++;
                break;
            case bounding_box_command:
                n_bounding_boxes++;
                break;
            default:
                break;
        }
        success = fgets(line, BUFSIZ, scenef);
    }

    scene->n_objects = n_objects;
    scene->objects = malloc(n_objects * sizeof(WorldObject));
    scene->object_ids = malloc(n_objects * sizeof(char*));
    for (size_t i = 0; i < n_objects; i++) {
        scene->object_ids[i] = malloc(BUFSIZ * sizeof(char));
    }

    scene->n_bounding_boxes = n_bounding_boxes;
    scene->bounding_boxes =
        malloc(scene->n_bounding_boxes * sizeof(BoundingBox));

    size_t current_obj = -1;
    size_t current_bbox = 0;

    size_t n_static_objects = 0;

    rewind(scenef);
    success = fgets(line, BUFSIZ, scenef);
    while (success) {
        SceneFileCommand command = parse_scene_file_command(line, &param);

        if (command == static_object_command) {
            skip = false;
            n_static_objects++;
            current_obj++;
            strcpy(scene->object_ids[current_obj], param.filename);
        } else if (command == object_command) {
            skip = true;
        }

        if (!skip) {
            if (command == bounding_box_command) {
                scene->bounding_boxes[current_bbox] =
                    bounding_box(&(scene->objects[current_obj]), param.vector);
                current_bbox++;
            } else {
                set_object_props(scene, current_obj, command, param);
            }
        }

        success = fgets(line, BUFSIZ, scenef);
    }

    scene->n_static_objects = n_static_objects;

    rewind(scenef);
    success = fgets(line, BUFSIZ, scenef);
    while (success) {
        SceneFileCommand command = parse_scene_file_command(line, &param);

        if (command == object_command) {
            skip = false;
            current_obj++;
            strcpy(scene->object_ids[current_obj], param.filename);
        } else if (command == static_object_command) {
            skip = true;
        }

        if (!skip) {
            if (command == bounding_box_command) {
                scene->bounding_boxes[current_bbox] =
                    bounding_box(&(scene->objects[current_obj]), param.vector);
                current_bbox++;
            } else {
                set_object_props(scene, current_obj, command, param);
            }
        }

        success = fgets(line, BUFSIZ, scenef);
    }

    fclose(scenef);
    return true;
}

size_t obj_index(const Scene* scene, char const* object_id) {
    size_t index = -1;

    for (size_t i = 0; i < scene->n_objects; i++) {
        if (strcmp(scene->object_ids[i], object_id) == 0) {
            index = i;
            break;
        }
    }

    return index;
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
        scene->turning_page.object->rotation.y += 180 * TURN_SPEED * time;
        scene->turning_page.object->scale.z -= 2 * TURN_SPEED * time;
        if (scene->turning_page.object->rotation.y >= 180) {
            scene->turning_page.turn_direction = 0;
            scene->pages_turned++;
            scene->left_page.object->texture_id =
                scene->drawing_textures[scene->pages_turned];
        }
    } else if (scene->turning_page.turn_direction == -1) {
        scene->turning_page.object->rotation.y -= 180 * TURN_SPEED * time;
        scene->turning_page.object->scale.z += 2 * TURN_SPEED * time;
        if (scene->turning_page.object->rotation.y <= 0) {
            scene->turning_page.turn_direction = 0;
            scene->pages_turned--;
            scene->right_page.object->texture_id =
                scene->drawing_textures[scene->pages_turned + 1];
        }
    }
}

void render_scene(const Scene* scene) {
    set_lighting();

    // draw_axes();

    render_static_objects(scene);
    render_pages(scene);

    render_grass(scene);
    render_skybox(scene);
}

void render_static_objects(const Scene* scene) {
    for (size_t i = 0; i < scene->n_static_objects; i++) {
        render_object(&scene->objects[i]);
    }
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

void render_grass(const Scene* scene) {
    double size = 50;
    double height = -1.6;

    float position[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glBindTexture(GL_TEXTURE_2D, scene->grass_texture);

    glBegin(GL_QUADS);

    glNormal3f(0, 0, 1.0);

    glTexCoord2f(0, 0);
    glVertex3f(-size, -size, height);

    glTexCoord2f(1, 0);
    glVertex3f(size, -size, height);

    glTexCoord2f(1, 1);
    glVertex3f(size, size, height);

    glTexCoord2f(0, 1);
    glVertex3f(-size, size, height);

    glEnd();
}

void render_skybox(const Scene* scene) {
    double size = 50;

    glBindTexture(GL_TEXTURE_2D, scene->skybox_texture);

    glBegin(GL_QUADS);

    glNormal3f(0, -1, 0);

    glTexCoord2f(0, 1 / (float) 3);
    glVertex3f(-size, size, size);

    glTexCoord2f(0.25, 1 / (float) 3);
    glVertex3f(size, size, size);

    glTexCoord2f(0.25, 2 / (float) 3);
    glVertex3f(size, size, -size);

    glTexCoord2f(0, 2 / (float) 3);
    glVertex3f(-size, size, -size);

    glNormal3f(-1, 0, 0);

    glTexCoord2f(0.25, 1 / (float) 3);
    glVertex3f(size, size, size);

    glTexCoord2f(0.5, 1 / (float) 3);
    glVertex3f(size, -size, size);

    glTexCoord2f(0.5, 2 / (float) 3);
    glVertex3f(size, -size, -size);

    glTexCoord2f(0.25, 2 / (float) 3);
    glVertex3f(size, size, -size);

    glNormal3f(0, 1, 0);

    glTexCoord2f(0.5, 1 / (float) 3);
    glVertex3f(size, -size, size);

    glTexCoord2f(0.75, 1 / (float) 3);
    glVertex3f(-size, -size, size);

    glTexCoord2f(0.75, 2 / (float) 3);
    glVertex3f(-size, -size, -size);

    glTexCoord2f(0.5, 2 / (float) 3);
    glVertex3f(size, -size, -size);

    glNormal3f(1, 0, 0);

    glTexCoord2f(0.75, 1 / (float) 3);
    glVertex3f(-size, -size, size);

    glTexCoord2f(1, 1 / (float) 3);
    glVertex3f(-size, size, size);

    glTexCoord2f(1, 2 / (float) 3);
    glVertex3f(-size, size, -size);

    glTexCoord2f(0.75, 2 / (float) 3);
    glVertex3f(-size, -size, -size);

    glNormal3f(0, 0, -1);

    glTexCoord2f(0.25, 1 / (float) 3);
    glVertex3f(size, size, size);

    glTexCoord2f(0.5, 1 / (float) 3);
    glVertex3f(size, -size, size);

    glTexCoord2f(0.5, 0);
    glVertex3f(-size, -size, size);

    glTexCoord2f(0.25, 0);
    glVertex3f(-size, size, size);

    glEnd();
}

void render_pages(const Scene* scene) {
    if (scene->turning_page.turn_direction != 0) {
        render_object(scene->turning_page.object);
    }
    render_object(scene->left_page.object);
    render_object(scene->right_page.object);
}

void turn_page(Scene* scene) {
    if (scene->turning_page.turn_direction == 0 &&
        scene->pages_turned < scene->n_pages) {
        scene->turning_page.object->rotation.y = 0;
        scene->turning_page.object->scale.z = 1;
        scene->turning_page.turn_direction = 1;
        scene->turning_page.object->texture_id =
            scene->drawing_textures[scene->pages_turned + 1];
        scene->right_page.object->texture_id =
            scene->drawing_textures[scene->pages_turned + 2];
    }
}

void turn_page_back(Scene* scene) {
    if (scene->turning_page.turn_direction == 0 && scene->pages_turned > 0) {
        scene->turning_page.object->rotation.y = 180;
        scene->turning_page.object->scale.z = -1;
        scene->turning_page.turn_direction = -1;
        scene->left_page.object->texture_id =
            scene->drawing_textures[scene->pages_turned - 1];
        scene->turning_page.object->texture_id =
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

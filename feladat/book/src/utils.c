#include "utils.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

double degree_to_radian(double degree) {
    return degree * M_PI / 180.0;
}

BoundingBox bounding_box(const WorldObject* object, vec3 padding) {
    BoundingBox box;
    GLfloat view[16];
    const Model* model = &(object->model);
    vec3 orientation = object->orientation;
    vec3 position = object->position;
    vec3 rotation = object->rotation;
    vec3 scale = object->scale;

    int i;
    double x, y, z;
    double min_x, max_x, min_y, max_y, min_z, max_z;

    if (model->n_vertices == 0) {
        box.min_x = 0;
        box.max_x = 0;
        box.min_y = 0;
        box.max_y = 0;
        box.min_z = 0;
        box.max_z = 0;
        return box;
    }

    min_x = model->vertices[0].x;
    max_x = model->vertices[0].x;
    min_y = model->vertices[0].y;
    max_y = model->vertices[0].y;
    min_z = model->vertices[0].z;
    max_z = model->vertices[0].z;

    for (i = 0; i < model->n_vertices; ++i) {
        x = model->vertices[i].x;
        y = model->vertices[i].y;
        z = model->vertices[i].z;
        if (x < min_x) {
            min_x = x;
        } else if (x > max_x) {
            max_x = x;
        }
        if (y < min_y) {
            min_y = y;
        } else if (y > max_y) {
            max_y = y;
        }
        if (z < min_z) {
            min_z = z;
        } else if (z > max_z) {
            max_z = z;
        }
    }

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glTranslatef(position.x, position.y, position.z);
    glRotatef(rotation.z, 0, 0, -1);
    glRotatef(rotation.y, 0, -1, 0);
    glRotatef(rotation.x, -1, 0, 0);
    glScalef(scale.x, scale.y, scale.z);
    glRotatef(orientation.z, 0, 0, -1);
    glRotatef(orientation.y, 0, -1, 0);
    glRotatef(orientation.x, -1, 0, 0);

    glGetFloatv(GL_MODELVIEW_MATRIX, view);
    glPopMatrix();

    box.min_x = view[0] * min_x + view[4] * min_y + view[8] * min_z + view[12];
    box.min_y = view[1] * min_x + view[5] * min_y + view[9] * min_z + view[13];
    box.min_z = view[2] * min_x + view[6] * min_y + view[10] * min_z + view[14];
    box.max_x = view[0] * max_x + view[4] * max_y + view[8] * max_z + view[12];
    box.max_y = view[1] * max_x + view[5] * max_y + view[9] * max_z + view[13];
    box.max_z = view[2] * max_x + view[6] * max_y + view[10] * max_z + view[14];

    if (box.min_x > box.max_x) {
        double min = box.max_x;
        box.max_x = box.min_x;
        box.min_x = min;
    }

    if (box.min_y > box.max_y) {
        double min = box.max_y;
        box.max_y = box.min_y;
        box.min_y = min;
    }

    if (box.min_z > box.max_z) {
        double min = box.max_z;
        box.max_z = box.min_z;
        box.min_z = min;
    }

    box.min_x -= padding.x;
    box.max_x += padding.x;
    box.min_y -= padding.y;
    box.max_y += padding.y;
    box.min_z -= padding.z;
    box.max_z += padding.z;

    return box;
}

bool is_in_bounding_box(vec3 position, BoundingBox bounding_box) {
    return (
        bounding_box.min_x < position.x && position.x < bounding_box.max_x &&
        bounding_box.min_y < position.y && position.y < bounding_box.max_y &&
        bounding_box.min_z < position.z && position.z < bounding_box.max_z);
}

void parse_filename(char* line, char* keyword, SceneFileParam* param) {
    line[strlen(line) - 1] = 0;
    line += strlen(keyword);
    while (isspace(*line)) {
        line++;
    }
    param->filename = line;
}

void parse_vector(char* line, char* keyword, SceneFileParam* param) {
    line += strlen(keyword);
    while (isspace(*line)) {
        line++;
    }
    param->vector.x = atof(line);
    while (!isspace(*line)) {
        line++;
    }
    while (isspace(*line)) {
        line++;
    }
    param->vector.y = atof(line);
    while (!isspace(*line)) {
        line++;
    }
    while (isspace(*line)) {
        line++;
    }
    param->vector.z = atof(line);
}

void parse_float(char* line, char* keyword, SceneFileParam* param) {
    line += strlen(keyword);
    while (isspace(*line)) {
        line++;
    }
    param->float_val = atof(line);
}

SceneFileCommand parse_scene_file_command(char* line, SceneFileParam* param) {
    char* keyword;

    while (isspace(*line)) {
        line++;
    }

    if (!(*line)) {
        return empty_command;
    }

    keyword = "object";
    if (strncmp(keyword, line, strlen(keyword)) == 0) {
        parse_filename(line, keyword, param);
        return object_command;
    }

    keyword = "model";
    if (strncmp(keyword, line, strlen(keyword)) == 0) {
        parse_filename(line, keyword, param);
        return model_command;
    }

    keyword = "texture";
    if (strncmp(keyword, line, strlen(keyword)) == 0) {
        parse_filename(line, keyword, param);
        return texture_command;
    }

    keyword = "mat_ambient";
    if (strncmp(keyword, line, strlen(keyword)) == 0) {
        parse_vector(line, keyword, param);
        return mat_ambient_command;
    }

    keyword = "mat_diffuse";
    if (strncmp(keyword, line, strlen(keyword)) == 0) {
        parse_vector(line, keyword, param);
        return mat_diffuse_command;
    }

    keyword = "mat_specular";
    if (strncmp(keyword, line, strlen(keyword)) == 0) {
        parse_vector(line, keyword, param);
        return mat_specular_command;
    }

    keyword = "mat_shininess";
    if (strncmp(keyword, line, strlen(keyword)) == 0) {
        parse_float(line, keyword, param);
        return mat_shininess_command;
    }

    keyword = "orientation";
    if (strncmp(keyword, line, strlen(keyword)) == 0) {
        parse_vector(line, keyword, param);
        return orientation_command;
    }

    keyword = "position";
    if (strncmp(keyword, line, strlen(keyword)) == 0) {
        parse_vector(line, keyword, param);
        return position_command;
    }

    keyword = "rotation";
    if (strncmp(keyword, line, strlen(keyword)) == 0) {
        parse_vector(line, keyword, param);
        return rotation_command;
    }

    keyword = "scale";
    if (strncmp(keyword, line, strlen(keyword)) == 0) {
        parse_vector(line, keyword, param);
        return scale_command;
    }

    keyword = "bounding_box";
    if (strncmp(keyword, line, strlen(keyword)) == 0) {
        parse_vector(line, keyword, param);
        return bounding_box_command;
    }

    return syntax_error;
}

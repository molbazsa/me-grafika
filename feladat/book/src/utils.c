#include "utils.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

double degree_to_radian(double degree) {
    return degree * M_PI / 180.0;
}

BoundingBox bounding_box(const Model* model,
                         vec3 position,
                         vec3 orientation,
                         vec3 rotation,
                         vec3 scale,
                         vec3 padding) {
    BoundingBox box;
    GLfloat view[16];

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

    printf("%f, %f, %f, %f\n", view[0], view[1], view[2], view[3]);
    printf("%f, %f, %f, %f\n", view[4], view[5], view[6], view[7]);
    printf("%f, %f, %f, %f\n", view[8], view[9], view[10], view[11]);
    printf("%f, %f, %f, %f\n", view[12], view[13], view[14], view[15]);

    box.min_x = view[0] * min_x + view[1] * min_y + view[2] * min_z + view[3];
    box.min_y = view[4] * min_x + view[5] * min_y + view[6] * min_z + view[7];
    box.min_z = view[8] * min_x + view[9] * min_y + view[10] * min_z + view[11];
    box.max_x = view[0] * max_x + view[1] * max_y + view[2] * max_z + view[3];
    box.max_y = view[4] * max_x + view[5] * max_y + view[6] * max_z + view[7];
    box.max_z = view[8] * max_x + view[9] * max_y + view[10] * max_z + view[11];

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

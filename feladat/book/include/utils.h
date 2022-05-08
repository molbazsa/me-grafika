#ifndef UTILS_H
#define UTILS_H

#include <obj/model.h>
#include <stdbool.h>
#include <GL/gl.h>

/**
 * GLSL-like three dimensional vector
 */
typedef struct vec3 {
    float x;
    float y;
    float z;
} vec3;

/**
 * Represents the bounding box around the object
 */
typedef struct BoundingBox {
    double min_x;
    double max_x;
    double min_y;
    double max_y;
    double min_z;
    double max_z;
} BoundingBox;

/**
 * Calculate the bounding box of the model.
 */
BoundingBox bounding_box(const Model* model,
                         vec3 position,
                         vec3 orientation,
                         vec3 rotation,
                         vec3 scale,
                         vec3 padding);

/**
 * Check if the position is inside the bounding box.
 */
bool is_in_bounding_box(vec3 position, BoundingBox bounding_box);

/**
 * Color with RGB components
 */
typedef struct Color {
    float red;
    float green;
    float blue;
} Color;

/**
 * Material
 */
typedef struct Material {
    struct Color ambient;
    struct Color diffuse;
    struct Color specular;
    float shininess;
} Material;

/**
 * Calculates radian from degree.
 */
double degree_to_radian(double degree);

#endif /* UTILS_H */

#ifndef UTILS_H
#define UTILS_H

#include <obj/model.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

typedef struct WorldObject {
    Model model;
    vec3 position;
    vec3 orientation;
    vec3 rotation;
    vec3 scale;
    Material material;
    GLuint texture_id;
} WorldObject;

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
BoundingBox bounding_box(const WorldObject* object, vec3 padding);

/**
 * Check if the position is inside the bounding box.
 */
bool is_in_bounding_box(vec3 position, BoundingBox bounding_box);

enum SceneFileCommand {
    static_object_command,
    object_command,
    model_command,
    texture_command,
    mat_ambient_command,
    mat_diffuse_command,
    mat_specular_command,
    mat_shininess_command,
    orientation_command,
    position_command,
    rotation_command,
    scale_command,
    bounding_box_command,
    empty_command,
    syntax_error
};
typedef enum SceneFileCommand SceneFileCommand;

typedef union SceneFileParam SceneFileParam;
union SceneFileParam {
    char* filename;
    float float_val;
    vec3 vector;
};

SceneFileCommand parse_scene_file_command(char* line, SceneFileParam* param);

/**
 * Calculates radian from degree.
 */
double degree_to_radian(double degree);

#endif /* UTILS_H */

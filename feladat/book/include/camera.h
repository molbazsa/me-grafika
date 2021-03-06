#ifndef CAMERA_H
#define CAMERA_H

#include "utils.h"

#include <stdlib.h>

/**
 * Camera, as a moving point with direction
 */
typedef struct Camera {
    vec3 position;
    vec3 rotation;
    vec3 speed;
    bool is_preview_visible;
} Camera;

/**
 * Initialize the camera to the start position.
 */
void init_camera(Camera* camera);

/**
 * Update the position of the camera.
 */
void update_camera(Camera* camera,
                   double time,
                   const BoundingBox* boxes,
                   size_t n_boxes);

/**
 * Apply the camera settings to the view transformation.
 */
void set_view(const Camera* camera);

/**
 * Apply the top down view to the view transformation.
 */
void set_parallel_view();

/**
 * Set the horizontal and vertical rotation of the view angle.
 */
void rotate_camera(Camera* camera,
                   double horizontal,
                   double vertical,
                   double sensitivity);

/**
 * Set the speed of forward and backward motion.
 */
void set_camera_speed(Camera* camera, double speed);

/**
 * Set the speed of left and right side steps.
 */
void set_camera_side_speed(Camera* camera, double speed);

/**
 * Set the speed of vertical motion.
 */
void set_camera_vertical_speed(Camera* camera, double speed);

/**
 * Show texture in front of the view.
 */
void show_texture_preview();

#endif /* CAMERA_H */

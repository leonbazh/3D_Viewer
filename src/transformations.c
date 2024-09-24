#include <math.h>
#include "transformations.h"

void translate_model(Model *model, float tx, float ty, float tz) {
    for (int i = 0; i < model->vertex_count; i++) {
        model->vertices[i].x += tx;
        model->vertices[i].y += ty;
        model->vertices[i].z += tz;
    }
}

void rotate_model(Model *model, float angle_x, float angle_y, float angle_z) {
    // Apply rotations about the X, Y, and Z axes (in radians)
    for (int i = 0; i < model->vertex_count; i++) {
        Vertex *v = &model->vertices[i];

        // Rotate around X-axis
        float y_new = v->y * cos(angle_x) - v->z * sin(angle_x);
        float z_new = v->y * sin(angle_x) + v->z * cos(angle_x);
        v->y = y_new;
        v->z = z_new;

        // Rotate around Y-axis
        float x_new = v->x * cos(angle_y) + v->z * sin(angle_y);
        v->z = -v->x * sin(angle_y) + v->z * cos(angle_y);
        v->x = x_new;

        // Rotate around Z-axis
        x_new = v->x * cos(angle_z) - v->y * sin(angle_z);
        v->y = v->x * sin(angle_z) + v->y * cos(angle_z);
        v->x = x_new;
    }
}

void scale_model(Model *model, float scale_factor) {
    for (int i = 0; i < model->vertex_count; i++) {
        model->vertices[i].x *= scale_factor;
        model->vertices[i].y *= scale_factor;
        model->vertices[i].z *= scale_factor;
    }
}


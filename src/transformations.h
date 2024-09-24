#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include "model_loader.h"

void translate_model(Model *model, float tx, float ty, float tz);
void rotate_model(Model *model, float angle_x, float angle_y, float angle_z);
void scale_model(Model *model, float scale_factor);

#endif 


#include "model_loader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Model *load_model_from_obj(const char *filepath) {
  FILE *file = fopen(filepath, "r");
  if (!file) {
    fprintf(stderr, "Error: Cannot open file %s\n", filepath);
    return NULL;
  }

  Model *model = (Model *)malloc(sizeof(Model));
  model->vertex_count = 0;
  model->face_count = 0;
  model->vertices = NULL;
  model->faces = NULL;

  char line[256];
  while (fgets(line, sizeof(line), file)) {
    // Parse vertex data (v x y z)
    if (strncmp(line, "v ", 2) == 0) {
      model->vertex_count++;
      model->vertices = (Vertex *)realloc(model->vertices,
                                          model->vertex_count * sizeof(Vertex));
      if (sscanf(line, "v %f %f %f",
                 &model->vertices[model->vertex_count - 1].x,
                 &model->vertices[model->vertex_count - 1].y,
                 &model->vertices[model->vertex_count - 1].z) != 3) {
        fprintf(stderr, "Error: Malformed vertex data\n");
        free_model(model);
        return NULL;
      }
    }

    // Parse face data (f v1 v2 v3)
    else if (strncmp(line, "f ", 2) == 0) {
      model->face_count++;
      model->faces =
          (Face *)realloc(model->faces, model->face_count * sizeof(Face));
      Face *face = &model->faces[model->face_count - 1];

      // Parse only the vertex indices
      if (sscanf(line, "f %d %d %d", &face->v1, &face->v2, &face->v3) != 3) {
        fprintf(stderr, "Error: Malformed face data\n");
        free_model(model);
        return NULL;
      }
    }
  }

  fclose(file);
  return model;
}

void free_model(Model *model) {
  free(model->vertices);
  free(model->faces);
  free(model);
}

#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

// Struct to represent a vertex in 3D space
typedef struct {
    float x, y, z;
} Vertex;

// Struct to represent a face (triangle) by indexing vertices
typedef struct {
    int v1, v2, v3;  // Vertex indices for the face
} Face;

// Struct to represent the entire 3D model
typedef struct {
    Vertex *vertices;  // Array of vertices
    Face *faces;       // Array of faces (triangles)
    int vertex_count;  // Number of vertices
    int face_count;    // Number of faces
} Model;

// Function to load a model from an OBJ file
Model *load_model_from_obj(const char *filepath);

// Function to free the memory allocated for the model
void free_model(Model *model);

#endif 

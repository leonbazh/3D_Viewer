#include "model_loader.h"

/**
 * @brief Загружает модель из OBJ файла.
 *
 * Эта функция загружает каркасную модель из файла формата OBJ. Она парсит вершины и грани из файла и 
 * возвращает указатель на структуру Model. Если файл не может быть открыт или данные в файле некорректны, 
 * функция возвращает NULL и выводит сообщение об ошибке.
 *
 * @param filepath Путь к OBJ файлу.
 * @return Указатель на структуру Model, содержащую вершины и грани модели, или NULL в случае ошибки.
 */
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

        else if (strncmp(line, "f ", 2) == 0) {
            model->face_count++;
            model->faces = (Face *)realloc(model->faces, model->face_count * sizeof(Face));
            Face *face = &model->faces[model->face_count - 1];

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

/**
 * @brief Освобождает память, занятую моделью.
 *
 * Эта функция освобождает память, выделенную под массивы вершин и граней модели, а затем освобождает саму структуру Model.
 *
 * @param model Указатель на структуру Model, которую нужно освободить.
 */
void free_model(Model *model) {
  free(model->vertices);
  free(model->faces);
  free(model);
}

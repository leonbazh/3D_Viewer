#include "transformations.h"

/**
 * @brief Перемещает модель на заданные расстояния по осям X, Y и Z.
 *
 * Эта функция сдвигает все вершины модели на указанные расстояния по осям X, Y
 * и Z.
 *
 * @param model Указатель на структуру модели, содержащую массив вершин.
 * @param tx Смещение по оси X.
 * @param ty Смещение по оси Y.
 * @param tz Смещение по оси Z.
 */
void translate_model(Model *model, float tx, float ty, float tz) {
  for (int i = 0; i < model->vertex_count; i++) {
    model->vertices[i].x += tx;
    model->vertices[i].y += ty;
    model->vertices[i].z += tz;
  }
}

/**
 * @brief Вращает модель на указанные углы по осям X, Y и Z.
 *
 * Эта функция вращает все вершины модели на заданные углы вокруг осей X, Y и Z.
 * Углы передаются в градусах, но конвертируются в радианы для
 * тригонометрических вычислений.
 *
 * @param model Указатель на структуру модели, содержащую массив вершин.
 * @param angle_x Угол вращения вокруг оси X в градусах.
 * @param angle_y Угол вращения вокруг оси Y в градусах.
 * @param angle_z Угол вращения вокруг оси Z в градусах.
 */
void rotate_model(Model *model, float angle_x, float angle_y, float angle_z) {
  // Переводим углы из градусов в радианы
  float rad_x = DEG_TO_RAD(angle_x);
  float rad_y = DEG_TO_RAD(angle_y);
  float rad_z = DEG_TO_RAD(angle_z);

  // Применяем вращения по осям X, Y и Z
  for (int i = 0; i < model->vertex_count; i++) {
    Vertex *v = &model->vertices[i];

    // Вращение вокруг оси X
    float y_new = v->y * cos(rad_x) - v->z * sin(rad_x);
    float z_new = v->y * sin(rad_x) + v->z * cos(rad_x);
    v->y = y_new;
    v->z = z_new;

    // Вращение вокруг оси Y
    float x_new = v->x * cos(rad_y) + v->z * sin(rad_y);
    v->z = -v->x * sin(rad_y) + v->z * cos(rad_y);
    v->x = x_new;

    // Вращение вокруг оси Z
    x_new = v->x * cos(rad_z) - v->y * sin(rad_z);
    v->y = v->x * sin(rad_z) + v->y * cos(rad_z);
    v->x = x_new;
  }
}

/**
 * @brief Масштабирует модель на указанный коэффициент.
 *
 * Эта функция увеличивает или уменьшает размер модели путем умножения координат
 * каждой вершины на указанный коэффициент масштабирования.
 *
 * @param model Указатель на структуру модели, содержащую массив вершин.
 * @param scale_factor Коэффициент масштабирования. Значение >1 увеличивает
 * модель, значение <1 уменьшает её.
 */
void scale_model(Model *model, float scale_factor) {
  for (int i = 0; i < model->vertex_count; i++) {
    model->vertices[i].x *= scale_factor;
    model->vertices[i].y *= scale_factor;
    model->vertices[i].z *= scale_factor;
  }
}

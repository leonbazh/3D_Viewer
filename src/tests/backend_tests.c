#include <check.h>
#include <stdlib.h>
#include "../backend/model_loader.h"
#include "../backend/transformations.h"

START_TEST(test_load_valid_model) {
    Model *model = load_model_from_obj("./obj/valid_model.obj");
    ck_assert_ptr_nonnull(model);  
    ck_assert_int_gt(model->vertex_count, 0);  
    ck_assert_int_gt(model->face_count, 0);  

    free_model(model);  
}
END_TEST

START_TEST(test_load_invalid_model) {
    Model *model = load_model_from_obj("./obj/invalid_model.obj");
    ck_assert_ptr_null(model);  
}
END_TEST

START_TEST(test_load_nonexistent_model) {
    Model *model = load_model_from_obj("./obj/nonexistent_model.obj");
    ck_assert_ptr_null(model);  // Модель должна быть NULL, так как файл не существует
}
END_TEST

START_TEST(test_translation_zero) {
    Model model;
    model.vertex_count = 1;
    model.vertices = (Vertex *)malloc(sizeof(Vertex));
    model.vertices[0].x = 1.0;
    model.vertices[0].y = 1.0;
    model.vertices[0].z = 1.0;

    // Перемещение на 0,0,0 не должно изменить координаты
    translate_model(&model, 0.0, 0.0, 0.0);

    ck_assert_float_eq(model.vertices[0].x, 1.0);
    ck_assert_float_eq(model.vertices[0].y, 1.0);
    ck_assert_float_eq(model.vertices[0].z, 1.0);

    free(model.vertices);
}
END_TEST

START_TEST(test_translation_positive_large) {
    Model model;
    model.vertex_count = 1;
    model.vertices = (Vertex *)malloc(sizeof(Vertex));
    model.vertices[0].x = 1.0;
    model.vertices[0].y = 1.0;
    model.vertices[0].z = 1.0;

    // Перемещаем модель на большие положительные значения
    translate_model(&model, 1e6, 1e6, 1e6);

    ck_assert_float_eq(model.vertices[0].x, 1.0 + 1e6);
    ck_assert_float_eq(model.vertices[0].y, 1.0 + 1e6);
    ck_assert_float_eq(model.vertices[0].z, 1.0 + 1e6);

    free(model.vertices);
}
END_TEST

START_TEST(test_translation_negative_large) {
    Model model;
    model.vertex_count = 1;
    model.vertices = (Vertex *)malloc(sizeof(Vertex));
    model.vertices[0].x = -1.0;
    model.vertices[0].y = -1.0;
    model.vertices[0].z = -1.0;

    // Перемещаем модель на большие отрицательные значения
    translate_model(&model, -1e6, -1e6, -1e6);

    ck_assert_float_eq(model.vertices[0].x, -1.0 - 1e6);
    ck_assert_float_eq(model.vertices[0].y, -1.0 - 1e6);
    ck_assert_float_eq(model.vertices[0].z, -1.0 - 1e6);

    free(model.vertices);
}
END_TEST

START_TEST(test_translation_mixed) {
    Model model;
    model.vertex_count = 1;
    model.vertices = (Vertex *)malloc(sizeof(Vertex));
    model.vertices[0].x = 0.0;
    model.vertices[0].y = 0.0;
    model.vertices[0].z = 0.0;

    // Перемещаем модель на разные значения для каждой оси
    translate_model(&model, 1e3, -5e2, 2e3);

    ck_assert_float_eq(model.vertices[0].x, 1e3);
    ck_assert_float_eq(model.vertices[0].y, -5e2);
    ck_assert_float_eq(model.vertices[0].z, 2e3);

    free(model.vertices);
}
END_TEST

START_TEST(test_translation_small_values) {
    Model model;
    model.vertex_count = 1;
    model.vertices = (Vertex *)malloc(sizeof(Vertex));
    model.vertices[0].x = 1.0;
    model.vertices[0].y = 1.0;
    model.vertices[0].z = 1.0;

    // Перемещаем модель на очень маленькие значения
    translate_model(&model, 1e-6, 1e-6, 1e-6);

    ck_assert_float_eq_tol(model.vertices[0].x, 1.0 + 1e-6, 1e-9);
    ck_assert_float_eq_tol(model.vertices[0].y, 1.0 + 1e-6, 1e-9);
    ck_assert_float_eq_tol(model.vertices[0].z, 1.0 + 1e-6, 1e-9);

    free(model.vertices);
}
END_TEST

START_TEST(test_model_scaling) {
    Model model;
    model.vertex_count = 1;
    model.vertices = (Vertex *)malloc(sizeof(Vertex));
    model.vertices[0].x = 1.0;
    model.vertices[0].y = 1.0;
    model.vertices[0].z = 1.0;

    scale_model(&model, 2.0);

    ck_assert_float_eq(model.vertices[0].x, 2.0);
    ck_assert_float_eq(model.vertices[0].y, 2.0);
    ck_assert_float_eq(model.vertices[0].z, 2.0);

    free(model.vertices);
}
END_TEST

START_TEST(test_rotation_zero_degrees) {
    Model model;
    model.vertex_count = 1;
    model.vertices = (Vertex *)malloc(sizeof(Vertex));
    model.vertices[0].x = 1.0;
    model.vertices[0].y = 2.0;
    model.vertices[0].z = 3.0;

    // Вращаем на 0 градусов по всем осям
    rotate_model(&model, 0.0, 0.0, 0.0);

    // Ожидаем, что координаты останутся такими же
    ck_assert_float_eq_tol(model.vertices[0].x, 1.0, 0.001);
    ck_assert_float_eq_tol(model.vertices[0].y, 2.0, 0.001);
    ck_assert_float_eq_tol(model.vertices[0].z, 3.0, 0.001);

    free(model.vertices);
}
END_TEST

// Тест вращения на 360 градусов (ожидается возврат к исходному состоянию)
START_TEST(test_rotation_360_degrees) {
    Model model;
    model.vertex_count = 1;
    model.vertices = (Vertex *)malloc(sizeof(Vertex));
    model.vertices[0].x = 1.0;
    model.vertices[0].y = 0.0;
    model.vertices[0].z = 0.0;

    // Вращаем на 360 градусов по оси Z
    rotate_model(&model, 0.0, 0.0, 360.0);

    // Ожидаем, что координаты останутся такими же, как исходные
    ck_assert_float_eq_tol(model.vertices[0].x, 1.0, 0.001);
    ck_assert_float_eq_tol(model.vertices[0].y, 0.0, 0.001);
    ck_assert_float_eq_tol(model.vertices[0].z, 0.0, 0.001);

    free(model.vertices);
}
END_TEST

// Тест вращения на 90 градусов вокруг оси X (вращение вверх/вниз)
START_TEST(test_rotation_90_degrees_x) {
    Model model;
    model.vertex_count = 1;
    model.vertices = (Vertex *)malloc(sizeof(Vertex));
    model.vertices[0].x = 0.0;
    model.vertices[0].y = 1.0;
    model.vertices[0].z = 0.0;

    // Вращаем на 90 градусов вокруг оси X
    rotate_model(&model, 90.0, 0.0, 0.0);

    // Ожидаем, что Y переместится в Z, а Y станет 0
    ck_assert_float_eq_tol(model.vertices[0].x, 0.0, 0.001);
    ck_assert_float_eq_tol(model.vertices[0].y, 0.0, 0.001);
    ck_assert_float_eq_tol(model.vertices[0].z, 1.0, 0.001);

    free(model.vertices);
}
END_TEST

START_TEST(test_rotation_180_degrees_y_axis) {
    // Модель с одной вершиной
    Model model;
    model.vertex_count = 1;
    model.vertices = (Vertex *)malloc(sizeof(Vertex));
    model.vertices[0].x = 1.0;
    model.vertices[0].y = 0.0;
    model.vertices[0].z = 0.0;

    // Вращаем модель на 180 градусов по оси Y
    rotate_model(&model, 0.0, 180.0, 0.0);

    // Ожидаем, что вершина переместится в точку (-1.0, 0.0, 0.0)
    ck_assert_float_eq_tol(model.vertices[0].x, -1.0, 0.001);
    ck_assert_float_eq_tol(model.vertices[0].y, 0.0, 0.001);
    ck_assert_float_eq_tol(model.vertices[0].z, 0.0, 0.001);

    free(model.vertices);
}
END_TEST

START_TEST(test_rotation_270_degrees_z_axis) {
    // Модель с одной вершиной
    Model model;
    model.vertex_count = 1;
    model.vertices = (Vertex *)malloc(sizeof(Vertex));
    model.vertices[0].x = 1.0;
    model.vertices[0].y = 0.0;
    model.vertices[0].z = 0.0;

    // Вращаем модель на 270 градусов по оси Z
    rotate_model(&model, 0.0, 0.0, 270.0);

    // Ожидаем, что координаты изменятся (X переместится в -Y)
    ck_assert_float_eq_tol(model.vertices[0].x, 0.0, 0.001);
    ck_assert_float_eq_tol(model.vertices[0].y, -1.0, 0.001);
    ck_assert_float_eq_tol(model.vertices[0].z, 0.0, 0.001);

    free(model.vertices);
}
END_TEST

Suite *backend_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Backend");

    tc_core = tcase_create("Core_ModelLoader");
    tcase_add_test(tc_core, test_load_valid_model);
    tcase_add_test(tc_core, test_load_invalid_model);
    tcase_add_test(tc_core, test_load_nonexistent_model);
    suite_add_tcase(s, tc_core);

    TCase *tc_affine = tcase_create("Core_AffineTransformations");
    tcase_add_test(tc_affine, test_translation_zero);
    tcase_add_test(tc_affine, test_translation_mixed);
    tcase_add_test(tc_affine, test_translation_small_values);
    tcase_add_test(tc_affine, test_translation_negative_large);
    tcase_add_test(tc_affine, test_translation_positive_large);
    tcase_add_test(tc_affine, test_model_scaling);
    tcase_add_test(tc_affine, test_rotation_360_degrees);
    tcase_add_test(tc_affine, test_rotation_zero_degrees);
    tcase_add_test(tc_affine, test_rotation_90_degrees_x);
    tcase_add_test(tc_affine, test_rotation_180_degrees_y_axis);
    tcase_add_test(tc_affine, test_rotation_270_degrees_z_axis);
    suite_add_tcase(s, tc_affine);

    return s;
}

int main(void) {
    int number_failed;
    SRunner *sr;

    sr = srunner_create(backend_suite());
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include "../model_loader.h"
#include "../transformations.h"

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

START_TEST(test_model_translation) {
    Model model;
    model.vertex_count = 1;
    model.vertices = (Vertex *)malloc(sizeof(Vertex));
    model.vertices[0].x = 0.0;
    model.vertices[0].y = 0.0;
    model.vertices[0].z = 0.0;

    translate_model(&model, 1.0, 1.0, 1.0);

    ck_assert_float_eq(model.vertices[0].x, 1.0);
    ck_assert_float_eq(model.vertices[0].y, 1.0);
    ck_assert_float_eq(model.vertices[0].z, 1.0);

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

START_TEST(test_model_rotation) {
    Model model;
    model.vertex_count = 1;
    model.vertices = (Vertex *)malloc(sizeof(Vertex));
    model.vertices[0].x = 1.0;
    model.vertices[0].y = 0.0;
    model.vertices[0].z = 0.0;

    rotate_model(&model, 90.0, 'Z');

    ck_assert_float_eq_tol(model.vertices[0].x, 0.0, 0.001);
    ck_assert_float_eq_tol(model.vertices[0].y, 1.0, 0.001);
    ck_assert_float_eq_tol(model.vertices[0].z, 0.0, 0.001);

    free(model.vertices);
}
END_TEST

START_TEST(test_free_model) {
    Model model;
    model.vertex_count = 10;
    model.vertices = (Vertex *)malloc(10 * sizeof(Vertex));

    free_model(&model);

    ck_assert_ptr_null(model.vertices);
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
    tcase_add_test(tc_affine, test_model_translation);
    tcase_add_test(tc_affine, test_model_scaling);
    tcase_add_test(tc_affine, test_model_rotation);
    suite_add_tcase(s, tc_affine);

    TCase *tc_memory = tcase_create("Core_FreeModel");
    tcase_add_test(tc_memory, test_free_model);
    suite_add_tcase(s, tc_memory);

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


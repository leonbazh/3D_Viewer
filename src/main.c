/* nuklear - public domain */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string.h>
#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING #define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT

#include "gui/nuklear_glfw_gl3/nuklear.h"
#include "gui/nuklear_glfw_gl3/nuklear_glfw_gl3.h"

#include "core.h"
#include "gui/NkSetDraculaTheme.c"
#include "gui/nk_transform_gui_module.c"
#include "model_loader.h"

void render_model(Model *model, GUI_view_settings *settings,
                  struct GUI_transform_panel *unit) {
  if (model == NULL) return;

  glClearColor(settings->bg_color[0], settings->bg_color[1],
               settings->bg_color[2], 1.0f);           // Set background color
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear buffers
  glLoadIdentity();                                    // Load identity matrix

  // Set projection type
  if (settings->projection_type == 0) {
    // Parallel projection
    glOrtho(-1.5, 1.5, -1.5, 1.5, -10.0, 10.0);
  } else {
    // Central (Perspective) projection
    gluPerspective(45.0, (double)WIN_WIDTH / (double)WIN_HEIGHT, 0.1, 100.0);
    glTranslatef(0.0f, 0.0f, -5.0f);  // Move the camera back a bit
  }

  // Set edge color and thickness
  glLineWidth(settings->edge_thickness);
  glColor3f(settings->edge_color[0], settings->edge_color[1],
            settings->edge_color[2]);

  // Set edge type (solid/dashed)
  if (settings->edge_type == 1) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x00FF);  // Dashed line
  } else {
    glDisable(GL_LINE_STIPPLE);  // Solid line
  }

  glTranslatef(unit->translation[0], unit->translation[1],
               unit->translation[2]);
  glRotatef(unit->rotation[0], 1.0, 0.0, 0.0);
  glRotatef(unit->rotation[1], 0.0, 1.0, 0.0);
  glRotatef(unit->rotation[2], 0.0, 0.0, 1.0);
  glScalef(unit->scale_factor, unit->scale_factor, unit->scale_factor);

  // Draw the model as a wireframe
  glBegin(GL_LINES);
  for (int i = 0; i < model->face_count; i++) {
    Face face = model->faces[i];

    Vertex v1 = model->vertices[face.v1 - 1];
    Vertex v2 = model->vertices[face.v2 - 1];
    Vertex v3 = model->vertices[face.v3 - 1];

    // Draw edges of the triangle (v1 -> v2, v2 -> v3, v3 -> v1)
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v2.x, v2.y, v2.z);

    glVertex3f(v2.x, v2.y, v2.z);
    glVertex3f(v3.x, v3.y, v3.z);

    glVertex3f(v3.x, v3.y, v3.z);
    glVertex3f(v1.x, v1.y, v1.z);
  }
  glEnd();

  // Draw vertices (circles or squares)
  if (settings->vertex_display != 0) {
    for (int i = 0; i < model->vertex_count; i++) {
      Vertex v = model->vertices[i];

      glPointSize(settings->vertex_size);
      glColor3f(settings->vertex_color[0], settings->vertex_color[1],
                settings->vertex_color[2]);

      glBegin(GL_POINTS);
      if (settings->vertex_display == 1) {
        // Render as circle (using point)
        glVertex3f(v.x, v.y, v.z);
      } else if (settings->vertex_display == 2) {
        // Render as square (by drawing a small square)
        glVertex3f(v.x, v.y, v.z);
      }
      glEnd();
    }
  }
}

static void error_callback(int e, const char *d) {
  printf("Error %d: %s\n", e, d);
}

int main(void) {
  int win_width = WIN_WIDTH;
  int win_height = WIN_HEIGHT;

  /* GLFW */
  struct nk_glfw glfw = {0};
  static GLFWwindow *win;
  glfwSetErrorCallback(error_callback);

  if (!glfwInit()) {
    fprintf(stdout, "[GFLW] failed to init!\n");
    exit(1);
  }

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  win = glfwCreateWindow(win_width, win_height, "3D_Viewer 1.0", NULL, NULL);
  glfwMakeContextCurrent(win);

  /* Glew */
  glewExperimental = 1;
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to setup GLEW\n");
    exit(1);
  }

  /* create context */
  glfwSetWindowSizeLimits(win, win_width, win_height, win_width, win_height);
  struct nk_context *ctx;
  ctx = nk_glfw3_init(&glfw, win, NK_GLFW3_INSTALL_CALLBACKS);

  {
    struct nk_font_atlas *atlas;
    struct nk_font_config cfg = nk_font_config(14);
    nk_glfw3_font_stash_begin(&glfw, &atlas);
    cfg.range = nk_font_cyrillic_glyph_ranges();
    struct nk_font *hero =
        nk_font_atlas_add_from_file(atlas, "Roboto-Regular.ttf", 16, &cfg);
    nk_style_set_font(ctx, &hero->handle);
    nk_glfw3_font_stash_end(&glfw);
  }

  set_style(ctx);

  Model *model = NULL;
  struct GUI_directory_data data;
  init_dir_data(&data, "obj");
  struct GUI_status_bar status_bar = {0};

  if (model != NULL) {
    status_bar.model = model;
  }

  struct GUI_transform_panel unit = {0};
  struct GUI_tabs_bar tab_unit;
  unit.scale_factor = 0.1f;

  unit.model_x_pos_property = 0.05f;
  unit.model_y_pos_property = 0.05f;
  unit.model_z_pos_property = 0.05f;

  unit.model_rotation_x_property = 0.05f;
  unit.model_rotation_y_property = 0.05f;
  unit.model_rotation_z_property = 0.05f;

  tab_unit.current_tab = 0;
  tab_unit.tabs_amount = 3;
  strcpy(tab_unit.tab_names[0], "Transform");
  strcpy(tab_unit.tab_names[1], "Style");
  strcpy(tab_unit.tab_names[2], "Load");

  GUI_view_settings settings = {0,
                                0,
                                {1.0f, 1.0f, 1.0f},
                                1.0f,
                                0,
                                {1.0f, 0.0f, 0.0f},
                                5.0f,
                                {0.0f, 0.0f, 0.0f}};
  ;
  load_settings(&settings);

  while (!glfwWindowShouldClose(win)) {
    /* Input */
    glfwPollEvents();
    nk_glfw3_new_frame(&glfw);
    glViewport(0, 0, GUI_WIN_WIDTH_X, win_height);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (model == NULL) {
      printf("Model is NULL. Please select a file to load.\n");

      GUI_browse_obj_files(ctx, &model, &data);
      nk_end(ctx);

      if (model != NULL) {
        printf("Model loaded successfully: %s\n", data.selected_obj_file);
        status_bar.model = model;
      } else {
        printf("Error: Model loading failed.\n");
      }
    } else {
      GUI_draw_tabs(ctx, &tab_unit);
      nk_end(ctx);

      GUI_draw_upper_tool_bar(ctx);
      nk_end(ctx);

      strcpy(status_bar.file_name, data.selected_obj_file);
      GUI_draw_status_bar(ctx, &status_bar);
      nk_end(ctx);

      switch (tab_unit.current_tab) {
        case 0:
          GUI_draw_transform_panel(ctx, &unit);
          nk_end(ctx);
          break;
        case 1:
          GUI_draw_view_panel(ctx, &settings);
          nk_end(ctx);
          break;
        case 2:
          GUI_browse_obj_files(ctx, &model, &data);
          status_bar.model = model;
          nk_end(ctx);
          break;

        default:
          break;
      }

      glViewport(GUI_WIN_WIDTH_X, 0, win_width - GUI_WIN_WIDTH_X, win_height);
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      if (model != NULL) {
        render_model(model, &settings, &unit);
      } else {
        printf("Error: Model is NULL, cannot render.\n");
      }
    }

    nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER,
                    MAX_ELEMENT_BUFFER);
    glfwSwapBuffers(win);
  }

  nk_glfw3_shutdown(&glfw);
  glfwTerminate();
  free_model(model);
  return 0;
}

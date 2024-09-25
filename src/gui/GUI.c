static void GUI_draw_transform_panel(struct nk_context *ctx,
                                     struct GUI_transform_panel *unit) {
  if (nk_begin(
          ctx, "Main",
          nk_rect(0, GUI_TRANSFORM_PANEL_Y_START, GUI_WIN_WIDTH_X,
                  GUI_TRANSFORM_PANEL_Y_END),
          NK_WINDOW_BORDER | NK_WINDOW_BACKGROUND)) {
    float ratio_two[] = {0.5f, 0.5f};

    nk_layout_row_dynamic(ctx, GUI_TRANSFORM_PANEL_LABEL_HEIGHT, 1);
    nk_label(ctx, "Position", NK_TEXT_CENTERED);

    nk_layout_row(ctx, NK_DYNAMIC, GUI_TRANSFORM_PANEL_POSITION_BOX_ITEM_HEIGHT,
                  2, ratio_two);
    nk_property_float(ctx, "X:", -5.0f, &unit->model_x_pos_property, 5.0f, 0.1f,
                      0.005f);
    nk_slider_float(ctx, -10.0f, &unit->translation[0], 10.0f,
                    unit->model_x_pos_property);

    nk_layout_row(ctx, NK_DYNAMIC, GUI_TRANSFORM_PANEL_POSITION_BOX_ITEM_HEIGHT,
                  2, ratio_two);
    nk_property_float(ctx, "Y:", -5.0f, &unit->model_y_pos_property, 5.0f, 0.1f,
                      0.005f);
    nk_slider_float(ctx, -10.0f, &unit->translation[1], 10.0f,
                    unit->model_y_pos_property);

    nk_layout_row(ctx, NK_DYNAMIC, GUI_TRANSFORM_PANEL_POSITION_BOX_ITEM_HEIGHT,
                  2, ratio_two);
    nk_property_float(ctx, "Z:", -5.0f, &unit->model_z_pos_property, 5.0f, 0.1f,
                      0.005f);
    nk_slider_float(ctx, -10.0f, &unit->translation[2], 10.0f,
                    unit->model_z_pos_property);

    nk_layout_row_dynamic(ctx, GUI_TRANSFORM_PANEL_POSITION_BOX_ITEM_HEIGHT, 1);
    if (nk_button_label(ctx, "Reset Position")) {
      unit->translation[0] = 0;
      unit->translation[1] = 0;
      unit->translation[2] = 0;
    }

    nk_layout_row_dynamic(ctx, GUI_TRANSFORM_PANEL_LABEL_HEIGHT, 1);
    nk_label(ctx, "Rotation", NK_TEXT_CENTERED);

    nk_layout_row(ctx, NK_DYNAMIC, GUI_TRANSFORM_PANEL_POSITION_BOX_ITEM_HEIGHT,
                  2, ratio_two);
    nk_property_float(ctx, "X:", -5.0f, &unit->model_rotation_x_property, 5.0f,
                      1.0f, 0.005f);
    nk_slider_float(ctx, -180.0f, &unit->rotation[0], 180.0f,
                    unit->model_rotation_x_property);

    nk_layout_row(ctx, NK_DYNAMIC, GUI_TRANSFORM_PANEL_POSITION_BOX_ITEM_HEIGHT,
                  2, ratio_two);
    nk_property_float(ctx, "Y:", -5.0f, &unit->model_rotation_y_property, 5.0f,
                      1.0f, 0.005f);
    nk_slider_float(ctx, -180.0f, &unit->rotation[1], 180.0f,
                    unit->model_rotation_y_property);

    nk_layout_row(ctx, NK_DYNAMIC, GUI_TRANSFORM_PANEL_POSITION_BOX_ITEM_HEIGHT,
                  2, ratio_two);
    nk_property_float(ctx, "Z:", -5.0f, &unit->model_rotation_z_property, 5.0f,
                      1.0f, 0.005f);
    nk_slider_float(ctx, -180.0f, &unit->rotation[2], 180.0f,
                    unit->model_rotation_z_property);

    nk_layout_row_dynamic(ctx, GUI_TRANSFORM_PANEL_LABEL_HEIGHT, 1);
    nk_label(ctx, "Scale", NK_TEXT_CENTERED);

    nk_layout_row_dynamic(ctx, GUI_TRANSFORM_PANEL_POSITION_BOX_ITEM_HEIGHT, 1);
    nk_slider_float(ctx, -1.0f, &unit->scale_factor, 1.0f, 0.005f);

    nk_layout_row_dynamic(ctx, GUI_TRANSFORM_PANEL_LABEL_HEIGHT, 1);
    nk_label(ctx, "Capture", NK_TEXT_CENTERED);

    nk_bool is_jpg_selected = (unit->picture_format == 1);  // Если формат JPG (0)
    nk_bool is_bmp_selected = (unit->picture_format == 0);

    nk_layout_row(ctx, NK_DYNAMIC, GUI_TRANSFORM_PANEL_POSITION_BOX_ITEM_HEIGHT,
                  2, ratio_two);
    if (nk_radio_label(ctx, "BMP", &is_bmp_selected)) {
      unit->picture_format = 0;
    } 

    if (nk_radio_label(ctx, "JPG", &is_jpg_selected)) {
      unit->picture_format = 1;
    } 

    nk_layout_row_dynamic(ctx, GUI_TRANSFORM_PANEL_POSITION_BOX_ITEM_HEIGHT, 1);
    if (nk_button_label(ctx, "Take a picture")) {
      unit->is_picture_needed = true;
    }

    nk_layout_row_dynamic(ctx, GUI_TRANSFORM_PANEL_POSITION_BOX_ITEM_HEIGHT, 1);
    if (nk_button_label(ctx, "Take a GIF")) {
      unit->is_gif_needed = true;
    }
  }
}

static void GUI_draw_view_panel(struct nk_context *ctx,
                                struct GUI_view_settings *settings) {
  if (nk_begin(
          ctx, "Main",
          nk_rect(0, GUI_TRANSFORM_PANEL_Y_START, GUI_WIN_WIDTH_X,
                  GUI_TRANSFORM_PANEL_Y_END),
          NK_WINDOW_BORDER | NK_WINDOW_BACKGROUND | NK_WINDOW_NO_SCROLLBAR)) {
    nk_layout_row_dynamic(ctx, GUI_VIEW_PANEL_LABEL_HEIGHT, 1);
    nk_label(ctx, "Projection type", NK_TEXT_CENTERED);
    nk_layout_row_dynamic(ctx, GUI_VIEW_PANEL_BOX_ITEM_HEIGHT, 2);
    if (nk_option_label(ctx, "Parallel", (int)settings->projection_type == 0))
      settings->projection_type = 0;
    if (nk_option_label(ctx, "Central", (int)settings->projection_type == 1))
      settings->projection_type = 1;

    nk_layout_row_dynamic(ctx, GUI_VIEW_PANEL_LABEL_HEIGHT, 1);
    nk_label(ctx, "Edge type", NK_TEXT_CENTERED);
    nk_layout_row_dynamic(ctx, GUI_VIEW_PANEL_BOX_ITEM_HEIGHT, 2);
    if (nk_option_label(ctx, "Parallel", settings->edge_type == 0))
      settings->edge_type = 0;
    if (nk_option_label(ctx, "Central", settings->edge_type == 1))
      settings->edge_type = 1;

    nk_layout_row_dynamic(ctx, GUI_VIEW_PANEL_BOX_ITEM_HEIGHT, 1);
    nk_property_float(ctx, "Edge Thickness", 1.0f, &settings->edge_thickness,
                      10.0f, 0.1f, 0.1f);

    nk_layout_row_dynamic(ctx, GUI_VIEW_PANEL_LABEL_HEIGHT, 1);
    nk_label(ctx, "Edge Color", NK_TEXT_LEFT);
    nk_layout_row_dynamic(ctx, GUI_VIEW_PANEL_BOX_ITEM_HEIGHT, 1);
    // struct nk_colorf edge_col = nk_rgb_cf(nk_color_fv(settings->edge_color));
    // // Convert to nk_colorf
    struct nk_colorf edge_col = {
        settings->edge_color[0], settings->edge_color[1],
        settings->edge_color[2], 1.0f};  // Initialize nk_colorf
    edge_col = nk_color_picker(ctx, edge_col, NK_RGB);
    settings->edge_color[0] = edge_col.r;
    settings->edge_color[1] = edge_col.g;
    settings->edge_color[2] = edge_col.b;

    nk_layout_row_dynamic(ctx, GUI_VIEW_PANEL_LABEL_HEIGHT, 1);
    nk_label(ctx, "Vertex Display", NK_TEXT_LEFT);
    nk_layout_row_dynamic(ctx, GUI_VIEW_PANEL_BOX_ITEM_HEIGHT, 3);
    if (nk_option_label(ctx, "None", settings->vertex_display == 0))
      settings->vertex_display = 0;
    if (nk_option_label(ctx, "Circle", settings->vertex_display == 1))
      settings->vertex_display = 1;
    if (nk_option_label(ctx, "Square", settings->vertex_display == 2))
      settings->vertex_display = 2;

    nk_layout_row_dynamic(ctx, GUI_VIEW_PANEL_BOX_ITEM_HEIGHT, 1);
    nk_property_float(ctx, "Vertex Size", 1.0f, &settings->vertex_size, 10.0f,
                      0.1f, 0.1f);

    nk_layout_row_dynamic(ctx, GUI_VIEW_PANEL_LABEL_HEIGHT, 1);
    nk_label(ctx, "Vertex Color", NK_TEXT_LEFT);
    nk_layout_row_dynamic(ctx, GUI_VIEW_PANEL_BOX_ITEM_HEIGHT, 1);
    struct nk_colorf vertex_col = {settings->vertex_color[0],
                                   settings->vertex_color[1],
                                   settings->vertex_color[2], 1.0f};
    vertex_col = nk_color_picker(ctx, vertex_col, NK_RGB);
    settings->vertex_color[0] = vertex_col.r;
    settings->vertex_color[1] = vertex_col.g;
    settings->vertex_color[2] = vertex_col.b;

    nk_layout_row_dynamic(ctx, GUI_VIEW_PANEL_LABEL_HEIGHT, 1);
    nk_label(ctx, "Background Color", NK_TEXT_LEFT);
    nk_layout_row_dynamic(ctx, GUI_VIEW_PANEL_BOX_ITEM_HEIGHT, 1);
    struct nk_colorf bg_col = {settings->bg_color[0], settings->bg_color[1],
                               settings->bg_color[2], 1.0f};
    bg_col = nk_color_picker(ctx, bg_col, NK_RGB);
    settings->bg_color[0] = bg_col.r;
    settings->bg_color[1] = bg_col.g;
    settings->bg_color[2] = bg_col.b;

    nk_layout_row_dynamic(ctx, GUI_VIEW_PANEL_BOX_ITEM_HEIGHT, 1);
    if (nk_button_label(ctx, "Save Settings")) {
      save_settings(settings);
    }
  }
}

static void GUI_draw_status_bar(struct nk_context *ctx,
                                struct GUI_status_bar *unit) {
  if (nk_begin(
          ctx, "StatusBar",
          nk_rect(0, WIN_HEIGHT - GUI_STATUS_BAR_HEIGHT, WIN_WIDTH,
                  GUI_STATUS_BAR_HEIGHT),
          NK_WINDOW_BORDER | NK_WINDOW_BACKGROUND | NK_WINDOW_NO_SCROLLBAR)) {
    nk_layout_row_dynamic(ctx, GUI_STATUS_BAR_ITEMS_HEIGHT, 3);
    nk_labelf(ctx, NK_TEXT_LEFT, "[FILE]: %s", unit->file_name);
    nk_labelf(ctx, NK_TEXT_LEFT, "Vertices %d", unit->model->vertex_count);
    nk_labelf(ctx, NK_TEXT_LEFT, "Polygons %d", unit->model->face_count);
  }
}

static void GUI_draw_upper_tool_bar(struct nk_context *ctx) {
  if (nk_begin(
          ctx, "ToolBar", nk_rect(0, 0, WIN_WIDTH, GUI_STATUS_BAR_HEIGHT),
          NK_WINDOW_BORDER | NK_WINDOW_BACKGROUND | NK_WINDOW_NO_SCROLLBAR)) {
  }
}

static void GUI_draw_tabs(struct nk_context *ctx, struct GUI_tabs_bar *unit) {
  if (nk_begin(ctx, "Tabs",
               nk_rect(0, GUI_TABS_BUTTONS_START - 3, GUI_TABS_BUTTONS_WIDTH,
                       GUI_TABS_BUTTONS_HEIGHT),
               NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
    nk_layout_row_dynamic(ctx, GUI_TABS_BUTTONS_ITEM_HEIGHT, unit->tabs_amount);
    for (int i = 0; i < unit->tabs_amount; i++) {
      if (nk_button_label(ctx, unit->tab_names[i])) {
        unit->current_tab = i;
      }
    }
  }
}
static void GUI_browse_obj_files(struct nk_context *ctx, Model **model,
                                 struct GUI_directory_data *data) {
  if (nk_begin(ctx, "OBJ File Browser",
               nk_rect(0, GUI_FILE_BROWSE_Y_START, GUI_WIN_WIDTH_X,
                       GUI_FILE_BROWSE_HEIGHT),  // Увеличим высоту окна
               NK_WINDOW_BORDER | NK_WINDOW_TITLE)) {
    nk_layout_row_dynamic(ctx, 30, 1);
    nk_label(ctx, "Select OBJ File from Directory", NK_TEXT_LEFT);

    nk_layout_row_dynamic(ctx, 30, 1);
    if (nk_button_label(ctx, "File")) {
      data->file_menu_active =
          !data->file_menu_active;  // Переключаем активность меню
      if (data->file_menu_active) {
        scan_obj_files(data);  // Сканируем директорию на наличие .obj файлов

        for (int i = 0; i < data->obj_file_count; i++) {
          printf("File %d: %s\n", i + 1, data->obj_files[i]);
        }
      }
    }

    // Меню с файлами .obj
    if (data->file_menu_active && data->obj_file_count > 0) {
      // Динамическое отображение для каждого файла как кнопки
      nk_layout_row_dynamic(ctx, 30, 1);  // Высота каждой строки под файл

      for (int i = 0; i < data->obj_file_count; i++) {
        // Отрисовка кнопки для каждого файла
        if (nk_button_label(ctx, data->obj_files[i])) {
          // Проверяем, не превышает ли полная длина пути максимальный размер
          int total_length = strlen(data->obj_directory) +
                             strlen(data->obj_files[i]) + 6;  // +1 для '/'
          if (total_length < MAX_PATH_LENGTH) {
            // Сохраняем выбранный файл
            snprintf(data->selected_obj_file, total_length, "%s/%s",
                     data->obj_directory, data->obj_files[i]);
            data->file_menu_active = 0;  // Закрываем меню после выбора
            printf("Selected file: %s\n", data->selected_obj_file);
          } else {
            printf("Error: Path is too long for file %s in directory %s\n",
                   data->obj_files[i], data->obj_directory);
          }
        }
      }
    }

    // Отображаем выбранный файл в текстовом поле
    int selected_file_length =
        strlen(data->selected_obj_file);  // Получаем текущую длину строки
    nk_layout_row_dynamic(ctx, 30, 1);
    nk_edit_string(ctx, NK_EDIT_READ_ONLY, data->selected_obj_file,
                   &selected_file_length, MAX_PATH_LENGTH, nk_filter_default);

    // Кнопка для загрузки выбранного файла
    if (strlen(data->selected_obj_file) > 0) {
      nk_layout_row_dynamic(ctx, 30, 1);
      if (nk_button_label(ctx, "Load OBJ File")) {
        // Освобождаем предыдущую модель, если она существует
        if (*model) {
          free_model(*model);
        }

        // Загружаем выбранный .obj файл
        *model = load_model_from_obj(data->selected_obj_file);

        if (*model) {
          printf("Successfully loaded OBJ file: %s\n", data->selected_obj_file);
        } else {
          printf("Failed to load OBJ file: %s\n", data->selected_obj_file);
        }
      }
    }
  }
}

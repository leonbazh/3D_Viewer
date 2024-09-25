#ifndef CORE_H
#define CORE_H
#include <assert.h>
#include <dirent.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "./backend/model_loader.h"
#include "./backend/transformations.h"

#ifdef _WIN32
#include <windows.h>
#endif

#define WIN_WIDTH 840
#define WIN_HEIGHT 620

#define GUI_GROUP_HEIGHT_END 23

#define GUI_WIN_WIDTH_X 240
#define GUI_WIN_WIDTH_Y 0

#define GUI_TOOL_BAR_Y_START 0
#define GUI_TOOL_BAR_Y_END 33
#define GUI_TOOL_BAR_HEIGHT GUI_TOOL_BAR_Y_END
#define GUI_TOOL_BAR_ITEMS_HEIGHT \
  (GUI_TOOL_BAR_HEIGHT / 2) + 3  // hardcoding :(

#define GUI_STATUS_BAR_HEIGHT 30
#define GUI_STATUS_BAR_ITEMS_HEIGHT \
  (GUI_STATUS_BAR_HEIGHT / 2) + 3  // hardcoding :(

#define GUI_TABS_BUTTONS_START GUI_TOOL_BAR_Y_END
#define GUI_TABS_BUTTONS_WIDTH GUI_WIN_WIDTH_X
#define GUI_TABS_BUTTONS_HEIGHT \
  (GUI_TRANSFORM_PANEL_Y_START - GUI_STATUS_BAR_HEIGHT)
#define GUI_TABS_BUTTONS_ITEM_HEIGHT (GUI_TABS_BUTTONS_HEIGHT - 12)

#define GUI_TRANSFORM_PANEL_Y_START GUI_TOOL_BAR_HEIGHT + GUI_GROUP_HEIGHT_END
#define GUI_TRANSFORM_PANEL_Y_END \
  WIN_HEIGHT - (GUI_TRANSFORM_PANEL_Y_START + GUI_STATUS_BAR_HEIGHT)
#define GUI_TRANSFORM_PANEL_LABEL_HEIGHT 12
#define GUI_TRANSFORM_PANEL_POSITION_BOX_HEIGHT        \
  (GUI_TRANSFORM_PANEL_Y_END - GUI_STATUS_BAR_HEIGHT - \
   GUI_TRANSFORM_PANEL_LABEL_HEIGHT) /                 \
      3
#define GUI_TRANSFORM_PANEL_POSITION_BOX_ITEM_HEIGHT \
  GUI_TRANSFORM_PANEL_POSITION_BOX_HEIGHT / 4

#define GUI_VIEW_PANEL_Y_START GUI_TRANSFORM_PANEL_Y_START
#define GUI_VIEW_PANEL_Y_END GUI_TRANSFORM_PANEL_Y_END
#define GUI_VIEW_PANEL_LABEL_HEIGHT 12
#define GUI_VIEW_PANEL_BOX_HEIGHT GUI_TRANSFORM_PANEL_POSITION_BOX_HEIGHT
#define GUI_VIEW_PANEL_BOX_ITEM_HEIGHT \
  GUI_TRANSFORM_PANEL_POSITION_BOX_ITEM_HEIGHT

#define GUI_FILE_BROWSE_WIDTH GUI_WIN_WIDTH_X
#define GUI_FILE_BROWSE_HEIGHT GUI_TRANSFORM_PANEL_Y_END
#define GUI_FILE_BROWSE_Y_START GUI_VIEW_PANEL_Y_START
#define GUI_FILE_BROWSE_Y_END GUI_VIEW_PANEL_Y_END
#define GUI_FILE_BROWSE_ITEM_HEIGHT GUI_TRANSFORM_PANEL_POSITION_BOX_ITEM_HEIGHT

#define WIN_MODEL_DRAW_AREA_X 200
#define WIN_MODEL_DRAW_AREA_Y 200
#define WIN_CENTRE_X (WIN_WIDTH / 2)
#define WIN_CENTRE_Y (WIN_HEIGHT / 2)

#define MAX_FILES 100
#define MAX_FILENAME_LENGTH 512
#define MAX_PATH_LENGTH 512

enum Notebook_charts { TRANSFORM_PANEL, STYLE_PANEL };

struct GUI_tabs_bar {
  int current_tab;
  int tabs_amount;
  char tab_names[3][256];
};

struct GUI_transform_panel {
  float model_x_pos, model_y_pos, model_z_pos;
  float model_x_pos_property, model_y_pos_property, model_z_pos_property;
  int model_rotation_x, model_rotation_y, model_rotation_z;
  float model_rotation_x_property, model_rotation_y_property,
      model_rotation_z_property;
  float translation[3];
  float rotation[3];
  float scale_factor;
};

struct GUI_status_bar {
  Model *model;
  char file_name[256];
};

typedef struct GUI_view_settings {
  int projection_type;    // 0: Parallel, 1: Central (Perspective)
  int edge_type;          // 0: Solid, 1: Dashed
  float edge_color[3];    // Edge color (RGB)
  float edge_thickness;   // Edge thickness
  int vertex_display;     // 0: None, 1: Circle, 2: Square
  float vertex_color[3];  // Vertex color (RGB)
  float vertex_size;      // Vertex size
  float bg_color[3];      // Background color (RGB)
} GUI_view_settings;

struct GUI_directory_data {
  char obj_files[MAX_FILES][MAX_FILENAME_LENGTH];
  int obj_file_count;
  char selected_obj_file[MAX_PATH_LENGTH];
  int file_menu_active;
  char obj_directory[MAX_PATH_LENGTH];
};

/**
 * @brief Сохраняет настройки интерфейса в файл settings.cfg.
 *
 * Эта функция записывает текущие настройки интерфейса, такие как тип проекции,
 * цвет вершин и ребер, толщину линий и цвет фона, в файл settings.cfg.
 *
 * @param settings Указатель на структуру GUI_view_settings, содержащую
 * настройки для сохранения.
 */
void save_settings(GUI_view_settings *settings);

/**
 * @brief Загружает настройки интерфейса из файла settings.cfg.
 *
 * Если файл settings.cfg существует, настройки загружаются из него. Если файл
 * не найден, настройки остаются по умолчанию.
 *
 * @param settings Указатель на структуру GUI_view_settings, куда будут
 * загружены настройки.
 */
void load_settings(GUI_view_settings *settings);

/**
 * @brief Устанавливает настройки интерфейса по умолчанию.
 *
 * Устанавливает значения настроек интерфейса по умолчанию для параметров, таких
 * как тип проекции, цвет фона, цвет ребер, и т.д.
 *
 * @param settings Указатель на структуру GUI_view_settings, куда будут записаны
 * значения по умолчанию.
 */
void set_default_settings(GUI_view_settings *settings);

/**
 * @brief Инициализирует структуру данных директории для поиска .obj файлов.
 *
 * Устанавливает директорию, в которой будут искаться файлы .obj, а также
 * обнуляет список найденных файлов и состояние меню выбора файла.
 *
 * @param data Указатель на структуру GUI_directory_data, которая будет
 * инициализирована.
 * @param directory Путь к директории для поиска файлов .obj.
 */
void init_dir_data(struct GUI_directory_data *data, const char *directory);

/**
 * @brief Сканирует указанную директорию на наличие .obj файлов.
 *
 * Функция проходит по файлам в указанной директории и ищет файлы с расширением
 * .obj. Найденные файлы добавляются в список файлов в структуре
 * GUI_directory_data.
 *
 * @param data Указатель на структуру GUI_directory_data, содержащую данные
 * директории и список файлов.
 */
void scan_obj_files(struct GUI_directory_data *data);

#endif

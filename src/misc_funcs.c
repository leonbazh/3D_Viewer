#include <stdio.h>

#include "core.h"

void load_settings(GUI_view_settings* settings) {
  FILE* file = fopen("settings.cfg", "r");

  if (file) {
    fread(settings, sizeof(GUI_view_settings), 1, file);
    fclose(file);
  } else {
    fprintf(stdout, ".cfg empty or not found, using default settings");
  }
}

void save_settings(GUI_view_settings* settings) {
  FILE* file = fopen("settings.cfg", "w");

  if (file) {
    fwrite(settings, sizeof(GUI_view_settings), 1, file);
    fclose(file);
  }
}

void init_dir_data(struct GUI_directory_data* data, const char* directory) {
  strncpy(data->obj_directory, directory, MAX_PATH_LENGTH - 1);
  data->obj_directory[MAX_PATH_LENGTH - 1] = '\0';
  data->file_menu_active = 0;
  data->obj_file_count = 0;
  data->selected_obj_file[0] = '\0';
}

// Сканирование директории на наличие .obj файлов
void scan_obj_files(struct GUI_directory_data* state) {
  DIR* dir = opendir(state->obj_directory);
  struct dirent* entry;

  state->obj_file_count = 0;

  if (dir == NULL) {
    printf("Error: Unable to open directory %s\n", state->obj_directory);
    return;
  }

  while ((entry = readdir(dir)) != NULL) {
    // Формируем полный путь к файлу
    struct stat file_stat;
    char full_path[MAX_FILENAME_LENGTH + MAX_PATH_LENGTH];
    snprintf(full_path, sizeof(full_path), "%s/%s", state->obj_directory,
             entry->d_name);

    // Проверяем, является ли файл обычным
    if (stat(full_path, &file_stat) == 0 && S_ISREG(file_stat.st_mode)) {
      const char* ext = strrchr(entry->d_name, '.');
      if (ext && strcmp(ext, ".obj") == 0) {
        strncpy(state->obj_files[state->obj_file_count], entry->d_name,
                MAX_FILENAME_LENGTH - 1);
        state->obj_files[state->obj_file_count][MAX_FILENAME_LENGTH - 1] = '\0';
        state->obj_file_count++;

        if (state->obj_file_count >= MAX_FILES)
          break;  // Лимит на количество файлов
      }
    }
  }

  closedir(dir);
  printf("Found %d OBJ files in %s\n", state->obj_file_count,
         state->obj_directory);
}

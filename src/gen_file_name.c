#include "core.h"

void generate_filename_with_timestamp(char *buffer, size_t buffer_size,
                                      const char *base_name,
                                      const char *extension) {
  // Получаем текущее время
  time_t raw_time;
  struct tm *time_info;
  time(&raw_time);
  time_info = localtime(&raw_time);

  // Формируем строку с датой и временем в формате YYYY-MM-DD_HH-MM-SS
  char time_str[64];
  strftime(time_str, sizeof(time_str), "%Y-%m-%d_%H-%M-%S", time_info);

  // Формируем итоговое имя файла
  snprintf(buffer, buffer_size, "%s_%s.%s", base_name, time_str, extension);
}

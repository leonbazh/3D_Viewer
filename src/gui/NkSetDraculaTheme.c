static void set_style(struct nk_context *ctx) {
  struct nk_color table[NK_COLOR_COUNT];
  struct nk_color background = nk_rgba(40, 42, 54, 255);
  struct nk_color currentline = nk_rgba(68, 71, 90, 255);
  struct nk_color foreground = nk_rgba(248, 248, 242, 255);
  struct nk_color comment = nk_rgba(98, 114, 164, 255);
  /* struct nk_color cyan = nk_rgba(139, 233, 253, 255); */
  /* struct nk_color green = nk_rgba(80, 250, 123, 255); */
  /* struct nk_color orange = nk_rgba(255, 184, 108, 255); */
  struct nk_color pink = nk_rgba(255, 121, 198, 255);
  struct nk_color purple = nk_rgba(189, 147, 249, 255);
  /* struct nk_color red = nk_rgba(255, 85, 85, 255); */
  /* struct nk_color yellow = nk_rgba(241, 250, 140, 255); */
  table[NK_COLOR_TEXT] = foreground;
  table[NK_COLOR_WINDOW] = background;
  table[NK_COLOR_HEADER] = currentline;
  table[NK_COLOR_BORDER] = currentline;
  table[NK_COLOR_BUTTON] = currentline;
  table[NK_COLOR_BUTTON_HOVER] = comment;
  table[NK_COLOR_BUTTON_ACTIVE] = purple;
  table[NK_COLOR_TOGGLE] = currentline;
  table[NK_COLOR_TOGGLE_HOVER] = comment;
  table[NK_COLOR_TOGGLE_CURSOR] = pink;
  table[NK_COLOR_SELECT] = currentline;
  table[NK_COLOR_SELECT_ACTIVE] = comment;
  table[NK_COLOR_SLIDER] = background;
  table[NK_COLOR_SLIDER_CURSOR] = currentline;
  table[NK_COLOR_SLIDER_CURSOR_HOVER] = comment;
  table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = comment;
  table[NK_COLOR_PROPERTY] = currentline;
  table[NK_COLOR_EDIT] = currentline;
  table[NK_COLOR_EDIT_CURSOR] = foreground;
  table[NK_COLOR_COMBO] = currentline;
  table[NK_COLOR_CHART] = currentline;
  table[NK_COLOR_CHART_COLOR] = comment;
  table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = purple;
  table[NK_COLOR_SCROLLBAR] = background;
  table[NK_COLOR_SCROLLBAR_CURSOR] = currentline;
  table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = comment;
  table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = purple;
  table[NK_COLOR_TAB_HEADER] = currentline;
  nk_style_from_table(ctx, table);
}

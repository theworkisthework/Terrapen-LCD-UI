#include "terrapen.h"

static void event_cb(lv_event_t *e)
{
    char buff[40];
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = (lv_obj_t *)lv_event_get_target(e);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        uint32_t id = lv_buttonmatrix_get_selected_button(obj);
        const char *txt = lv_buttonmatrix_get_button_text(obj, id);
        // snprintf(buff, 40, "%s was pressed\n", txt);
        LV_LOG_INFO("%s was pressed\n", txt);
    }
}

void create_jog_panel(lv_obj_t *parent)
{
    lv_obj_set_scrollbar_mode(parent, LV_SCROLLBAR_MODE_OFF);
    static lv_style_t style_bg;
    lv_style_init(&style_bg);
    lv_style_set_bg_color(&style_bg, lv_palette_darken(LV_PALETTE_BLUE_GREY, 4));
    lv_style_set_pad_all(&style_bg, 0);
    lv_style_set_pad_gap(&style_bg, 0);
    lv_style_set_clip_corner(&style_bg, true);
    lv_style_set_radius(&style_bg, LV_RADIUS_CIRCLE);
    lv_style_set_border_width(&style_bg, 0);

    static lv_style_t style_btn;
    lv_style_init(&style_btn);
    lv_style_set_bg_color(&style_btn, lv_palette_darken(LV_PALETTE_BLUE_GREY, 2));
    lv_style_set_bg_opa(&style_btn, LV_OPA_100);
    lv_style_set_radius(&style_btn, 0);
    lv_style_set_border_width(&style_btn, 1);
    lv_style_set_border_opa(&style_btn, LV_OPA_80);
    lv_style_set_border_color(&style_btn, lv_palette_main(LV_PALETTE_BLUE_GREY));
    lv_style_set_border_side(&style_btn, LV_BORDER_SIDE_INTERNAL);
    lv_style_set_radius(&style_btn, 0);

    static int32_t col_dsc[] = {70, 70, 70, 70, LV_GRID_TEMPLATE_LAST};
    static int32_t row_dsc[] = {50, 50, 50, 50, LV_GRID_TEMPLATE_LAST};

    /*Create a container with grid*/
    lv_obj_t *cont = lv_obj_create(parent);
    lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_layout(cont, LV_LAYOUT_GRID);
    lv_obj_set_size(cont, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_grid_dsc_array(cont, col_dsc, row_dsc);
    lv_obj_set_size(cont, 320, 210);
    lv_obj_center(cont);

    lv_obj_t *obj;
    lv_obj_t *label;
    // create button array

    obj = lv_obj_create(cont);
    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_CENTER, 3, 1,
                         LV_GRID_ALIGN_CENTER, 0, 3);
    static const char *map[] = {"100", "\n", "10", "\n", "1", "\n", "0.1", ""};

    lv_obj_t *btnm = lv_buttonmatrix_create(obj);
    lv_buttonmatrix_set_map(btnm, map);
    lv_obj_add_style(btnm, &style_bg, 0);
    lv_obj_add_style(btnm, &style_btn, LV_PART_ITEMS);
    lv_obj_add_event_cb(btnm, event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_set_size(btnm, 35, 180);

    /*Allow selecting on one number at time*/
    lv_buttonmatrix_set_button_ctrl_all(btnm, LV_BUTTONMATRIX_CTRL_CHECKABLE);
    lv_buttonmatrix_set_button_ctrl(btnm, 2, LV_BUTTONMATRIX_CTRL_CHECKED);

    lv_buttonmatrix_set_one_checked(btnm, true);
    lv_buttonmatrix_set_button_ctrl(btnm, 1, LV_BUTTONMATRIX_CTRL_CHECKED);

    lv_obj_center(btnm);

    // BUTTON UP
    obj = lv_obj_create(cont);
    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_CENTER, 1, 1,
                         LV_GRID_ALIGN_CENTER, 0, 1);
    lv_obj_t *btn1 = lv_button_create(obj);
    // lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_style(btn1, &style_btn, 0);
    // lv_obj_align(btn1, LV_ALIGN_CENTER, 0, 0);
    //   lv_obj_remove_flag(btn1, LV_OBJ_FLAG_PRESS_LOCK);

    label = lv_label_create(btn1);
    lv_label_set_text(label, LV_SYMBOL_UP);
    lv_obj_center(label);

    // BUTTON LEFT
    obj = lv_obj_create(cont);
    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_CENTER, 0, 1,
                         LV_GRID_ALIGN_CENTER, 1, 1);
    lv_obj_t *btn2 = lv_button_create(obj);
    // lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_style(btn2, &style_btn, 0);
    // lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 0);
    //  lv_obj_remove_flag(btn1, LV_OBJ_FLAG_PRESS_LOCK);

    label = lv_label_create(btn2);
    lv_label_set_text(label, LV_SYMBOL_LEFT);
    lv_obj_center(label);

    // BUTTON RIGHT
    obj = lv_obj_create(cont);
    lv_obj_t *btn3 = lv_button_create(obj);
    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_CENTER, 2, 1,
                         LV_GRID_ALIGN_CENTER, 1, 1);
    // lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_style(btn3, &style_btn, 0);
    // lv_obj_align(btn3, LV_ALIGN_CENTER, 0, 0);
    //  lv_obj_remove_flag(btn1, LV_OBJ_FLAG_PRESS_LOCK);

    label = lv_label_create(btn3);
    lv_label_set_text(label, LV_SYMBOL_RIGHT);
    lv_obj_center(label);

    // BUTTON DOWN
    obj = lv_obj_create(cont);
    lv_obj_t *btn4 = lv_button_create(obj);
    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_CENTER, 1, 1,
                         LV_GRID_ALIGN_CENTER, 2, 1);
    // lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_style(btn4, &style_btn, 0);
    // lv_obj_align(btn4, LV_ALIGN_CENTER, 0, 0);
    //  lv_obj_remove_flag(btn1, LV_OBJ_FLAG_PRESS_LOCK);

    label = lv_label_create(btn4);
    lv_label_set_text(label, LV_SYMBOL_DOWN);
    lv_obj_center(label);
}

void terrapen_setup(void)
{
    lv_obj_t *tabview;
    lv_obj_t *label;

    tabview = lv_tabview_create(lv_screen_active());
    lv_tabview_set_tab_bar_size(tabview, 30);

    lv_theme_t *th = lv_theme_default_init(lv_display_get_default(),                                                      /*Use the DPI, size, etc from this display*/
                                           lv_palette_lighten(LV_PALETTE_BLUE_GREY, 4), lv_palette_main(LV_PALETTE_BLUE), /*Primary and secondary palette*/
                                           true,                                                                          /*Light or dark mode*/
                                           &lv_font_montserrat_14);

    lv_display_set_theme(lv_display_get_default(), th); /*Assign the theme to the display*/

    /*Add 3 tabs (the tabs are page (lv_page) and can be scrolled*/
    lv_obj_t *tab1 = lv_tabview_add_tab(tabview, "Jog");
    lv_obj_t *tab2 = lv_tabview_add_tab(tabview, "Pen Load");
    lv_obj_t *tab3 = lv_tabview_add_tab(tabview, "Play/Pause");
    lv_obj_t *tab4 = lv_tabview_add_tab(tabview, "Settings");

    create_jog_panel(tab1);

    label = lv_label_create(tab2);
    lv_label_set_text(label, "Second tab");

    label = lv_label_create(tab3);
    lv_label_set_text(label, "Third tab");

    label = lv_label_create(tab4);
    lv_label_set_text(label, "Forth tab");
}
#include "ui.h"
#include "esp_log.h"
#include "lv_conf.h"
#include "lvgl.h"
#include "item_registry.h"

static const char *TAG = "MAIN";

static lv_obj_t* devices_list;
static lv_obj_t* passwords_list;

static void list_item_cb(lv_event_t *e, lv_obj_t* list) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED) {
        lv_list_get_button_text(list, obj);
        ESP_LOGI(TAG, "Button clicked");
    }
}

static void device_list_item_cb(lv_event_t *e) {
    list_item_cb(e, devices_list);
}

static void password_list_item_cb(lv_event_t *e) {
    list_item_cb(e, passwords_list);
}

static void updale_list_items(lv_obj_t *list, registry_api *registry, lv_event_cb_t cb) {
    lv_color_t row_bg_color1 = lv_color_make(70, 65, 60);
    lv_color_t row_bg_color2 = lv_color_make(60, 60, 80);
    lv_color_t row_text_color = lv_color_make(240, 240, 240);

    lv_obj_clean(list);
    for (int i = 0; i < registry->get_count(); i++) {
        lv_obj_t *btn = lv_list_add_button(list, LV_SYMBOL_FILE, registry->get_name(i));
        lv_obj_set_style_bg_color(btn, (i % 2) ? row_bg_color1 : row_bg_color2, 0);
        lv_obj_set_style_text_color(btn, row_text_color, 0);
        lv_obj_set_style_border_side(btn, LV_BORDER_SIDE_NONE, 0);
        lv_obj_add_event_cb(btn, cb, LV_EVENT_CLICKED, NULL);
    }
}

static lv_obj_t * init_tab_view(char *text, lv_obj_t *parent, registry_api *registry, lv_event_cb_t cb) {
    lv_obj_t *tab = lv_tabview_add_tab(parent, text);
    lv_obj_remove_flag(tab, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_margin_all(tab, 0, 0);
    lv_obj_set_style_pad_all(tab, 0, 0);
    lv_obj_set_style_bg_opa(tab, LV_OPA_0, 0);

    lv_obj_t *list = lv_list_create(tab);
    lv_obj_align(list, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_width(list, SCREEN_W-5, 0);


    lv_obj_set_style_bg_opa(list, LV_OPA_0, 0);
    lv_obj_set_style_margin_all(list, 0, 0);
    lv_obj_set_style_pad_all(list, 0, 0);
    lv_obj_set_style_border_side(list, LV_BORDER_SIDE_NONE, 0);
    
    updale_list_items(list, registry, cb);
    return list;
}

void tab_switch_event_cb(lv_event_t * e) {
    lv_obj_t * tabview = lv_event_get_target(e);
    uint32_t id = lv_tabview_get_tab_active(tabview);
    lv_tabview_set_active(tabview, id, LV_ANIM_OFF);
    //printf("Tab switch to %d\n", id);
    ESP_LOGI(TAG, "Tab switch to %d", (int)id);
}


void init_ui() {
    ESP_LOGI(TAG, "init_ui()");

    lv_obj_t *tabview = lv_tabview_create(lv_screen_active());
    lv_tabview_set_tab_bar_size(tabview, TAB_BAR_H);
    lv_obj_set_style_bg_color(tabview, lv_color_black(), 0);

    lv_obj_t * tab_buttons = lv_tabview_get_tab_bar(tabview);
    lv_obj_set_style_bg_color(tab_buttons, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
    lv_obj_set_style_text_color(tab_buttons, lv_palette_lighten(LV_PALETTE_GREY, 5), 0);
    lv_obj_set_style_border_side(tab_buttons, LV_BORDER_SIDE_BOTTOM, LV_PART_ITEMS | LV_STATE_CHECKED);

    lv_obj_add_event_cb(tabview, tab_switch_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    devices_list = init_tab_view("Devices", tabview, &device_registry, device_list_item_cb);
    passwords_list = init_tab_view("Passwords", tabview, &password_registry, password_list_item_cb);

}

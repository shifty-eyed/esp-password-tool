#include "esp_log.h"
#include "lv_conf.h"
#include "lcd_touch.h"

static const char *TAG = "PSWMAIN";

static lv_obj_t *avatar;

void app_button_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e); // Get the event code

    if (code == LV_EVENT_CLICKED) {
        ESP_LOGI(TAG, "Button clicked");
    }
    
}

void create_circle(void) {
lv_obj_t * label;

    lv_obj_t * btn1 = lv_btn_create(lv_scr_act());
    lv_obj_add_event_cb(btn1, app_button_cb, LV_EVENT_ALL, NULL);
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -40);

    label = lv_label_create(btn1);
    lv_label_set_text(label, "Button");
    lv_obj_center(label);

    lv_obj_t * btn2 = lv_btn_create(lv_scr_act());
    lv_obj_add_event_cb(btn2, app_button_cb, LV_EVENT_ALL, NULL);
    lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 40);
    lv_obj_add_flag(btn2, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_height(btn2, LV_SIZE_CONTENT);

    label = lv_label_create(btn2);
    lv_label_set_text(label, "Toggle");
    lv_obj_center(label);
    
}

void app_main(void) {
    init_lcd_and_touch();

    
    lvgl_port_lock(-1);

    create_circle();
    
    lvgl_port_unlock();
}

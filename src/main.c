#include "esp_log.h"
#include "lcd_touch.h"
#include "item_registry.h"
#include "ui.h"

static const char *TAG = "MAIN";

void app_main(void) {
    init_lcd_and_touch();
    set_lcd_brightness(50);

    device_registry.load();
    password_registry.load();

    lvgl_port_lock(-1);
    init_ui();
    lvgl_port_unlock();

}

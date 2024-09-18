#include <WiFi.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include <lvgl.h>
#include <LovyanGFX.hpp>
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>
#include <Wire.h>
#include <TAMC_GT911.h>
#include "terrapen.h"
#include "TPConfiguration.hpp"

#define TFT_BL 2

static const uint16_t screenWidth = TFT_HOR_RES;
static const uint16_t screenHeight = TFT_VER_RES;
const unsigned long lvBufferSize = screenWidth * screenHeight / 10 * (LV_COLOR_DEPTH / 8);
uint8_t *lvBuffer;

const char *ssid = "tp_controller";
const char *password = "12345678";

unsigned long lastTickMillis = 0;

class LGFX : public lgfx::LGFX_Device
{
public:
  lgfx::Bus_RGB _bus_instance;
  lgfx::Panel_RGB _panel_instance;

  LGFX(void)
  {

    {
      auto cfg = _bus_instance.config();
      cfg.panel = &_panel_instance;

      cfg.pin_d0 = GPIO_NUM_8;  // B0
      cfg.pin_d1 = GPIO_NUM_3;  // B1
      cfg.pin_d2 = GPIO_NUM_46; // B2
      cfg.pin_d3 = GPIO_NUM_9;  // B3
      cfg.pin_d4 = GPIO_NUM_1;  // B4

      cfg.pin_d5 = GPIO_NUM_5;  // G0
      cfg.pin_d6 = GPIO_NUM_6;  // G1
      cfg.pin_d7 = GPIO_NUM_7;  // G2
      cfg.pin_d8 = GPIO_NUM_15; // G3
      cfg.pin_d9 = GPIO_NUM_16; // G4
      cfg.pin_d10 = GPIO_NUM_4; // G5

      cfg.pin_d11 = GPIO_NUM_45; // R0
      cfg.pin_d12 = GPIO_NUM_48; // R1
      cfg.pin_d13 = GPIO_NUM_47; // R2
      cfg.pin_d14 = GPIO_NUM_21; // R3
      cfg.pin_d15 = GPIO_NUM_14; // R4
      cfg.pin_henable = GPIO_NUM_40;
      cfg.pin_vsync = GPIO_NUM_41;
      cfg.pin_hsync = GPIO_NUM_39;
      cfg.pin_pclk = GPIO_NUM_0;
      cfg.freq_write = 15000000;

      cfg.hsync_polarity = 0;
      cfg.hsync_front_porch = 8;
      cfg.hsync_pulse_width = 4;
      cfg.hsync_back_porch = 43;

      cfg.vsync_polarity = 0;
      cfg.vsync_front_porch = 8;
      cfg.vsync_pulse_width = 4;
      cfg.vsync_back_porch = 12;

      cfg.pclk_active_neg = 1;
      cfg.de_idle_high = 0;
      cfg.pclk_idle_high = 0;

      _bus_instance.config(cfg);
    }
    {
      auto cfg = _panel_instance.config();
      cfg.memory_width = 800;
      cfg.memory_height = 480;
      cfg.panel_width = 800;
      cfg.panel_height = 480;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      _panel_instance.config(cfg);
    }
    _panel_instance.setBus(&_bus_instance);
    setPanel(&_panel_instance);
  }
};

static LGFX lcd;

#define TOUCH_GT911
#define TOUCH_GT911_SCL 20 // 20
#define TOUCH_GT911_SDA 19 // 19
#define TOUCH_GT911_INT -1 //-1
#define TOUCH_GT911_RST -1 // 38
#define TOUCH_GT911_ROTATION ROTATION_NORMAL
#define TOUCH_MAP_X1 800 // 480
#define TOUCH_MAP_X2 0
#define TOUCH_MAP_Y1 480 // 272
#define TOUCH_MAP_Y2 0

int touch_last_x = 0, touch_last_y = 0;

TAMC_GT911 ts = TAMC_GT911(TOUCH_GT911_SDA, TOUCH_GT911_SCL, TOUCH_GT911_INT, TOUCH_GT911_RST, max(TOUCH_MAP_X1, TOUCH_MAP_X2), max(TOUCH_MAP_Y1, TOUCH_MAP_Y2));
uint16_t touchX,
    touchY;

void my_touchpad_read(lv_indev_t *indev_driver, lv_indev_data_t *data)
{
  ts.read();
  if (ts.isTouched)
  {

    touch_last_x = map(ts.points[0].x, TOUCH_MAP_X1, TOUCH_MAP_X2, 0, lcd.width() - 1);
    touch_last_y = map(ts.points[0].y, TOUCH_MAP_Y1, TOUCH_MAP_Y2, 0, lcd.height() - 1);
    data->state = LV_INDEV_STATE_PR;

    /*Set the coordinates*/
    data->point.x = touch_last_x;
    data->point.y = touch_last_y;
  }
  else
  {
    data->state = LV_INDEV_STATE_REL;
  }
}

void my_log_cb(lv_log_level_t level, const char *buf)
{
  Serial.println(buf);
}

void setup()
{
  Serial.begin(115200); /*serial init */
  Serial.println("Starting terraPen controller");
  Wire.begin(TOUCH_GT911_SDA, TOUCH_GT911_SCL);

  TPConfiguration config;
  config.readConfig();

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  ArduinoOTA
      .onStart([]()
               {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
      } else {  // U_SPIFFS
        type = "filesystem";
      }

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type); })
      .onEnd([]()
             { Serial.println("\nEnd"); })
      .onProgress([](unsigned int progress, unsigned int total)
                  { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
      .onError([](ota_error_t error)
               {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth Failed");
      } else if (error == OTA_BEGIN_ERROR) {
        Serial.println("Begin Failed");
      } else if (error == OTA_CONNECT_ERROR) {
        Serial.println("Connect Failed");
      } else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("Receive Failed");
      } else if (error == OTA_END_ERROR) {
        Serial.println("End Failed");
      } });

  ArduinoOTA.begin();
  Serial.println("Starting terraPen screen");
  lcd.begin();

  ts.begin();
  ts.setRotation(TOUCH_GT911_ROTATION);

  lv_log_register_print_cb(my_log_cb);

  // lvgl init
  lv_init();

  lvBuffer = (uint8_t *)heap_caps_malloc(lvBufferSize, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);

  static auto *lvDisplay = lv_display_create(screenWidth, screenHeight);

  lv_display_set_color_format(lvDisplay, LV_COLOR_FORMAT_RGB565);
  lv_display_set_flush_cb(lvDisplay, [](lv_display_t *display, const lv_area_t *area, unsigned char *data)
                          {
    uint32_t w = lv_area_get_width(area);
    uint32_t h = lv_area_get_height(area);
    lv_draw_sw_rgb565_swap(data, w * h);
    lcd.pushImage(area->x1, area->y1, w, h, (uint16_t *)data);
    lv_display_flush_ready(display); });

  lv_display_set_buffers(lvDisplay, lvBuffer, nullptr, lvBufferSize, LV_DISPLAY_RENDER_MODE_PARTIAL);

  static lv_indev_t *indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, my_touchpad_read);

  terrapen_setup();

#ifdef TFT_BL
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
#endif

  Serial.println("Setup done");
}

void loop()
{
  unsigned tickPeriod = millis() - lastTickMillis;
  lv_tick_inc(tickPeriod);
  lastTickMillis = millis();
  lv_timer_handler();
  delay(5);
}

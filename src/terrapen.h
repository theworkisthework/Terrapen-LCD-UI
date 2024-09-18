#ifndef TERRAPEN_H
#define TERRAPEN_H

#define TFT_HOR_RES 800
#define TFT_VER_RES 480

// #define TARGET_SIMULATOR

#include <lvgl.h>

#ifdef TARGET_SIMULATOR
#include <stdio.h>
#else
#include <LovyanGFX.hpp>
#endif // TARGET_SIMULATOR

void terrapen_setup(void);
void create_jog_panel(lv_obj_t *parent);

#endif // TERRAPEN_H
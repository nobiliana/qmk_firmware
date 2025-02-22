#pragma once

#include "keyboards/fingerpunch/src/config_pre.h"

#define MATRIX_COLS 8
#define MATRIX_ROWS  5

#define SPI_SCK_PIN     GP2
#define SPI_MOSI_PIN    GP3
#define SPI_MISO_PIN    GP4

#define SHIFTREG_MATRIX_COL_CS  GP0
#define SHIFTREG_DIVISOR        8

#define DIODE_DIRECTION COL2ROW

#define MATRIX_ROW_PINS_SR { GP26, GP27, GP28, GP29, GP6 }

#define POINTING_DEVICE_CS_PIN GP1

#define BOOTMAGIC_LITE_ROW 0
#define BOOTMAGIC_LITE_COLUMN 0

#ifdef CIRQUE_ENABLE
  #define POINTING_DEVICE_ROTATION_90
  #define CIRQUE_PINNACLE_TAP_ENABLE
  #define POINTING_DEVICE_TASK_THROTTLE_MS 5
  #define CIRQUE_PINNACLE_DIAMETER_MM 35
#endif

#include "keyboards/fingerpunch/src/config_post.h"

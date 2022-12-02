
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_spi_flash.h"
#include "esp_system.h"
#include "esp_timer.h"
#include "image_provider.h"
#include <sys/param.h>
#include <string.h>

#ifndef portTICK_RATE_MS
#define portTICK_RATE_MS portTICK_PERIOD_MS
#endif
#include "esp_camera.h"

#define BOARD_ESP32CAM_AITHINKER 1

// ESP32Cam (AiThinker) PIN Map
#ifdef BOARD_ESP32CAM_AITHINKER

#define CAM_PIN_PWDN 32
#define CAM_PIN_RESET -1 //software reset will be performed
#define CAM_PIN_XCLK 0
#define CAM_PIN_SIOD 26
#define CAM_PIN_SIOC 27

#define CAM_PIN_D7 35
#define CAM_PIN_D6 34
#define CAM_PIN_D5 39
#define CAM_PIN_D4 36
#define CAM_PIN_D3 21
#define CAM_PIN_D2 19
#define CAM_PIN_D1 18
#define CAM_PIN_D0 5
#define CAM_PIN_VSYNC 25
#define CAM_PIN_HREF 23
#define CAM_PIN_PCLK 22
#define FLASH_LIGHT 33
#endif

// static const char *TAG = "example:take_picture";

static camera_config_t camera_config = {
    .pin_pwdn = CAM_PIN_PWDN,
    .pin_reset = CAM_PIN_RESET,
    .pin_xclk = CAM_PIN_XCLK,
    .pin_sccb_sda = CAM_PIN_SIOD,
    .pin_sccb_scl = CAM_PIN_SIOC,

    .pin_d7 = CAM_PIN_D7,
    .pin_d6 = CAM_PIN_D6,
    .pin_d5 = CAM_PIN_D5,
    .pin_d4 = CAM_PIN_D4,
    .pin_d3 = CAM_PIN_D3,
    .pin_d2 = CAM_PIN_D2,
    .pin_d1 = CAM_PIN_D1,
    .pin_d0 = CAM_PIN_D0,
    .pin_vsync = CAM_PIN_VSYNC,
    .pin_href = CAM_PIN_HREF,
    .pin_pclk = CAM_PIN_PCLK,
    .xclk_freq_hz = 20000000,
   .ledc_timer = LEDC_TIMER_0,
   .ledc_channel = LEDC_CHANNEL_0,
   .pixel_format = PIXFORMAT_GRAYSCALE,
   .frame_size = FRAMESIZE_QVGA,
   .jpeg_quality = 12, 
   .fb_count = 1,       
   .fb_location = CAMERA_FB_IN_DRAM
};


static const char* TAG = "app_camera";



// Get the camera module ready
TfLiteStatus InitCamera() {

  esp_err_t err = esp_camera_init(&camera_config);
  if (err != ESP_OK) {
    MicroPrintf("Camera init failed\n");
    return kTfLiteError;
  }
  MicroPrintf("Camera Initialized\n");
  return kTfLiteOk;
}


int MaxPool(int j , int width, camera_fb_t * fb){
  //  uint8_t* max = (uint8_t*)malloc(1);
    uint8_t max = 0;  
    for (int k = 0; k < 2; k++) {
      for (int i = 0; i < 2; i++) {
        if ((fb->buf)[k+j + i*width] > max) {
          max = (fb->buf)[j+k + i*width];
        }
      }
    }
  // free memory
  return max^0x80;
}


// Get an image from the camera module
TfLiteStatus GetImage(int image_width, int image_height, int channels, int8_t* image_data) {
  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) {
    ESP_LOGE(TAG, "Camera capture failed");
    return kTfLiteError;
  }

  printf("Image Captured ... ");
  printf("Image Size: %d x %d\n", fb->width, fb->height);
  printf("Printing Image \n");
  int i = 0;
  int j = 20584;
  int line_no = 0;
  while (i < image_width * image_height) {
    if ((i+1)% image_width == 0) {
      line_no = line_no+ 2;
      j = line_no* fb->width + 104;
    }
    image_data[i] = MaxPool(j, fb->width, fb);
    // if (image_data[i] < 0) {
    //   printf("0");
    // }
    // else {
    //   printf("1");
    // }
    printf("%d ", image_data[i]);
    if ((i+1) % image_width == 0) {
      printf("\n");
    }
    i++;
    j = j + 2;
  }

  printf("\n Quantized !\n");
  esp_camera_fb_return(fb);
  /* here the esp camera can give you grayscale image directly */
  return kTfLiteOk;
}

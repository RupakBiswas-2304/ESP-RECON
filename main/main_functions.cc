#include "main_functions.h"
#include "image_provider.h" // user defined
#include "tensorflow/lite/micro/micro_interpreter.h" 
#include "tensorflow/lite/micro/micro_log.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <esp_heap_caps.h>
#include <esp_timer.h>
#include <esp_log.h>
#include "esp_main.h"
#include "model_data.h"

namespace {
  const tflite::Model* model = nullptr;
  tflite::MicroInterpreter* interpreter = nullptr;
  TfLiteTensor* input = nullptr;

  #ifdef CONFIG_IDF_TARGET_ESP32S3
    constexpr int scratchBufSize = 39 * 1024;
  #else
    constexpr int scratchBufSize = 0;
  #endif
  // An area of memory to use for input, output, and intermediate arrays.
  constexpr int kTensorArenaSize = 81 * 1024 + scratchBufSize;
  static uint8_t *tensor_arena;//[kTensorArenaSize]; // Maybe we should move this to external
}  // namespace

// The name of this function is important for Arduino compatibility.
void setup() {
  model = tflite::GetModel(model_data);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    MicroPrintf("Model provided is schema version %d not equal to supported "
                "version %d.", model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }
  if (tensor_arena == NULL) {
    tensor_arena = (uint8_t *) heap_caps_malloc(kTensorArenaSize, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  }
  if (tensor_arena == NULL) {
    printf("Couldn't allocate memory of %d bytes\n", kTensorArenaSize);
    return;
  }
  static tflite::MicroMutableOpResolver<8> micro_op_resolver;
  micro_op_resolver.AddQuantize();
  micro_op_resolver.AddConv2D();
  micro_op_resolver.AddRelu();
  micro_op_resolver.AddMaxPool2D();
  micro_op_resolver.AddReshape();
  micro_op_resolver.AddFullyConnected();
  micro_op_resolver.AddSoftmax();
  micro_op_resolver.AddDequantize();

  // Build an interpreter to run the model with.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::MicroInterpreter static_interpreter(
      model, micro_op_resolver, tensor_arena, kTensorArenaSize);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    MicroPrintf("AllocateTensors() failed");
    return;
  }
  printf("Model loaded");

  // Get information about the memory area to use for the model's input.
  input = interpreter->input(0);

  // Initialize Camera
  TfLiteStatus init_status = InitCamera();
  if (init_status != kTfLiteOk) {
    MicroPrintf("InitCamera failed\n");
    return;
  }
  printf("Camera initialized");
}


// The name of this function is important for Arduino compatibility.
void loop() {
  // Get image from provider.
  printf("Getting image...\n");
  if (kTfLiteOk != GetImage(28, 28, 1, input->data.int8)) {
    MicroPrintf("Image capture failed.");
  }
  
  // Run the model on this input and make sure it succeeds.
  if (kTfLiteOk != interpreter->Invoke()) {
    MicroPrintf("Invoke failed.");
  }

  TfLiteTensor* output = interpreter->output(0);
  int numElements = output->dims->data[1];
  int argmax = -1;
  float argmax_val = 0.5;
  printf("Number of Elements = %d \n",numElements);
  for (int i = 0; i < numElements; i++)
    {
      if (output->data.f[i] > argmax_val)
      {
        argmax_val = output->data.f[i];
        argmax = i;
      }
    }
  printf("prediction = %d\n", argmax);
  vTaskDelay(100 );
}

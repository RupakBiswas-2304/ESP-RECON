# Handwritten Digit Recognition
The goal of the project is to implement CNN based digit-recognition system in a edge and resource constrained device. More specifically, I used a esp32-cam module, which have 2mp camera, 512kb RAM and 4Mb flash memory. My object is to capture image in a loop, crop that image, send to the cnn model’s input tensor, run the model ( invoke ) and output the result based on maximum probability returned


### Install the ESP IDF
- Guide : https://rupakbiswas-2304.github.io/esp32/installation/

### File Structure:
- main.cc : main file that runs
- model_data.h : contains the model’s input and output tensor
- model_data_archive.h : contains the old model’s weights and bias ( not used )
- CMakeLists.txt : cmake file to build the project
- Makefile : make file to build the project
- README.md : this file
- image_provider.h : contains the image provider function definition
- image_provider.cc : contains the image provider function implementation

### Dependencies
- IDF 
- ESP32-CAM
- ESP32-CAM Arduino Library
- Tensorflow Lite for Microcontrollers
- Tensorflow
- Matplotlib
### Building the Model
- Goto the python folder
- run the main.py file 
- It will download the mnist dataset and train the model
- It will save the model in the models folder
- Next for converting the model, run the convert_lite.py file
- It will convert the model to tflite format
- For testing the file, run `python test.py`
### Load and run the example

To flash (replace `/dev/ttyUSB0` with the device serial port):
```
idf.py --port /dev/ttyUSB0 flash
```

Monitor the serial output:
```
idf.py --port /dev/ttyUSB0 monitor
```

Use `Ctrl+]` to exit.

The previous two commands can be combined:
```
idf.py --port /dev/ttyUSB0 flash monitor
```

### Some Output Dump 
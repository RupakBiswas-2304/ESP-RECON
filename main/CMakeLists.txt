
#
# Main component of TF Micro project 'person_detection'.
#

idf_component_register(
    SRCS
       
        "image_provider.cc"
        "main.cc"
        "main_functions.cc"
       
       
       
       

    PRIV_REQUIRES  tflite-lib esp32-camera spi_flash fb_gfx
    INCLUDE_DIRS "")

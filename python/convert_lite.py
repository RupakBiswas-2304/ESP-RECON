import tensorflow as tf
import numpy as np
# converter = tf.lite.TFLiteConverter.from_saved_model('epic_num_reader.#model') # path to the SavedModel directory
# converter.optimizations = [tf.lite.Optimize.OPTIMIZE_FOR_SIZE]
# tflite_model = converter.conve  rt()

# import tensorflow_model_optimization as tfmot

mnist = tf.keras.datasets.mnist
(x_train, y_train), (x_test, y_test) = mnist.load_data()
x_train = tf.keras.utils.normalize(x_train, axis=1)
# x_test = tf.keras.utils.normalize(x_test, axis=1)


def representative_data_gen():
  for input_value in tf.data.Dataset.from_tensor_slices(x_train).batch(1).take(100):

    # Model has only one input so each data point has one element.
    input_img = np.expand_dims(input_value, 3)
    input_img = input_img.astype('float32')
    print(input_img.shape, input_img.dtype)
    yield [input_img]
  



# model = tf.keras.models.load_model("epic_num_reader.model")
# quantized_model = tfmot.quantization.keras.quantize_model
# new_model = quantized_model(model)
# new_model.compile(
#   optimizer = 'adam',
#   loss = tf.losses.SparseCategoricalCrossentropy(from_logits=True),
#   metrics=['accuracy'],
#   representative_dataset=representative_data_gen
# )


converter = tf.lite.TFLiteConverter.from_saved_model("epic_num_reader.model")
converter.optimizations = [tf.lite.Optimize.DEFAULT]
converter.representative_dataset = representative_data_gen
print("Flag 1")
# converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
# converter.inference_input_type = tf.int8  # or tf.uint8
# converter.inference_output_type = tf.int8  # or tf.uint8
# print("Flag 2")
tflite_model = converter.convert()
print("flag3")
# def representative_data_gen():
#   for input_value in tf.data.Dataset.from_tensor_slices(x_train).batch(1).take(100):
#     yield [input_value]
# 
# converter = tf.lite.TFLiteConverter.from_keras_model(model)
# converter.optimizations = [tf.lite.Optimize.DEFAULT]
# converter.representative_dataset = representative_data_gen
# # Ensure that if any ops can't be quantized, the converter throws an error
# converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
# # Set the input and output tensors to uint8 (APIs added in r2.3)
# converter.inference_input_type = tf.uint8
# converter.inference_output_type = tf.uint8
# 
# tflite_model = converter.convert()

# Save the model.
with open('model2.tflite', 'wb') as f:
  f.write(tflite_model)






# Function: Convert some hex value into an array for C programming
def hex_to_c_array(hex_data, var_name):

  c_str = ''
  # Create header guard
  c_str += f'#ifndef {var_name.upper()}_H\n'
  c_str += f'#define {var_name.upper()}_H\n\n'

  # Add array length at top of file
  c_str += f'\nunsigned int { var_name }_len = {(len(hex_data))};\n'

  # Declare C variable
  c_str += f'unsigned char {var_name}[] = {{'
  hex_array = []
  for i, val in enumerate(hex_data) :

    # Construct string from hex
    hex_str = format(val, '#04x')

    # Add formatting so each line stays within 80 characters
    if (i + 1) < len(hex_data):
      hex_str += ','
    if (i + 1) % 12 == 0:
      hex_str += '\n '
    hex_array.append(hex_str)

  # Add closing brace
  c_str += '\n ' + format(' '.join(hex_array)) + '\n};\n\n'

  # Close out header guard
  c_str += '#endif //' + var_name.upper() + '_H'

  return c_str

# Write TFLite model to a C source (or header) file
c_model_name = 'model_data'
with open(c_model_name+'2.h', 'w') as file:
  file.write(hex_to_c_array(tflite_model, c_model_name))

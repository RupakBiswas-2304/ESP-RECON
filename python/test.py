
import os
import cv2
import numpy as np
import matplotlib.pyplot as plt
import tensorflow as tf


mnist = tf.keras.datasets.mnist
(x_train, y_train), (x_test, y_test) = mnist.load_data()

# train_images = x_train.astype(np.float32) / 255.0
# test_images = x_test.astype(np.float32) / 255.0

x_train = tf.keras.utils.normalize(x_train, axis=1)
x_test = tf.keras.utils.normalize(x_test, axis=1)

model = tf.keras.models.load_model('model2.tflite')
loss, acc = model.evaluate(x_test, y_test)
print(f"Restored model, accuracy: {acc}% and loss: {loss}")

# predictions = model.predict(test_images[0])
# print(predictions)

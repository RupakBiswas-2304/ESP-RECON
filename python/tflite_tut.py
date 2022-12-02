
# Imports

import os
import numpy as np
import h5py
import matplotlib.pyplot as plt
import tensorflow as tf

from tensorflow import keras
from tensorflow.keras.layers import Flatten, Dense
from tensorflow.keras.losses import SparseCategoricalCrossentropy
# from sklearn.metrics import accuracy_score
from sys import getsizeof

# 

print(tf.__version__)

def get_file_size(file_path):
    size = os.path.getsize(file_path)
    return size

def convert_bytes(size, unit=None):
    if unit == 'KB':
        return print(f"File Size : {round(size/1024 , 3)} + Kilobytes.")
    elif unit == 'MB':
        return print(f"File Size : {round(size/1024 , 3)} + Kilobytes.")
    else:
        return print(f'File Size : {size} bytes.')


mnist = tf.keras.datasets.mnist
(train_image, train_labels), ( test_image, test_labels ) = mnist.load_data()


print(f"Train Image shape :  {train_image.shape} && Train label shape : {train_labels.shape} ")

# Showing some images
#  

# normalizing
train_image = train_image/255.0
test_image = test_image/255.0

model = keras.Sequential([
    Flatten((28,28)),
    Dense(128, activation = 'relu'),
    Dense(10)
])

print(model.summary())

model.compile(
    optimizer='adam',
    loss = SparseCategoricalCrossentropy,
    metrics = ['accuracy']
)

model.fit(train_image, train_labels, epoch = 10 )
model_name = 'modelV5.h5'
model.save(model_name)

convert_bytes(get_file_size(model_name), "MB")

test_loss, test_acc = model.evaluate(test_image, test_labels, verbose=2)


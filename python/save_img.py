import tensorflow as tf
import cv2
mnist = tf.keras.datasets.mnist
(x_train, y_train), (x_test, y_test) = mnist.load_data()

for x in x_train:
    cv2.imwrite('x_train/' + str(x_train.index(x)) + '.png', x)
for y in x_test:
    cv2.imwrite('x_text/' + str(x_test.index(y)) + '.png', y)


#!/usr/bin/env python3

"""Generate a binary file from a sample image of the MNIST dataset.
Pixel of the sample are stored as float32, images have size 8x8.
"""

import argparse
import struct

import matplotlib.pyplot as plt

from sklearn.model_selection import train_test_split
from sklearn import datasets

import serial

SERIAL_PORT = '/dev/ttyUSB0'
SERIAL_BAUDRATE = 19200


def main(args):
    digits = datasets.load_digits()

    rnd = 42
    _, data, _, _ = train_test_split(digits.data, digits.target, random_state=rnd)
    data = data[args.index]

    plt.gray()
    plt.imshow(data.reshape(8, 8))

    ser = serial.Serial(SERIAL_PORT, SERIAL_BAUDRATE)
    for pixel in data.astype('float32'):
        for byte_ in bytearray(struct.pack("f", pixel)):
            ser.write(byte_)
    ser.flush()
    ser.close()
    plt.show()


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--index", type=int, default=0,
                        help="Image index in MNIST test dataset")
    try:
        main(parser.parse_args())
    except Exception as e:
        print(f"Error: {e}")

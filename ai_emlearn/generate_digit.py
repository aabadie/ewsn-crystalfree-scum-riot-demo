#!/usr/bin/env python3

"""Generate a binary file from a sample image of the MNIST dataset.
Pixel of the sample are stored as float32, images have size 8x8.
"""

import argparse
import struct
import time

import matplotlib.pyplot as plt

from sklearn.model_selection import train_test_split
from sklearn import datasets

import serial

SERIAL_PORT = '/dev/ttyUSB0'
SERIAL_BAUDRATE = 19200
IMG_LINE_LENGTH = 32


def main(args):
    digits = datasets.load_digits()

    rnd = 42
    _, data, _, _ = train_test_split(digits.data, digits.target, random_state=rnd)
    data = data[args.index]
    # Store the image a buffer of 256 bytes
    img_bytes = bytearray()
    for pixel in data.astype('float'):
        for byte_ in bytearray(struct.pack("f", pixel)):
            img_bytes.append(byte_)

    # Display the selected digit
    plt.gray()
    plt.imshow(data.reshape(8, 8))
    plt.show()

    # Interact with the RIOT shell
    ser = serial.Serial(SERIAL_PORT, SERIAL_BAUDRATE)
    ser.write("clear\n".encode())
    time.sleep(0.1)
    for idx, line in enumerate(range(0, len(img_bytes), IMG_LINE_LENGTH)):
        bytes_str = "".join(f"{byte_:<02x}" for byte_ in img_bytes[line:line + IMG_LINE_LENGTH])
        command = f"load {idx} {bytes_str}\n"
        print(command, end="")
        ser.write(command.encode())
        time.sleep(0.1)
    ser.close()


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--index", type=int, default=0,
                        help="Image index in MNIST test dataset")
    try:
        main(parser.parse_args())
    except Exception as e:
        print(f"Error: {e}")

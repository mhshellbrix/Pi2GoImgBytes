# π2GoImgBytes

## Overview

`Pi2GoImgBytes` is a C program designed to read a PNG image file and convert its alpha channel data into a binary representation.
Each bit in the binary array corresponds to a pixel in the image, where the bit is set to `1` if the alpha value of the pixel is greater than 128,
and `0` otherwise. The program then outputs this binary data as hexadecimal values, which can be used to program the π2Go-Stick with MicroPython.
This conversion process ensures that the image data is efficiently encoded for use with the π2Go-Stick, facilitating easy and accurate image
representation in MicroPython applications.

## Requirements

- CMake 3.28 or higher
- C11 standard compiler
- libpng

## Installation

1. **Clone the repository:**

    ```sh
    git clone <repository_url>
    cd Pi2GoImgBytes
    ```

2. **Build the project using CMake:**

    ```sh
    mkdir build
    cd build
    cmake ..
    make
    ```

## Usage

Run the executable with the path to a PNG file as an argument:

```sh
./Pi2GoImgBytes <file.png>

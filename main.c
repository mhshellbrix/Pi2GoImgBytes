#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <string.h>

void read_png_file(const char* filename)
{
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        perror("File could not be opened for reading");
        exit(EXIT_FAILURE);
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png == NULL)
    {
        perror("png_create_read_struct failed");
        exit(EXIT_FAILURE);
    }

    png_infop info = png_create_info_struct(png);
    if (info == NULL)
    {
        perror("png_create_info_struct failed");
        png_destroy_read_struct(&png, NULL, NULL);
        exit(EXIT_FAILURE);
    }

    if (setjmp(png_jmpbuf(png)))
    {
        perror("Error during init_io");
        png_destroy_read_struct(&png, &info, NULL);
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    png_init_io(png, fp);
    png_read_info(png, info);

    const int width = png_get_image_width(png, info);
    const int height = png_get_image_height(png, info);
    const png_byte color_type = png_get_color_type(png, info);
    const png_byte bit_depth = png_get_bit_depth(png, info);

    png_read_update_info(png, info);

    if (setjmp(png_jmpbuf(png)))
    {
        perror("Error during read_image");
        png_destroy_read_struct(&png, &info, NULL);
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++)
    {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));
    }

    png_read_image(png, row_pointers);

    if (color_type != PNG_COLOR_TYPE_RGB_ALPHA)
    {
        fprintf(stderr, "Unsupported color type %d\n", color_type);
        png_destroy_read_struct(&png, &info, NULL);
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    const int number_of_channels = 4;
    const int pixel_size = (bit_depth / 8) * number_of_channels;

    const int byte_width = (width + 7) / 8; // Calculate the number of bytes per row
    unsigned char* bit_array = (unsigned char*)malloc(byte_width * height);
    memset(bit_array, 0, byte_width * height); // Initialize the array to 0

    for (int y = 0; y < height; y++)
    {
        const png_bytep row = row_pointers[y];
        for (int x = 0; x < width; x++)
        {
            const png_bytep px = &(row[x * pixel_size]);
            if (px[3] > 128) // Check the alpha value
            {
                bit_array[y * byte_width + (x / 8)] |= (1 << (7 - (x % 8))); // Set the corresponding bit
            }
        }
    }

    // Convert bit_array to HEX
    for (int i = 0; i < byte_width * height; i++)
    {
        printf("0x%02X", bit_array[i]);
        if ((i + 1) < byte_width * height)
        {
            printf(",");
        }
        if ((i + 1) % 16 == 0)
        {
            printf("\n");
        }
    }

    free(bit_array);

    for (int y = 0; y < height; y++)
    {
        free(row_pointers[y]);
    }
    free(row_pointers);

    png_destroy_read_struct(&png, &info, NULL);
    fclose(fp);
}

int main(const int argc, char** argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <file.png>\n", argv[0]);
        return EXIT_FAILURE;
    }

    read_png_file(argv[1]);

    return EXIT_SUCCESS;
}

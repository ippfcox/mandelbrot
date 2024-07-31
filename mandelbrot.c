#include <stdbool.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define PIC_HOR_MAX 2000
#define PIC_VER_MAX 1000
#define RATIO 200
#define X 0
#define Y 0

#define REPEAT 1000
#define ESCAPE 2.0f

int main(int argc, char *argv[])
{
    int pic_hor_max = PIC_HOR_MAX;
    int pic_ver_max = PIC_VER_MAX;
    int ratio = RATIO;
    double x = X;
    double y = Y;
    bool aux_line;

    if (argc != 7)
    {
        fprintf(stderr, "%s 2000 1000 200 0 0 0\n", argv[0]);
        exit(0);
    }

    pic_hor_max = atoi(argv[1]);
    pic_ver_max = atoi(argv[2]);
    ratio = atoi(argv[3]);
    x = atof(argv[4]);
    y = atof(argv[5]);
    aux_line = atoi(argv[6]);

    double val_hor_min = x - (double)pic_hor_max / 2 / ratio;
    double val_hor_max = x + (double)pic_hor_max / 2 / ratio;
    double val_ver_min = y - (double)pic_ver_max / 2 / ratio;
    double val_ver_max = y + (double)pic_ver_max / 2 / ratio;

    fprintf(stderr, "(%lf, %lf) -> (%lf, %lf)\n", val_hor_min, val_ver_min, val_hor_max, val_ver_max);

    unsigned char *data = calloc(pic_hor_max * pic_ver_max * 3, 1);

    for (int i = 0; i < pic_ver_max; ++i) // imag
    {
        if (i % (pic_ver_max / 100) == 0)
            fprintf(stderr, "\r%.2f%%", (double)i * 100 / pic_ver_max);

        for (int j = 0; j < pic_hor_max; ++j) // real
        {
            double c_real = ((val_hor_max - val_hor_min) / pic_hor_max) * j + val_hor_min;
            double c_imag = ((val_ver_max - val_ver_min) / pic_ver_max) * i + val_ver_min;
            double z_real = 0.0, z_imag = 0.0;

            int iter_count = 0;
            do
            {
                double z_real_new = z_real * z_real - z_imag * z_imag + c_real;
                double z_imag_new = 2 * z_real * z_imag + c_imag;
                z_real = z_real_new;
                z_imag = z_imag_new;
                ++iter_count;
            }
            while (iter_count < REPEAT && z_real * z_real + z_imag * z_imag < ESCAPE * ESCAPE);

            if (iter_count < REPEAT)
            {
                data[(i * pic_hor_max + j) * 3 + 0] = iter_count % 256;
                data[(i * pic_hor_max + j) * 3 + 1] = (iter_count * 2) % 256;
                data[(i * pic_hor_max + j) * 3 + 2] = (iter_count * 3) % 256;
            }
            else
            {
                data[(i * pic_hor_max + j) * 3 + 0] = 0;
                data[(i * pic_hor_max + j) * 3 + 1] = 0;
                data[(i * pic_hor_max + j) * 3 + 2] = 0;
            }

            if (aux_line && (i == pic_ver_max / 2 || j == pic_hor_max / 2))
            {
                data[(i * pic_hor_max + j) * 3 + 0] = 255;
                data[(i * pic_hor_max + j) * 3 + 1] = 255;
                data[(i * pic_hor_max + j) * 3 + 2] = 255;
            }
        }
    }

    stbi_write_png("mandelbrot.png", pic_hor_max, pic_ver_max, 3, data, pic_hor_max * 3);
}

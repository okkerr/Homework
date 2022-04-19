#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{   
    if (x >= im.w)
    {
        x = im.w  - 1;
    }
    if (y >= im.h)
    {
        y = im.h - 1;
    }
    if (c >= im.c)
    {
        c = im.c - 1;
    }
    return im.data[(im.w*y) + x + (im.w*im.h*c)];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    if (x >= im.w)
    {
        return;
    }
    if (y >= im.h)
    {
        return;
    }
    if (c >= im.c)
    {
        return;
    }
    im.data[(im.w*y) + x + (im.w*im.h*c)] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    memcpy( copy.data, im.data, sizeof(float)*im.w*im.h*im.c );
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    
    for (int i = 0; i < im.w; i++)
    {
        for (int j = 0; j < im.h; j++)
        {
            float new = 0.299*get_pixel(im, i, j, 0)+0.587*get_pixel(im, i, j, 1)+0.114*get_pixel(im, i, j, 2);
            set_pixel(gray, i, j, 0, new);
        }
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    for (int i = 0; i < im.w; i++)
    {
        for (int j = 0; j < im.h; j++)
        {
            float new = get_pixel(im, i, j, c)+v;
            set_pixel(im, i, j, c, new);
        }
    }
    return;

}

void clamp_image(image im)
{
    for (int i = 0; i < im.w; i++)
    {
        for (int j = 0; j < im.h; j++)
        {
            for (int k = 0; k < im.c; k++)
            {
                float pix = get_pixel(im, i, j, k);
                if (pix > 1)
                {
                    set_pixel(im, i, j, k, 1);
                }
                if (pix < 0)
                {
                    set_pixel(im, i, j, k, 0);
                }
            }
        }
    }
    return;
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    for (int i = 0; i < im.w; i++)
    {
        for (int j = 0; j < im.h; j++)
        {
            float R = get_pixel(im, i, j, 0);
            float G = get_pixel(im, i, j, 1);
            float B = get_pixel(im, i, j, 2);

            float hue;
            float saturation;
            float value = three_way_max(R, G, B);

            float min = three_way_min(R, G, B);
            
            float C = value - min;

            if (C == 0)
            {
                saturation = 0;
                hue = 0;
                set_pixel(im, i, j, 0, hue);
                set_pixel(im, i, j, 1, saturation);
                set_pixel(im, i, j, 2, value);
                continue;
            }
            
            saturation = C/value;

            if (value == R)
            {
                hue = (G-B)/C;
            }
            else if (value == G)
            {
                hue = ((B-R)/C) + 2;
            }
            else if (value == B)
            {
                hue = ((R-G)/C) + 4;
            }

            if (hue < 0)
            {
                hue = (hue/6) + 1;
            }
            else
            {
                hue = hue/6;
            }
            set_pixel(im, i, j, 0, hue);
            set_pixel(im, i, j, 1, saturation);
            set_pixel(im, i, j, 2, value);
        }
    }
    return;
}

void hsv_to_rgb(image im)
{
    for (int i = 0; i < im.w; i++)
    {
        for (int j = 0; j < im.h; j++)
        {
            float H = get_pixel(im, i, j, 0)*6;
            float S = get_pixel(im, i, j, 1);
            float V = get_pixel(im, i, j, 2);

            float R;
            float G;
            float B;

            float Hi = floor(H);
            float F = H - Hi;
            float P = V*(1-S);
            float Q = V*(1-(F*S));
            float T = V*(1-(1-F)*S);

            if (Hi == 0)
            {
                R = V;
                G = T;
                B = P;
            }
            else if (Hi == 1)
            {
                R = Q;
                G = V;
                B = P;
            }
            else if (Hi == 2)
            {
                R = P;
                G = V;
                B = T;
            }
            else if (Hi == 3)
            {
                R = P;
                G = Q;
                B = V;
            }
            else if (Hi == 4)
            {
                R = T;
                G = P;
                B = V;
            }
            else if (Hi == 5)
            {
                R = V;
                G = P;
                B = Q;
            }

            set_pixel(im, i, j, 0, R);
            set_pixel(im, i, j, 1, G);
            set_pixel(im, i, j, 2, B);
        }
    }

}


void scale_image(image im, int c, float v)
{
    for (int i = 0; i < im.w; i++)
    {
        for (int j = 0; j < im.h; j++)
        {
            float new = get_pixel(im, i, j, c)*v;
            set_pixel(im, i, j, c, new);
        }
    }
    return;

}

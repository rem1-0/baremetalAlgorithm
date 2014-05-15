/*****************************************************************************
 * example2.c : Edge detection
 *****************************************************************************/

#include <memory.h>

#include "rgb_image.h"
#include "macros.h"

static void tr(const planar_rgb_image_t* in_image,
               planar_rgb_image_t* out_image,
               void* ptr);

CONFIG_START
PLANAR_RGB_HANDLE(tr);
INT_STAT("contour points detected");
CONFIG_END

int abs_max(int a, int b, int c, int d)
{
    int abs_a = (a < 0 ? -a : a);
    int abs_b = (b < 0 ? -b : b);
    int abs_c = (c < 0 ? -c : c);
    int abs_d = (d < 0 ? -d : d);

    if (abs_a > abs_b)
    {
        if (abs_c > abs_d)
        {
            if (abs_a > abs_c)
            {
                return abs_a;
            }
            else
            {
                return abs_c;
            }
        }
        else
        {
            if (abs_a > abs_d)
            {
                return abs_a;
            }
            else
            {
                return abs_d;
            }
        }
    }
    else
    {
        if (abs_c > abs_d)
        {
            if (abs_b > abs_c)
            {
                return abs_b;
            }
            else
            {
                return abs_c;
            }
        }
        else
        {
            if (abs_b > abs_d)
            {
                return abs_b;
            }
            else
            {
                return abs_d;
            }
        }
    }
}

int tri_max(int a, int b, int c)
{
    if (a > b)
    {
        if (a > c)
        {
            return a;
        }
        else
        {
            return c;
        }
    }
    else
    {
        if (b > c)
        {
            return b;
        }
        else
        {
            return c;
        }
    }
}

void tr(const planar_rgb_image_t* in_image,
        planar_rgb_image_t* out_image,
        void* ptr)
{
    const uint8_t thresh = 40;

    uint8_t* out = out_image->r_plane + (in_image->pitch + 1);

    const uint8_t* r_pos_in = in_image->r_plane;
    const uint8_t* g_pos_in = in_image->g_plane;
    const uint8_t* b_pos_in = in_image->b_plane;

    int counter = 0;

    const uint8_t* r_ul = in_image->r_plane;
    const uint8_t* r_u = in_image->r_plane + 1;
    const uint8_t* r_ur = in_image->r_plane + 2;
    const uint8_t* r_l = in_image->r_plane + in_image->pitch;
    const uint8_t* r_r = in_image->r_plane + (in_image->pitch + 2);
    const uint8_t* r_dl = in_image->r_plane + 2 * in_image->pitch;
    const uint8_t* r_d = in_image->r_plane + (2* in_image->pitch + 1);
    const uint8_t* r_dr = in_image->r_plane + 2 * (in_image->pitch + 1);

    const uint8_t* g_ul = in_image->g_plane;
    const uint8_t* g_u = in_image->g_plane + 1;
    const uint8_t* g_ur = in_image->g_plane + 2;
    const uint8_t* g_l = in_image->g_plane + in_image->pitch;
    const uint8_t* g_r = in_image->g_plane + (in_image->pitch + 2);
    const uint8_t* g_dl = in_image->g_plane + 2 * in_image->pitch;
    const uint8_t* g_d = in_image->g_plane + (2* in_image->pitch + 1);
    const uint8_t* g_dr = in_image->g_plane + 2 * (in_image->pitch + 1);

    const uint8_t* b_ul = in_image->b_plane;
    const uint8_t* b_u = in_image->b_plane + 1;
    const uint8_t* b_ur = in_image->b_plane + 2;
    const uint8_t* b_l = in_image->b_plane + in_image->pitch;
    const uint8_t* b_r = in_image->b_plane + (in_image->pitch + 2);
    const uint8_t* b_dl = in_image->b_plane + 2 * in_image->pitch;
    const uint8_t* b_d = in_image->b_plane + (2* in_image->pitch + 1);
    const uint8_t* b_dr = in_image->b_plane + 2 * (in_image->pitch + 1);

    const uint8_t* out_endline = out_image->r_plane + (in_image->pitch);

    while (r_dr < (in_image->r_plane + in_image->pitch * in_image->lines))
    {
        out_endline += in_image->pitch;

        while (out < out_endline)
        {
            int r_h_sum = *r_ul + *r_l + *r_l + *r_dl -
                          *r_ur - *r_r - *r_r - *r_dr;
            int r_v_sum = *r_ul + *r_u + *r_u + *r_ur -
                          *r_dl - *r_d - *r_d - *r_dr;
            int r_d1_sum = *r_l + *r_ul + *r_ul + *r_u -
                           *r_d - *r_dr - *r_dr - *r_r;
            int r_d2_sum = *r_l + *r_dl + *r_dl + *r_d -
                           *r_r - *r_ur - *r_ur - *r_u;

            int g_h_sum = *g_ul + *g_l + *g_l + *g_dl -
                          *g_ur - *g_r - *g_r - *g_dr;
            int g_v_sum = *g_ul + *g_u + *g_u + *g_ur -
                          *g_dl - *g_d - *g_d - *g_dr;
            int g_d1_sum = *g_l + *g_ul + *g_ul + *g_u -
                           *g_d - *g_dr - *g_dr - *g_r;
            int g_d2_sum = *g_l + *g_dl + *g_dl + *g_d -
                           *g_r - *g_ur - *g_ur - *g_u;

            int b_h_sum = *b_ul + *b_l + *b_l + *b_dl -
                          *b_ur - *b_r - *b_r - *b_dr;
            int b_v_sum = *b_ul + *b_u + *b_u + *b_ur -
                          *b_dl - *b_d - *b_d - *b_dr;
            int b_d1_sum = *b_l + *b_ul + *b_ul + *b_u -
                           *b_d - *b_dr - *b_dr - *b_r;
            int b_d2_sum = *b_l + *b_dl + *b_dl + *b_d -
                           *b_r - *b_ur - *b_ur - *b_u;

            int r_max = abs_max(r_h_sum, r_v_sum, r_d1_sum, r_d2_sum) / 4;
            int g_max = abs_max(g_h_sum, g_v_sum, g_d1_sum, g_d2_sum) / 4;
            int b_max = abs_max(b_h_sum, b_v_sum, b_d1_sum, b_d2_sum) / 4;

            *out = tri_max(r_max, g_max, b_max);

            if (*out > thresh)
            {
                *out = *out > 127 ? 255 : 2*(*out);
                ++counter;
            }
            else
            {
                *out = 0;
            }

            ++r_ul;
            ++r_u;
            ++r_ur;
            ++r_l;
            ++r_r;
            ++r_dl;
            ++r_d;
            ++r_dr;

            ++g_ul;
            ++g_u;
            ++g_ur;
            ++g_l;
            ++g_r;
            ++g_dl;
            ++g_d;
            ++g_dr;

            ++b_ul;
            ++b_u;
            ++b_ur;
            ++b_l;
            ++b_r;
            ++b_dl;
            ++b_d;
            ++b_dr;

            ++out;
        }

        r_ul += 2;
        r_u += 2;
        r_ur += 2;
        r_l += 2;
        r_r += 2;
        r_dl += 2;
        r_d += 2;
        r_dr += 2;

        g_ul += 2;
        g_u += 2;
        g_ur += 2;
        g_l += 2;
        g_r += 2;
        g_dl += 2;
        g_d += 2;
        g_dr += 2;

        b_ul += 2;
        b_u += 2;
        b_ur += 2;
        b_l += 2;
        b_r += 2;
        b_dl += 2;
        b_d += 2;
        b_dr += 2;

        out += 2;
    }

    SET_INT_STAT("contour points detected", counter);

    memcpy(out_image->g_plane, out_image->r_plane,
           out_image->lines * out_image->pitch * sizeof(uint8_t));
    memcpy(out_image->b_plane, out_image->r_plane,
           out_image->lines * out_image->pitch * sizeof(uint8_t));
}

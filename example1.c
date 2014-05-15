/*****************************************************************************
 * example1.c : Permutation of the components red, blue and green
 *****************************************************************************/

#include <memory.h>

#include "macros.h"
#include "rgb_image.h"

static void tr(const planar_rgb_image_t* in_image,
               planar_rgb_image_t* out_image,
               void* ptr);

CONFIG_START
    PLANAR_RGB_HANDLE(tr);
CONFIG_END

static void tr(const planar_rgb_image_t* in_image,
               planar_rgb_image_t* out_image,
               void* ptr)
{
    memcpy(out_image->r_plane, in_image->b_plane,
           in_image->pitch*in_image->lines);
    memcpy(out_image->g_plane, in_image->r_plane,
           in_image->pitch*in_image->lines);
    memcpy(out_image->b_plane, in_image->g_plane,
           in_image->pitch*in_image->lines);
}

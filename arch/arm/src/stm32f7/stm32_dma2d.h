
#ifndef __ARCH_ARM_SRC_STM32F7_STM32_DMA2D_H
#define __ARCH_ARM_SRC_STM32F7_STM32_DMA2D_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>
#include <ecr/video/fb.h>
#include "stm32_ltdc.h"

#ifdef CONFIG_STM32F7_DMA2D

/****************************************************************************
 * Public Types
 ****************************************************************************/

struct ltdc_area_s; /* see arch/arm/src/stm32f7/stm32_ltdc.h */

/* Blend mode definitions */

enum dma2d_blend_e
{
  DMA2D_BLEND_NONE           = 0,   /* Disable all blend operation */
  DMA2D_BLEND_ALPHA          = 0x1, /* Enable alpha blending */
  DMA2D_BLEND_PIXELALPHA     = 0x2, /* Enable alpha blending from pixel color */
};

/* The layer is controlled through the following structure */

struct dma2d_layer_s
{
  /* Name: getvideoinfo
   *
   * Description:
   *   Get video information about the layer
   *
   * Parameter:
   *   layer  - Reference to the layer control structure
   *   vinfo  - Reference to the video info structure
   *
   * Returned Value:
   *   On success - OK
   *   On error   - -EINVAL
   */

  int (*getvideoinfo)(FAR struct dma2d_layer_s *layer,
                    FAR struct fb_videoinfo_s *vinfo);

  /* Name: getplaneinfo
   *
   * Description:
   *   Get plane information about the layer
   *
   * Parameter:
   *   layer   - Reference to the layer control structure
   *   planeno - Number of the plane
   *   pinfo   - Reference to the plane info structure
   *
   * Returned Value:
   *   On success - OK
   *   On error   - -EINVAL
   */

  int (*getplaneinfo)(FAR struct dma2d_layer_s *layer, int planeno,
                      FAR struct fb_planeinfo_s *pinfo);

  /* Name: getlid
   *
   * Description:
   *   Get a specific layer identifier.
   *
   * Parameter:
   *   layer - Reference to the layer structure
   *   lid   - Reference to store the layer id
   *
   * Returned Value:
   *   On success - OK
   *   On error   - -EINVAL
   */

  int (*getlid)(FAR struct dma2d_layer_s *layer, int *lid);

#ifdef CONFIG_STM32F7_DMA2D_L8
  /* Name: setclut
   *
   * Description:
   *   Configure layer clut (color lookup table).
   *   Non clut is defined during initializing.
   *
   * Parameter:
   *   layer  - Reference to the layer structure
   *   cmap   - color lookup table with up the 256 entries
   *
   * Returned Value:
   *   On success - OK
   *   On error   - -EINVAL
   */

  int (*setclut)(FAR struct dma2d_layer_s *layer,
                    const FAR struct fb_cmap_s *cmap);

  /* Name: getclut
   *
   * Description:
   *   Get configured layer clut (color lookup table).
   *
   * Parameter:
   *   layer - Reference to the layer structure
   *   cmap  - Reference to valid color lookup table accept up the 256 color
   *           entries
   *
   * Returned Value:
   *   On success - OK
   *   On error   - -EINVAL
   */

  int (*getclut)(FAR struct dma2d_layer_s *layer, FAR struct fb_cmap_s *cmap);
#endif

  /* Name: setalpha
   *
   * Description:
   *   Configure layer alpha value factor into blend operation.
   *   During the layer blend operation the source alpha value is multiplied
   *   with this alpha value. If the source color format doesn't support alpha
   *   channel (e.g. non ARGB8888) this alpha value will be used as constant
   *   alpha value for blend operation.
   *   Default value during initializing: 0xff
   *
   * Parameter:
   *   layer - Reference to the layer structure
   *   alpha - Alpha value
   *
   * Returned Value:
   *   On success - OK
   *   On error - -EINVAL
   */

  int (*setalpha)(FAR struct dma2d_layer_s *layer, uint8_t alpha);

  /* Name: getalpha
   *
   * Description:
   *   Get configured layer alpha value factor for blend operation.
   *
   * Parameter:
   *   layer - Reference to the layer structure
   *   alpha - Reference to store the alpha value
   *
   * Returned Value:
   *   On success - OK
   *   On error - -EINVAL
   */

  int (*getalpha)(FAR struct dma2d_layer_s *layer, uint8_t *alpha);

  /* Name: setblendmode
   *
   * Description:
   *   Configure blend mode of the layer.
   *   Default mode during initializing: DMA2D_BLEND_NONE
   *   Blendmode is active after next update.
   *
   * Parameter:
   *   layer - Reference to the layer structure
   *   mode  - Blend mode (see DMA2D_BLEND_*)
   *
   * Returned Value:
   *   On success - OK
   *   On error - -EINVAL
   *
   * Procedure information:
   *   DMA2D_BLEND_NONE:
   *     Informs the driver to disable all blend operation for the given layer.
   *     That means the layer is opaque.
   *
   *   DMA2D_BLEND_ALPHA:
   *     Informs the driver to enable alpha blending for the given layer.
   *
   *   DMA2D_BLEND_PIXELALPHA:
   *     Informs the driver to use the pixel alpha value of the layer instead
   *     the constant alpha value. This is only useful for ARGB8888
   *     color format.
   */

  int (*setblendmode)(FAR struct dma2d_layer_s *layer, uint32_t mode);

  /* Name: getblendmode
   *
   * Description:
   *   Get configured blend mode of the layer.
   *
   * Parameter:
   *   layer - Reference to the layer structure
   *   mode  - Reference to store the blend mode
   *
   * Returned Value:
   *   On success - OK
   *   On error - -EINVAL
   */

  int (*getblendmode)(FAR struct dma2d_layer_s *layer, uint32_t *mode);

  /* Name: blit
   *
   * Description:
   *   Copy selected area from a source layer to selected position of the
   *   destination layer.
   *
   * Parameter:
   *   dest     - Reference to the destination layer
   *   destxpos - Selected x target position of the destination layer
   *   destypos - Selected y target position of the destination layer
   *   src      - Reference to the source layer
   *   srcarea  - Reference to the selected area of the source layer
   *
   * Returned Value:
   *    OK        - On success
   *   -EINVAL    - If one of the parameter invalid or if the size of the
   *                selected source area outside the visible area of the
   *                destination layer. (The visible area usually represents the
   *                display size)
   *   -ECANCELED - Operation cancelled, something goes wrong.
   */

   int (*blit)(FAR struct dma2d_layer_s *dest,
                fb_coord_t destxpos, fb_coord_t destypos,
                FAR const struct dma2d_layer_s *src,
                FAR const struct ltdc_area_s *srcarea);

  /* Name: blend
   *
   * Description:
   *   Blends the selected area from a background layer with selected position
   *   of the foreground layer. Copies the result to the selected position of
   *   the destination layer. Note! The content of the foreground and background
   *   layer keeps unchanged as long destination layer is unequal to the
   *   foreground and background layer.
   *
   * Parameter:
   *   dest     - Reference to the destination layer
   *   fore     - Reference to the foreground layer
   *   forexpos - Selected x target position of the foreground layer
   *   foreypos - Selected y target position of the foreground layer
   *   back     - Reference to the background layer
   *   backarea - Reference to the selected area of the background layer
   *
   * Returned Value:
   *    OK        - On success
   *   -EINVAL    - If one of the parameter invalid or if the size of the
   *                selected source area outside the visible area of the
   *                destination layer. (The visible area usually represents the
   *                display size)
   *   -ECANCELED - Operation cancelled, something goes wrong.
   */

   int (*blend)(FAR struct dma2d_layer_s *dest,
                fb_coord_t destxpos, fb_coord_t destypos,
                FAR const struct dma2d_layer_s *fore,
                fb_coord_t forexpos, fb_coord_t foreypos,
                FAR const struct dma2d_layer_s *back,
                FAR const struct ltdc_area_s *backarea);

  /* Name: fillarea
   *
   * Description:
   *   Fill the selected area of the whole layer with a specific color.
   *
   * Parameter:
   *   layer    - Reference to the layer structure
   *   area     - Reference to the valid area structure select the area
   *   color    - Color to fill the selected area. Color must be formatted
   *              according to the layer pixel format.
   *
   * Returned Value:
   *    OK        - On success
   *   -EINVAL    - If one of the parameter invalid or if the size of the
   *                selected area outside the visible area of the layer.
   *   -ECANCELED - Operation cancelled, something goes wrong.
   */

   int (*fillarea)(FAR struct dma2d_layer_s *layer,
                    FAR const struct ltdc_area_s *area,
                    uint32_t color);
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

struct stm32_ltdc_s; /* Forward declaration */

/****************************************************************************
 * Name: stm32_dma2dinitltdc
 *
 * Description:
 *   Get a reference to the dma2d layer coupled with the ltdc layer.
 *   It not intends to use this function by user space applications.
 *   It resolves the following requirements:
 *   1. Share the color lookup table
 *   2. Share the planeinfo information
 *   3. Share the videoinfo information
 *
 * Input Parameters:
 *   layer  - a valid reference to the low level ltdc layer structure
 *
 * Returned Value:
 *   On success - A valid dma2d layer reference
 *   On error   - NULL and errno is set to
 *                -EINVAL if one of the parameter is invalid
 *
 ****************************************************************************/

FAR struct dma2d_layer_s *stm32_dma2dinitltdc(FAR struct stm32_ltdc_s *layer);

/****************************************************************************
 * Name: up_dma2dgetlayer
 *
 * Description:
 *   Get a dma2d layer structure by the layer identifier
 *
 * Input Parameters:
 *   lid - Layer identifier
 *
 * Returned Value:
 *   Reference to the dma2d layer control structure on success or Null if no
 *   related exist.
 *
 ****************************************************************************/

FAR struct dma2d_layer_s *up_dma2dgetlayer(int lid);

/****************************************************************************
 * Name: up_dma2dcreatelayer
 *
 * Description:
 *   Create a new dma2d layer object to interact with the dma2d controller
 *
 * Input Parameters:
 *   width  - Layer width
 *   height - Layer height
 *   fmt    - Pixel format of the layer
 *
 * Returned Value:
 *   On success - A valid dma2d layer reference
 *   On error   - NULL and errno is set to
 *                -EINVAL if one of the parameter is invalid
 *                -ENOMEM if no memory available or exceeds
 *                 CONFIG_STM32F7_DMA2D_NLAYERS
 *
 ****************************************************************************/

FAR struct dma2d_layer_s *up_dma2dcreatelayer(fb_coord_t width,
                                              fb_coord_t height,
                                              uint8_t fmt);

/****************************************************************************
 * Name: up_dma2dremovelayer
 *
 * Description:
 *  Remove and deallocate the dma2d layer
 *
 * Input Parameters:
 *   layer  - Reference to the layer to remove
 *
 * Returned Value:
 *   On success - OK
 *   On error   - -EINVAL
 *
 ****************************************************************************/

int up_dma2dremovelayer(FAR struct dma2d_layer_s *layer);

/****************************************************************************
 * Name: up_dma2dinitialize
 *
 * Description:
 *   Initialize the dma2d controller
 *
 * Returned Value:
 *   OK - On success
 *   An error if initializing failed.
 *
 ****************************************************************************/

int up_dma2dinitialize(void);

/****************************************************************************
 * Name: up_dma2duninitialize
 *
 * Description:
 *   Uninitialize the dma2d controller
 *
 ****************************************************************************/

void up_dma2duninitialize(void);

#endif /* CONFIG_STM32F7_DMA2D */
#endif /* __ARCH_ARM_SRC_STM32F7_STM32_DMA2D_H */

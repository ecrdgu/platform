
#ifndef __ARCH_ARM_SRC_STM32_STM32_LTDC_H
#define __ARCH_ARM_SRC_STM32_STM32_LTDC_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <ecr/config.h>

#include <stdbool.h>
#include <semaphore.h>

#include <ecr/video/fb.h>
#include <ecr/nx/nxglib.h>

#ifdef CONFIG_STM32_LTDC

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/************************************************************************************
 * Public Types
 ************************************************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/

struct dma2d_layer_s; /* see arch/arm/src/stm32/stm32_dma2d.h */

/* Blend mode definitions */

enum ltdc_blend_e
{
  LTDC_BLEND_NONE           = 0,   /* Disable all blend operation */
  LTDC_BLEND_ALPHA          = 0x1, /* Enable alpha blending */
  LTDC_BLEND_PIXELALPHA     = 0x2, /* Enable alpha blending from pixel color */
  LTDC_BLEND_COLORKEY       = 0x4, /* Enable colorkey */
  LTDC_BLEND_ALPHAINV       = 0x8, /* Inverse alpha blending of source */
  LTDC_BLEND_PIXELALPHAINV  = 0x10 /* Invers pixel alpha blending of source */
};

/* layer control definitions */

enum ltdc_layer_e
{
  LTDC_LAYER_OWN            = 0,   /* The given layer */
  LTDC_LAYER_TOP            = 0x1, /* The initialized top layer */
  LTDC_LAYER_BOTTOM         = 0x2, /* the initialized bottom layer */
  LTDC_LAYER_ACTIVE         = 0x4, /* The current visible flip layer */
  LTDC_LAYER_INACTIVE       = 0x8  /* The current invisible flip layer */
#ifdef CONFIG_STM32_DMA2D
 ,LTDC_LAYER_DMA2D          = 0x10  /* The dma2d interface layer id */
#endif
};

/* Update operation flag */

enum ltdc_update_e
{
  LTDC_UPDATE_NONE          = 0,   /* Update given layer only */
  LTDC_UPDATE_SIM           = 0x1, /* Update both layer simultaneous */
  LTDC_UPDATE_FLIP          = 0x2, /* Perform flip operation */
  LTDC_UPDATE_ACTIVATE      = 0x4  /* Set the given layer to the active layer */
};

/* sync mode definitions */

enum ltdc_sync_e
{
  LTDC_SYNC_NONE            = 0,     /* Immediately */
  LTDC_SYNC_VBLANK          = 0x100, /* Upon vertical sync */
  LTDC_SYNC_WAIT            = 0x200  /* Waits upon vertical sync */
};

/* Definition of the visible layer position and size */

struct ltdc_area_s
{
  fb_coord_t xpos; /* X position in pixel */
  fb_coord_t ypos; /* Y position in pixel */
  fb_coord_t xres; /* X resolution in pixel */
  fb_coord_t yres; /* Y resolution in pixel */
};

/* The layer is controlled through the following structure */

struct ltdc_layer_s
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

  int (*getvideoinfo)(FAR struct ltdc_layer_s *layer,
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

  int (*getplaneinfo)(FAR struct ltdc_layer_s *layer, int planeno,
                      FAR struct fb_planeinfo_s *pinfo);

  /* Name: getlid
   *
   * Description:
   *   Get a specific layer identifier.
   *
   * Parameter:
   *   layer - Reference to the layer structure
   *   lid   - Reference to store the layer id
   *   flag  - Operation flag describe the layer identifier
   *           e.g. get the current active or inactive layer.
   *           See LTDC_LAYER_* for possible values
   *
   * Returned Value:
   *   On success - OK
   *   On error   - -EINVAL
   */

  int (*getlid)(FAR struct ltdc_layer_s *layer, int *lid, uint32_t flag);

#ifdef CONFIG_FB_CMAP
  /* Name: setclut
   *
   * Description:
   *   Configure layer clut (color lookup table).
   *   Non clut is defined during initializing.
   *   Clut is active during next vertical blank period. Do not need an update.
   *
   * Parameter:
   *   layer  - Reference to the layer structure
   *   cmap   - color lookup table with up the 256 entries
   *   enable - Enable or disable clut support (if false cmap is ignored and can
   *            be NULL)
   *
   * Returned Value:
   *   On success - OK
   *   On error   - -EINVAL
   */

  int (*setclut)(FAR struct ltdc_layer_s *layer,
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

  int (*getclut)(FAR struct ltdc_layer_s *layer, FAR struct fb_cmap_s *cmap);
#endif

  /* Name: setcolor
   *
   * Description:
   *   Configure layer color for the non active layer area.
   *   Default value during initializing: 0x00000000
   *   Color is active after next update.
   *
   * Parameter:
   *   layer - Reference to the layer structure
   *   argb  - ARGB8888 color value
   *
   * Returned Value:
   *   On success - OK
   *   On error   - -EINVAL
   */

  int (*setcolor)(FAR struct ltdc_layer_s *layer, uint32_t argb);

  /* Name: getcolor
   *
   * Description:
   *   Get configured layer color for the non active layer area.
   *
   * Parameter:
   *   layer - Reference to the layer structure
   *   argb  - Reference to store the ARGB8888 color value
   *
   * Returned Value:
   *   On success - OK
   *   On error   - -EINVAL
   */

  int (*getcolor)(FAR struct ltdc_layer_s *layer, uint32_t *argb);

  /* Name: setcolorkey
   *
   * Description:
   *   Configure the layer color key (chromakey) for transparence.
   *   Default value during initializing: 0x00000000
   *   Colorkey is active after next update.
   *
   * Parameter:
   *   layer  - Reference to the layer structure
   *   rgb    - RGB888 color key
   *
   * Returned Value:
   *   On success - OK
   *   On error   - -EINVAL
   */

  int (*setcolorkey)(FAR struct ltdc_layer_s *layer, uint32_t rgb);

  /* Name: getcolorkey
   *
   * Description:
   *   Get the configured layer color key (chromakey) for transparence.
   *
   * Parameter:
   *   layer  - Reference to the layer structure
   *   rgb    - Reference to store the RGB888 color key
   *
   * Returned Value:
   *   On success - OK
   *   On error   - -EINVAL
   */

  int (*getcolorkey)(FAR struct ltdc_layer_s *layer, uint32_t *rgb);

  /* Name: setalpha
   *
   * Description:
   *   Configure layer alpha value factor into blend operation.
   *   During the layer blend operation the source alpha value is multiplied
   *   with this alpha value. If the source color format doesn't support alpha
   *   channel (e.g. non ARGB8888) this alpha value will be used as constant
   *   alpha value for blend operation.
   *   Default value during initializing: 0xff
   *   Alpha is active after next update.
   *
   * Parameter:
   *   layer - Reference to the layer structure
   *   alpha - Alpha value
   *
   * Returned Value:
   *   On success - OK
   *   On error - -EINVAL
   */

  int (*setalpha)(FAR struct ltdc_layer_s *layer, uint8_t alpha);

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

  int (*getalpha)(FAR struct ltdc_layer_s *layer, uint8_t *alpha);

  /* Name: setblendmode
   *
   * Description:
   *   Configure blend mode of the layer.
   *   Default mode during initializing: LTDC_BLEND_NONE
   *   Blendmode is active after next update.
   *
   * Parameter:
   *   layer - Reference to the layer structure
   *   mode  - Blend mode (see LTDC_BLEND_*)
   *
   * Returned Value:
   *   On success - OK
   *   On error - -EINVAL
   *
   * Procedure information:
   *   LTDC_BLEND_NONE:
   *     Informs the driver to disable all blend operation for the given layer.
   *     That means the layer is opaque. Note this has no effect on the
   *     colorkey settings.
   *
   *   LTDC_BLEND_ALPHA:
   *     Informs the driver to enable alpha blending for the given layer.
   *
   *   LTDC_BLEND_COLORKEY:
   *     Informs the driver to enable colorkeying for the given layer.
   *
   *   LTDC_BLEND_SRCPIXELALPHA:
   *     Informs the driver to use the pixel alpha value of the layer instead
   *     the constant alpha value. This is only useful for ARGB8888
   *     color format.
   *
   *   LTDC_BLEND_DESTPIXELALPHA:
   *     Informs the driver to use the pixel alpha value of the subjacent layer
   *     instead the constant alpha value. This is only useful for ARGB8888
   *     color format.
   */

  int (*setblendmode)(FAR struct ltdc_layer_s *layer, uint32_t mode);

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

  int (*getblendmode)(FAR struct ltdc_layer_s *layer, uint32_t *mode);

  /* Name: setarea
   *
   * Description:
   *    Configure visible layer area and the reference position of the first
   *    pixel of the whole layer which is the first visible top left pixel in
   *    the active area.
   *    Default value during initializing:
   *      xpos = 0
   *      ypos = 0
   *      xres = display x resolution
   *      yres = display y resolution
   *
   *   Area is active after next update.
   *
   * Parameter:
   *   layer   - Reference to the layer control structure
   *   area    - Reference to the valid area structure for the new active area
   *   srcxpos - x position of the visible pixel of the whole layer
   *   srcypos - y position of the visible pixel of the whole layer
   *
   * Returned Value:
   *   On success - OK
   *   On error - -EINVAL
   *
   * Procedure Information:
   *   If the srcxpos and srcypos unequal the xpos and ypos of the coord
   *   structure this acts like moving the visible area to another position on
   *   the screen during the next update operation.
   */

  int (*setarea)(FAR struct ltdc_layer_s *layer,
                  FAR const struct ltdc_area_s *area,
                  fb_coord_t srcxpos,
                  fb_coord_t srcypos);

  /* Name: getarea
   *
   * Description:
   *    Get configured visible layer area.
   *
   * Parameter:
   *   layer   - Reference to the layer control structure
   *   area    - Reference to the area structure to store the active area
   *   srcxpos - Reference to store the referenced x position of the whole layer
   *   srcypos - Reference to store the reterenced y position of the whole layer
   *
   * Returned Value:
   *   On success - OK
   *   On error - -EINVAL
   */

  int (*getarea)(FAR struct ltdc_layer_s *layer,
                  FAR struct ltdc_area_s *area,
                  fb_coord_t *srcxpos,
                  fb_coord_t *srcypos);

  /* Name: update
   *
   * Description:
   *   Update current layer settings and make changes visible.
   *
   * Parameter:
   *   layer   - Reference to the layer structure
   *   mode    - operation mode (see LTDC_UPDATE_*)
   *
   * Returned Value:
   *    OK        - On success
   *   -EINVAL    - If one of the parameter invalid
   *   -ECANCELED - Operation cancelled, something goes wrong
   *
   * Procedure information:
   *   LTDC_UPDATE_SIM:
   *     Informs the driver to update both ltdc layers simultaneously. Otherwise
   *     update the given layer only.
   *
   *   LTDC_UPDATE_FLIP:
   *     Informs the driver to perform a flip operation.
   *     This only effects the ltdc layer 1 and 2 and can be useful for double
   *     buffering. Each flip operation changed the active layer ot the inactive
   *     and vice versa. In the context of the ltdc that means, the inactive layer
   *     is complete disabled. So the subjacent layer is the background layer
   *     (background color). To reactivate both layer and their settings perform
   *     an update without LTDC_UPDATE_FLIP flag.
   *
   *   LTDC_UPDATE_ACTIVATE:
   *     Informs the driver that the given layer should be the active layer when
   *     the operation is complete.
   *
   *   LTDC_SYNC_VBLANK:
   *     Informs the driver to update the layer upon vertical blank. Otherwise
   *     immediately.
   */

  int (*update)(FAR struct ltdc_layer_s *layer, uint32_t mode);

#ifdef CONFIG_STM32_DMA2D
  /* Name: blit
   *
   * Description:
   *   Copy selected area from a source layer to selected position of the
   *   destination layer.
   *
   * Parameter:
   *   dest     - Reference to the destination layer
   *   destxpos - Selected x position of the destination layer
   *   destypos - Selected y position of the destination layer
   *   src      - Reference to the source layer
   *   srcarea  - Reference to the selected area of the source layer
   *
   * Returned Value:
   *    OK      - On success
   *   -EINVAL  - If one of the parameter invalid or if the size of the selected
   *              source area outside the visible area of the destination layer.
   *              (The visible area usually represents the display size)
   */

   int (*blit)(FAR struct ltdc_layer_s *dest,
                fb_coord_t destxpos, fb_coord_t destypos,
                FAR const struct dma2d_layer_s *src,
                FAR const struct ltdc_area_s *srcarea);

  /* Name: blend
   *
   * Description:
   *   Blends the selected area from a foreground layer with selected position
   *   of the background layer. Copy the result to the destination layer. Note!
   *   The content of the foreground and background layer is not changed.
   *
   * Parameter:
   *   dest     - Reference to the destination layer
   *   destxpos - Selected x position of the destination layer
   *   destypos - Selected y position of the destination layer
   *   fore     - Reference to the foreground layer
   *   forexpos - Selected x position of the foreground layer
   *   foreypos - Selected y position of the foreground layer
   *   back     - Reference to the background layer
   *   backarea - Reference to the selected area of the background layer
   *
   * Returned Value:
   *    OK      - On success
   *   -EINVAL  - If one of the parameter invalid or if the size of the selected
   *              source area outside the visible area of the destination layer.
   *              (The visible area usually represents the display size)
   */

  int (*blend)(FAR struct ltdc_layer_s *dest,
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
   *    OK      - On success
   *   -EINVAL  - If one of the parameter invalid or if the size of the selected
   *              area outside the visible area of the layer.
   */

  int (*fillarea)(FAR struct ltdc_layer_s *layer,
                   FAR const struct ltdc_area_s *area,
                   uint32_t color);
#endif
};

/* Common layer state structure for the LTDC and DMA2D controller */

struct stm32_ltdc_s
{
  /* Fixed settings */

  int lid;                      /* Layer identifier */
  struct fb_videoinfo_s vinfo;  /* Layer videoinfo */
  struct fb_planeinfo_s pinfo;  /* Layer planeinfo */

  /* Positioning */

  struct ltdc_area_s area;      /* Active layer area */
  fb_coord_t xpos;              /* Reference x position */
  fb_coord_t ypos;              /* Reference y position */

  /* Coloring */

  uint32_t color;               /* Layer color definition */
#ifdef CONFIG_FB_CMAP
  uint32_t *clut;               /* 32-bit aligned clut color table */
#endif

  /* Blending */

  uint8_t  alpha;               /* Layer constant alpha value */
  uint32_t colorkey;            /* Layer colorkey */
  uint32_t blendmode;           /* Layer blend factor */

  /* Operation */

  sem_t *lock;                  /* Ensure mutually exclusive access */
};

/************************************************************************************
 * Public Data
 ************************************************************************************/

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/* The STM32 LTDC driver uses the common framebuffer interfaces declared in
 * include/ecr/video/fb.h.
 */

/************************************************************************************
 * Name: stm32_ltdcinitialize
 *
 * Description:
 *   Initialize the ltdc controller
 *
 * Returned Value:
 *   OK
 *
 ************************************************************************************/

int stm32_ltdcinitialize(void);
FAR struct fb_vtable_s *stm32_ltdcgetvplane(int vplane);
void stm32_ltdcuninitialize(void);

/************************************************************************************
 * Name: stm32_ltdcgetlayer
 *
 * Description:
 *   Get the ltdc layer structure to perform hardware layer operation
 *
 * Input Parameters:
 *   lid - Layer identifier
 *
 * Returned Value:
 *   Reference to the layer control structure on success or Null if parameter
 *   invalid.
 *
 ************************************************************************************/

FAR struct ltdc_layer_s *stm32_ltdcgetlayer(int lid);

/************************************************************************************
 * Name: stm32_lcdclear
 *
 * Description:
 *   This is a non-standard LCD interface just for the STM32 LTDC. Clearing the
 *   display in the normal way by writing a sequences of runs that covers the
 *   entire display can be slow.  Here the display is cleared by simply setting
 *   all video memory to the specified color.
 *
 ************************************************************************************/

void stm32_lcdclear(nxgl_mxpixel_t color);

/************************************************************************************
 * Name: stm32_lcd_backlight
 *
 * Description:
 *   If CONFIG_STM32_LCD_BACKLIGHT is defined, then the board-specific logic must
 *   provide this interface to turn the backlight on and off.
 *
 ************************************************************************************/

#ifdef CONFIG_STM32_LCD_BACKLIGHT
void stm32_backlight(bool blon);
#endif

#endif /* CONFIG_STM32_LTDC */
#endif /* __ARCH_ARM_SRC_STM32_STM32_LTDC_H */

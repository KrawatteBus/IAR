/*
 * Additional helper routines for the ICLED FeatherWing driver.
 *
 * This file defines convenience functions which are not part of the
 * original driver implementation contained in icled.c.  By keeping this
 * implementation separate we avoid modifying the original file which
 * contains comments encoded in ISO‑8859‑1.
 */

#include "icled.h"

/*
 * @brief Set all LEDs on the ICLED panel to a single colour.
 *
 * This helper loops through the entire chain of LEDs (specified by
 * ICLED_LED_COUNT) and sends the same colour to each pixel.  After
 * transmitting all pixel data the function returns without issuing the
 * latch/reset pulse.  The caller is responsible for calling
 * ICLED_showReset() afterwards to update the display.
 *
 * @param[in] green  Green colour component (0–255)
 * @param[in] red    Red colour component (0–255)
 * @param[in] blue   Blue colour component (0–255)
 */
void ICLED_setAll(uint8_t green, uint8_t red, uint8_t blue)
{
    for (uint32_t i = 0; i < ICLED_LED_COUNT; ++i)
    {
        ICLED_setPixel(green, red, blue);
    }
}
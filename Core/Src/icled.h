/*
 * icled.h - Driver definitions for controlling the Würth Elektronik ICLED
 *           display on a Nucleo board.
 *
 * This header exposes a minimal API for sending color data to the ICLED
 * FeatherWing.  The functions are implemented in icled.c.  See the
 * corresponding source file for more details.
 */

#ifndef ICLED_H
#define ICLED_H

#include <stdint.h>

/*
 * Number of LEDs on the ICLED FeatherWing.  The display consists of 7 rows
 * and 15 columns (7 × 15 = 105 LEDs).  Adjust this value if you are using
 * a different sized ICLED matrix.
 */
#define ICLED_LED_COUNT 105U

/*
 * Initialise the Data Watchpoint and Trace (DWT) unit used for precise
 * timing and perform an initial reset on the data line.  This function must
 * be called before any other ICLED function.
 */
void ICLED_Init(void);

/*
 * Send a single pixel to the display.  The ICLED requires data in the
 * order green, red, blue (GRB).  Each colour component is an 8‑bit value.
 *
 * Passing values greater than 255 will wrap due to the uint8_t type.
 */
void ICLED_setPixel(uint8_t green, uint8_t red, uint8_t blue);

/*
 * Generate the >200 µs reset pulse required to latch previously sent pixel
 * data.  After calling this function the ICLED will update all LEDs with
 * the data sent since the last reset.  Failing to call this function will
 * result in no visible change on the display.
 */
void ICLED_showReset(void);

/*
 * Set all LEDs on the display to the same colour.  This helper simply
 * iterates ICLED_LED_COUNT times calling ICLED_setPixel() for the given
 * colour and then latches the data using ICLED_showReset().  Use this
 * function when you want to fill the entire panel with a solid colour.
 */
void ICLED_setAll(uint8_t green, uint8_t red, uint8_t blue);

/*
 * Optional: perform a simple self‑test of the DWT counter.  Toggles a
 * dedicated test GPIO (PA5) to indicate whether the counter is running.
 */
void ICLED_DWT_SelfTest(void);

#endif /* ICLED_H */
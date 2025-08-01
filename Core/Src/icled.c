#include "icled.h"
#include "stm32l4xx.h"

// Datenpin: PB10
#define ICLED_GPIO_PORT GPIOB
#define ICLED_GPIO_PIN  GPIO_PIN_10

// Feste Cycle-Werte bei 80 MHz SystemCoreClock
#define HIGH_CYCLES_0 24  // Bit0: high 300ns
#define LOW_CYCLES_0  72  // Bit0: low  900ns
#define HIGH_CYCLES_1 72  // Bit1: high 900ns
#define LOW_CYCLES_1  24  // Bit1: low  300ns

static void DWT_Delay_Init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    __DSB();
    __ISB();

    uint32_t before = DWT->CYCCNT;
    for (volatile int i = 0; i < 100; ++i) __NOP();
    if (DWT->CYCCNT == before) {
        // Fehler: Counter läuft nicht. Optional: Fehleranzeige über Testpin
        #ifdef TEST_GPIO_PORT
        HAL_GPIO_WritePin(TEST_GPIO_PORT, TEST_GPIO_PIN, GPIO_PIN_SET);
        HAL_Delay(100);
        HAL_GPIO_WritePin(TEST_GPIO_PORT, TEST_GPIO_PIN, GPIO_PIN_RESET);
        #endif
    }
}


static inline void delay_cycles(uint32_t cycles)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t timeout = cycles + 1000000U;
    while ((DWT->CYCCNT - start) < cycles)
    {
        if ((DWT->CYCCNT - start) > timeout) break;
        __NOP();
    }
}

static void sendBit(uint8_t bit)
{
    if (bit) {
        // Bit1: high 900ns, low 300ns
        HAL_GPIO_WritePin(ICLED_GPIO_PORT, ICLED_GPIO_PIN, GPIO_PIN_SET);
        delay_cycles(HIGH_CYCLES_1);
        HAL_GPIO_WritePin(ICLED_GPIO_PORT, ICLED_GPIO_PIN, GPIO_PIN_RESET);
        delay_cycles(LOW_CYCLES_1);
    } else {
        // Bit0: high 300ns, low 900ns
        HAL_GPIO_WritePin(ICLED_GPIO_PORT, ICLED_GPIO_PIN, GPIO_PIN_SET);
        delay_cycles(HIGH_CYCLES_0);
        HAL_GPIO_WritePin(ICLED_GPIO_PORT, ICLED_GPIO_PIN, GPIO_PIN_RESET);
        delay_cycles(LOW_CYCLES_0);
    }
}

void ICLED_showReset(void)
{
    // Reset: Low für >200µs => bei 80MHz  16.000 cycles
    HAL_GPIO_WritePin(ICLED_GPIO_PORT, ICLED_GPIO_PIN, GPIO_PIN_RESET);
    delay_cycles(16000);
}

void ICLED_setPixel(uint8_t green, uint8_t red, uint8_t blue)
{
    for (int8_t bit = 7; bit >= 0; --bit) sendBit((green >> bit) & 0x1);
    for (int8_t bit = 7; bit >= 0; --bit) sendBit((red >> bit) & 0x1);
    for (int8_t bit = 7; bit >= 0; --bit) sendBit((blue >> bit) & 0x1);
}

void ICLED_Init(void)
{
    DWT_Delay_Init();
    HAL_GPIO_WritePin(ICLED_GPIO_PORT, ICLED_GPIO_PIN, GPIO_PIN_RESET);
    ICLED_showReset();
}


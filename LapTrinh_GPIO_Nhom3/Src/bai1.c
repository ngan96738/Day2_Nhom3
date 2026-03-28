#include <stdint.h>
#include "stm32f401re_gpio.h"
#include "stm32f401re_rcc.h"

//Define Logic GPIO_PIN-
#define GPIO_PIN_SET        1
#define GPIO_PIN_RESET      0
#define GPIO_PIN_LOW        0
#define GPIO_PIN_HIGH       1

//Define GPIO PIN---
#define LED_GPIO_PORT       GPIOA
#define LED_GPIO_PIN        GPIO_Pin_5
#define LED_PINS            5
#define LEDControl_SetClock RCC_AHB1Periph_GPIOA

#define BUTTON_GPIO_PORT       GPIOC
#define BUTTON_GPIO_PIN        GPIO_Pin_13
#define BUTTON_PIN13           13
#define BUTTONControl_SetClock RCC_AHB1Periph_GPIOC


//Ham tao tre de chong rung nut an
void Delay(uint32_t time)
{
    for (uint32_t i = 0; i < time; i++);
}

static
void Led_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(LEDControl_SetClock, ENABLE);

    GPIO_InitStructure.GPIO_Pin = LED_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);
}

static
void Button_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(BUTTONControl_SetClock, ENABLE);
    GPIO_InitStructure.GPIO_Pin = BUTTON_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(BUTTON_GPIO_PORT, &GPIO_InitStructure);
}

static
void LedControl_SetStatus(GPIO_TypeDef *GPIOx, uint8_t GPIO_PIN, uint8_t Status)
{
    //Set bit in BSRR Registers-
    if (Status == GPIO_PIN_SET)
    {
        GPIOx->BSRRL = 1 << GPIO_PIN;
    }

    //Reset bit in BSRR Registers
    if (Status == GPIO_PIN_RESET)
    {
        GPIOx->BSRRH |= 1 << GPIO_PIN;
    }
}

static
uint8_t ButtonRead_Status(GPIO_TypeDef *GPIOx, uint32_t GPIO_PIN)
{
    uint32_t Read_Pin;

    Read_Pin = (GPIOx->IDR) >> GPIO_PIN;
    Read_Pin = Read_Pin & 0x01;

    return Read_Pin;
}

int main(void)
{
    uint8_t current_led_status = GPIO_PIN_RESET;

    Led_Init();
    Button_Init();

    LedControl_SetStatus(LED_GPIO_PORT, LED_PINS, GPIO_PIN_RESET);

    while(1)
    {
        if (ButtonRead_Status(BUTTON_GPIO_PORT, BUTTON_PIN13) == 0)
        {
            //Tao va su dung them ham Delay de chong rung nut an
            Delay(50000);

            if (ButtonRead_Status(BUTTON_GPIO_PORT, BUTTON_PIN13) == 0)
            {//Thuc hien thuat toan dao trang thai led khi duoc an nut
                if (current_led_status == GPIO_PIN_RESET)
                {
                    current_led_status = GPIO_PIN_SET;
                    LedControl_SetStatus(LED_GPIO_PORT, LED_PINS, GPIO_PIN_SET);
                }
                else
                {
                    current_led_status = GPIO_PIN_RESET;
                    LedControl_SetStatus(LED_GPIO_PORT, LED_PINS, GPIO_PIN_RESET);
                }

                while (ButtonRead_Status(BUTTON_GPIO_PORT, BUTTON_PIN13) == 0);
                Delay(50000);
            }
        }
    }
}

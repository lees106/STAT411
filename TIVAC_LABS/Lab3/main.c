#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

uint8_t ui8PinData1=2; //red
uint8_t ui8PinData2=4; //blue
uint8_t ui8PinData3=8; //green

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    while(1)
    {
        //Red
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, ui8PinData1); //Red
        SysCtlDelay(6666666);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);
        SysCtlDelay(6666666);

        //Green
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, ui8PinData3); //Green
        SysCtlDelay(6666666);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00);
        SysCtlDelay(6666666);

        //Blue
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, ui8PinData2); //Blue
        SysCtlDelay(6666666);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x00);
        SysCtlDelay(6666666);

        //Red Green
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, ui8PinData1); //Red
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, ui8PinData3); //Green
        SysCtlDelay(6666666);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00);
        SysCtlDelay(6666666);

        //Red Blue
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, ui8PinData1); //Red
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, ui8PinData2); //Blue
        SysCtlDelay(6666666);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x00);
        SysCtlDelay(6666666);

        //Green Blue
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, ui8PinData3); //Green
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, ui8PinData2); //Blue
        SysCtlDelay(6666666);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x00);
        SysCtlDelay(6666666);

        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, ui8PinData1);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, ui8PinData2);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, ui8PinData3);
        SysCtlDelay(6666666);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x00);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00);
        SysCtlDelay(6666666);
    }
}


//red
//blue
//green

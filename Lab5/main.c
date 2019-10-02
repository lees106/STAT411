#include <stdint.h>
#include <stdbool.h>
#include "\ti\tivaware_c_series_2_1_4_178\inc\tm4c123gh6pm.h"
#include "\ti\tivaware_c_series_2_1_4_178\inc\hw_memmap.h"
#include "\ti\tivaware_c_series_2_1_4_178\inc\hw_types.h"
#include "\ti\tivaware_c_series_2_1_4_178\driverlib\sysctl.h"
#include "\ti\tivaware_c_series_2_1_4_178\driverlib\interrupt.h"
#include "\ti\tivaware_c_series_2_1_4_178\driverlib\gpio.h"
#include "\ti\tivaware_c_series_2_1_4_178\driverlib\timer.h"
#include "\ti\tivaware_c_series_2_1_4_178\driverlib\adc.h"
#include "\ti\tivaware_c_series_2_1_4_178\driverlib\debug.h"

#ifdef DEBUG
void__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

void configureTimer1A();

uint32_t ui32ADC0Value[1];
volatile uint32_t ui32TempAvg;
volatile uint32_t ui32TempValueC;
volatile uint32_t ui32TempValueF;

int main()
{
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    ADCHardwareOversampleConfigure(ADC0_BASE, 32);
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_TS | ADC_CTL_IE | ADC_CTL_END);

    configureTimer1A();

    ADCSequenceEnable(ADC0_BASE, 3);
    ADCIntEnable(ADC0_BASE, 3);

    while(1)
    {
    }
}

void configureTimer1A()
{
    int32_t ui32PeriodHigh = (SysCtlClockGet() / 1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1); // Must call before calling peripheral specific driverlib function, or else Fault ISR
    IntMasterEnable();
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC); // Configures Timer 1 as a 32-bit timer in periodic mode (combines Timer 0A and 0B)

    TimerLoadSet(TIMER1_BASE, TIMER_A, 5 * (SysCtlClockGet() / 10)); // Since the interrupt fires at zero, you must subtract 1.

    IntEnable(INT_TIMER1A);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER1_BASE, TIMER_A);
}

void Timer1IntHandler(void)
{
    int32_t ui32PeriodHigh = 0.5 * (SysCtlClockGet());

    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    TimerLoadSet(TIMER1_BASE, TIMER_A, ui32PeriodHigh);

    ADCIntClear(ADC0_BASE, 3); //clear adc conversion done flag before writing code that depends on it. change to sequence 2
    //Changed all sequence numbers below to sequence two
    ADCProcessorTrigger(ADC0_BASE, 3);

    while(!ADCIntStatus(ADC0_BASE, 3, false)) //wait for conversion to finish
    {
    } //if loop exited conversion is complete

    ADCSequenceDataGet(ADC0_BASE, 3, ui32ADC0Value); //gets samples from the array
    ui32TempValueC = (1475 -((2475 * ui32ADC0Value[0])) / 4096)/10;
    ui32TempValueF = ((ui32TempValueC * 9) + 160) / 5;

    if (ui32TempValueF > 66) {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4); // Turn off the LED
    }
    else {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0); // Turn on the blue LED
    }
}


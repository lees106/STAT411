#include <stdint.h>
#include <stdbool.h>
#include "\ti\tivaware_c_series_2_1_4_178\inc\tm4c123gh6pm.h"
#include "\ti\tivaware_c_series_2_1_4_178\inc\hw_memmap.h"
#include "\ti\tivaware_c_series_2_1_4_178\inc\hw_types.h"
#include "\ti\tivaware_c_series_2_1_4_178\driverlib\sysctl.h"
#include "\ti\tivaware_c_series_2_1_4_178\driverlib\interrupt.h"
#include "\ti\tivaware_c_series_2_1_4_178\driverlib\gpio.h"
#include "\ti\tivaware_c_series_2_1_4_178\driverlib\timer.h"

void configureTimer0();
void configureTimer1A();
void PORTFPin4IntHandler();


int main(void)
{
    // Clock Setup
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    // GPIO Configuration
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_4);
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_INT_PIN_4, GPIO_RISING_EDGE);
    IntEnable(INT_GPIOF);

    configureTimer0();

    while(1)
    {
    }
}

void configureTimer1A()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1); // Must call before calling peripheral specific driverlib function, or else Fault ISR
    IntMasterEnable();
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC); // Configures Timer 1 as a 32-bit timer in periodic mode (combines Timer 0A and 0B)

    TimerLoadSet(TIMER1_BASE, TIMER_A, 0); // Since the interrupt fires at zero, you must subtract 1.

    IntEnable(INT_TIMER1A);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER1_BASE, TIMER_A);
}

void configureTimer0()
{
    // Timer Configuration
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); // Must call before calling peripheral specific driverlib function, or else Fault ISR
    IntMasterEnable();
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC); // Configures Timer 0 as a 32-bit timer in periodic mode (combines Timer 0A and 0B)

    /* Delay Calculation
      *
      *  To toggle GPIO at 2hz and 75% Duty cycle
      *  You need to generate an interrupt at 50% of the desired period.
      *  Steps:
      *      1.) Call SysCltClockGet()
      *      2.) Divide that by desired frequency
      *      3.) Divide the whole thing by the duty cycle
      *
      * This calculation is loaded into the timer's INTERVAL LOAD RESISTER using TimerLoadSet from the Timer API (driverLib)
      */

     TimerLoadSet(TIMER0_BASE, TIMER_A, 0); // Since the interrupt fires at zero, you must subtract 1.


     /* Interrupt Enable
      * You have to enable the interrupt in both:
      *      1.) Timer Module
      *      2.) Nested Vector Interrupt Controller (NVIC)
      *
      * IntMasterEnable() is the master interrupt enable API for all interrupts.
      * IntEnable() enables the specific vector associated with Timer0A.
      * TimerIntEnable() enables a specific event within the timer to generate an interrupt.
      */
     IntEnable(INT_TIMER0A);
     TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

     /* Timer Enable
      * This will start the timer and interrupts will begin triggering the timeouts
      */
     TimerEnable(TIMER0_BASE, TIMER_A);
}

/* Timer Interrupt Handler
 *  Since this program is interrupt driven, we must add an interrupt handler (ISR) for the timer.
 *  We must first clear the interrupt source
 *  Then toggle the GPIO pin based on the current state.
 *
 *  In case my last program left any LEDs powered on, I called GPIOPinWrite() to turn off all LEDs.
 */
void Timer0IntHandler(void)
{
    int32_t ui32PeriodHigh =  0.43 * (SysCtlClockGet() / 10);
    int32_t ui32PeriodLow =   0.57 * (SysCtlClockGet() / 10);

    // Clear the timer interrupt

    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // Read the current state of the GPIO pin and
    // write back the opposite state
    if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2))
    {
        TimerLoadSet(TIMER0_BASE, TIMER_A, ui32PeriodLow);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0); // Turn off all LEDs
    }
    else
    {
        TimerLoadSet(TIMER0_BASE, TIMER_A, ui32PeriodHigh);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4); //Turns on blue LED
    }
}
void Timer1IntHandler(void)
{
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet());

}

void PORTFPin4IntHandler(void){
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_INT_PIN_4);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0); //Turns off blue LED
    configureTimer1A();
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2); //Turns on blue LED

}



#include <stdio.h>
#include platform.h
#include xil_printf.h
#include xgpio.h
#include xparameters.h
#include xstatus.h
#include <sleep.h>

#define LED_CHANNEL 1
#define SWITCH_CHANNEL 2
#define LED_MASK 0x0FF
#define SWITCHES_MASK 0x0FF


int main()
{

    init_platform();
    XGpio Gpio;
    int Status;

    xil_printf(Program Startedrn);
    
	/* Initialize the GPIO driver */
    #ifndef SDT
	    Status = XGpio_Initialize(&Gpio, XPAR_AXI_GPIO_0_BASEADDR);
    #else
	    Status = XGpio_Initialize(&Gpio, XPAR_AXI_GPIO_0_BASEADDR);
        #endif
	if (Status != XST_SUCCESS) {
		xil_printf(Gpio Initialization Failedrn);
		return XST_FAILURE;
	}
    
    XGpio_SetDataDirection(&Gpio, LED_CHANNEL, ~LED_MASK);
    XGpio_SetDataDirection(&Gpio, SWITCH_CHANNEL, SWITCHES_MASK);

    
    

    print(Hello Worldnr);
    while(1){
        u32 switches = XGpio_DiscreteRead(&Gpio, SWITCH_CHANNEL);
        XGpio_DiscreteWrite(&Gpio, LED_CHANNEL, switches);
        xil_printf(checked! %xnr,switches);
        usleep(500000);
    }

    cleanup_platform();
    return 0;
}


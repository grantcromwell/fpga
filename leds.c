

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xgpio.h"
#include "xparameters.h"
#include "xstatus.h"
#include <sleep.h>
#include <xbram_hw.h>
#include "xbram.h"

#define LED_CHANNEL 1
#define SWITCH_CHANNEL 2
#define LED_MASK 0x0FF
#define SWITCHES_MASK 0x0FF


int main()
{

    init_platform();
    XGpio Gpio;
    XBram Bram;
    int Status;
    u32 outData;
    XBram_Config *ConfigPtr;
    xil_printf("Program Started\r\n");
   
    
	
    #ifndef SDT
	    Status = XGpio_Initialize(&Gpio, XPAR_AXI_GPIO_0_BASEADDR);
    #else
	    Status = XGpio_Initialize(&Gpio, XPAR_AXI_GPIO_0_BASEADDR);
        #endif
	if (Status != XST_SUCCESS) {
		xil_printf("Gpio Initialization Failed\r\n");
		return XST_FAILURE;
	}

    #ifndef SDT
	    ConfigPtr = XBram_LookupConfig(XPAR_BRAM_0_DEVICE_ID);
    #else
        ConfigPtr = XBram_LookupConfig(XPAR_AXI_BRAM_0_BASEADDRESS);
    #endif
	    if (ConfigPtr == (XBram_Config *) NULL) {
            return XST_FAILURE;
        }

	Status = XBram_CfgInitialize(&Bram, ConfigPtr,
	    ConfigPtr->CtrlBaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
    
    XGpio_SetDataDirection(&Gpio, LED_CHANNEL, ~LED_MASK);
    XGpio_SetDataDirection(&Gpio, SWITCH_CHANNEL, SWITCHES_MASK);

    
    

    print("Hello World\n\r");

    for (int i = 0; i < 16*4; i=i+4){
        XBram_WriteReg(XPAR_AXI_BRAM_0_BASEADDRESS, i, i);
    }
        for (int i = 0; i < 16*4; i=i+4){
        outData = XBram_ReadReg(XPAR_AXI_BRAM_0_BASEADDRESS, i);
        xil_printf("checked! %d\n\r", outData);
    }
    
    
    while(1){
        u32 switches = XGpio_DiscreteRead(&Gpio, SWITCH_CHANNEL);
        XGpio_DiscreteWrite(&Gpio, LED_CHANNEL, switches);
        xil_printf("checked! %x\n\r",switches);
        usleep(500000);
    }

    cleanup_platform();
    return 0;
}

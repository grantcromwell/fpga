#include "xil_printf.h"
#include <stdint.h>
#include "xil_cache.h"
#include "xil_io.h"

#define COPROCESSOR_BASE 0x40000000U
#define SCORE_OUT_LOW_OFFSET  0x34U
#define SCORE_OUT_MID_OFFSET  0x38U
#define SCORE_OUT_HIGH_OFFSET 0x3CU
#define MATRIX_SIZE 16



int main(void) {
    xil_printf("Your app is starting\r\n");

    int16_t matrixA[MATRIX_SIZE] = {
        2, 3, 4, 7,
        5, 3, 2, 2,
        8, 3, 2, 4,
        7, 7, 7, 7
    };

    int16_t matrixB[MATRIX_SIZE] = {
        2, 4, 6, 3,
        1, 0, 0, 1,
        3, 4, 5, 7,
        3, 6, 7, 2
    };


    static volatile int64_t matrixC[MATRIX_SIZE] __attribute__((aligned(64))) = {0};
    for (int i = 0; i < MATRIX_SIZE; i++) {
        matrixC[i] = -1;
    }

    Xil_DCacheFlushRange((INTPTR)matrixA, sizeof(matrixA));
    Xil_DCacheFlushRange((INTPTR)matrixB, sizeof(matrixB));

    Xil_DCacheFlushRange((INTPTR)matrixC, sizeof(matrixC));
    Xil_DCacheInvalidateRange((INTPTR)matrixC, sizeof(matrixC));

    while ((Xil_In32(COPROCESSOR_BASE) & 0x4) == 0) {
        // waits until ap_idle is set before updating the buffer addresses.
    }

    Xil_Out32(COPROCESSOR_BASE + 0x10, (uint32_t)(uintptr_t)matrixA);
    Xil_Out32(COPROCESSOR_BASE + 0x14, 0);

    Xil_Out32(COPROCESSOR_BASE + 0x1C, (uint32_t)(uintptr_t)matrixB);
    Xil_Out32(COPROCESSOR_BASE + 0x20, 0);

    Xil_Out32(COPROCESSOR_BASE + 0x28, (uint32_t)(uintptr_t)matrixC);
    Xil_Out32(COPROCESSOR_BASE + 0x2C, 0);

    Xil_Out32(COPROCESSOR_BASE, 1);

    while ((Xil_In32(COPROCESSOR_BASE) & 2) == 0) {
    }

    Xil_DCacheInvalidateRange((INTPTR)matrixC, sizeof(matrixC));

    for (int i = 0; i < MATRIX_SIZE; ++i) {
        uint64_t value = (uint64_t)matrixC[i];
        xil_printf("C[%d] = %x:%x\r\n", i, (unsigned int)(value >> 32), (unsigned int)value);
    }

    uint32_t scoreLow = Xil_In32(COPROCESSOR_BASE + SCORE_OUT_LOW_OFFSET);
    uint32_t scoreMid = Xil_In32(COPROCESSOR_BASE + SCORE_OUT_MID_OFFSET);
    uint32_t scoreHigh = Xil_In32(COPROCESSOR_BASE + SCORE_OUT_HIGH_OFFSET);


    xil_printf("quadratic score = %x:%x:%x\r\n", (unsigned int)(scoreHigh & 0x1F), (unsigned int)scoreMid, (unsigned int)scoreLow);

    while (1) {
    }
    return 0;
}
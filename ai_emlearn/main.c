/*
 * Copyright (C) 2019 Inria
 */

#include <inttypes.h>
#include <stdalign.h>
#include <stdio.h>

#include "board.h"
#include "stdio_uart.h"
#include "thread.h"
#include "periph/uart.h"

#include "model.h"

#define IMG_SIZE 256U

static alignas(float) uint8_t _digit[IMG_SIZE] = { 0 };
static uint8_t _digit_pos = 0;

static kernel_pid_t _main_pid;

static void _uart_callback(void *arg, uint8_t data) {
    (void)arg;
    msg_t msg;
    msg.content.value = (uint32_t)data;
    msg_send(&msg, _main_pid);
}

int main(void)
{
    _main_pid = thread_getpid();

    /* Initialize first UART */
    uart_init(UART_DEV(0), STDIO_UART_BAUDRATE, _uart_callback, NULL);

    /* Wait for UART data */
    msg_t msg;
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    while (1) {
        msg_receive(&msg);
        _digit[_digit_pos++] = (uint8_t)msg.content.value;
        if (_digit_pos == 0) {
            const float *digit_as_float = (const float *)(uintptr_t)_digit;
            printf("Predicted digit: %" PRIi32 "\n",
                   model_predict(digit_as_float, IMG_SIZE >> 2));
        }
    }

    return 0;
}

/*
 * Copyright (C) 2025 Inria
 */

#include <inttypes.h>
#include <stdalign.h>
#include <stdio.h>
#include <string.h>

#include "shell.h"
#include "fmt.h"

#include "model.h"

#define IMG_SIZE 256U
static alignas(float) uint8_t _digit[IMG_SIZE] = { 0 };

static int _clear_handler(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    memset(_digit, 0, sizeof(_digit));
    return 0;
}

static int _load_handler(int argc, char **argv)
{
    if (argc < 3) {
        printf("Usage: load <idx> <bytes as hex>\n");
        return -1;
    }

    uint8_t idx = atoi(argv[1]);
    fmt_hex_bytes(&_digit[idx * 32], argv[2]);

    // For debugging what was received
    // if (idx == 7) {
    //     for (uint16_t i = 0; i < IMG_SIZE; i++) {
    //         printf("%02x ", _digit[i]);
    //         if (i % 32 == 31) {
    //             puts("");
    //         }
    //     }
    // }

    return 0;
}

static int _infer_handler(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    const float *digit_as_float = (const float *)(uintptr_t)_digit;
    printf("Predicted digit: %" PRIi32 "\n",
    model_predict(digit_as_float, IMG_SIZE >> 2));
    return 0;
}

/* Declare the list of shell commands */
static const shell_command_t shell_commands[] = {
    { "clear", "Clear the image buffer", _clear_handler },
    { "load", "Load an image line into internal buffer", _load_handler },
    { "infer", "Infer the digit using emlearn", _infer_handler },
    { NULL, NULL, NULL }
};

int main(void)
{
    /* Configure and start the shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    return 0;
}

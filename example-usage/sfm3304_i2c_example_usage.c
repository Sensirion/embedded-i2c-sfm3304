/*
 * THIS FILE IS AUTOMATICALLY GENERATED
 *
 * Generator:     sensirion-driver-generator 1.1.2
 * Product:       sfm3304
 * Model-Version: 1.0.0
 */
/*
 * Copyright (c) 2025, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include "sensirion_common.h"
#include "sensirion_i2c_hal.h"
#include "sfm3304_i2c.h"
#include <inttypes.h>  // PRIx64
#include <stdio.h>     // printf

#define sensirion_hal_sleep_us sensirion_i2c_hal_sleep_usec

int main(void) {
    int16_t error = NO_ERROR;
    sensirion_i2c_hal_init();
    sfm3304_init(SFM3304_I2C_ADDR_2E);

    sfm3304_stop_continuous_measurement();
    sensirion_hal_sleep_us(100000);
    uint32_t product_identifier = 0;
    uint64_t serial_number = 0;
    error =
        sfm3304_read_product_identifier(&product_identifier, &serial_number);
    if (error != NO_ERROR) {
        printf("error executing read_product_identifier(): %i\n", error);
        return error;
    }
    printf("product_identifier: %u ", product_identifier);
    printf("serial_number: %" PRIx64 "\n", serial_number);
    error = sfm3304_start_continuous_measurement();
    if (error != NO_ERROR) {
        printf("error executing start_continuous_measurement(): %i\n", error);
        return error;
    }
    int16_t flow = 0;
    int16_t temperature = 0;
    sfm3304_status_word_t status_word = {.value = 0u};
    uint16_t repetition = 0;
    for (repetition = 0; repetition < 100; repetition++) {
        sensirion_hal_sleep_us(100000);
        error = sfm3304_read_measurement_data_raw(&flow, &temperature,
                                                  &status_word);
        if (error != NO_ERROR) {
            printf("error executing read_measurement_data_raw(): %i\n", error);
            continue;
        }
        printf("flow: %i ", flow);
        printf("temperature: %i ", temperature);
        printf("status_word: %02x\n", status_word.value);
    }

    error = sfm3304_stop_continuous_measurement();
    if (error != NO_ERROR) {
        return error;
    }
    return NO_ERROR;
}

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

#ifndef SFM3304_I2C_H
#define SFM3304_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sensirion_config.h"
#define SFM3304_I2C_ADDR_2E 0x2e

typedef enum {
    SFM3304_START_CONTINUOUS_MEASUREMENT_CMD_ID = 0x3603,
    SFM3304_START_CONTINUOUS_MEASUREMENT_WITH_FILTER_CMD_ID = 0x3603,
    SFM3304_STOP_CONTINUOUS_MEASUREMENT_CMD_ID = 0x3ff9,
    SFM3304_CONFIGURE_AVERAGING_CMD_ID = 0x366a,
    SFM3304_READ_SCALE_OFFSET_UNIT_CMD_ID = 0x3661,
    SFM3304_ENTER_SLEEP_CMD_ID = 0x3677,
    SFM3304_EXIT_SLEEP_CMD_ID = 0x0,
    SFM3304_READ_PRODUCT_IDENTIFIER_CMD_ID = 0xe102,
} SFM3304_CMD_ID;

typedef enum {
    SFM3304_ERROR_CODE_I2C_ERROR = 0,
    SFM3304_ERROR_CODE_TIMEOUT = 1,
} sfm3304_error_code_t;

typedef enum {
    SFM3304_FILTER_OPTIONS_T63_1MS = 33601,
    SFM3304_FILTER_OPTIONS_T63_3MS = 50961,
    SFM3304_FILTER_OPTIONS_T63_5MS = 56105,
    SFM3304_FILTER_OPTIONS_T63_10MS = 60527,
} sfm3304_filter_options;

typedef union {
    struct {
        uint16_t nr_of_averages : 9;
        uint16_t command_argument_reception : 1;
        uint16_t avg_mode_flag : 1;
        uint16_t exp_smoothing_flag : 1;
        uint16_t command_id : 4;
    };
    uint16_t value;
} sfm3304_status_word_t;

typedef union {
    struct {
        uint16_t prefix : 4;
        uint16_t time_base : 4;
        uint16_t unit : 5;
    };
    uint16_t value;
} sfm3304_flow_unit_t;

/**
 * @brief Initialize i2c address of driver
 *
 * @param[in] i2c_address Used i2c address
 *
 */
void sfm3304_init(uint8_t i2c_address);

/**
 * @brief Start continuous measurement
 *
 * The sensor starts measuring both flow and temperature and provides a status
 * word. All three measurement results can be read out through one single I2C
 * read when the continuous measurement is running. This command uses the
 * default low pass filter settin with a T63 of 3ms. To configure different low
 * pass filter, use the specific method.
 *
 * @note The first measurement result will be available after 4ms. Small
 * accuracy deviations (few % of reading) can occur during the first 50ms
 * (including the 4ms)
 *
 * @return error_code 0 on success, an error code otherwise.
 */
int16_t sfm3304_start_continuous_measurement();

/**
 * @brief Start continuous measurement with custom filter
 *
 * The sensor starts measuring both flow and temperature and provides a status
 * word. All three measurement results can be read out through one single I2C
 * read when the continuous measurement is running.
 *
 * @param[in] a_filter Filter setting for low pass filter on flow measurement.
 * You can configure a time constant T63 by passing an integer from the
 * following options: 1ms -> 33601, 3ms (default) -> 50961, 5ms -> 56105, 10ms
 * -> 60527
 *
 * @note The first measurement result will be available after 4ms. Small
 * accuracy deviations (few % of reading) can occur during the first 50ms
 * (including the 4ms).
 *
 * @return error_code 0 on success, an error code otherwise.
 *
 * Example:
 * --------
 *
 * @code{.c}
 *
 *     int16_t local_error = 0;
 *     local_error = sfm3304_start_continuous_measurement_with_filter(50961);
 *     if (local_error != NO_ERROR) {
 *         return local_error;
 *     }
 *
 * @endcode
 *
 */
int16_t sfm3304_start_continuous_measurement_with_filter(uint16_t a_filter);

/**
 * @brief Read out the data from the sensor.
 *
 * After a start continuous measurement command, the measurement results can be
 * read out continuously with this command. The temperature and the consecutive
 * bytes do not need to be read out (every time). The read sequence can be
 * aborted by a NACK and a STOP condition.
 *
 * @param[out] flow Calibrated flow signal. Convert to gas flow in slm by (value
 * - offset) / scale
 * @param[out] temperature Calibrated temperature. Convert to degrees celsius by
 * value / 200.
 * @param[out] status_word Gives information about the measurement command that
 * is currently running. A detailed description can be found in the data sheet.
 *
 * @note The first measurement result will be available after 4ms. Small
 * accuracy deviations (few % of reading) can occur during the first 50ms
 * (including the 4ms)
 *
 * @return error_code 0 on success, an error code otherwise.
 */
int16_t sfm3304_read_measurement_data_raw(int16_t* flow, int16_t* temperature,
                                          sfm3304_status_word_t* status_word);

/**
 * @brief Read out only flow measurement from the sensor.
 *
 * @param[out] flow Calibrated flow signal read from the sensor.
 *
 * @return error_code 0 on success, an error code otherwise.
 */
int16_t sfm3304_read_measurement_flow_raw(int16_t* flow);

/**
 * @brief This transfer stops the continuous measurement and puts the sensor in
 * idle mode.
 *
 * This command stops the continuous measurement and puts the sensor in idle
 * mode. After it receives the stop command, the sensor needs up to 0.5ms to
 * power down the heater, enter idle mode and be receptive for a new command.
 *
 * @return error_code 0 on success, an error code otherwise.
 */
int16_t sfm3304_stop_continuous_measurement();

/**
 * @brief Configures the sensor's averaging mode.
 *
 * This command configures the sensor’s averaging mode:
 *   * N=0 (default): average-until-read mode
 *   * 1≤N≤128: fixed-N averaging mode. N is the number of internal measurements
 *     that are averaged for one returned measurement value (i.e. the average
 *     over N flow samples, where N = CmdArgument, c.f. Sec. 3).
 * The configured averaging mode will be used for flow measurements until a
 * reset or re-execution of this command is performed. After a reset, averaging
 * is set to fixed-N averaging mode with N = 2. The highest averaging number
 * allowed is 128. If a higher number is used in the command argument, it will
 * be overruled by the maximal value of 128 samples to average. If no averaging
 * is desired, set N to 1.
 *
 * @param[in] average_window Average window configuration value.
 *
 * @return error_code 0 on success, an error code otherwise.
 *
 * Example:
 * --------
 *
 * @code{.c}
 *
 *     int16_t local_error = 0;
 *     local_error = sfm3304_configure_averaging(2);
 *     if (local_error != NO_ERROR) {
 *         return local_error;
 *     }
 *
 * @endcode
 *
 */
int16_t sfm3304_configure_averaging(uint16_t average_window);

/**
 * @brief This command provides the actually used scale factor and offset of the
 * sensor.
 *
 * This command provides the scale factor and offset to convert flow readings
 * into physical units. The scale factor and offset are specific to the
 * calibrated gas and its corresponding lookup table used for the flow
 * measurement. Therefore, the gas needs to be specified in the command argument
 * by the command code of the corresponding start continuous measurement.
 *
 * @param[in] command_code For the SFM3304-xxx-D the only calibrated gas is air,
 * the command argument is 0x3603.
 * @param[out] flow_scale_factor Scale factor used by the sensor.
 * @param[out] flow_offset Offset used by the sensor.
 * @param[out] flow_unit Applicable flow unit.
 *
 * @note For the SFM3304-xxx-D, the flow unit is a fixed value 0x0148 and
 * corresponds to slm: standard liter per minute at 20°C and 1013.25 hPa
 * pressure
 *
 * @return error_code 0 on success, an error code otherwise.
 */
int16_t sfm3304_read_scale_offset_unit(uint16_t command_code,
                                       int16_t* flow_scale_factor,
                                       int16_t* flow_offset,
                                       sfm3304_flow_unit_t* flow_unit);

/**
 * @brief Enters sleep mode
 *
 * In sleep mode the sensor uses a minimum amount of power. The mode can only be
 * entered from idle mode, i.e. when the sensor is not performing measurements.
 * This mode is particularly useful for battery operated devices. To minimize
 * the current in this mode, the complexity of the sleep mode circuit has been
 * reduced as much as possible, which is mainly reflected by the way the sensor
 * exits the sleep mode. In sleep mode the sensor cannot be soft reset.
 *
 * @return error_code 0 on success, an error code otherwise.
 */
int16_t sfm3304_enter_sleep();

/**
 * @brief Exits from sleep mode
 *
 * The sensor exits the sleep mode and enters the idle mode when it receives the
 * valid I2C address and a write bit (‘0’). Note that the I2C address is not
 * acknowledged. It is necessary to poll the sensor to see whether the sensor
 * has received the address and has woken up. This should take typically 16ms.
 *
 * @return error_code 0 on success, an error code otherwise.
 */
int16_t sfm3304_exit_sleep();

/**
 * @brief This command reads product identifier and the serial number.
 *
 * This command allows to read product identifier and the serial number. The
 * command can only be executed from the idle mode, i.e. when the sensor is not
 * performing measurements.
 *
 * @param[out] product_identifier 32-bit unique product and revision number. The
 * number is listed in Table 13 below. Note that the last 8 bits are the
 * revision number.
 * @param[out] serial_number 64-bit unique serial number in the format of an
 * unsigned long integer. The serial number can be converted from binary into
 * decimal, whereby in decimal it has the following format: yywwxxxxxx, where:
 * yy: last 2 digits of calibration year, ww: calibration week, xxxxxx: unique
 * 6-digit sequential number within the calibration week.
 *
 * @return error_code 0 on success, an error code otherwise.
 */
int16_t sfm3304_read_product_identifier(uint32_t* product_identifier,
                                        uint64_t* serial_number);

#ifdef __cplusplus
}
#endif
#endif  // SFM3304_I2C_H

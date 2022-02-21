/* 
 * File:   6813_cmds.h
 * Author: Elena
 *
 * Created on January 27, 2022, 4:55 PM
 */

#ifndef LTC_CMDS_H
#define	LTC_CMDS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
    
// send command to start ADC conversion for cell voltages
void start_cell_voltage_adc_conversion(void);
// send command to start ADC conversion for pack temperatures
void start_temperature_adc_conversion(void);
// send command to poll ADC status
void poll_adc_status(void);
// receive cell voltage register data
void rdcv_register(uint8_t which_reg, uint16_t* buf, uint8_t* cell_voltages_valid);
// receive GPIO voltage register data
void rdaux_register(uint8_t which_reg, uint16_t* buf);
// send command to start open sense line check
void open_wire_check(uint8_t pull_dir);
void rdcfga(uint8_t* buffer);
void wrcfga(uint8_t* data_to_write);

#ifdef	__cplusplus
}
#endif

#endif	/* LTC_CMDS_H */


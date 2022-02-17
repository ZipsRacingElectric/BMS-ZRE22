/*
 * contains functions for interacting with the CAN peripheral
 */
////////////////includes///////////////////////////////////////////////////////
#include "can_driver.h"
#include "LTC/LTC_utilities.h"
#include "soc_fns.h"
#include "fault_handler.h"
#include "mcc_generated_files/can1.h"
#include "mcc_generated_files/can2.h"
#include "mcc_generated_files/can_types.h"
#include "mcc_generated_files/pin_manager.h"
////////////////defines////////////////////////////////////////////////////////

////////////////globals////////////////////////////////////////////////////////

////////////////prototypes/////////////////////////////////////////////////////
static void send_cell_voltage_message(uint16_t* cell_voltages, uint16_t id);
static void send_pack_temperature_message(uint16_t* cell_voltages, uint16_t id);

////////////////functions//////////////////////////////////////////////////////
// initialize 2 CAN peripherals, set standby pins low
void can_initialize(void)
{
    CAN1_TransmitEnable();
    CAN1_ReceiveEnable();
    CAN1_OperationModeSet(CAN_NORMAL_OPERATION_MODE);
    
    CAN2_TransmitEnable();
    CAN2_ReceiveEnable();
    CAN2_OperationModeSet(CAN_NORMAL_OPERATION_MODE);
    
    CAN1_STDBY_SetLow();
    CAN2_STDBY_SetLow();
}

// put cell voltages on BMS CAN bus
void report_cell_voltages(uint16_t* cell_voltages)
{
    uint8_t i = 0;
    uint8_t upper_bound = (NUM_CELLS + 4 - (NUM_CELLS % 4)) / 4;
    for(i = 0; i < upper_bound; ++i)
    {
        uint16_t msg_id = i + CAN_ID_CELL_VOLTAGES;
        send_cell_voltage_message(&cell_voltages[4*i], msg_id); 
    }
}

// put pack temperatures on the CAN bus
void report_pack_temperatures(uint16_t* pack_temperatures)
{
    uint8_t i = 0;
    uint8_t upper_bound = (NUM_TEMP_SENSORS + 4 - (NUM_TEMP_SENSORS % 4)) / 4;
    for(i = 0; i < upper_bound; ++i)
    {
        uint16_t msg_id = i + CAN_ID_PACK_TEMPERATURE;
        send_pack_temperature_message(&pack_temperatures[4*i], msg_id); 
    }
}

// put status message on main vehicle CAN bus
void report_status(void)
{
    int16_t cs_lo = get_cs_lo_xten();
    int16_t cs_hi = get_cs_hi_xten();
    uint16_t soc = get_soc_xten();
    uint8_t fault_codes = get_fault_codes();

    uint8_t can_data[7] = {(uint8_t)(soc & 0xFF), (uint8_t)(soc >> 8),
                           (uint8_t)(cs_hi & 0xFF), (uint8_t)(cs_hi >> 8),
                           (uint8_t)(cs_lo & 0xFF), (uint8_t)(cs_lo >> 8),
                           fault_codes};

    CAN_MSG_FIELD overhead = {
        .idType = CAN_FRAME_STD,
        .frameType = CAN_FRAME_DATA,
        .dlc = CAN_DLC_7,
    };

    CAN_MSG_OBJ msg = {
        .msgId = CAN_ID_STATUS,
        .field = overhead,
        .data = can_data,
    };

    CAN_TX_MSG_REQUEST_STATUS status = CAN1_Transmit(CAN_PRIORITY_MEDIUM, &msg);
    if(status == CAN_TX_MSG_REQUEST_SUCCESS)
    {
        LED2_CAN_STATUS_SetHigh();
    }
    else
    {
        LED2_CAN_STATUS_SetLow();
    }
}

static void send_cell_voltage_message(uint16_t* cell_voltages, uint16_t id)
{
    uint8_t can_data[8] = {(uint8_t)(cell_voltages[0] >> 8), (uint8_t)(cell_voltages[0] & 0xFF), 
                           (uint8_t)(cell_voltages[1] >> 8), (uint8_t)(cell_voltages[1] & 0xFF), 
                           (uint8_t)(cell_voltages[2] >> 8), (uint8_t)(cell_voltages[2] & 0xFF), 
                           (uint8_t)(cell_voltages[3] >> 8), (uint8_t)(cell_voltages[3] & 0xFF)};

    CAN_MSG_FIELD overhead = {
        .idType = CAN_FRAME_STD,
        .frameType = CAN_FRAME_DATA,
        .dlc = CAN_DLC_8,
    };

    CAN_MSG_OBJ msg = {
        .msgId = id,
        .field = overhead,
        .data = can_data,
    };

    CAN_TX_MSG_REQUEST_STATUS status = CAN1_Transmit(CAN_PRIORITY_MEDIUM, &msg);
    if(status == CAN_TX_MSG_REQUEST_SUCCESS)
    {
        LED2_CAN_STATUS_SetHigh();
    }
    else
    {
        LED2_CAN_STATUS_SetLow();
    }
}

static void send_pack_temperature_message(uint16_t* pack_temperatures, uint16_t id)
{
    uint8_t can_data[8] = {(uint8_t)(pack_temperatures[0] >> 8), (uint8_t)(pack_temperatures[0] & 0xFF), 
                           (uint8_t)(pack_temperatures[1] >> 8), (uint8_t)(pack_temperatures[1] & 0xFF), 
                           (uint8_t)(pack_temperatures[2] >> 8), (uint8_t)(pack_temperatures[2] & 0xFF), 
                           (uint8_t)(pack_temperatures[3] >> 8), (uint8_t)(pack_temperatures[3] & 0xFF)};

    CAN_MSG_FIELD overhead = {
        .idType = CAN_FRAME_STD,
        .frameType = CAN_FRAME_DATA,
        .dlc = CAN_DLC_8,
    };

    CAN_MSG_OBJ msg = {
        .msgId = id,
        .field = overhead,
        .data = can_data,
    };

    CAN_TX_MSG_REQUEST_STATUS status = CAN1_Transmit(CAN_PRIORITY_MEDIUM, &msg);
    if(status == CAN_TX_MSG_REQUEST_SUCCESS)
    {
        LED2_CAN_STATUS_SetHigh();
    }
    else
    {
        LED2_CAN_STATUS_SetLow();
    }
}

#pragma once

#include "Arduino.h"
#include <SPI.h>
#include <stdint.h>

#define     NO_OP_CODE              0xF0
#define     DIGIT_0_CODE            0xF1
#define     DIGIT_1_CODE            0xF2
#define     DIGIT_2_CODE            0xF3
#define     DIGIT_3_CODE            0xF4
#define     DIGIT_4_CODE            0xF5
#define     DIGIT_5_CODE            0xF6
#define     DIGIT_6_CODE            0xF7
#define     DIGIT_7_CODE            0xF8
#define     DECODE_MODE             0xF9
#define     INTENSITY_CODE          0xFA
#define     SCAN_LIMIT_CODE         0xFB
#define     SHUTDOWN_CODE           0xFC
#define     DISPLAY_TEST_CODE       0xFF

//REGISTER DATA USED FOR DISPLAY TEST CODE
#define     DISPLAY_NORMAL_OP_MODE  0x00
#define     DISPLAY_TEST_MODE       0x01

//DECODE MODE OPTIONS, TO BE USED AS DATA WHEN CONFIG DECODE MODE
#define     NO_DECODE_MODE          0x00
#define     DECODE_0_MODE           0x01
#define     DECODE_0_3_MODE         0x0F
#define     DECODE_0_7_MODE         0xFF

//SCAN LIMIT REGISTER USED TO CONTROL ROWS
#define     DISPLAY_DIGIT_UP_TO_0   0x00
#define     DISPLAY_DIGIT_UP_TO_1   0x01
#define     DISPLAY_DIGIT_UP_TO_2   0x02
#define     DISPLAY_DIGIT_UP_TO_3   0x03
#define     DISPLAY_DIGIT_UP_TO_4   0x04
#define     DISPLAY_DIGIT_UP_TO_5   0x05
#define     DISPLAY_DIGIT_UP_TO_6   0x06
#define     DISPLAY_DIGIT_UP_TO_7   0x07

//SHUTDOWN MODE
#define     SHUTDOWN_MODE   0x00
#define     NORMAL_MODE     0x01

//CODES FOR DUTY CYCLE INTENSITY 
#define     INTENSITY_3_PERCENT     0x00
#define     INTENSITY_9_PERCENT     0x01
#define     INTENSITY_15_PERCENT    0x02
#define     INTENSITY_21_PERCENT    0x03
#define     INTENSITY_28_PERCENT    0x04
#define     INTENSITY_32_PERCENT    0x05
#define     INTENSITY_40_PERCENT    0x06
#define     INTENSITY_46_PERCENT    0x07
#define     INTENSITY_53_PERCENT    0x08
#define     INTENSITY_60_PERCENT    0x09
#define     INTENSITY_66_PERCENT    0x0A
#define     INTENSITY_72_PERCENT    0x0B
#define     INTENSITY_78_PERCENT    0x0C
#define     INTENSITY_84_PERCENT    0x0D
#define     INTENSITY_90_PERCENT    0x0E
#define     INTENSITY_99_PERCENT    0x0F

const int chip_select_pin = 10;
const int speed_hz = 10000000;

void write(int opcode, int data);
void write_all_matrix(int opcode, int data, int matrix_number);
void write_to_matrix(uint8_t* data);
void write_one_letter(char letter);
void write_one_letter(char letter, uint8_t matrix_no);
void flush_data();
void shift_one_letter(char letter, uint8_t matrix_no);
void shift_one_letter_all(char letter);
void shift_one_letter_all_matrix(char letter);
void shift_message(String message);
void write_mine(char letter, char letter2, char letter3, char letter4);
void write_data_mine(uint8_t* letter, uint8_t* letter2, uint8_t* letter3, uint8_t* letter4);
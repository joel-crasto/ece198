﻿### ECE 198 Project: WatCard Attendance System

 Current Capabilities:
 -Reject cards scanned twice in a row
 -Reject non-Waterloo cards (except those that follow a similar pattern)
 -Audio and visual cue (active buzzer and LED) to indicate successful card scan
 -Interface between RFID and STM32 using SPI
 -Interface from periphery boards to central board using UART (tested with >1m long cables)
 -Do not save duplicates to final array on central board
 -Output final array to LCD (use button to play array)

Potential for future implementation (not implemented):
 -Output to SD card using SPI (implemented but not tested)
 -Match array to student number/name using dictionary for attendance taking purposes
 -Use up to 3 UART channels to connect with 3 periphery boards to scan WatCards

(these features were not implemented as external processing power for testing, including the viewing of SD card contents and using functions to take in student names/numbers to pair with their WatCard serial numbers using computers is not allowed)

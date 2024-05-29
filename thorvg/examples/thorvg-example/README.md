# Thorvg example

This is a minimalistic display + thorvg graphics library example.
In few function calls it sets up the display and shows Lottie animations.

## Building and running

Run the application as usual for an ESP-IDF project. For example, for ESP32P4:
```
idf.py set-target esp32p4
idf.py -p PORT flash monitor
```

## Hardware Required

esp32_p4_function_ev_board.


## Example output

The example should output the following:

```
I (1614) main_task: Calling app_main()
I (1665) ESP32_P4_EV: Partition size: total: 956561, used: 90109
I (1666) ESP32_P4_EV: MIPI DSI PHY Powered on
I (1667) ESP32_P4_EV: Install MIPI DSI LCD control panel
I (1672) ili9881c: ID1: 0x98, ID2: 0x81, ID3: 0x5c
I (1821) ESP32_P4_EV: Install MIPI DSI LCD data panel
I (1846) ESP32_P4_EV: Display initialized
I (1846) ESP32_P4_EV: Setting LCD backlight: 100%
I (1847) example: capiShapes start
I (6874) example: capiLottie start
I (6981) example: set 1.000000 / 24.000000
I (7059) example: set 2.000000 / 24.000000
I (7138) example: set 3.000000 / 24.000000
I (7217) example: set 4.000000 / 24.000000
I (7296) example: set 5.000000 / 24.000000
I (7375) example: set 6.000000 / 24.000000
I (7451) example: set 7.000000 / 24.000000
I (7531) example: set 8.000000 / 24.000000
I (7611) example: set 9.000000 / 24.000000
I (7690) example: set 10.000000 / 24.000000
I (7770) example: set 11.000000 / 24.000000
I (7850) example: set 12.000000 / 24.000000
I (7930) example: set 13.000000 / 24.000000
I (8010) example: set 14.000000 / 24.000000
I (8090) example: set 15.000000 / 24.000000
I (8169) example: set 16.000000 / 24.000000
I (8248) example: set 17.000000 / 24.000000
I (8327) example: set 18.000000 / 24.000000
I (8402) example: set 19.000000 / 24.000000
I (8480) example: set 20.000000 / 24.000000
I (8558) example: set 21.000000 / 24.000000
I (8637) example: set 22.000000 / 24.000000
I (8716) example: set 23.000000 / 24.000000
I (8795) example: set 24.000000 / 24.000000
I (8876) main_task: Returned from app_main()
```

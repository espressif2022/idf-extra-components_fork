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
I (1633) main_task: Calling app_main()
I (1690) ESP32_P4_EV: Partition size: total: 956561, used: 90109
I (1691) ESP32_P4_EV: MIPI DSI PHY Powered on
I (1692) ESP32_P4_EV: Install MIPI DSI LCD control panel
I (1697) ili9881c: ID1: 0x98, ID2: 0x81, ID3: 0x5c
I (1846) ESP32_P4_EV: Install MIPI DSI LCD data panel
I (1871) ESP32_P4_EV: Display initialized
I (1872) ESP32_P4_EV: Setting LCD backlight: 100%
I (1899) main_task: Returned from app_main()
I (2016) example: set 1.000000 / 24.000000
I (2106) example: set 2.000000 / 24.000000
I (2198) example: set 3.000000 / 24.000000
I (2290) example: set 4.000000 / 24.000000
I (2381) example: set 5.000000 / 24.000000
I (2472) example: set 6.000000 / 24.000000
I (2560) example: set 7.000000 / 24.000000
I (2653) example: set 8.000000 / 24.000000
I (2746) example: set 9.000000 / 24.000000
I (2838) example: set 10.000000 / 24.000000
I (2930) example: set 11.000000 / 24.000000
I (3022) example: set 12.000000 / 24.000000
I (3114) example: set 13.000000 / 24.000000
I (3207) example: set 14.000000 / 24.000000
I (3299) example: set 15.000000 / 24.000000
I (3391) example: set 16.000000 / 24.000000
I (3483) example: set 17.000000 / 24.000000
I (3575) example: set 18.000000 / 24.000000
I (3662) example: set 19.000000 / 24.000000
I (3753) example: set 20.000000 / 24.000000
I (3844) example: set 21.000000 / 24.000000
I (3935) example: set 22.000000 / 24.000000
I (4026) example: set 23.000000 / 24.000000
I (4118) example: set 24.000000 / 24.000000
```

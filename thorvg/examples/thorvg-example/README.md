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

esp32_p4_function_ev_board or esp32_s3_lcd_ev_board.


## Example output

The example should output the following:

```
I (1912) main_task: Calling app_main()
I (1972) example: d_name:emoji-animation.json
I (1972) example: d_name:lolo_walk.json
I (1982) example: d_name:test7.json
I (2002) example: capiShapes start
I (2042) example: capiLottie start
I (2212) example: set 1.000000 / 24.000000
I (2352) example: set 2.000000 / 24.000000
I (2492) example: set 3.000000 / 24.000000
I (2632) example: set 4.000000 / 24.000000
I (2762) example: set 5.000000 / 24.000000
I (2902) example: set 6.000000 / 24.000000
I (3032) example: set 7.000000 / 24.000000
I (3172) example: set 8.000000 / 24.000000
I (3312) example: set 9.000000 / 24.000000
I (3442) example: set 10.000000 / 24.000000
I (3582) example: set 11.000000 / 24.000000
I (3722) example: set 12.000000 / 24.000000
I (3862) example: set 13.000000 / 24.000000
I (4002) example: set 14.000000 / 24.000000
I (4142) example: set 15.000000 / 24.000000
I (4282) example: set 16.000000 / 24.000000
I (4412) example: set 17.000000 / 24.000000
I (4552) example: set 18.000000 / 24.000000
I (4682) example: set 19.000000 / 24.000000
I (4822) example: set 20.000000 / 24.000000
I (4952) example: set 21.000000 / 24.000000
I (5092) example: set 22.000000 / 24.000000
I (5232) example: set 23.000000 / 24.000000
I (5372) example: set 24.000000 / 24.000000
I (5502) main_task: Returned from app_main()
```

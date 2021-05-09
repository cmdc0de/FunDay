# FunDay
Good Times with ESP 32 and FPGA

# Materials
* [DHT11 Temperature Humidity Sensor Module Digital](https://www.amazon.com/gp/product/B01DKC2GQ0/ref=ppx_yo_dt_b_asin_title_o02_s00?ie=UTF8&psc=1)
  * [Datasheet](https://github.com/cmdc0de/FunDay/blob/main/docs/DHT11-Technical-Data-Sheet-Translated-Version-1143054.pdf)
* [OLED Display Module SSD1306 Driver IIC I2C](https://www.amazon.com/gp/product/B08RYSVG5Y/ref=ppx_yo_dt_b_asin_title_o02_s00?ie=UTF8&psc=1)
  * [Datasheet](https://github.com/cmdc0de/FunDay/blob/main/docs/SSD1306.pdf) 
* [ESP32-DevKitC](https://www.amazon.com/gp/product/B0811LGWY2/ref=ppx_yo_dt_b_asin_title_o02_s00?ie=UTF8&psc=1)
  * [docs](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/hw-reference/esp32/get-started-devkitc.html)
* [Breadboard Jumper Wires](https://www.amazon.com/EDGELEC-Breadboard-Optional-Assorted-Multicolored/dp/B07GD2BWPY/ref=pd_bxgy_2/139-4347215-7394026?_encoding=UTF8&pd_rd_i=B07GD2BWPY&pd_rd_r=229bcb14-5541-40f1-97ee-828203e33857&pd_rd_w=IfRZB&pd_rd_wg=i4Cxa&pf_rd_p=fd3ebcd0-c1a2-44cf-aba2-bbf4810b3732&pf_rd_r=BWMP5SNBZKHE2AXACH2H&psc=1&refRID=BWMP5SNBZKHE2AXACH2H)
* [Solderless Board](https://www.amazon.com/DEYUE-breadboard-Set-Prototype-Board/dp/B07LFD4LT6/ref=pd_bxgy_3/139-4347215-7394026?_encoding=UTF8&pd_rd_i=B07LFD4LT6&pd_rd_r=229bcb14-5541-40f1-97ee-828203e33857&pd_rd_w=IfRZB&pd_rd_wg=i4Cxa&pf_rd_p=fd3ebcd0-c1a2-44cf-aba2-bbf4810b3732&pf_rd_r=BWMP5SNBZKHE2AXACH2H&psc=1&refRID=BWMP5SNBZKHE2AXACH2H)
* [EspoTek Labrador](https://www.amazon.com/gp/product/B07CVB7ZJG/ref=ppx_yo_dt_b_asin_title_o02_s01?ie=UTF8&psc=1)
  * https://github.com/espotek/labrador/wiki
* [Resistors varying values](https://www.amazon.com/gp/product/B016NXK6QK/ref=ppx_yo_dt_b_asin_title_o02_s01?ie=UTF8&psc=1)
* [APA106 Super LED](https://www.espruino.com/WS2811)
  * [Datasheet](https://github.com/cmdc0de/FunDay/blob/main/docs/APA106.pdf)
* [TinyFPGA BX](https://www.arrow.com/en/products/bx/tinyfpga?gclid=CjwKCAjwj6SEBhAOEiwAvFRuKC7xSG16d-C2hRvBXGUWFlLOY-2m2u4k8rmGTDVco7wUXTf_i3W6mBoCGjwQAvD_BwE)
  * [User Guide](https://tinyfpga.com/bx/guide.html)
* [240x320 3.3V 2.4" SPI TFT LCD Touch](https://www.ebay.com/itm/171983887298?hash=item280b09dbc2:g:a-UAAOSwKz5dqAec)
  * [Datasheet](https://github.com/cmdc0de/FunDay/blob/main/docs/ILI9341%20Datasheet.pdf)
* 3x USB Micro B
  * 1 for the ESP32 DevKitC
  * 1 for EspoTek Labrador
  * 1 for Tiny FPGA Bx

# Project 0:
* ESP32 blink simple LED
* Set up Open Logic Analyzer to watch signal change

# Project 1:
* Use ESP32 to start an wifi AP and fetch a web page

# Project 2:
* Use ESP32 to OLED - teaches I2c

# Project 3a:
* Use ESP32 display on LCD and use touch capabilities to move menus

# Project 3b:
* Use touch screen to change color on super LED (APA106) or an RGB LED.

# Project 4:
* Blink LED with FPGA
* Build a clock divider module and blink 2 or 3 different LEDs at different rates

# Project 5:
* esp 32 connected to FPGA, touch a number on the ESP32/LCD Touch screen output a 4 bit number on 4 pins. those 4 pins are connected to FPGA and displays the number on a 7 segment display

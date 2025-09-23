<h2>Main Research</h2>

* What size and resolution display do we want  
    * Under 4” but above 2”, probably to higher end  
* What interface do we want it to have  
    * Good info here: https://tailorpixels.com/6-most-common-types-of-display-interfaces/  
    * MCU is entirely what this whole thing is, a display powered by a microcontroller  
    * SPI, slow transmission (in the low Mbps) of SPI will make the screen  look laggy and it can only handle low res  
    * TTL-RGB looks promising, used in small to medium displays and is fast  
    * MIPI-DSI looks the best and most widely used  
    * EDP will have way too much overhead and will be too much for the STM32  

| <h3>Port Type</h3>| <h3>Displays</h3>    |
| ----------- | ----------- |
| <h4>SPI</h4>| [ADAFRUIT 2.13 MONOCHROME E-INK B](https://www.digikey.com/en/products/detail/adafruit-industries-llc/4687/13157992) <br> • Uses less power, good in sun, although might have issues with RF and refresh rate         |
| <h4>TTL-RGB</h4>| [GRAPHIC DISPLAY FSTN BLACK (WHITE)](https://www.digikey.com/en/products/detail/newhaven-display-intl/NHD-C12864LZ-FSW-FBW-3V3/2625201)  <br> •	Good in sun, although low res and black and white    |
|<h4>MIPI-DSI</h4>|[GRAPHIC DISPLAY TFT RGB 3"](https://www.digikey.com/en/products/detail/focus-lcds/E30RB-I-MW340-N/13683633)  <br> •	Bad in sun, much higher res and is in color  |
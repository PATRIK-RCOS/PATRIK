<h1>Displays</h1>
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

<h2>Display Decision Tables</h2>
<h3>SPI</h3>

| Link     | Size    | Resolution|Type|Mode|Cost|Notes|
| -------- | ------- |-------    |--- | ---|----|-----|
|[4687](https://www.digikey.com/en/products/detail/adafruit-industries-llc/4687/13157992)      |2.13”    |250x122    |E-Ink|Reflective|$19.95|
<h3>TTL-RGB</h3>

| Link     | Size    | Resolution|Type|Mode|Cost|Notes|
| -------- | ------- |-------    |--- | ---|----|-----|
|[NHD-C12864LZ-FSW-FBW-3V3](https://www.digikey.com/en/products/detail/newhaven-display-intl/NHD-C12864LZ-FSW-FBW-3V3/2625201)      |3.17”    |128x64    |FSTN|Transflective|$17.64|
|[NHD-3.5-320240JF-ASXP-T](https://www.digikey.com/en/products/detail/newhaven-display-intl/NHD-3-5-320240JF-ASXP-T/16677483)|3.5”    |320x240 |TFT-Color|Transmissive|$42.42|
<h3>MIPI-DSI</h3>

| Link     | Size    | Resolution|Type|Mode|Cost|Notes|
| -------- | ------- |-------    |--- | ---|----|-----|
|[E30RB-I-MW340-N](https://www.digikey.com/en/products/detail/focus-lcds/E30RB-I-MW340-N/13683633)|3”    |640x360|TFT-Color|Transmissive|$34.45|
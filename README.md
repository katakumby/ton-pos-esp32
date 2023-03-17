# ton-pos-esp32

## Idea description 
We are excited to present our solution for merchants to process daily transactions, without relying on a single payment solution, through the use of Dex integration. This repository showcases the proof-of-concept of an affordable prototype device of approximately $40 that can be connected to Dex (wifi) and controlled by Bluetooth simultaneously while maintaining an extended battery lifetime (minimum of a few months). Device can be extended by optional receipt printer module to print onchain information to real world in a form of NFT.


## Example scenario
Our device can be efficiently used by merchant employees to control the flow of orders. The device's user can quickly perform token/currency pairs swaps and generate receipts with on-chain confirmations/informations. 
Used modules:
- Board Espressif ESP32 DevKit ESP-WROOM-32 V2 7,7$ (https://sklep.msalamon.pl/produkt/esp32-devkit-esp-wroom-32-v2/)
- E-Ink screen 34$ (https://elty.pl/pl/p/Modul-z-wyswietlaczem-E-Ink-4.2-400x300/2036, after tests we consider it too big)
- NFC PN532 reader ,7$ (https://kamami.pl/rfid/564011-modrfid-nfc-pn532-modul-bezstykowej-identyfikacji-rfidnfc.html) 
- 3D Print - priceless :) check example betacase.stl (https://cults3d.com/en/3d-model/gadget/tonpos-beta-example)
![alt text](https://files.cults3d.com/uploaders/27558711/illustration-file/e9c71df2-d33f-4180-a889-bd99da1e45e6/Przechwytywanie.png "Example case")


Optional module:
- Recipt printer (comming soon)

## Wiring guide

### E-INK Screen 
```
#define RST_PIN         14
#define DC_PIN          27
#define CS_PIN          26
#define BUSY_PIN        12
```
### NFC 
```
PN532_HSU pn532hsu(Serial2); <-- Serial  
```

### 3D Print
Import STL file into slicer, (im using cura) set:
- Infill 70%
- Layer 0.2mm

Examples printed on Ender 3 S1 Plus.

### How to compile
Firstly install dependencies from libraries.zip( right now its a compressed modified version  version of 3rd party libraries with resolved conflicts)

Then using Arduino IDE open  ble_with_screen.ino, remember to set and import ESP32 board ex. https://www.upesy.com/blogs/tutorials/install-esp32-on-arduino-ide-complete-guide



# SDK
Here is repo in which you can find example how to connect straight from the browser with device and execute DeepLink action.
Repo link: https://github.com/Kubaszenko/TonpayIoTSDK



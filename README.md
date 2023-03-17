# ton-pos-esp32

## Whats this about? 
We want to create open way for merchants to process transactions without a need to rely on current processors.
This repository contains basic PoC of approx $40 prototype device that can be connected to DEX (wifi) and controlled by bluetoth at the same time keeping very long vattery lifetime ( some charging may be beedev once per few months)

## Example scenario
Device can be used by Merchant employee just to control flow of orders, device can automaticlly handle tokens / currency pairs additionally with option for reciept print with onchain confirmations / informations  


Used modules:
- Board Espressif ESP32 DevKit ESP-WROOM-32 V2 7,7$ (https://sklep.msalamon.pl/produkt/esp32-devkit-esp-wroom-32-v2/)
- E-Ink screen 34$ (https://elty.pl/pl/p/Modul-z-wyswietlaczem-E-Ink-4.2-400x300/2036, after tests we consider it too big)
- NFC PN532 reader ,7$ (https://kamami.pl/rfid/564011-modrfid-nfc-pn532-modul-bezstykowej-identyfikacji-rfidnfc.html) 
- 3D Print - priceless :) 

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

# SDK
Repo link: https://github.com/Kubaszenko/TonpayIoTSDK



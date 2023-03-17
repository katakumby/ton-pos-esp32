#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <SPI.h>
#include "epd4in2b_V2.h"
#include "imagedata.h"
#include "epdpaint.h"
#include "qrcode.h"

#include "emulatetag.h"
#include "NdefMessage.h"
#include <PN532_HSU.h>
#include <PN532.h>

PN532_HSU pn532hsu(Serial2);
EmulateTag nfc(pn532hsu);


//#include <NfcAdapter.h>

#define PIN_RED    4 // GIOP23
#define PIN_GREEN  0 // GIOP22
#define PIN_BLUE   2 // GIOP21
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define COLORED 0
#define UNCOLORED 1
#define QRCODE_VERSION 5
Epd epd;

QRCode qrcode;
const uint8_t QR_VERSION = 3;
const uint8_t QR_QUIET_ZONE = 4; // quiet zone all around



/**
 * Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
 * In this case, a smaller image buffer is allocated and you have to
 * update a partial display several times.
 * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
 */
  unsigned char image[16000];
  Paint paint(image, 400, 300);    //width should be the multiple of 8 

char character;
String cmd_message; // Message buffer
//NFC:
uint8_t ndefBuf[256];
NdefMessage message;
int messageSize;

uint8_t uid[3] = {0x12, 0x34, 0x56};

void setColor(int R, int G, int B) {
  analogWrite(PIN_RED,   R);
  analogWrite(PIN_GREEN, G);
  analogWrite(PIN_BLUE,  B);
}


void PrintQRCode(const char *url, uint16_t x0, uint16_t y0)
{
//  epd.ClearFrame();
  const int ps = 3; // pixels / square : used to define the size of QR
  uint8_t qrcodeData[qrcode_getBufferSize(QRCODE_VERSION)];
  qrcode_initText(&qrcode, qrcodeData, QRCODE_VERSION, ECC_LOW, url);

  PrintQRPart(x0, y0);
  // PrintQRPart(x0+80, y0+80);
}

void PrintQRPart(uint16_t offsetX, uint16_t offsetY)
{

   paint.SetRotate(ROTATE_90);
  // LOOP
  // paint.Clear(COLORED);
  // paint.SetWidth(80);
  // paint.SetHeight(80);
  paint.Clear(UNCOLORED);
  uint8_t blockSize = (EPD_HEIGHT - (2 * QR_QUIET_ZONE)) / qrcode.size; 
  offsetX = offsetX + blockSize;
  offsetY = offsetY +  blockSize;
  //qrcode.size = 26;
  uint8_t page = 0;

  uint8_t perPage = 64 / blockSize;//WTF

  for (uint8_t y = 0; y < qrcode.size; y++)
  {

    
    for (uint8_t x = 0; x < qrcode.size; x++)
    {
      if (qrcode_getModule(&qrcode, x, y))
      {
        
        printQRBlock(offsetX + (x * blockSize) + QR_QUIET_ZONE,
                     offsetY + (y * blockSize)+100 + QR_QUIET_ZONE,
                     blockSize,
                     (qrcode_getModule(&qrcode, x, y)) ? COLORED : UNCOLORED);
        // epd.SetPartialWindowBlack(paint.GetImage(), x, y, paint.GetWidth(), paint.GetHeight());
      }
    }


  }

  // epd.EPD_ClearScreen_White();  // Display whole screen in white
  // epd.EPD_Dis_Part(30, 200, paint.GetImage(), paint.GetWidth(), paint.GetHeight());
  // epd.EPD_UpdateDisplay();
  // epd.ClearFrame();
  // epd.SetPartialWindowRed(paint.GetImage(), paint.GetWidth(), paint.GetHeight());
 paint.DrawStringAt(blockSize, blockSize, "Status: WAITING", &Font20, COLORED);
 paint.DrawStringAt(blockSize, blockSize+40, "Total: 100 TON", &Font24, COLORED);
  epd.SetPartialWindowBlack(paint.GetImage(), offsetX, offsetY, paint.GetWidth(), paint.GetHeight());
//  
  
//   
  epd.DisplayFrame();

  
  
}
void printQRBlock(uint16_t x, uint16_t y, uint8_t size, uint16_t col)
// Draw a square block of size pixels. Drawing individual pixels as this is faster
// that line segments and much faster that a filled rectangle.
{
  for (uint8_t i = 0; i < size; i++)
    for (uint8_t j = 0; j < size; j++)
      paint.DrawPixel(x + i, y + j, col);
}
void testNfc()
{

  Serial.println("\ntestNfc occured !");
  message = NdefMessage();
  message.addUriRecord(cmd_message);
  messageSize = message.getEncodedSize();
  if (messageSize > sizeof(ndefBuf))
  {
    Serial.println("ndefBuf is too small");
    while (1)
    {
    }
  }

  Serial.print("Ndef encoded message size: ");
  Serial.println(messageSize);
  Serial.print("cmd_message: ");

  Serial.println(cmd_message);

  message.encode(ndefBuf);

  // comment out this command for no ndef message
  nfc.setNdefFile(ndefBuf, messageSize);

  nfc.emulate();

  Serial.println("\n exit Nfc!");
  delay(1000);
}
class DeepLink: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        setColor(255,178,102);//orange
        
        Serial.println("*********");
        Serial.print("New value: ");
        for (int i = 0; i < value.length(); i++){
          cmd_message.concat(value[i]);
          Serial.print(value[i]);
        }

      PrintQRCode(cmd_message.c_str(),0,0);
      setColor(0,255,0);//green?
       testNfc();
setColor(255,178,102);//orange
        cmd_message = "";
        Serial.println();
        Serial.println("*********");

      }
    }
};


void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT);

  setColor(255,0,0);


  BLEDevice::init("TonPos");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setValue("#blank");
  pCharacteristic->setCallbacks(new DeepLink());
  
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");

   if (epd.Init() != 0)
  {
    Serial.println("e-Paper init failed2");
    return;
  }
  else
  {
    Serial.println("e-Paper init SUCCESS3");
  }

  /* This clears the SRAM of the e-paper display */
 epd.ClearFrame();

//  PrintQRCode("https://executeit.pl/about-us/",0,0);
  /* Deep sleep */
  // epd.Sleep();
  Serial.println("------- Emulate Tag --------");
  nfc.init();
  Serial.println("------- NFC INIT--------");
setColor(0,255,0);
  
}




void loop(void)
{
   delay(2000);
}

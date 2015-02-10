/**************************************************************************/
/*! 
    @Modified FROM:
    
    @file     readMifareClassic.pde
    @author   Adafruit Industries
	@license  BSD (see license.txt)

    This example will wait for any ISO14443A card or tag, and
    depending on the size of the UID will attempt to read from it.
   
    If the card has a 4-byte UID it is probably a Mifare
    Classic card, and the following steps are taken:
   
    Reads the 4 byte (32 bit) ID of a MiFare Classic card.
    Since the classic cards have only 32 bit identifiers you can stick
	them in a single variable and use that to compare card ID's as a
	number. This doesn't work for ultralight cards that have longer 7
	byte IDs!
   
    Note that you need the baud rate to be 115200 because we need to
	print out the data and read from the card at the same time!

This is an example sketch for the Adafruit PN532 NFC/RFID breakout boards
This library works with the Adafruit NFC breakout 
  ----> https://www.adafruit.com/products/364
 
Check out the links above for our tutorials and wiring diagrams 
These chips use SPI to communicate, 4 required to interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!
*/
/**************************************************************************/

#include <Adafruit_PN532.h>

#define SCK  (2)
#define MOSI (5)
#define SS   (4)
#define MISO (3)

//#define SCK  (15)
//#define MOSI (14)
//#define SS   (10)
//#define MISO (16)

Adafruit_PN532 nfc(SCK, MISO, MOSI, SS);

void setup(void) {
  //Keyboard.begin(115200);
  delay(1000);  //used to allow keyboard to register on computer
  //Keyboard.println("delay 1000 123");

  nfc.begin();
  
  uint32_t versiondata = nfc.getFirmwareVersion();
  Keyboard.println(versiondata);
  if (! versiondata) {
    Keyboard.println("Didn't find PN53x board");
    while (1); // halt
  }
  // Got ok data, print it out!
  Keyboard.print("Found chip PN5"); Keyboard.println((versiondata>>24) & 0xFF, HEX); 
  Keyboard.print("Firmware ver. "); Keyboard.print((versiondata>>16) & 0xFF, DEC); 
  Keyboard.print('.'); Keyboard.println((versiondata>>8) & 0xFF, DEC);
  
  // configure board to read RFID tags
  nfc.SAMConfig();
  
  Keyboard.println("Waiting for an ISO14443A Card ...");
}


void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  
  if (success) {
    // Display some basic information about the card
    //Keyboard.println("Found an ISO14443A card");
    //Keyboard.print("  UID Length: ");Keyboard.print(uidLength, DEC);Keyboard.println(" bytes");
    //Keyboard.print("  UID Value: ");
    //nfc.PrintHex(uid, uidLength);
    
    if (uidLength == 4)
    {
      // We probably have a Mifare Classic card ... 
      uint32_t cardid = uid[0];
      cardid <<= 8;
      cardid |= uid[1];
      cardid <<= 8;
      cardid |= uid[2];  
      cardid <<= 8;
      cardid |= uid[3]; 
      //Keyboard.print("Seems to be a Mifare Classic card #");
      Keyboard.println(cardid);
    }
    delay(3000);  //used to allow check in system to display the print dialog
    Keyboard.println("");  //enter or return 
  }
}


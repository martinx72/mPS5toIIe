// by MarTinX ,  martinx72@yahoo.com
// Working with Arduino UNO with USB Host Shield,
// You need include the USB Host Shield 2.0 library.

#include <PS5USB.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
PS5USB PS5(&Usb);

union SNKBtns {
        struct {
                uint8_t left : 1;
                uint8_t right : 1;
                uint8_t down : 1;
                uint8_t up : 1;
                uint8_t start : 1;
                uint8_t select : 1;
                uint8_t touchpad : 1;
                uint8_t ps : 1;
                
                uint8_t square : 1;
                uint8_t cross : 1;
                uint8_t circle : 1;
                uint8_t triangle : 1;
                uint8_t l1 : 1;
                uint8_t r1 : 1;
                uint8_t l2 : 1;
                uint8_t r2 : 1;
        } __attribute__((packed));
        uint16_t val;
} __attribute__((packed));

SNKBtns btn;
SNKBtns prevbtn;

#define SW_0 A0
#define SW_1 A1

uint16_t lastMessageCounter = -1;

void setup() {

  pinMode(2, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(7, INPUT);
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);

  pinMode(SW_0, OUTPUT);
  digitalWrite(SW_0, LOW);
  pinMode(SW_1, OUTPUT);
  digitalWrite(SW_1, LOW);
        
  prevbtn.val = 0xffff;
  btn.val = 0xffff;
    
  if (Usb.Init() == -1) {
    while (1); // Halt
  }
}

void loop() {
  Usb.Task();

  if (PS5.connected() && lastMessageCounter != PS5.getMessageCounter()) {
    lastMessageCounter = PS5.getMessageCounter();

    if (PS5.getButtonPress(LEFT))
      btn.left = 0;
    if (PS5.getButtonPress(RIGHT))
      btn.right = 0;
    if (PS5.getButtonPress(DOWN))
      btn.down = 0;
    if (PS5.getButtonPress(UP))
      btn.up = 0;

    if (PS5.getButtonPress(CROSS))
      btn.cross = 0;
    if (PS5.getButtonPress(CIRCLE))
      btn.circle = 0;
      
    if ( btn.cross != prevbtn.cross )
      digitalWrite(SW_0, !btn.cross);

    if ( btn.circle != prevbtn.circle )
      digitalWrite(SW_1, !btn.circle);
      
    // D-Pad
    pinMode(2, INPUT);
    pinMode(4, INPUT);
    pinMode(5, INPUT);
    pinMode(7, INPUT);
    pinMode(3, OUTPUT);
    digitalWrite(3, HIGH);
    pinMode(6, OUTPUT);
    digitalWrite(6, HIGH);
    if ( !btn.up )
    {
      pinMode(3, INPUT);
      pinMode(2, OUTPUT);
      digitalWrite(2, HIGH);
    }
    else if ( !btn.down )
    {
      pinMode(3, INPUT);
      pinMode(4, OUTPUT);
      digitalWrite(4, HIGH);
    }
      
    if ( !btn.right )
    {
      pinMode(6, INPUT);
      pinMode(7, OUTPUT);
      digitalWrite(7, HIGH);
    }
    else if ( !btn.left )
    {
      pinMode(6, INPUT);
      pinMode(5, OUTPUT);
      digitalWrite(5, HIGH);
    }
      
    prevbtn.val = btn.val;
    btn.val = 0xffff;      
  }
  
}

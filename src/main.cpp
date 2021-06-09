#include <Arduino.h>
#include <Wire.h>

unsigned short lenth_val = 0;
unsigned char i2c_rx_buf[16];

void SensorRead(unsigned char addr, unsigned char* datbuf, unsigned char cnt) 
{
  unsigned short result=0;
  // step 1: instruct sensor to read echoes
  Wire.beginTransmission(82); // transmit to device #82 (0x52), you can also find this address using the i2c_scanner code, which is available on electroniclinic.com
  // the address specified in the datasheet is 164 (0xa4)
  // but i2c adressing uses the high 7 bits so it's 82
  Wire.write(byte(addr));      // sets distance data address (addr)
  Wire.endTransmission();      // stop transmitting
  // step 2: wait for readings to happen
  delay(1);                   // datasheet suggests at least 30uS
  // step 3: request reading from sensor
  Wire.requestFrom(82, cnt);    // request cnt bytes from slave device #82 (0x52)
  // step 5: receive reading from sensor
  if (cnt <= Wire.available()) { // if two bytes were received
    *datbuf++ = Wire.read();  // receive high byte (overwrites previous reading)
    *datbuf++ = Wire.read();  // receive low byte as lower 8 bits
  }
}

int ReadDistance(){
  SensorRead(0x00, i2c_rx_buf, 2);
  lenth_val=i2c_rx_buf[0];
  lenth_val=lenth_val<<8;
  lenth_val|=i2c_rx_buf[1];
  delay(30); 
  return lenth_val;
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
}

void loop() {
  int x_mm = ReadDistance();
  Serial.print(x_mm);
  Serial.println(" mm");
}
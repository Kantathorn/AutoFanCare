// usb
#include <usbdrv.h>
#define RQ_SET_FAN 0

// sensor
#include <OneWire.h>
#include <DallasTemperature.h>
//LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//sensor
#define ONE_WIRE_BUS PIN_PC0
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
//LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);
//FAN
int INA = PIN_PC3; 
int INB = PIN_PC2;
int status = 0;

usbMsgLen_t usbFunctionSetup(uint8_t data[8])
{
  usbRequest_t *rq = (usbRequest_t*)data;
  if (rq->bRequest == RQ_SET_FAN)
  {
    uint8_t fan_val = rq->wValue.bytes[0];

    if (fan_val == 0){
      digitalWrite(INA,LOW);
      digitalWrite(INB,LOW);
      status = 0;
    }
    else if (fan_val == 1) {
      digitalWrite(INA,LOW);
      digitalWrite(INB,HIGH);
      status = 1;
    }
    return 0;  // return no data back to host
  }
}

void setup()
{
  // sensor
  Serial.begin(9600);
  sensors.begin();
  //LCD
  lcd.begin();
  lcd.backlight();
  //FAN
  pinMode(INA,OUTPUT);
  pinMode(INB,OUTPUT); 
  
  usbInit();
  usbDeviceDisconnect();
  delay(300);
  usbDeviceConnect();
} 
 
void loop() 
{
  //sensor
  if (status == 0) 
  {
    usbPoll();
  sensors.requestTemperatures();
  usbPoll();
  float T = sensors.getTempCByIndex(0);
  usbPoll();
  lcd.setCursor(0, 0);
  usbPoll();
  lcd.print("Temperature is : ");
  usbPoll();
  lcd.setCursor(0, 1);
  usbPoll();
  lcd.print(T);
  usbPoll();
  lcd.setCursor(5, 1);
  usbPoll();
  lcd.print((char)223);
  usbPoll();
  lcd.setCursor(6, 1);
  usbPoll();
  lcd.print("C");
  usbPoll();
  lcd.setCursor(9, 1);
  usbPoll();
  lcd.print(sensors.getTempFByIndex(0));
  usbPoll();
  lcd.setCursor(14, 1);
  usbPoll();
  lcd.print((char)223);
  usbPoll();
  lcd.setCursor(15, 1);
  usbPoll();
  lcd.print("F");
  usbPoll();
  //FAN
  if (T >= 33.2)
  {
    usbPoll();
    digitalWrite(INA, LOW);
    usbPoll();
    digitalWrite(INB, HIGH); 
    usbPoll();
  }else{
    usbPoll();
    digitalWrite(INA, LOW);
    usbPoll();
    digitalWrite(INB, LOW); 
    usbPoll();
  } 
  usbPoll();
  }
  else if (status == 1) {
    while (status != 0){
      usbPoll();
      digitalWrite(INA,LOW);
      usbPoll();
      digitalWrite(INB,HIGH);
      usbPoll();
      lcd.setCursor(0, 0);
      usbPoll();
      lcd.print("Auto Mode: OFF");
    }
  }
  
}

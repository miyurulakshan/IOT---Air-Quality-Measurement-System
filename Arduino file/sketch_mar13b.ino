
//Include the library
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <MQUnifiedsensor.h>
/************************Hardware Related Macros************************************/
#define         Board                   ("Arduino UNO")
#define         Pin0                     (A0)  //Analog input 2 of your arduino
#define         Pin1                     (A1)  //Analog input 3 of your arduino
#define         Pin2                     (A2)  //Analog input 4 of your arduino
#define         Pin3                     (A3)  //Analog input 5 of your arduino
#define         Buz                       (8)  // Buzzer
#define         Red                       (7)  // Red light
#define         Ap                        (9)


/***********************Software Related Macros************************************/

#define         RatioMQ135CleanAir        (3.6)//RS / R0 = 3.6 ppm  
#define         RatioMQ6CleanAir          (10) //RS / R0 = 10 ppm 
#define         RatioMQ7CleanAir          (27.5) //RS / R0 = 27.5 ppm  
#define         RatioMQ9CleanAir          (9.6) //RS / R0 = 9.6 ppm 

#define         ADC_Bit_Resolution        (10) // 10 bit ADC 
#define         Voltage_Resolution        (5) // Volt resolution to calc the voltage
#define         Type                      ("Arduino UNO") //Board used
/*****************************Globals***********************************************/
//Declare Sensor

MQUnifiedsensor MQ135_1(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin0, Type);
MQUnifiedsensor MQ135_2(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin0, Type);
MQUnifiedsensor MQ135_3(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin0, Type);
MQUnifiedsensor MQ135_4(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin0, Type);
MQUnifiedsensor MQ135_5(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin0, Type);
MQUnifiedsensor MQ135_6(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin0, Type);

MQUnifiedsensor MQ6(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin2, Type);
MQUnifiedsensor MQ7(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin3, Type);
MQUnifiedsensor MQ9(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin1, Type);

unsigned long oldTime = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  pinMode(Buz, OUTPUT);
  pinMode(Ap, OUTPUT);
  pinMode(Red, OUTPUT);

  lcd.init();         // initialize the lcd
  lcd.backlight();    // Turn on the LCD screen backlight

  //Init serial port
  Serial.begin(9600);
  //init the sensor
 
 //----------------------------------------------------------------------------------------------------------------------------------------------------
  
  MQ135_1.init(); 
  MQ135_1.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ135_1.setA(605.18); MQ135_1.setB(-3.937); // Configure the equation to calculate CO concentration value
  MQ135_1.setR0(530);


  MQ135_2.init(); 
  MQ135_2.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ135_2.setA(77.255); MQ135_2.setB(-3.18); //Configure the equation to calculate Alcohol concentration value
  MQ135_2.setR0(530);

  MQ135_3.init(); 
  MQ135_3.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ135_3.setA(110.47); MQ135_3.setB(-2.862); // Configure the equation to calculate CO2 concentration value
  MQ135_3.setR0(530);

  MQ135_4.init(); 
  MQ135_4.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ135_4.setA(44.947); MQ135_4.setB(-3.445); // Configure the equation to calculate Toluen concentration value
  MQ135_4.setR0(530);

  MQ135_5.init(); 
  MQ135_5.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ135_5.setA(102.2 ); MQ135_5.setB(-2.473); // Configure the equation to calculate NH4 concentration value
  MQ135_5.setR0(530);

  MQ135_6.init(); 
  MQ135_6.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ135_6.setA(34.668); MQ135_6.setB(-3.369); // Configure the equation to calculate Aceton concentration value
  MQ135_6.setR0(530);

  //----------------------------------------------------------------------------------------------------------------------------------------------------
  MQ6.init(); 
  MQ6.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ6.setA(2127.2); MQ6.setB(-2.526); // Configure the equation to to calculate CH4 concentration
  MQ6.setR0(13.4285714);
  
  MQ7.init(); 
  MQ7.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ7.setA(99.042); MQ7.setB(-1.518); // Configure the equation to calculate CO concentration value
  MQ7.setR0(4);

  MQ9.init(); 
  MQ9.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ9.setA(1000.5); MQ9.setB(-2.186); // Configure the equation to to calculate LPG concentration
  MQ9.setR0(9.42857143);


  Serial.print("Calibrating please wait.");
  lcd.print("Calibrating");
  lcd.setCursor(0, 1);

  float MQ135_1calcR0 = 0; 
  float MQ135_2calcR0 = 0; 
  float MQ135_3calcR0 = 0; 
  float MQ135_4calcR0 = 0;
  float MQ135_5calcR0 = 0; 
  float MQ135_6calcR0 = 0;
  float MQ2calcR0 = 0; 
  float MQ6calcR0 = 0;
  float MQ7calcR0 = 0;
  float MQ9calcR0 = 0;




  for(int i = 1; i<=15; i ++)
  {
    //Update the voltage Values

    MQ135_1.update();
    MQ135_2.update();
    MQ135_3.update();
    MQ135_4.update();
    MQ135_5.update();
    MQ135_6.update();

    MQ6.update();
    MQ7.update();
    MQ9.update();

  
    MQ135_1calcR0 += MQ135_1.calibrate(RatioMQ135CleanAir);
    MQ135_2calcR0 += MQ135_2.calibrate(RatioMQ135CleanAir);
    MQ135_3calcR0 += MQ135_3.calibrate(RatioMQ135CleanAir);
    MQ135_4calcR0 += MQ135_4.calibrate(RatioMQ135CleanAir);
    MQ135_5calcR0 += MQ135_5.calibrate(RatioMQ135CleanAir);
    MQ135_6calcR0 += MQ135_6.calibrate(RatioMQ135CleanAir);

    MQ6calcR0 += MQ6.calibrate(RatioMQ6CleanAir);
    MQ7calcR0 += MQ7.calibrate(RatioMQ7CleanAir);
    MQ9calcR0 += MQ9.calibrate(RatioMQ9CleanAir);

    Serial.print(".");
    lcd.print(".");
  }
  MQ135_1.setR0(MQ135_1calcR0/20);
  MQ135_2.setR0(MQ135_2calcR0/20);
  MQ135_3.setR0(MQ135_3calcR0/20);
  MQ135_4.setR0(MQ135_4calcR0/20);
  MQ135_5.setR0(MQ135_5calcR0/20);
  MQ135_6.setR0(MQ135_6calcR0/20);

  MQ6.setR0(MQ6calcR0/20);
  MQ7.setR0(MQ7calcR0/20);
  MQ9.setR0(MQ9calcR0/20);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Done!");
  delay(2000);
  Serial.println("  done!.");
  
  Serial.print("R0 values for each sensors (MQ2 - MQ9):");

  Serial.print(MQ135_1calcR0/10); Serial.print(" |");
  Serial.print(MQ135_2calcR0/10); Serial.print(" |");
  Serial.print(MQ135_3calcR0/10); Serial.print(" |");
  Serial.print(MQ135_4calcR0/10); Serial.print(" |");
  Serial.print(MQ135_5calcR0/10); Serial.print(" |");
  Serial.print(MQ135_6calcR0/10); Serial.print(" |");

  Serial.print(MQ6calcR0/10); Serial.print(" | ");
  Serial.print(MQ7calcR0/10); Serial.print(" | ");
  Serial.print(MQ9calcR0/10); Serial.println(" |");

  if( isinf(MQ135_1calcR0) ||isinf(MQ135_2calcR0) ||isinf(MQ135_3calcR0) ||isinf(MQ135_4calcR0) ||isinf(MQ135_5calcR0) ||isinf(MQ135_6calcR0) || isinf(MQ6calcR0) || isinf(MQ7calcR0) || isinf(MQ9calcR0)) {Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply"); while(1);}
  if( MQ135_1calcR0 == 0 ||MQ135_2calcR0 == 0 ||MQ135_3calcR0 == 0 ||MQ135_4calcR0 == 0 ||MQ135_5calcR0 == 0 ||MQ135_6calcR0 == 0 || MQ6calcR0 == 0 || MQ7calcR0 == 0 || MQ9calcR0 == 0){Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply"); while(1);}
  /*****************************  MQ CAlibration ********************************************/ 
 
  //Print in serial monitor
  Serial.println(F("MQ6 to MQ9 - lecture"));
  Serial.println(F("*************************** Values from MQ-board ***************************"));
  Serial.println(F("|   CO    |   Alchohol    |   CO2      |   Toluen    |   NH4      |   Aceton  |   CH4    |    CO    |    Flamable gases   |"));  
  Serial.println(F("| MQ-135_1|   MQ-135_2    |   MQ-13_3  |   MQ-135_4  |   MQ-135_5 |  MQ-135_6 |   MQ-6   |   MQ-7   |   MQ-9              |"));  
  //pinMode(calibration_button, INPUT);
}

void loop() {
  oldTime = millis();
  while(millis() - oldTime <= (60*1000))
  { 
    // VH 5 Volts
    analogWrite(5, 255); // 255 is DC 5V output
    readAllSensors();
    delay(1000);
  }
  // 90s cycle
  oldTime = millis();
  while(millis() - oldTime <= (90*1000))
  {
    // VH 1.4 Volts
    analogWrite(5, 20); // 255 is 100%, 20.4 is aprox 8% of Duty cycle for 90s
    readAllSensors();
    delay(500);
  }
}

void readAllSensors()
{
  //-------------------------
  float MQ135_1Lecture ;
  float MQ135_2Lecture ;
  float MQ135_3Lecture ;
  float MQ135_4Lecture   ;
  float MQ135_5Lecture   ;
  float MQ135_6Lecture  ;
  float MQ6Lecture  ;
  float MQ7Lecture  ;
  float MQ9Lecture  ;
  //Update the voltage Values

  MQ135_1.update();
  MQ135_2.update();
  MQ135_3.update();
  MQ135_4.update();
  MQ135_5.update();
  MQ135_6.update();

  MQ6.update();
  MQ7.update();
  MQ9.update();


  MQ135_1Lecture =  MQ135_1.readSensor();
  MQ135_2Lecture =  MQ135_2.readSensor();
  MQ135_3Lecture =  MQ135_3.readSensor();
  MQ135_4Lecture =  MQ135_4.readSensor();
  MQ135_5Lecture =  MQ135_5.readSensor();
  MQ135_6Lecture =  MQ135_6.readSensor();

  MQ6Lecture =  MQ6.readSensor();
  MQ7Lecture =  MQ7.readSensor();
  MQ9Lecture =  MQ9.readSensor();

//------------------------------------------------------------------------------------------------------------------------------------



  if( (MQ135_1Lecture >= 100) ||  (MQ135_2Lecture >= 300) ||  (MQ135_3Lecture >= 1000) || (MQ135_4Lecture >= 300) || (MQ135_5Lecture >= 100) || (MQ135_6Lecture >= 100) || (MQ6Lecture >= 300) || (MQ7Lecture >= 300) || (MQ9Lecture >= 20))
  { 
    int i = 0;
    while(1){
    digitalWrite(Buz, HIGH);
    digitalWrite(Red, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("Warning: Poor air");
    lcd.setCursor(0, 1);
    lcd.print("quality detected!");
    delay(5000);

    if(i == 0 ){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Turning ON the");
      lcd.setCursor(0, 1);
      lcd.print("Air Purifier");
      digitalWrite(Ap, HIGH);
      delay(5000);
      lcd.clear();
      i = 1;
    }
    
  
    MQ135_1.update();
    MQ135_2.update();
    MQ135_3.update();
    MQ135_4.update();
    MQ135_5.update();
    MQ135_6.update();
    MQ6.update();
    MQ7.update();
    MQ9.update();

    MQ135_1Lecture =  MQ135_1.readSensor();
    MQ135_2Lecture =  MQ135_2.readSensor();
    MQ135_3Lecture =  MQ135_3.readSensor();
    MQ135_4Lecture =  MQ135_4.readSensor();
    MQ135_5Lecture =  MQ135_5.readSensor();
    MQ135_6Lecture =  MQ135_6.readSensor();
    MQ6Lecture =  MQ6.readSensor();
    MQ7Lecture =  MQ7.readSensor();
    MQ9Lecture =  MQ9.readSensor();
    

    if((MQ135_1Lecture < 100) &&  (MQ135_2Lecture < 300) &&  (MQ135_3Lecture < 1000) && (MQ135_4Lecture < 300)&& (MQ135_5Lecture < 100) && (MQ135_6Lecture < 100) && (MQ6Lecture < 300) && (MQ7Lecture < 300) && (MQ9Lecture < 20)){
    digitalWrite(Buz, LOW);
    digitalWrite(Red, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Turning OFF the");
    lcd.setCursor(0, 1);
    lcd.print("Air Purifier");
    delay(3000);
    digitalWrite(Ap, LOW);
    lcd.clear();

    break;
    }
    }
    

  }

  lcd.clear();
 
  //Serial.print(F("|   ")); Serial.print(MQ135_1Lecture); 

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PM2.5:");lcd.print(MQ135_1Lecture+8.74);lcd.print(" mg/m3");

  //Serial.print(F("   |   ")); Serial.print(MQ135_2Lecture);

  lcd.setCursor(0, 1);
  lcd.print("Ozone:");lcd.print(MQ135_2Lecture+29);lcd.print(" PPM");
  delay(1500);
  lcd.clear();

  //Serial.print(F("   |   ")); Serial.print( MQ135_3Lecture+ 400); 

  lcd.setCursor(0, 0);
  lcd.print("CO2 :");lcd.print(MQ135_3Lecture+405);lcd.print(" PPM");
  
  //Serial.print(F("   |   ")); Serial.print(MQ135_4Lecture); 

  lcd.setCursor(0, 1);
  lcd.print("Toluene:");lcd.print(MQ135_4Lecture);lcd.print(" PPM");
  delay(1500);
  lcd.clear();
  
  //Serial.print(F("   |   ")); Serial.print(MQ135_5Lecture); 

  lcd.setCursor(0, 0);
  lcd.print("NH4: ");lcd.print(MQ135_5Lecture-1);lcd.print(" PPM");
  
  //Serial.print(F("   |   ")); Serial.print(MQ135_6Lecture);

  lcd.setCursor(0, 1);
  lcd.print("Aceton:");lcd.print(MQ135_6Lecture);lcd.print(" PPM");
  delay(1500);
  lcd.clear();

  //Serial.print(F("   |   ")); Serial.print(MQ6Lecture);

  lcd.setCursor(0, 0);
  lcd.print("CH4: ");lcd.print(MQ6Lecture);lcd.print(" PPM");

  //Serial.print(F("   |   ")); Serial.print(MQ7Lecture);

  lcd.setCursor(0, 1);
  lcd.print("CO: ");lcd.print(MQ7Lecture);lcd.print(" PPM");
  delay(1500);
  lcd.clear();

  //Serial.print(F("   |   ")); Serial.print(MQ9Lecture);
  //Serial.println("|");

  lcd.setCursor(0, 0);
  lcd.print("Fl.Gas:");lcd.print(MQ9Lecture);lcd.print(" PPM");
  delay(1500);
  lcd.clear();
  

 // output the data to the serial monitor to get the input values for ESP 32 board via TX/ RX pins
 // Arduino TX -  ESP 32 RS
 // Arduino RX - ESP 32 TX
 
  Serial.print(MQ135_1Lecture+8.74);
  Serial.print(",");
  Serial.print(MQ135_2Lecture+29);
  Serial.print(",");
  Serial.print(MQ135_3Lecture+405);
  Serial.print(",");
  Serial.print(MQ7Lecture);
  Serial.print(",");
  Serial.print(MQ9Lecture);
  Serial.print("\n");

}


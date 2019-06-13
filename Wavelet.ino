
// >> LIBRARIES
#include <Wire.h>           //For I2C Devices such as the MPU-6050
#include <IRremote.h>       //For the IR System
#include <SoftwareSerial.h> //For Serial Devices such as the HC-05 Bluetooth Module
#include <MPU6050.h>        //For the MPU-6050 Sensor

// >> GLOBAL DECLARATIONS
// 1. MPU-6050
MPU6050 mpu;
const int MPU_addr = 0x68;                  //I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;       //Accelerometer & Gyro Variables
char Acx[10], Acy[10], Acz[10],             //String versions of the Variables
     Gyx[10], Gyy[10], Gyz[10];             
int  xpt,ypt,zpt,xnt,ynt,znt;
char  gst[5] = "";
int g  = 0;
int lg = 0;
int gx = 0;
int gy = 0;
int gz = 0;
int mxg = 0;
int tm = 0;
int ggap = 10;
int ssty = 130;

// 2. RGB Module
const int Lred = 9;       //RGB Red value
const int Lgreen = 10;    //RGB Green value
const int Lblue = 11;     //RGB Blue value

// 3. IR Module
IRsend irsend;              
unsigned int irSignal[6];   //Array containing readings to be sent over IR
int khz = 38;               //38kHz carrier frequency (for the NEC protocol)
int cmd = 0;                //Used to toggle and select between cmd1 and cmd2 while sending
unsigned int cmd1[99]  = {4350, 4300, 550, 1550, 550, 500, 550, 1550, 600, 1550, 550, 500, 550, 550, 550, 1550, 550, 500, 550, 500, 550, 1600, 550, 500, 550, 500, 600, 1550, 550, 1550, 600, 450, 600, 1550, 550, 500, 550, 500, 600, 450, 600, 1550, 550, 1550, 600, 1550, 550, 1550, 600, 1500, 600, 1550, 550, 1550, 600, 1550, 550, 500, 600, 450, 600, 450, 600, 500, 550, 500, 500, 600, 550, 500, 550, 500, 600, 450, 600, 1550, 550, 500, 550, 500, 550, 500, 550, 1600, 550, 1550, 600, 1550, 550, 1550, 550, 500, 550, 1550, 550, 1600, 500, 1600, 550};
unsigned int cmd2[100] = {4350, 4300, 600, 1500, 600, 450, 600, 1500, 600, 1550, 600, 450, 600, 500, 550, 1550, 600, 450, 600, 450, 600, 1550, 600, 450, 550, 500, 600, 1550, 550, 1550, 600, 450, 600, 1550, 600, 450, 600, 1500, 600, 1550, 550, 1550, 600, 1550, 600, 450, 600, 1500, 600, 1500, 600, 1550, 600, 450, 600, 500, 600, 450, 600, 450, 600, 1500, 600, 500, 600, 450, 600, 1550, 600, 1500, 600, 1500, 600, 450, 600, 500, 600, 450, 600, 500, 550, 450, 600, 500, 600, 450, 600, 450, 600, 1550, 600, 1500, 600, 1550, 550, 1550, 600, 1500, 600, 5050 };
                            //Really long arrays that each contain a complete IR signal

// 4. Bluetooth Module
SoftwareSerial WaveBlue(7, 8);    //Uses specified pins for serial communication. 
                                  //Format: (RX,TX) -> Recieve/Transmit 

void setup()
{ // >> Initialises various components
  Serial.begin(9600);              //Initialises Serial Port to 9600 bps
  WaveBlue.begin(9600);            //Initialise Bluetooth port to 9600 bps
  pinMode(13, OUTPUT);             //PIN 13 controls the Arduino's indicator light
  
  //INITIALISING SENSOR
  while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) 
  {                                   //Waits till MPU-6050 responds
    Serial.println("Sensor Error");   //Till response, show error via...
    WaveBlue.write("Sensor Error");     //  ...serial and bluetooth
    delay(500);                       //Small delay between error messages
  }

  mpu.setThreshold(1);              //Tells the sensor how far readings should go before...
                                      //    ...raising an interrupt
  xpt=ypt=zpt=xnt=ynt=znt=ssty;
}

void rgbrand()
{ // >> Generates random colors for RGB LED
  analogWrite(Lred, random(0, 255));    //Random amount of red 
  analogWrite(Lgreen, random(0, 255));  //Random amount of green
  analogWrite(Lblue, random(0, 255));   //Random amount of blue
                                        //Combines to form a distinct colour
}

void check_gest()
{ 
  int i;
  if (g==5)
  strcpy(gst,"0000");
  else if (g!=lg&&g!=0)
  {
    for(i=4;i>0;i--) gst[i] = gst[i-1];
    gst[4]='\0';
    gst[0]=g+'0';
    lg=g;
  }
  Serial.print("CODE:\t");
  Serial.println(gst);
  if (strcmp(gst,"9191")==0||strcmp(gst,"1919")==0)
  {
    Serial.println("Z\n"); //delay(1000);
    WaveBlue.write("Z\n"); 
    tm=-1;strcpy(gst,"0000");
  }
  if (strcmp(gst,"7373")==0||strcmp(gst,"3737")==0)
  {
    Serial.println("X\n"); //delay(1000);
    WaveBlue.write("X\n"); 
    tm=-1;strcpy(gst,"0000");
  }
   if (strcmp(gst,"8282")==0||strcmp(gst,"2828")==0)
  {
    Serial.println("Y\n"); //delay(1000);
    WaveBlue.write("Y\n"); 
    tm=-1;strcpy(gst,"0000");
  }
                 
}

void sendir()                 
{ // >> Sends VOLTAS ON/OFF signal
  if(cmd==1) irsend.sendRaw(cmd1, sizeof(cmd1), khz);     //Transmits ON/OFF command via IR...
  else irsend.sendRaw(cmd2,sizeof(cmd2), khz);              //  ...and toggles each time
  digitalWrite(13, HIGH); cmd=abs(cmd-1);                 //Blinks Arduino LED and toggles                                                    
}

void loop()
{

  //READING NORMALISED SENSOR VALUES
  
  Vector normGyro  = mpu.readNormalizeGyro();       //Reads Raw Gyroscope values as vector
  
  //char nGx[10]; dtostrf(normGyro.XAxis, 4, 2, nGx); //Converts Norm Gyro X value to string
  //char nGy[10]; dtostrf(normGyro.YAxis, 4, 2, nGy); //Converts Norm Gyro Y value to string
  //char nGz[10]; dtostrf(normGyro.ZAxis, 4, 2, nGz); //Converts Norm Gyro Z value to string
  
  //Serial.print("GYRO: \t");Serial.print(nGx);            //Shares Norm Gyro X value over Serial
  //Serial.print("\t");Serial.print(nGy);            //Shares Norm Gyro Y value over Serial
  //erial.print("\t");Serial.println(nGz);          //Shares Norm Gyro Z value over Serial
  
  //WaveBlue.write(nGx); WaveBlue.write("\t");       //Shares Norm Gyro X value over Bluetooth
  //WaveBlue.write(nGy); WaveBlue.write("\t");       //Shares Norm Gyro Y value over Bluetooth
  //WaveBlue.write(nGz); WaveBlue.write("\n\n");    //Shares Norm Gyro Z value over Bluetooth

  
  
  mxg = 0;gx = 0; gy = 0; gz = 0;g = 0;                             //Reset all 'gest' flags

  
  if (normGyro.XAxis > xpt || normGyro.XAxis < -xnt)              //X-Rotation Detected
  {
    analogWrite(Lred, 0);                                     //Make RGB red value zero
    analogWrite(Lblue, 0);                                    //Make RGB blue value zero
    analogWrite(Lgreen, 255);                                 //Make RGB fully Green
    //delay(100);
    tm=ggap; 
    gx = normGyro.XAxis;                              
  }
  if (normGyro.YAxis > ypt || normGyro.YAxis < -ynt)         //Y-Rotation Detected
  {
    analogWrite(Lred, 255);                                   //Make RGB fully Red
    analogWrite(Lblue, 0);                                    //Make RGB blue value zero
    analogWrite(Lgreen, 0);                                   //Make RGB green value zero
    //delay(100); 
    tm=ggap;
    gy = normGyro.YAxis;                            
  }
  if (normGyro.ZAxis > zpt || normGyro.ZAxis < -znt)         //Z-Rotation Detected
  {
    analogWrite(Lred, 0);                                     //Make RGB red value zero
    analogWrite(Lblue, 255);                                  //Make RGB fully Blue
    analogWrite(Lgreen, 0);                                   //Make RGB green value zero
    //delay(100); 
    tm=ggap;
    gz = normGyro.ZAxis;
  }
  
  if (gx==0||gy==0||gz==0)
  {
    //??
  }
  
  mxg = max(max(abs(gx),abs(gy)),max(abs(gy),abs(gz))); 
  if (mxg!=0)
  {
    if (abs(gx)== mxg) { if(gx>=0) g=1; else g= 9; }
    else if (abs(gy) == mxg) { if(gy>=0) g=2; else g=8; }
    else if (abs(gz) == mxg) { if(gz>=0) g=3; else g=7; }
  }
  else g=0;
  
  tm--;
  if (tm==0)
  {
    analogWrite(Lred, 255);                       //Make RGB Purple by mixing red 
    analogWrite(Lblue, 255);                      //Make RGB Purple by mixing blue
    analogWrite(Lgreen, 0);                       //Make RGB green value zero
    digitalWrite(13, LOW);                        //Turn off Arduino LED
    g=5;
  } 
  if (tm<-1000)
  {
    analogWrite(Lred, 255);                      
    analogWrite(Lblue, 0);                      
    analogWrite(Lgreen, 0);                       
    while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) 
    {                                   //Waits till MPU-6050 responds
      Serial.println("Sensor Error");   //Till response, show error via...
      WaveBlue.write("Sensor Error");     //  ...serial and bluetooth
      analogWrite(Lred, 0);       
      delay(500);                       //Small delay between error messages
      analogWrite(Lred, 255);       
    }
    analogWrite(Lred, 0);       
    delay(500);                       //Small delay between error messages
    analogWrite(Lred, 255);      
    g=5;
  }
  check_gest();                                   //Check whether gesture was detected
  //delay(500);                                     //Slight delay to facilitate proper reading
}

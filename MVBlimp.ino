#include <Arduino.h>


/**************************************************************
 * By Chris Anderson, 2016. Based on original Blimpduino code by Jordi Munoz and Chris Anderson 

All code is freely released under an Apache 2.0 open source licence

 ***************************************************************/
#include <avr/interrupt.h>
#include <avr/io.h> 
#include <SPI.h>
#include <Adafruit_WINC1500.h>
#include <Servo.h>


#define minSpeed 0 //min speed of differential thrusters
<<<<<<< HEAD
#define maxSpeed 250 // max speed of differential thrusters
=======
#define maxSpeed 50 // max speed of differential thrusters
>>>>>>> origin/master
#define anti_static_friction_breaker 80 //This value is power that will be applied to the motor in the first second to kickstart it.. 
// (like the capacitor used to start big electric motors)





//Motor power adjust for navigation mode

#define forward_motor_right 40 //The amount of thrust applied to the right motor when traveling forward, you need to adjust this value if the blimp doesn't fly straight 
#define forward_motor_left 40 //Left motor, same as above..

#define turn_motors 50 // The total thrust applied to the motor when you are making turns (right or left)... Try to put a low value to avoid blimp oscillations.. 

#define PID_dt 100 //The refresh rate in milliseconds   
#define PID_dt_seconds PID_dt/1000 

#define low_battery_limit 3000 //3.0volts

#define read_ch2 (PINB & (1 << 6))//Port B, pin 6 as channel 1;
#define read_ch1 (PINB & (1 << 7))//Port B, pin 7 as channel 2;



//Altitude hold dead zone variables 
//remember we only use one motor at a time. One pushes up and the other pushes down 
#define up_dead_zone 30 // We use a greater value here because the propeller is less efficient in this direction
#define down_dead_zone 20

//PID gains Kp, Ki , Kd
//Holding altitude mode only gains (no navigation).
#define alt_Kp 3 
#define alt_Ki .1
#define alt_Kd 200
//Navigation mode altitude gains.
#define nav_Kp 5
#define nav_Ki 0
#define nav_Kd 0

// ***************************************************************/

/* UltraSonic stuff   */
#define ping_rx 4 // the Maxbotix sensor Rx pin to control ping rate
#define ping_pw 5 // the Maxbotix sensor PWM or Analog output pin
#define ping_mode 1 // 0= digital mode, 1=analog mode. 

#define max16 2000 //max position in useconds
#define min16 1000 //min position useconds

unsigned long timer_ultrasonic=0;
unsigned int altitude=0;
float average_altitude=0;

/*Servo stuff */
#define ServoPin 5
unsigned long timer_servo=0;
byte servo_position=0;
Servo myservo;  // create servo object to control a servo

/*Motor Stuff */
#define MAX_motor 45 //Maximum speed of the motors
#define MIN_motor -45
#define MAX_vect 135 //Maximum pitch of the vector in navigation mode... 
#define MIN_vect 45
<<<<<<< HEAD
#define MotorPin1 5
#define MotorPin2 6
#define MotorPin3 A2
#define MotorPin4 A3
=======
#define MotorPin1 6
#define MotorPin2 10
#define MotorPin3 11
#define MotorPin4 12
>>>>>>> origin/master
#define FORWARD HIGH //defining FORWARD as HIGH
#define REVERSE LOW
#define UP HIGH
#define DOWN LOW
byte static_friction_breaker_right=0;
byte static_friction_breaker_left=0;

/*System variables */
byte system_state=0;
float average_battery=1023;
float battery_voltage=6000;
byte last_state=5;
int startup_altitude=30;
unsigned long system_timer=0;

/*WiFi stuff*/

// Define the WINC1500 board co nnections below.
// If you're following the Adafruit WINC1500 board
// guide you don't need to modify these:
#define WINC_CS   8
#define WINC_IRQ  7
#define WINC_RST  4
#define WINC_EN   2     // or, tie EN to VCC
// The SPI pins of the WINC1500 (SCK, MOSI, MISO) should be
// connected to the hardware SPI port of the Arduino.
// On an Uno or compatible these are SCK = #13, MISO = #12, MOSI = #11.
// On an Arduino Zero use the 6-pin ICSP header, see:
//   https://www.arduino.cc/en/Reference/SPI


// Setup the WINC1500 connection with the pins above and the default hardware SPI.
Adafruit_WINC1500 WiFi(WINC_CS,  WINC_IRQ, WINC_RST);
Adafruit_WINC1500Server server(80);

// Or just use hardware SPI (SCK/MOSI/MISO) and defaults, SS -> #10, INT -> #7, RST -> #5, EN -> 3-5V
//Adafruit_WINC1500 WiFi;

#define LED_PIN  13  // This example assumes you have a LED connected to pin 13
                     // (with a resistor to limit current!).  Connect LED anode
                     // (longer leg) to pin 9, then LED cathode (shorter pin)
                     // through a resistor (~300-1k ohm) to ground.
bool WiFi_connected = false;  // this will tell us if the WiFi is connected or not


/*AutoPilot Stuff */
float kp=20.0;
float ki=5.9;
float kd=0.1;

float PID_I_altitude=0;
float PID_D_altitude=0;
float PID_average_altitude=0;
float PID_prev_error_altitude=0;
float PID_output_altitude=0;
float PID_error=0;
float PID_P=0;
//float PID_current_Point=0;;Adafruit_WINC1500Server server(80);
unsigned long timerPID_altitude=0;

float constrain_float(float value, float max, float min);

void Init_ultrasonic(void);
unsigned int raw_ultrasonic(void);
bool Init_WiFi(void); 
void pulse_servo(int angle);
void smart_delay(unsigned int time);

void Init_motors(void)
{
  pinMode(MotorPin1,OUTPUT);
  pinMode(MotorPin2,OUTPUT);
  pinMode(MotorPin3,OUTPUT);
  pinMode(MotorPin4,OUTPUT);
  Serial.println("Testing motors");
<<<<<<< HEAD
  Serial.println("Right...forward");
  motorRight(200,FORWARD);
//  analogWrite(MotorPin1, 250); 
//  analogWrite(MotorPin2, 0);
  delay(1000);
  Serial.println("Right...backwards");
  motorRight(200,REVERSE);
//  analogWrite(MotorPin1, 0); 
//  analogWrite(MotorPin2, 250);
  delay(1000);
  motorRight(0,FORWARD); // turn it off
//  analogWrite(MotorPin1, 0); // turn them off
//  analogWrite(MotorPin2, 0);
  Serial.println("Left...forwards");
  motorLeft(200,FORWARD);
//  analogWrite(MotorPin3, 250); 
//  analogWrite(MotorPin4, 0);
  delay(1000);
  Serial.println("Left...backwards");
  motorLeft(200,REVERSE);
//  analogWrite(MotorPin3, 0); 
//  analogWrite(MotorPin4, 250);
  Serial.println("Done testing motors");
  delay(1000);
  motorLeft(0,FORWARD); // turn it off
//  analogWrite(MotorPin3, 0); // turn them off
//  analogWrite(MotorPin4, 0);
=======
  analogWrite(MotorPin1, 250); 
  analogWrite(MotorPin3, 250);
  delay(1000);
  analogWrite(MotorPin1, 100); 
  analogWrite(MotorPin3, 100);
  delay(1000);
  analogWrite(MotorPin1, 50); 
  analogWrite(MotorPin3, 50);
  delay(1000);
  analogWrite(MotorPin1, 0); 
  analogWrite(MotorPin3, 0);
  Serial.println("Done testing motors");
>>>>>>> origin/master
}
void Init_servo(void)
{  
  myservo.attach(ServoPin);  // attaches the servo on assigned pin to the servo object
  Serial.println("Testing servo");
  test_servos(1000);
}

void setup()
{
  Serial.begin(38400);
  Serial.print("Starting System... ");
  digitalWrite(LED_PIN, HIGH);  // turn on the red LED to show that the system is working 
  WiFi_connected = Init_WiFi();
  Init_ultrasonic();   
  Init_motors();
  Init_servo();
  Serial.println("OK!!!");
  average_battery=analogRead(0);//refreshing..     
  Serial.print("Battery Analog: ");
  Serial.println((int)average_battery);
}
void loop()
{ 
/*
  switch(system_state)//This is the system state switch.. 
  {
    /**************************************************************
     * 
     ***************************************************************/
/*
  case 0:
    
    Serial.print("Reading startup altitude... ");
    startup_altitude=analogRead(2); //Reading the startup altitude, which the system will try to maintain
    Serial.println(startup_altitude);
    delay(1000); // wait one second
    Serial.println("Checking RC connection...");  
    if (WiFi_connected) // WiFi connection detected 
    {
      Serial.println("WiFi detected, activating RC settings...");
      system_state=3; 
    }
   else
   {
      Serial.println("No RC detected, starting normally...");
      Serial.println("Starting timer1 Fast PWM, to pulse vector servo...");
      system_state=1; //Now jump to system state 1.. 
   }
    delay(400);
    //system_state=5;//temporal
    break;
    /**************************************************************
     * System State 1: This state is only to hold altitude
     ***************************************************************/
/*
  case 1: //Hold altitude only (enters only when no beacon has been detected)
    nav_pilot(1);// Tell the autopilot to be in the autopilot state 0, that means hold altitude only
    autopilot_state(); //Verfy the autopilot state, if it changes, it will restart the PID and adjust the variables, etc.
    system_refresh(150); //This is the system refresh loop, which runs every 250 miliseconds
    break;
    /**************************************************************
     *System State 2: This state is to hold altitude and navigate to the beacon
     ***************************************************************/
/*
  case 2: //Navigation and hold altitude
    nav_pilot(0); // Tell the autopilot to be in the autopilot state 1. That means hold altitude and navigate
    autopilot_state(); //Verfy the autopilot state, if it changes, it will restart the PID and adjust the variables, etc.
    system_refresh(150); //This is the system refresh loop, which runs every 250 miliseconds
    break;
    /**************************************************************
     * System State 3: RC mode or manual mode.....
     ***************************************************************/
/*
  case 3:
    system_refresh(150); //This is the system refresh loop, which runs every 250 miliseconds
    break;
    /**************************************************************
     * System State 4: Will only enter if we are running out of battery, just to protect your LiPo
     ***************************************************************/
/*
  case 4:
    motorLeft(0, FORW); //So I shut down the motors... etc. 
    motorRight(0, FORW); 

    Serial.print("LOW BATTERY!: "); 
    Serial.println((int)battery_voltage);
    Battery_protection();

    break;

   case 5: //The test state
   break;  
    /**************************************************************
     * 
     ***************************************************************/
/*
  default:
    system_state=0;
    break;
  }
*/
    Serial.print("Batt: "); //Prints important values for debugging... 
    Serial.print((int)battery_voltage);
    Serial.print ("\t");
    Serial.print(" Alt: ");
    Serial.print((int)raw_ultrasonic());
    Serial.print ("\t");
    Serial.print ("\r\n");  
}// End of main loop 



/**************************************************************
 * Function to process system services, like battery level and print some parameters. 
 ***************************************************************/
void system_refresh(unsigned int refresh_rate)
{
  if(millis() - system_timer > refresh_rate) 
  {
    system_timer=millis();
    Battery_protection(); //Function locate bellow this to check the battery power... 
  }   
}
/**************************************************************
 * Function to analyse the battery level and trigger the safe mode, (to avoid killing the lipo battery)
 ***************************************************************/
void Battery_protection(void)
{

  average_battery=(((float)average_battery*.99)+(float)((float)analogRead(0)*.01));// I call it a dynamic average

  battery_voltage=(float)((float)average_battery*(float)4.887586)*(float)2;//converting values to millivolts.......


  if(battery_voltage < low_battery_limit)//If the battery voltage is less than the low_battery_limit, change the states and turn off the LED's... 
  {
    system_state=4;

  }   
}
<<<<<<< HEAD

=======

>>>>>>> origin/master

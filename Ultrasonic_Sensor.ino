void Init_ultrasonic(void)
{
  pinMode(ping_pw, INPUT);
  pinMode(ping_rx, OUTPUT);
  digitalWrite(ping_rx, HIGH);//enabling ultrasonic sensor.
  //delay(1000);
} 
    /**************************************************************
    * 
    **************************************************************/
unsigned int raw_ultrasonic(void) //Return the altitude exactly at the moment you request it... 
{
  unsigned int pulse_length=0; //declaring variable
  #ifdef ping_mode == 1
    static float average_pulse;
    digitalWrite(ping_rx, HIGH); 
    delay(37);
//    smart_delay(40);
    average_pulse=(((float)average_pulse*.50)+(float)((float)analogRead(ping_pw)*.50));
    pulse_length=average_pulse;
  #else
    digitalWrite(ping_rx, HIGH); 
    delay(37);
//    smart_delay(40);
    pulse_length=pulseIn(ping_pw,HIGH,200000); //pulseIn function to measure the length of the pulse
    digitalWrite(ping_rx, LOW); 
    pulse_length=(pulse_length/147); 
  #endif
    
    return pulse_length;
}
    /**************************************************************
    * 
    *************************************************************/

    
unsigned int pulse_ultrasonic(unsigned int refresh_rate) //Returns the altitude with the specified refresh rate in milliseconds
{
  if((millis() - timer_ultrasonic) > refresh_rate)
  {
    altitude=raw_ultrasonic();
    timer_ultrasonic=millis();
  }  
  return altitude;
}  

/**************************************************************
 * The smart delay will improve performace of the sampling. Instead of wasting CPU time 
 with a normal delay loop, this function will delay x time in miliseconds, using that time to capture more IR 
 packets from the beacon.. =) 
 ***************************************************************/
void smart_delay(unsigned int time)
{
  unsigned long start_time= millis();

  while((millis() - start_time) < time)
  {
    // Do a sample here such as readcv()  
  }   
<<<<<<< HEAD
}
=======
}
>>>>>>> origin/master

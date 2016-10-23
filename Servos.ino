
    /**************************************************************
    * 
    ***************************************************************/


    /**************************************************************
    * 
    ***************************************************************/
void pulse_servo(int angle)//Will convert the angle to the equivalent servo position... 
{
  constrain(angle,180,0);
  //angle=180-angle;//invert the vectoring
  myservo.write(angle);   
}
     /**************************************************************
    * 
    ***************************************************************/


void test_servos(unsigned int time)//Routine to test the servo at the begining (max, min and then central position)
{
  pulse_servo(180);
  delay(time);

  pulse_servo(90);
  delay(time);

  pulse_servo(0);
  delay(time);  
}  
  
<<<<<<< HEAD
  
=======
  
>>>>>>> origin/master

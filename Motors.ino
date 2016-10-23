/**************************************************************
 * Subroutine to control right motor = motorRight(Speed, direction);
 ***************************************************************/
void motorRight(byte PWM, boolean dir)
{
  byte oldPWM;
  if(PWM==0) //If Speed = 0, shut down the motors
  {
<<<<<<< HEAD
    analogWrite(MotorPin1, 0); 
    analogWrite(MotorPin2, 0);
=======
    digitalWrite(MotorPin1, LOW); 
    digitalWrite(MotorPin2, LOW);
>>>>>>> origin/master
    static_friction_breaker_right=0; //And restart the state of the motor to indicate that it's stopped.. 
  }  
  else{
    restart_right:
    
    PWM= constrain(PWM, minSpeed, maxSpeed); //Change higher values to the specified ranges
    
    if(static_friction_breaker_right == 0)//Verify if the static friction is 0, if so that means that the motor was stopped so pulse harder
    {
     oldPWM=PWM; //Storing the original PWM value... 
     PWM=anti_static_friction_breaker; //This value is defined at the begining, pulsing the motor
    }  
    if(dir == true) //If direction is 1 or true or HIGH, the motor will go forward
    {
<<<<<<< HEAD
      analogWrite(MotorPin1, 0);
=======
      digitalWrite(MotorPin1, LOW);
>>>>>>> origin/master
      analogWrite(MotorPin2, PWM); 
    }
    if(dir == false) //If direction is 0 or false or LOW, the motor will go backwards
    {
<<<<<<< HEAD
      analogWrite(MotorPin1, PWM); 
      analogWrite(MotorPin2, 0); //Trick to regulate speed in reverse direction
=======
      digitalWrite(MotorPin1, HIGH); 
      analogWrite(MotorPin2, 250-PWM); //Trick to regulate speed in reverse direction
>>>>>>> origin/master
    }
   if(static_friction_breaker_right == 0)//Then verify again if the motor was stopped. If yes...
    {
     smart_delay(50); //Delay for 10 milliseconds
     static_friction_breaker_right=1; //and then change the state to 1, to indicate to the system that the motor is running
     PWM=oldPWM; //Restoring desired speed...
     goto restart_right; //Jumping to "restart_right:" defined above... 
    }  
  }
}  

/**************************************************************
 * Sub-rutine to control left motor
 ***************************************************************/
void motorLeft(byte PWM, boolean dir)
{
  byte oldPWM;
  if(PWM==0)//If Speed = 0, shut down the motor
  {
<<<<<<< HEAD
    analogWrite(MotorPin3, 0);
    analogWrite(MotorPin4, 0); 
=======
    digitalWrite(MotorPin3, LOW);
    digitalWrite(MotorPin4, LOW); 
>>>>>>> origin/master
    static_friction_breaker_left=0; //And restart the state of the motor to indicate it's stopped.. 
  }  
  else{
    restart_left:
    PWM= constrain(PWM, minSpeed, maxSpeed);//Change higher values to the specified ranges
    if(static_friction_breaker_left == 0)//Verify if the static friction is 0. If so, that means that the motor has stopped so pulse harder
    {
     oldPWM=PWM; //Storing the original PWM value...
     PWM=anti_static_friction_breaker; //This value is defined at the begining  
    }  
    if(dir == true)//If direction is 1 or true or HIGH, the motor will go forward
    {

<<<<<<< HEAD
      analogWrite(MotorPin3, 0);
=======
      digitalWrite(MotorPin3, LOW);
>>>>>>> origin/master
      analogWrite(MotorPin4, PWM); 
    }
    if(dir == false)//If direction is 0 or false or LOW, the motor will go backwards
    {
<<<<<<< HEAD
      analogWrite(MotorPin3, PWM); 
      analogWrite(MotorPin4, 0);
=======
      digitalWrite(MotorPin3, HIGH); 
      analogWrite(MotorPin4, 250-PWM);
>>>>>>> origin/master
    }
    if(static_friction_breaker_left == 0)
    {
     smart_delay(50);//Change higher values to the specified ranges
     static_friction_breaker_left=1; //and then change the state to 1, to indicate to the system that the motor is running
     PWM=oldPWM; //Restoring desired speed...
     goto restart_left; //Jumping to "restart_left:" defined above... 
    }  
  }
<<<<<<< HEAD
}
=======
}
>>>>>>> origin/master

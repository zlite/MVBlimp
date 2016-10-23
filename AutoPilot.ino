/**************************************************************
 * 
 ***************************************************************/
 
void nav_pilot(byte mode)
{
  int motor_speed=0;  

 if(millis() - timerPID_altitude > PID_dt) 
 {
  
  if(mode == 1)
  {
    //this is autopilot state 1, hold altitude only..
    
    motor_speed=PID_altitude(startup_altitude,raw_ultrasonic(), MAX_motor, MIN_motor);//Pulsing motor in function of the altitude
    
    if(motor_speed>=1)//this point will eliminate the dead zone of the motors,
    {
      motor_speed+=down_dead_zone; //push down
    }
    if(motor_speed<=-1)
    {
      motor_speed-=up_dead_zone; //push up; I've put greater values here because the propeller is less efficient in this direction
    }

    
    
    if(motor_speed >0) //Verify the direction we need to pulse the motors 
    {
      motorLeft(abs(0), REVERSE);
      motorRight(abs(motor_speed), REVERSE);   
    }
    else
    {
      motorLeft(abs(motor_speed), FORWARD);
      motorRight(abs(0), FORWARD);  
 
    }
  } 
  /**************************************************************
   * autopilot state 1, hold altitude and navigate...
    ***************************************************************/
  else//OHH beacon available.. 
  {     
    system_state=2;  //So we are in autopilot state 2 , hold altitude and navigate...

    pulse_servo(PID_altitude(startup_altitude,raw_ultrasonic(),MAX_vect,MIN_vect)); //Pulsing servo to hold altitude... 
  }
   timerPID_altitude=millis();//Restarting the timer
}
}
/**************************************************************
 * 
 ***************************************************************/
void autopilot_state(void)
{

  if(system_state != last_state)//Check if the autopilot mode has changed
  {
    last_state=system_state;//restarting the status state

      //Here we are clearing the PID paramenters, just to avoid undesired behavior
    PID_I_altitude=0;
    PID_D_altitude=0;
    PID_prev_error_altitude=0;
    PID_output_altitude=0;
    PID_error=0;
    PID_P=0;
    switch(system_state)//System to setup the correct PID gains 
    {
    case 1://Autopilot mode 1 is hold altitude only
      pulse_servo(0); //Pointing motors to down
      //here I'm changing the PID constant values to hold altitude unsing only motor power
      kp=alt_Kp;
      ki=alt_Ki;
      kd=alt_Kd;
      
      break;

    case 2://Autopilot mode 2 is hold altitude and navigate... 
    // Here I'm changing the PID constants to hold altitude using the vectoring
      kp=nav_Kp;
      ki=nav_Ki;
      kd=nav_Kd;
      break;

    }   
  }  

}

/****************************************************************************************
PID= P+I+D
 ***************************************************************/
 
 //PID_altitude(Desired altitude, current altitude, max output, min output)
float PID_altitude(float PID_set_Point, float PID_current_Point, int MAX_altitude, int MIN_altitude)
{

  //if (millis() - timerPID_altitude > PID_dt) {//Runs the module every x milisecond declared in PID_dt
    //Computes the error,
    average_altitude=(float)(average_altitude*0.5 + PID_current_Point*0.5); //The best way to average something, I do it to avoid glitches
    


    PID_error=average_altitude-PID_set_Point;//error = setpoint - actual_position

    /******************Proportional part****************/
    PID_P= kp*PID_error;

    /******************Integrator part****************/
    
    if((PID_output_altitude < MAX_altitude) && (PID_output_altitude > MIN_altitude))//This limits the output integrator, if not it will increase till overflow
    {
     
      PID_I_altitude= PID_I_altitude+((float)((float)ki*(float)PID_error)*(float)PID_dt_seconds); // (1000 milliseconds / 1000) = 1 second
    }

    /******************Derivation part****************/

    PID_D_altitude= (float)((float)kd)*(((float)PID_error-(float)PID_prev_error_altitude)/(float)PID_dt_seconds);
   
   /******************output= P+I+D****************/
    //Add up all the PID results and limit the output... P+I+D and also constrain the output to the maximun and minimun value... 
    PID_output_altitude=(float)constrain_float((float)PID_P+(float)PID_I_altitude+(float)PID_D_altitude,MAX_altitude,MIN_altitude);//PID_P+PID_I+PID_D

    PID_prev_error_altitude=(float)PID_error; //Store the current error to use it later (very important for derivative part)

  //}
  return (constrain_float(PID_output_altitude,MAX_altitude,MIN_altitude));
  //return (PID_output_altitude);  //To invert the result 
}




/**************************************************************
 * Special module to limit float values. It's like constrain but this one is for variables IEEE 754 (Don't care that it's just floating point variables). =)  
 ***************************************************************/

float constrain_float(float value, float max, float min)
{
  if (value > max)
  {
    value=max;
  }
  if (value < min)
  {
    value=min;
  }
  return value;
<<<<<<< HEAD
}
=======
}
>>>>>>> origin/master

#define IRA 11 
#define IRB 12
#define forwardPin 5
#define backwardPin 9

/*
 * All of our variables
 */
int outerIR, innerIR;

const float Kp = 4;
float desired_angle = 0;
float currentAngle = 0;
float err;
float angle;
float velocity;
float t1, t2;

//array to hold data
int in_data[7];
// various descriptors for the received data
int sign;
int modPart;
int divPart;
int checksum;
int tickCount = 0;

/*
 * Sets up serial to read and fwd/bckpin, angle and interrupt
 */
void setup() 
{
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(forwardPin, OUTPUT);
  pinMode(backwardPin, OUTPUT);
  pinMode(IRB, INPUT);
  pinMode(IRA, INPUT);
  attachInterrupt(digitalPinToInterrupt(IRA), tickInterrupt, CHANGE);
  currentAngle = 0;
  t1 = millis();
}

/*
 * Checks for angle from user and writes it as [255 255 sign(number) number/256 number%256 checksum]
 * where each entry is one character
 */
void loop()
{
    while(true)
    {
      if(Serial1.available() >= 6)
      {
        for(int i = 0; i < 7; i++)
        {
          in_data[i] = (Serial1.read());
          Serial.println(in_data[i]);
        }
      }
      if(in_data[0] == 255 && in_data[1] == 255)
      {
        sign = in_data[2];
        divPart = in_data[3];
        modPart = in_data[4];
        checksum = (divPart + modPart - sign) % 255;
        divPart = 256*divPart;
        desired_angle = divPart + modPart;
        if(checksum == in_data[5])
        {
          if(sign > 0)
          {
            desired_angle = abs(desired_angle);
            //Serial.println(desired_angle);
          }
          else
          {
              desired_angle = -1 * abs(desired_angle);
              //Serial.println(desired_angle);
          }
        }
      }
      if(desired_angle > 720)
      {
          desired_angle = 720;
      }
      else if(desired_angle < -720)
      {
          desired_angle = -720;
      }
      currentAngle = tickCount*1.2; //because 1.2 degrees per tick
      
        t1 = millis();
        velocity = (Kp*(desired_angle - currentAngle)) + 127;
        setVelocity(velocity);
        t2 = t1;
    }
}

/*
 * Takes care of tick interrupt so can't just put finger in front of sensor anymore
 */
void tickInterrupt()
{
  outerIR = digitalRead(IRA);
  innerIR = digitalRead(IRB);

  if(outerIR != innerIR)
  {
    tickCount++;
  }
  else
  {
    tickCount--;
  }  
}


/*
 * Controls which direction motor spins based on velocity
 */
void setVelocity(float velocityy)
{
  
   if(velocityy >= 127) 
   {
    analogWrite(forwardPin, 127);
    analogWrite(backwardPin, 2*(velocity-127));
  }
  if(velocityy < 127) 
  {
    analogWrite(backwardPin, 127);
    analogWrite(forwardPin, 2*(127-velocity));
  }
  return;
}

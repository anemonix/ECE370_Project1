/*
 * All variables
 */
int LEDPin = 13;
int inputPin = 12; 
int forwardPin = 10; 
int backwardPin = 9;
int coastClear = LOW; 
int currentTick = 0;
int numberOfTicks;
int angleInput;
int currentAngle = 0;
float numOfRotations;
int numberOfWhite = 0;

/*
 * Assign proper pins
 */
void setup() 
{
  pinMode(LEDPin, OUTPUT);
  pinMode(forwardPin, OUTPUT);
  pinMode(backwardPin, OUTPUT);
  pinMode(inputPin, INPUT);
  Serial.begin(9600);
}

/*
 * Where all the magic happens
 * Checks for angle input, convert angle to amount of ticks
 * Spin wheel the proper direction
 * Count ticks on wheel until reach desired ticks/angle and stop
 */
void loop() 
{
  if(Serial.available() > 0)
  {
    angleInput = Serial.parseInt();
    //Make sure angle in in-bound
    if((angleInput < -720) || (angleInput > 720))
    {
      Serial.print("Angle out of bound, please enter another angle between -720 and +720");
    }
    else
    //Convert angle to amount of ticks!
    {
      Serial.print("Angle is set to: ");
      Serial.println(angleInput);
      numOfRotations = ((float)angleInput)/360.0;
      numberOfTicks = (int)(numOfRotations*75);
      if(numOfRotations != 0)
      {
        Serial.print("Number of ticks: ");
        Serial.println(numberOfTicks);
      }
    }
  }


  /*
   * If desired number of ticks is reached then stop spinning wheels
   * If not then keep spinning in desired direction
   */
  if(currentTick == abs(numberOfTicks))
  {
    analogWrite(forwardPin, 127);
    analogWrite(backwardPin, 127);
    currentTick = 0;
    numberOfTicks = 0;
  }
  else if(numberOfTicks != 0)
  {
    if(numberOfTicks > 0)
    {
      Serial.println("Going forward");
      analogWrite(backwardPin, 127);
      analogWrite(forwardPin, 255);
    }
    else if(numberOfTicks < 0)
    {
      Serial.println("Going backward");
      analogWrite(forwardPin, 127);
      analogWrite(backwardPin, 255);
    }
  }

  /*
   * Count ticks using logic from encoder homework
   */
  coastClear=digitalRead(inputPin);
  if(numberOfTicks !=0)
  {
    if (coastClear)
    {
      digitalWrite(LEDPin, LOW);
    }
    else
    {
      numberOfWhite++;
      digitalWrite(LEDPin, HIGH);
    }
  }
  
   if(numberOfWhite == 2)
  {
    Serial.print("Current Ticks: ");
    Serial.println(currentTick);
    currentTick++;
    numberOfWhite = 0;
  }
  //Calibration
  delay(6);
}

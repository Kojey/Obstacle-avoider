/***From DestinationInputs**/
// Setting of the lcd
#include <LiquidCrystal.h>
//LiquidCrystal lcd(9, 8, 7, 6, 5, 4);
LiquidCrystal lcd(2, 4, 6, 7, 8, 9);
// Definition of variables
int xAxis = A0, yAxis = A1;
int zButton = 12;
int countButton = 13;
int X, Y, validate, start, count, addX, addY, xPos=0, yPos=0,i=0;
const int EPSI = 50;
int xo, yo;

/***SFrom ObstacleViewer***/
#include <Servo.h>
const int outIR=A2; //Analog Pin for  output of sensor
const int range =1.75;
Servo head;

/***From Move***/
int x = 0, y=0;
const int blockTime = 1000, forwardTime=500,turningTime=500;
const int leftMotors = 10, rightMotors = 11;
const int blockSize = 20;


void setup() {
  /***From ObstacleViewer***/
  head.attach(5); //Servo is connected to PWM pin no. 5
  //head.write(80); //This assigns it to an angle of 80 degrees. The center
  Serial.begin(9600);
  /***From DestinationInputs***/
  // Setup of the screen
  lcd.begin(16,2);
  lcd.clear();
  // Setup for the analog input
  int x1 = analogRead(A0);
  int y1 = analogRead(A1);
  xo=x1; yo=y1;

}

void loop() {
  /***From DesinationInput***/ 
  // Set initial coordinates
  head.write(80);
  //head.write(80); 
  int* coordinates = destination();
  int c0 = coordinates[0];
  int c1 = coordinates[1];
  move(c0/blockSize , c1/blockSize);
}


/***From Move***/
void turnLeft()
{
  digitalWrite(leftMotors,HIGH);
  delay(turningTime);
  digitalWrite(leftMotors,LOW); 
}
void turnRight()
{
  digitalWrite(rightMotors,HIGH);
  delay(turningTime);
  digitalWrite(rightMotors,LOW);
}

void goForward()
{
  digitalWrite(leftMotors, HIGH);
  digitalWrite(rightMotors, HIGH);
  delay(forwardTime);
  digitalWrite(leftMotors, LOW);
  digitalWrite(rightMotors, LOW);
}
void stop() //stop the car
{
  digitalWrite(leftMotors, LOW);
  digitalWrite(rightMotors, LOW);
}

void move( int x, int y) //move recursively
{
  lcd.clear();
  lcd.print("here x =");
  lcd.print(x);
  lcd.setCursor(0,1);
  lcd.print("here y =");
  lcd.print(y);
  if( y == 0)
  {
      if (x== 0) {
        lcd.clear();
        lcd.print("Arrived");
        delay(2000);
        return;
      }
      //watch();
      //lookAround();
      else if ( x > 0 ) //assume there will be no obstacle on the right of the Y-end so no check obstacle initially 
      {
        turnRight();
        lcd.clear();
        lcd.print("right turn");
        delay(500);
        move(y, x);
      }
      else//assume there will be no obstacle on the right of the Y-end so no check obstacle initially 
      {
        turnLeft();
        lcd.clear();
        lcd.print("left turn");
        delay(500);
        move(y, abs(x));
      }
  }
  else if ( y > 0)
  {
    lookForward();
    delay(300);
    goForward();
    y--;
    move(x, y);
  }
  else
  {
      turnLeft();
      turnLeft();
      lcd.clear();
      lcd.print("change direction");
      delay(500);
      move(-x, -y);
  }

}

/***From ObstacleViewer***/
float watch(){
  float howfar = analogRead(outIR); //LOOKS AT THE SIGNAL FROM IR SENSOR
  float volts = (howfar/1023) *5;
  //float distance = 65*pow(volts, -1.10);
  return (volts);
}
//Still need to add list to loop


boolean lookLeft(){ //Looks in leftscanval
  boolean leftObstacle = false;
  head.write(140);
  delay(500); //I used 80 degrees because its the central angle of my 160 degrees span (use 90 degrees if you are moving your servo through the whole 180 degrees)
  if(watch() > range) {leftObstacle =true;Serial.println("Obstacle at leftt");}
  else{  
    head.write(160);
    delay(500);
    if(watch() > range) {leftObstacle =true;Serial.println("Obstacle at left");}  
}
  //Finish looking around (look forward again)
  delay(500);
  head.write(80);
  return leftObstacle;
}

boolean lookRight(){ //Looks in  rightscanval
  boolean rightObstacle = false;
  head.write(20); //I used 80 degrees because its the central angle of my 160 degrees span (use 90 degrees if you are moving your servo through the whole 180 degrees)
  delay(500);
  if(watch() > range) {rightObstacle =true;Serial.println("Obstacle at rightt");}
  else{    
    head.write(2);
    delay(500);
    if(watch() > range) {rightObstacle =true;Serial.println("Obstacle at right");}
   }
  //Finish looking around (look forward again)
  delay(500);
  head.write(80);
  return rightObstacle;
}


boolean lookForward()
{
  boolean obstacle = false;
  if(watch() > range) {obstacle =true;Serial.println("Obstacle in front");} // Obstacle in front
  else{
    head.write(93); // Look left
    delay(100);
    if(watch() > range) {obstacle =true;Serial.println("Obstacle at left");} // obstacle at left
    else{
      head.write(67); // Look right
      delay(100);
      if(watch() > range) {obstacle =true; Serial.println("Obstacle at right");} // Obstacle at right
    }
    head.write(80);
    delay(100);
  }
 
  return obstacle;
}

/***From DestinationInputs***/
int* destination() 
{
  boolean up = false, down = false, left = false, right = false;
  int x=0, y=0 ,xi=0, yi=0;
  int I = 100, i = 20;
  int coordinates[] = {0,0};
  // Reset the screen
 // Serial.println(digitalRead(10));
  lcd.clear();
  lcd.print("Enter x,y coord.");
  lcd.setCursor(0,1);
  lcd.print("X = ?   Y = ?");
  validate = digitalRead(zButton);
  // Wait until the user validates the coodinates
  while(validate != HIGH)
  {
    // Read user inputs
    X = analogRead(xAxis);
    Y = analogRead(yAxis);
    int count = digitalRead(countButton);
    
    // Increment values according to user's inputs
    if(up && Y-yo > EPSI) y+=I; // analog up
    else if (up && count == HIGH) y+=i; // count y-up
    else if(down && yo-Y > EPSI) y-=I; // analog down
    else if (down && count == HIGH) y-=i; // count y-down
    else if(right && X-xo > EPSI) x+=I; // analog right
    else if (right && count == HIGH) x+=i; // count x-right
    else if(left && xo-X > EPSI) x-=I; // analog left
    else if (left && count == HIGH) x-=i; // count x-left
    
    // Choose the side of the axis
    if(xo-X > EPSI)// Left
      {left = true; up = false; down = false; right = false;}
    else if(X-xo > EPSI)// Right
      {left = false; up = false; down = false; right = true;}
    else if(yo-Y > EPSI)// Down
      {left = false; up = false; down = true; right = false;}
    else if(Y-yo > EPSI)// Up
      {left = false; up = true; down = false; right = false;}
    // Reset the screen
    if(x==xi && y==yi){}
    else{
      lcd.clear();
      lcd.print("Actual coor.");
      lcd.setCursor(0,1);
      lcd.print("X = "); lcd.print(y);
      lcd.print(" Y = "); lcd.print(x);
    }
    delay(200); // pause between inputs reading
    validate = digitalRead(zButton); // Check for validation
    xi=x;
    yi=y;
  }
 coordinates[0] = y;
 coordinates[1] = x;
 lcd.clear();
 lcd.print("Final dest.");
 lcd.setCursor(0,1);
 lcd.print("X= "); lcd.print(y);
 lcd.print(" Y= "); lcd.print(x);
 delay(2000);
 return coordinates; 
}

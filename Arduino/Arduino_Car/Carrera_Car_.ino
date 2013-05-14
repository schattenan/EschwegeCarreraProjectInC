int speedPin =12;                      // Voltage control pin
int num_sensors = 16;                  // Number of the sensors
int y;                                 // Lightsensor value
int z=0;                               // Toggle variable
int j;                                 // Lightsensor
int In1= 2;                            // High/Low [H-Bridge: Direction]
int In2= 3;                            // Low/High [H-Bridge: Direction_Inverted]
int m=0;
int test=85;                          //Base speed
int race=50;                         //Pin for track 1
boolean raceActive=false;

int speed[16]={test+25,test+25,test+15,test,test+85,test+15,test-25,test+25,test,test+85,test+15,test+35,test,test+25,test,255};
int port[16]={22,23,25,26,27,28,29,30,31,32,34,35,36,37,38,39};


void setup() {
        Serial.begin(9600);
        pinMode(speedPin, OUTPUT);
        pinMode(In1, OUTPUT); 
        pinMode(In2, OUTPUT); 
        
        digitalWrite(In1, HIGH);
        digitalWrite(In2, LOW);

        
        for (int i=0; i<num_sensors; i++) 
        {
          j = i + 22 + m;         
        
          if (j == 24) 
          {
            j++; 
            m++;
          }
          if (j == 33) 
          {
            j++;
            m++;
          }
          pinMode(j, INPUT);                                               
          // connect pull up resistor
          digitalWrite(j, HIGH);
          if(j==39)
          {
             m=0;
          }
        };  
}
void loop()
{   

    int  v;
    j=0;
    if(z==0)
    analogWrite(speedPin, test+95);

   for(j=0;j<num_sensors-1;j++)
   {
      v=digitalRead(port[j]);
      if( v == 0)  //Pin active
      {
        analogWrite(speedPin, speed[j]);       
        z=1;
      }
   }

    //  Check if Race is active or not
    v=digitalRead(race);
    if( v == 0)   //No active Race
    {
      raceActive=false;
      v=digitalRead(port[15]); 
      if(v == 0) //Pin active
      {
        analogWrite(speedPin, 0);
        z=1;
      }
    }else{  // Active Race  
      v=digitalRead(port[15]);
      if(v == 0)   //Pin active
      {  
        analogWrite(speedPin, speed[15]);
        z=1; 
      }
      else{
          if(!raceActive)
          {
            analogWrite(speedPin, 180);
            z=1;
            raceActive=true;
          }       
      }
    }
 
}

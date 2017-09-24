

int count = 0;
String line = "4 B#";

void setup()
{
 Serial.begin(9600); 
  
}

void loop()
{
    if(Serial.available())
   { 
     line = Serial.readString();   
      Serial.print(line);
   } 
  
   while(Serial.available())
   {
    Serial.flush(); 
   }
   
     
   Serial.print(line);
   delay(1000);   
}


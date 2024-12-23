#include <Adafruit_NeoPixel.h>
#include "fix_fft.h"
 
const int displayPIN = 8;
const int numberLEDs=64;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numberLEDs, displayPIN, NEO_GRB + NEO_KHZ800);
 
const int AUDIOPIN=A0;
char im[128], data[128];
char data_avgs[16];
int i=0,j=0,n=0,val;
bool equalizer=true;
int maxValue=30;
 
String message;
 
void setup()
{
    strip.begin();
    strip.setBrightness(200);
    strip.show();
 
    Serial.begin(9600);
}
 
void showEqualizer()
{
    for(i=0;i<numberLEDs;i++)
    {
        strip.setPixelColor(i,0,0,0);
    }
    
    for (i=0; i < 128; i++){                                     
    val = analogRead(AUDIOPIN);                                    
    data[i] = val;                                       
    im[i] = 0;                                                     
  };
 
  fix_fft(data,im,7,0);
 
  for (i=0; i< 64;i++){                                      
    data[i] = sqrt(data[i] * data[i] + im[i] * im[i]);  // this gets the absolute value of the values in the array, so we're only dealing with positive numbers
  };     
   
  // average bars together
  for (i=0; i<16; i++) {
    data_avgs[i] = data[i*4] + data[i*4 + 1] + data[i*4 + 2] + data[i*4 + 3]; 
    if(data_avgs[i]>maxValue)
    {
        data_avgs[i]=maxValue;
    }
    data_avgs[i] = map(data_avgs[i], 0, maxValue, 0,  16);
    j=15-i; 
    for(n=0;n<data_avgs[i];n++)
    {
        uint32_t magenta;
        if(n<5)
            magenta = strip.Color(0, 255, 0); //Red Green Blue
        else if(n<10)
            magenta = strip.Color(255, 255, 0);
        else magenta = strip.Color(255, 0, 0);
        
        if(j%2)
        {
            strip.setPixelColor((j+1)*16-1-n,magenta);
        }
        else
        {
            strip.setPixelColor(j*16+n,magenta);
        }
    }
  }
  strip.show();
}
 
void loop()
{
    if(Serial.available())
    {
        char m=Serial.read();       
        message+=m;
        if(m=='/') //command: "enableEq:/" "disableEq:/" "img:i,r,g,b/" "showImg:/"
        {
            int c=message.indexOf(':');
            String command=message.substring(0,c);
            if(command=="enableEq")
            {
                equalizer=true;
            }
            else if(command=="disableEq")
            {
                equalizer=false;
            }
            else if(command=="img")
            {
                command=message.substring(c+1,message.length()-1);
                c=command.indexOf(',');
                i=command.substring(0,c).toInt();
                
                command=command.substring(c+1);
                c=command.indexOf(',');
                int r=command.substring(0,c).toInt();
 
                command=command.substring(c+1);
                c=command.indexOf(',');
                int g=command.substring(0,c).toInt();
 
                command=command.substring(c+1);
                int b=command.toInt();              
                strip.setPixelColor(i,r,g,b);
                Serial.println("r");
            }
            else if(command=="showImg")
            {               
                strip.show();
            }
            else if(command=="clear")
            {
                for(i=0;i<numberLEDs;i++)
                    strip.setPixelColor(i,0,0,0);
                strip.show();
            }
            message="";
        }
    }
    if(equalizer) showEqualizer();
}

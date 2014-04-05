//NOTE: Need to modify cgi_parse.h lines 122 & 126 if you change how many pairs are passed through the URL!

#include <math.h> //because of the math you must compile with the -lm option
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cgi_parse.h" //Downloaded file
#include "/home/pi/i2c/i2c_dev.h" //This is LMsensors file
#include <fcntl.h>
#include <wiringPi.h> //not necessary, I had 2 LEDs hooked to 2 GPIO pins 

/*********** Includes for the DSP ************/
#include "RaspPi_IC_1.h" //For Main Program and Register Declaration CHANGE NAME FOR YOUR PROGRAM	
#include "SigmaStudioFW.h"
#include "RaspPi_IC_1_PARAM.h" //CHANGE NAME FOR YOUR PROGRAM!!!


/********** All other setup ************/
int deviceHandle, deviceHandle2, err, i;
char buffer[7];
int tempC; 
float tempF;
int addr = 0x00;
int deviceI2CAddress1 = 0x49, deviceI2CAddress2 = 0x4a, deviceI2CAddress3 = 0x4b, deviceI2CAddress4 = 0x4c, deviceI2CAddress5 = 0x4d;

float user_input;
float freqReadBack=0;




int main (void) {

/*************************I2C Temp Sensors*************************/

char **cgivars;

  cgivars = getcgivars();

  printf("%s%c%c\n",
  "Content-Type:text/html;charset=iso-8859-1",13,10);


  // initialize buffer
  buffer[0] = 0x00;

  printf("<p>");

  // open device on /dev/i2c-0
  deviceHandle = open("/dev/i2c-0", O_RDWR);

  ioctl(deviceHandle, I2C_SLAVE, deviceI2CAddress1);

  // begin transmission and request acknowledgement
  tempC = i2c_smbus_read_byte_data(deviceHandle, addr);
  printf("\n\rSensor 1 C: %i\r", tempC);
  printf("<br>");
  tempF = (tempC*1.8)+32;
  printf("Sensor 1 F: %5.2f \n", tempF);
  fflush(stdout);


  printf("</p>");
  printf("<p>");


  ioctl(deviceHandle, I2C_SLAVE, deviceI2CAddress2);

  // begin transmission and request acknowledgement
  tempC = i2c_smbus_read_byte_data(deviceHandle, addr);
  printf("\n\rSensor 2 C: %i \n\r", tempC);
  tempF = (tempC*1.8)+32;
  printf("<br>");
  printf("Sensor 2 F: %5.2f \n", tempF);
  fflush(stdout);


  printf("</p>");
  printf("<p>");


  ioctl(deviceHandle, I2C_SLAVE, deviceI2CAddress3);

  // begin transmission and request acknowledgement
  tempC = i2c_smbus_read_byte_data(deviceHandle, addr);
  printf("\n\rSensor 3 C: %i \n\r", tempC);
  printf("<br>");
  tempF = (tempC*1.8)+32;
  printf("Sensor 3 F: %5.2f \n", tempF);
  fflush(stdout);


  printf("</p>");
  printf("<p>");


 ioctl(deviceHandle, I2C_SLAVE, deviceI2CAddress4);

  // begin transmission and request acknowledgement
  tempC = i2c_smbus_read_byte_data(deviceHandle, addr);
  printf("\n\rSensor 4 C: %i \n\r", tempC);
  printf("<br>");
  tempF = (tempC*1.8)+32;
  printf("Sensor 4 F: %5.2f \n", tempF);
  fflush(stdout);


  printf("</p>");
  printf("<p>");



  ioctl(deviceHandle, I2C_SLAVE, deviceI2CAddress5);

  // begin transmission and request acknowledgement
  tempC = i2c_smbus_read_byte_data(deviceHandle, addr);
  printf("\n\rSensor 5 C: %i \n\r", tempC);
  tempF = (tempC*1.8)+32;
  printf("<br>");
  printf("Sensor 5 F: %5.2f \n", tempF);
  fflush(stdout);
  printf("</p>");


  // close connection and return
  close(deviceHandle);


/////////////////////////////END OF SENSORS////////////////////////



/***********************************SigmaDSP Code**************************************************/


  //Frequency Setting
  printf("<form action=\"\" method=\"get\" onSubmit=\"window.location.reload()\">");
  printf("<select name=\"freq\">");
  printf("<option value=\"0\">LED Frequency</option>");
  printf("<option value=\"1\">1 Hz</option>");
  printf("<option value=\"5\">5 Hz</option>");
  printf("<option value=\"10\">10 Hz</option>");
  printf("<option value=\"15\">15 Hz</option>");
  printf("<option value=\"20\">20 Hz</option>");
  printf("<option value=\"25\">25 Hz</option>");
  printf("</select>");

  printf("<select name=\"linearGain\">");
  printf("<option value=\"0\">Linear Gain</option>");
  printf("<option value=\"1\">1</option>");
  printf("<option value=\"0.5\">0.5</option>");
  printf("<option value=\"0.1\">0.10</option>");
  printf("<option value=\"0.05\">0.05</option>");
  printf("<option value=\"0.01\">0.01</option>");
  printf("<option value=\"0.005\">0.005</option>");
  printf("</select>");

  printf("<select name=\"atten\">");
  printf("<option value=\"0\">Attenuation</option>");
  printf("<option value=\"-1\">1 dB</option>");
  printf("<option value=\"-5\">5 dB</option>");
  printf("<option value=\"-10\">10 dB</option>");
  printf("<option value=\"-15\">15 dB</option>");
  printf("<option value=\"-20\">20 dB</option>");
  printf("<option value=\"-25\">25 dB</option>");
  printf("<option value=\"-30\">30 dB</option>");
  printf("<option value=\"-35\">35 dB</option>");
  printf("<option value=\"-40\">40 dB</option>");
  printf("<option value=\"-45\">45 dB</option>");
  printf("<option value=\"-50\">50 dB</option>");
  printf("<option value=\"-55\">55 dB</option>");
  printf("</select>");
  printf("<input type=\"submit\" value=\"Submit\">");
  printf("</form>");


  user_input = atoi(cgivars[1]);

  if(user_input != 0)
    SIGMA_WRITE_FREQ(DEVICE_ADDR_IC_1, MOD_SQUARE1_ALG0_FREQ_ADDR, user_input);
 
  user_input = atof(cgivars[3]);

  if(user_input != 0)
    SIGMA_WRITE_LINEAR_GAIN(DEVICE_ADDR_IC_1, MOD_GAIN1_GAIN1940ALGNS1_ADDR, user_input);

  user_input = atof(cgivars[5]);

  if(user_input != 0)
    SIGMA_WRITE_VOLUME_FADER(DEVICE_ADDR_IC_1, MOD_SINGLE1_GAIN1940ALGNS2_ADDR, user_input);
  
  
  fflush(stdout);


/********************************Read Back Calls*****************************************/

  printf("<p>");

  //Frequency Readback
  freqReadBack = SIGMA_READ_FREQ(DEVICE_ADDR_IC_1, MOD_SQUARE1_ALG0_FREQ_ADDR);

  printf("\nCurrent LED Frequency: %f", freqReadBack);
  printf("</p>");

  printf("<p>");

  //Linear Gain Readback
  freqReadBack = SIGMA_READ_LINEAR_GAIN(DEVICE_ADDR_IC_1, MOD_GAIN1_GAIN1940ALGNS1_ADDR);

  printf("\nCurrent Linear Gain: %f", freqReadBack);
  printf("</p>");

  printf("<p>");

  //Volume Fader Readback
  freqReadBack = SIGMA_READ_VOLUME_FADER(DEVICE_ADDR_IC_1, MOD_SINGLE1_GAIN1940ALGNS2_ADDR);

  printf("\nCurrent Volume Fader: %f", freqReadBack);
  printf("</p>");

  printf("<p>");

  //Readback box Readback haha
  freqReadBack = SIGMA_READBACK(DEVICE_ADDR_IC_1, MOD_READBACK1_REABACALGSIGMA2001_ADDR );

  printf("\nCurrent Readback: %f\n", freqReadBack);
  printf("</p>");



return 0;
}

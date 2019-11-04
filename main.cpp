/* November 2019
 * Lab 7
 *
 */


#include"mbed.h" 
 
DigitalOut clk(p27);  
DigitalOut rst(p28);  
DigitalOut data(p30); 

DigitalIn row1(p26);
DigitalIn row2(p25);
DigitalIn row3(p24);
DigitalIn row4(p23); 

I2C i2c(p32,p31); //SDA and SCL pins

DigitalOut col1(p8);
DigitalOut col2(p7);
DigitalOut col3(p6);
DigitalOut col4(p5);

//-----------Global Variables-----------//

int arr[16][8] = {  {1,1,0,0,0,0,0,0}, //0 
				{1,1,1,1,1,0,0,1}, //1 
				{1,0,1,0,0,1,0,0}, //2 
				{1,0,1,1,0,0,0,0}, //3 
				{1,0,0,1,1,0,0,1}, //4 
				{1,0,0,1,0,0,1,0}, //5 
				{1,0,0,0,0,0,1,0}, //6 
				{1,1,1,1,1,0,0,0}, //7 
				{1,0,0,0,0,0,0,0}, //8 
				{1,0,0,1,1,0,0,0}, //9 
				{1,0,0,0,1,0,0,0}, //A
				{1,0,0,0,0,0,1,1}, //B
				{1,1,0,0,0,1,1,0}, //C
				{1,0,1,0,0,0,0,1}, //d
				{1,0,0,0,0,1,1,0}, //E
				{1,0,0,0,1,1,1,0}}; //F


 
void EdgeClock(){ 
 wait(0.000001); 
 clk = 1; 
 wait(0.000001); 
 clk=0; 
  
} 

//------------------Displays the values on the 7-segment--------------//
void displayVal(int val1, int val2 , int val3){
	//print third digit MS
	for (int j = 0; j < 8 ; j++) { 	
		data = arr[val1][j]; 
		EdgeClock(); 
	} 	
	//print second digit
	for (int j = 0; j < 8 ; j++) { 	
		data = arr[val2][j]; 
		EdgeClock(); 
	}
	//print first digit LS
	for (int j = 0; j < 8 ; j++) { 	
		data = arr[val3][j]; 
		EdgeClock(); 
	}
	
	wait(0.2);// Paused to show 
}

//function to parse the sensor reading to display
//work with unsigned reading
void parseMyReading(unsigned int temp){
	
	int dig1, dig2, dig3, dig4;
	float ctemp;
	ctemp = temp * 0.0625;
	
	dig1 = int(temp)%10;
	dig2 = temp/10;
	
	//display Celsius reading
	displayVal(dig1,dig2,12);
	
	float tempf;
	
	tempf = (temp * (1.8)) + 32;
	
	dig3 = int(tempf)%10;
	dig4 = tempf/10;
	
	wait(0.5);
	
	//display  Farenheit reading
	displayVal(dig3,dig4,15);
	
	wait(0.5);
}

//------------Clears the 7-segment display-----------//
void clearDisplay(){
	displayVal(0,0,0);
}

int main(){ 
	rst = 0; 
	EdgeClock(); 
	rst = 1; 
	EdgeClock(); 
	displayVal(0,0,0);
	
	unsigned int temp;	
	
	//start i2c communication using object
	i2c.start();	
	
	//arbitrary address for communications
	i2c.write(0x90);
	
	//access the 1-byte configuration register
	i2c.write(0xAC);
	
	//configure itself to continuously monitor temperature
	i2c.write(0x2);
	
	//communication from master is over. BYE
	i2c.stop();
	
	//collect data continuously
	i2c.start();
	
	i2c.write(0x90);
	
	//initiate temperature conversions
	i2c.write(0x51);
	
	i2c.stop();
	
	i2c.start();
	
	//writing to tell it to read 
	i2c.write(0x90);
	
	i2c.write(0xAA);	
	
	//continuously read from sensor
	while(1){
	
		//start communication
		i2c.start();
	
		//take reading
		i2c.write(0x91);
		
		//get the first input
		temp = i2c.read(0);
		
		//end comms
		i2c.stop();
		
		//analyze reading from sensor
		parseMyReading(temp);		

		//Scuderia FERRARI BRO (vroom vroom)
	
	}	
}	//end main()



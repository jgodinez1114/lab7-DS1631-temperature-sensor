/* October 2019
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
int first=0;// first number entered
int temp1=0;//temporary variable used to store first number
int second=0;//second number entered
int temp2=0;//temporary variable used to store second number
int third=0;//Third number entered
int A=0;//first number for arithmetic operation
int B=0;//second number for arithmetic operation
int operand;//type of operation
int counter=0;//counter used to keep track the number of input(limited to two)
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

//function to parse the sensor reading to display


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

void parseMyReading(unsigned int temp){
	int dig1, dig2, dig3, dig4;
	float ctemp;
	ctemp = temp * 0.0625;
	
	dig1 = int(temp)%10;
	dig2 = temp/10;
	
	//call to display Celsius reading
	displayVal(dig1,dig2,12);
	
	float tempf;
	tempf = (temp * (1.8)) + 32;
	
	dig3 = int(tempf)%10;
	dig4 = tempf/10;
	
	wait(0.5);
	
	//call to display  Farenheit reading
	displayVal(dig3,dig4,15);
	
	wait(0.5);
}

//------------Clears the 7-segment display-----------//
void clearDisplay(){
	displayVal(0,0,0);
}






//------------------Save the value of the key pressed-------------//
void saveValue(int val){
	// if number of digits is less than 2 and is a valid number 0-9, save the values for each digit
	if (counter<2 && val<10){
		temp1=first;
		first=val;
		temp2=second;
		second=temp1;
		third=temp2;
		counter++;
		displayVal(first,second, 0);
	//if number is 10(A), 11(B), 12(C), 13(D), then save the previous digits as A, and save the current value as operand
	}else if(val>9 && val != 14 && val != 15) {
		A=(10*second)+first;
		first=0;
		second=0;
		third=0;
		counter=0;
		displayVal(val,0,0);
		operand=val;
	//if number is F(15), then reset
	}else if (val==15){
		first=0;
		second=0;
		third=0;
		counter=0;
		displayVal(0,0,0);
	//if value is E(14), then save the previous digits as B, reset first, second, third and counter
  // and perform the correspondig operation
	}else{
		B=(10*second)+first;
		first=0;
		second=0;
		third=0;
		counter=0;
		displayVal(val,0,0);
		
	}
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
	
	i2c.write(0x90);
	
	i2c.write(0xAC);
	
	//configure itself to continuously monitor temperature
	i2c.write(0x2);
	
	//communication from master is over. BYE
	i2c.stop();
	
	//collect data continuously
	i2c.start();
	
	i2c.write(0x90);
	
	i2c.write(0x51);
	
	i2c.stop();
	
	i2c.start();
	
	//writing to tell it to read 
	i2c.write(0x90);
	
	i2c.write(0xAA);	
	
	while(1){
	
		i2c.start();
	
		//read
		i2c.write(0x91);
		
		//get the first input
		temp = i2c.read(0);
		
		i2c.stop();
		
		parseMyReading(temp);		
		
		//write
		
		//read
		//NOte: work with HEX values per Mark Minton
		//say goodbye
		//Scudeira FERRARI BRO (vroom vroom)
	
	}
	
}



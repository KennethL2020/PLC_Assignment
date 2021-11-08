#include "Galil.h"
#include "EmbeddedFunctions.h"
#include "gclib.h"
#include "gclib_errors.h"
#include "gclib_record.h"
#include "gclibo.h"

using namespace System;
Galil::Galil(){
	Functions = new EmbeddedFunctions();
	Functions->GOpen("192.168.0.120 -d", &g);
}												// Default constructor. Initialize variables, open Galil connection and allocate memory. NOT AUTOMARKED
Galil::Galil(EmbeddedFunctions* Funcs, GCStringIn address){
	Functions = Funcs;
	Functions->GOpen(address, &g);
}	// Constructor with EmbeddedFunciton initialization
Galil::~Galil(){
	Functions->GClose(&g);
}												// Default destructor. Deallocate memory and close Galil connection. NOT AUTOMARKED

// DIGITAL OUTPUTS
void Galil::DigitalOutput(uint16_t value){
	GBufIn buffer = (value == 1) ? "OP255,255;\r\n" : "OP0,0;\r\n";
	GSize bufferLn = strlen(buffer);
	printf("\n%s", buffer);
	Functions->GWrite(g, buffer, bufferLn);
}						// Write to all 16 bits of digital output, 1 command to the Galil
void Galil::DigitalByteOutput(bool bank, uint8_t value){
	GBufIn buffer;
	if (bank) {
		buffer = (value == 1) ? "OP,255;\r\n" : "OP,0;\r\n";
	}
	else {
		buffer = (value == 1) ? "OP255;\r\n" : "OP0;\r\n";
	}
	GSize bufferLn = strlen(buffer);
	printf("\n%s", buffer);
	Functions->GWrite(g, buffer, bufferLn);
}		// Write to one byte, either high or low byte, as specified by user in 'bank'
														// 0 = low, 1 = high
void Galil::DigitalBitOutput(bool val, uint8_t bit){
	char tmp [256];
	GBufIn buffer = tmp;
	sprintf((char *)buffer, "OP%d,%d;\r\n", bit, val);
	GSize bufferLn = strlen(buffer);
	printf("\n%s", buffer);
	Functions->GWrite(g, buffer, bufferLn);
}			// Write single bit to digital outputs. 'bit' specifies which bit


// DIGITAL INPUTS
uint16_t Galil::DigitalInput() {
	uint16_t result = 0;
	char tmp[256];
	GBufIn buffer = tmp;
	char readBuffer[256];
	GBufOut readBuf = readBuffer;
	for (int i = 0; i < 16; i++) {
		sprintf((char*)buffer, "MG @IN[%d];\r\n", i);
		printf("\n%s", buffer);
		Functions->GCommand(g, buffer, readBuf, 256, NULL);
		result = result | (readBuf[0] << i);
	}
	
	return result; 
}				// Return the 16 bits of input data
										// Query the digital inputs of the GALIL, See Galil command library @IN
uint8_t Galil::DigitalByteInput(bool bank){ 
	uint8_t result = 0;
	char tmp[256];
	GBufIn buffer = tmp;
	char readBuffer[8];
	GBufOut readBuf = readBuffer;
	int start = 0;
	if (bank) {
		start = 8;
	}
	
	for (int i = start; i < start+8; i++) {
		sprintf((char*)buffer, "MG @IN[%d];\r\n", i);
		printf("\n%s", buffer);
		Functions->GCommand(g, buffer, readBuf, 8, NULL);
		result = result | (readBuf[0] << i);
	}

	return result;
}	// Read either high or low byte, as specified by user in 'bank'
										// 0 = low, 1 = high
bool Galil::DigitalBitInput(uint8_t bit){ 
	bool result;
	char tmp[256];
	GBufIn buffer = tmp;
	char readBuffer[8];
	GBufOut readBuf = readBuffer;
	sprintf((char*)buffer, "MG @IN[%d];\r\n", bit);
	printf("\n%s", buffer);
	Functions->GCommand(g, buffer, readBuf, 8, NULL);
	result = readBuf[0];
	return result; 
}		// Read single bit from current digital inputs. Above functions
										// may use this function

bool Galil::CheckSuccessfulWrite(){
	char readBuffer[8];
	GBufOut readBuf = readBuffer;
	Functions->GRead(g, readBuf, 8, NULL);
	if (readBuf[0] != '?') {
		return false;
	}
	return true; 
}	// Check the string response from the Galil to check that the last 
								// command executed correctly. 1 = succesful. NOT AUTOMARKED

// ANALOG FUNCITONS
float Galil::AnalogInput(uint8_t channel){ 
	char tmp[256];
	GBufIn buffer = tmp;
	char readBuffer[sizeof(float)];
	GBufOut readBuf = readBuffer;
	sprintf((char*)buffer, "MG @AN[%d];\r\n", channel);
	printf("\n%s", buffer);
	Functions->GCommand(g, buffer, readBuf, sizeof(float), NULL);
	int result = 0;
	for (int i = 0; i < sizeof(float); i++) {
		result = result | (readBuf[i] << i*8);
	}
	return (float)result; 
}						// Read Analog channel and return voltage			
void Galil::AnalogOutput(uint8_t channel, double voltage){
	char tmp[256];
	GBufIn buffer = tmp;
	sprintf((char*)buffer, "AO%d,%.2lf;\r\n", channel, voltage);
	GSize bufferLn = strlen(buffer);
	printf("\n%s", buffer);
	Functions->GWrite(g, buffer, bufferLn);
}		// Write to any channel of the Galil, send voltages as
														// 2 decimal place in the command string
void Galil::AnalogInputRange(uint8_t channel, uint8_t range){
	char tmp[256];
	GBufIn buffer = tmp;
	sprintf((char*)buffer, "AQ%d,%d;\r\n", channel, range);
	GSize bufferLn = strlen(buffer);
	printf("\n%s", buffer);
	Functions->GWrite(g, buffer, bufferLn);
}	// Configure the range of the input channel with
														// the desired range code

// ENCODER / CONTROL FUNCTIONS
void Galil::WriteEncoder(){
	char tmp[256];
	GBufIn buffer = tmp;
	sprintf((char*)buffer, "WE%d,%d;\r\n", 0, 0);
	GSize bufferLn = strlen(buffer);
	printf("\n%s", buffer);
	Functions->GWrite(g, buffer, bufferLn);
}								// Manually Set the encoder value to zero
int Galil::ReadEncoder(){ 
	char tmp[256];
	GBufIn buffer = tmp;
	char readBuffer[sizeof(int)];
	GBufOut readBuf = readBuffer;
	sprintf((char*)buffer, "QE %d;\r\n", 0);
	printf("\n%s", buffer);
	Functions->GCommand(g, buffer, readBuf, sizeof(int), NULL);
	int result = 0;
	for (int i = 0; i < sizeof(int); i++) {
		result = result | (readBuf[i] << i * 8);
	}
	return result; 
}									// Read from Encoder
void Galil::setSetPoint(int s){
	char tmp[256];
	GBufIn buffer = tmp;
	sprintf((char*)buffer, "PS %d;\r\n", s);
	GSize bufferLn = strlen(buffer);
	printf("\n%s", buffer);
	Functions->GWrite(g, buffer, bufferLn);
}							// Set the desired setpoint for control loops, counts or counts/sec
void Galil::setKp(double gain){
	char tmp[256];
	GBufIn buffer = tmp;
	sprintf((char*)buffer, "KP %lf;\r\n", gain);
	GSize bufferLn = strlen(buffer);
	printf("\n%s", buffer);
	Functions->GWrite(g, buffer, bufferLn);
}							// Set the proportional gain of the controller used in controlLoop()
void Galil::setKi(double gain){
	char tmp[256];
	GBufIn buffer = tmp;
	sprintf((char*)buffer, "KI %lf;\r\n", gain);
	GSize bufferLn = strlen(buffer);
	printf("\n%s", buffer);
	Functions->GWrite(g, buffer, bufferLn);
}							// Set the integral gain of the controller used in controlLoop()
void Galil::setKd(double gain){
	char tmp[256];
	GBufIn buffer = tmp;
	sprintf((char*)buffer, "KD %lf;\r\n", gain);
	GSize bufferLn = strlen(buffer);
	printf("\n%s", buffer);
	Functions->GWrite(g, buffer, bufferLn);
}							// Set the derivative gain of the controller used in controlLoop()
//void Galil::PositionControl(bool debug, int Motorchannel){}	// Run the control loop. ReadEncoder() is the input to the loop. The motor is the output.
													// The loop will run using the PID values specified in the data of this object, and has an 
													// automatic timeout of 10s. You do not need to implement this function, it is defined in
													// GalilControl.lib
//void Galil::SpeedControl(bool debug, int Motorchannel){}	// same as above. Setpoint interpreted as counts per second


// Operator overload for '<<' operator. So the user can say cout << Galil{} This function should print out the
// output of GInfo and GVersion, with two newLines after each.
std::ostream& operator<<(std::ostream& output, Galil& galil) {
	char versionbuffer[128];
	char infobuffer[128];
	GCStringOut gcversion = versionbuffer;
	GCStringOut gcinfo = infobuffer;
	GSize gsize = 128;
	galil.Functions->GInfo(galil.g, gcinfo, gsize);
	galil.Functions->GVersion(gcversion, gsize);

	return output << gcinfo << gcversion; 
}
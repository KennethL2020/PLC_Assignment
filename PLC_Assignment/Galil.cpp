#include "Galil.h"
#include "EmbeddedFunctions.h"
#include "gclib.h"
#include "gclib_errors.h"
#include "gclib_record.h"
#include "gclibo.h"

using namespace System;
Galil::Galil(){

}												// Default constructor. Initialize variables, open Galil connection and allocate memory. NOT AUTOMARKED
Galil::Galil(EmbeddedFunctions* Funcs, GCStringIn address){
	Galil();
	Functions = Funcs;
	Functions->GOpen(address, &g);
}	// Constructor with EmbeddedFunciton initialization
Galil::~Galil(){
	Functions->GClose(&g);
}												// Default destructor. Deallocate memory and close Galil connection. NOT AUTOMARKED

// DIGITAL OUTPUTS
void Galil::DigitalOutput(uint16_t value){
	GBufIn buffer = value == 1 ? "OP255,255" : "OP0,0";
	GSize bufferLn = strlen(buffer);
	Functions->GWrite(g, buffer, bufferLn);
}						// Write to all 16 bits of digital output, 1 command to the Galil
void Galil::DigitalByteOutput(bool bank, uint8_t value){
	GBufIn buffer;
	if (bank) {
		buffer = value == 1 ? "OP,255" : "OP,0";
	}
	else {
		buffer = value == 1 ? "OP255" : "OP0";
	}
	GSize bufferLn = strlen(buffer);
	Functions->GWrite(g, buffer, bufferLn);
}		// Write to one byte, either high or low byte, as specified by user in 'bank'
														// 0 = low, 1 = high
void Galil::DigitalBitOutput(bool val, uint8_t bit){
	char tmp [256];
	GBufIn buffer = tmp;
	sprintf((char *)buffer, "OP%d,%d", bit, val);
	GSize bufferLn = strlen(buffer);
	Functions->GWrite(g, buffer, bufferLn);
}			// Write single bit to digital outputs. 'bit' specifies which bit


// DIGITAL INPUTS
uint16_t Galil::DigitalInput() { return 0; }				// Return the 16 bits of input data
										// Query the digital inputs of the GALIL, See Galil command library @IN
uint8_t Galil::DigitalByteInput(bool bank){ return 0; }	// Read either high or low byte, as specified by user in 'bank'
										// 0 = low, 1 = high
bool Galil::DigitalBitInput(uint8_t bit){ return 0; }		// Read single bit from current digital inputs. Above functions
										// may use this function

bool Galil::CheckSuccessfulWrite(){ return 0; }	// Check the string response from the Galil to check that the last 
								// command executed correctly. 1 = succesful. NOT AUTOMARKED

// ANALOG FUNCITONS
float Galil::AnalogInput(uint8_t channel){ return 0; }						// Read Analog channel and return voltage			
void Galil::AnalogOutput(uint8_t channel, double voltage){}		// Write to any channel of the Galil, send voltages as
														// 2 decimal place in the command string
void Galil::AnalogInputRange(uint8_t channel, uint8_t range){}	// Configure the range of the input channel with
														// the desired range code

// ENCODER / CONTROL FUNCTIONS
void Galil::WriteEncoder(){}								// Manually Set the encoder value to zero
int Galil::ReadEncoder(){ return 0; }									// Read from Encoder
void Galil::setSetPoint(int s){}							// Set the desired setpoint for control loops, counts or counts/sec
void Galil::setKp(double gain){}							// Set the proportional gain of the controller used in controlLoop()
void Galil::setKi(double gain){}							// Set the integral gain of the controller used in controlLoop()
void Galil::setKd(double gain){}							// Set the derivative gain of the controller used in controlLoop()
//void Galil::PositionControl(bool debug, int Motorchannel){}	// Run the control loop. ReadEncoder() is the input to the loop. The motor is the output.
													// The loop will run using the PID values specified in the data of this object, and has an 
													// automatic timeout of 10s. You do not need to implement this function, it is defined in
													// GalilControl.lib
//void Galil::SpeedControl(bool debug, int Motorchannel){}	// same as above. Setpoint interpreted as counts per second


// Operator overload for '<<' operator. So the user can say cout << Galil{} This function should print out the
// output of GInfo and GVersion, with two newLines after each.
std::ostream& operator<<(std::ostream& output, Galil& galil) { return output; }

int main() {
	EmbeddedFunctions* Funcs = new EmbeddedFunctions();
	Galil* galil = new Galil(Funcs, "192.168.0.120");


}
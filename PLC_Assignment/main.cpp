#include "Galil.h"
using namespace System;

int main() {
	Galil galil;

	while (1) {
		char key = Console::ReadKey().KeyChar;
		switch (key) {
		case '1':
			galil.DigitalOutput(0);
			break;
		case '2':
			galil.DigitalByteOutput(1, 1);
			break;
		case '3':
			galil.DigitalBitOutput(1, 7);
			break;
		case '4':
			printf("%d\n", galil.DigitalInput());
			break;
		case '5':
			printf("%d\n", galil.DigitalByteInput(0));
			break;
		case '6':
			printf("%d\n", galil.DigitalBitInput(7));
			break;
		case '7':
			galil.DigitalBitOutput(1, 10);
			printf("%d\n", galil.CheckSuccessfulWrite());
			break;
		case '8':
			printf("%f\n", galil.AnalogInput(0));
			break;
		case '9':
			galil.AnalogOutput(0, 5);
			break;
		case '0':

			break;
		case 'q':
			break;
		case 'w':
			break;
		case 'e':
			break;
		case 'r':
			break;
		case 't':
			break;
		case 'y':
			break;
		}
	}
}
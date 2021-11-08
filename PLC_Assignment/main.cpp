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
			printf("%u\n", galil.DigitalInput());
			break;
		case '5':
			printf("%u\n", galil.DigitalByteInput(0));
			break;
		case '6':
			printf("%u\n", galil.DigitalBitInput(7));
			break;
		case '7':
			galil.DigitalBitOutput(1, 10);
			galil.DigitalBitOutput(1, 0);
			printf("%d\n", galil.CheckSuccessfulWrite());
			break;
		case '8':
			printf("%f\n", galil.AnalogInput(0));
			break;
		case '9':
			galil.AnalogOutput(0, 5);
			break;
		case '0':
			galil.AnalogInputRange(0, 1);
			printf("%d\n", galil.CheckSuccessfulWrite());
			break;
		case 'q':
			galil.WriteEncoder();
			printf("%d\n", galil.CheckSuccessfulWrite());
			break;
		case 'w':
			printf("%d\n", galil.ReadEncoder());
			break;
		case 'e':
			galil.setSetPoint(5);
			printf("%d\n", galil.CheckSuccessfulWrite());
			break;
		case 'r':
			galil.setKd(1);
			printf("%d\n", galil.CheckSuccessfulWrite());
			break;
		case 't':
			galil.setKp(2);
			printf("%d\n", galil.CheckSuccessfulWrite());
			break;
		case 'y':
			galil.setKi(3);
			printf("%d\n", galil.CheckSuccessfulWrite());
			break;
		}
	}
}
#include "Galil.h"
using namespace System;

int main() {
	EmbeddedFunctions* Funcs = new EmbeddedFunctions();
	Galil* galil = new Galil(Funcs, "192.168.0.120");

	while (1) {
		char key = Console::ReadKey().KeyChar;
		switch (key) {
		case '1':
			galil->DigitalOutput(1);
			break;
		case '2':
			galil->DigitalByteOutput(1, 1);
			break;
		case '3':
			galil->DigitalBitOutput(1, 7);
			break;
		case '4':
			break;
		case '5':
			break;
		case '6':
			break;
		case '7':
			break;
		case '8':
			break;
		case '9':
			break;
		}
	}
	galil->DigitalOutput(1);
}
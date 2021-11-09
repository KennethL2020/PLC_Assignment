#include "Galil.h"
using namespace System;

int main() {
	Galil galil;
	galil.DigitalOutput(0);
	float ai = galil.AnalogInput(0);
	int aii = (int)ai;
	int aid = (ai - aii)*10;

	for (int i = 0; i < aii; i++) {
		galil.DigitalBitOutput(1, i);
	}
	for (int i = 0; i < aid; i++) {
		galil.DigitalBitOutput(1, i+8);
	}
	Console::ReadKey();
	galil.AnalogOutput(0, 3);
	while (1) {
		int d = galil.ReadEncoder();
		for (int i = 0; i < 8; i++) {
			int val = (d >> i) & 1;
			galil.DigitalBitOutput(val, i);
		}
		for (int i = 0; i < 8; i++) {
			int val = (d >> i) & 1;
			galil.DigitalBitOutput(val, i+8);
		}
	}
}
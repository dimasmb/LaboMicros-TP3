#include"encoder.h"

void process_uart();

void encoder_poll(){

	process_uart();
	static int first_pass = 1;
	if (!first_pass) {
		if (IsPeriodDone()) {
			if (bufferEmpty()) {
				SetMarkSpace(true); // si no tengo nada en el buffer de UART mando idle
			}
			else {

				SetMarkSpace(getChar());
			}
		}
	}
	else {
		SetMarkSpace(true);
		first_pass = 0;

	}
}


void process_uart(){
	if (!inputEmpty()){
			char newch = retreiveInput();
			uint16_t message=uart_2_fsk(newch);

			message = message<<5;
			for(int e=0; e<11; e++){
				char newbit;
				newbit = (((message<<e) & 0x8000)>>15);
				insertChar(newbit);
			}
	}
}


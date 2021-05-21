#include "ledDisplayAux.h"


#define NUM_COLUMN_DISPLAYAUX 8

uchar *data;
uchar display[8];

/* Load font */
#include "font-8x8.inc"

void Delay_xms(uint x)
{
	delay(x);
}

int power(int x, int y){
	int res = 1;
	int i;
	for(i=0; i<y; i++) {
		res *=x;
	}
	return res;
}

int* split_num(int x){
	static int arr[2];

	arr[0]= x%10;
	arr[1]= (x/10);

	return arr;
}

void Write_SPI(uchar reg, uchar data)
{
	uint8_t buf[2];
	buf[0] = reg;
	buf[1] = data;

	wiringPiSPIDataRW(SPI_CHANNEL, buf, 2);
}

void ActualizaDisplayAux(){
	int i;
	for(i = 0; i < NUM_COLUMN_DISPLAYAUX; i++) {
		/* Write_SPI(i+1, *((disp+char1*NUM_COLUMN_DISPLAYAUX)+i)); */
		Write_SPI(i+1, display[i]);
	}
}

void InitSPI()
{
	if (wiringPiSPISetup(SPI_CHANNEL, CLOCK_SPEED) < 0) {
	    fprintf (stderr, "Unable to setup wiringPiSpi: %s\n", strerror (errno)) ;
	    return;
	}

	Write_SPI(0x09,0x00);
	Write_SPI(0x0A,0xFF);
	/* Write_SPI(0x0A,0x03); */
	Write_SPI(0x0B,0x07);
	Write_SPI(0x0C,0x01);
	Write_SPI(0x0F,0x00);

	display_clear();
}

void push(char col){
	int i;
	for(i = 0; i < NUM_COLUMN_DISPLAYAUX - 1; i++) {
		display[i] = display[i+1];
	}
	display[NUM_COLUMN_DISPLAYAUX-1] = col;
}

void display_clear(){
	int i;
	for(i = 0; i < NUM_COLUMN_DISPLAYAUX; i++) {
		push(0);
	}
	ActualizaDisplayAux();
}


void display_score(int score[], int nPlayers){
	if(nPlayers > 2) {
		printf("Too many characters to display\n");
		return; 
	}
	
	// if(nPlayers == 1 && score[0]>=10) // do not show zero on the left
	if(nPlayers == 1 ) {
		score = split_num(score[0]);
		nPlayers = 2;
	}

	int i;
	for (i = 0; i< NUM_COLUMN_DISPLAYAUX; i++){
		data = nums[score[0]][i];

		int player;
		for(player = 1; player < nPlayers; player++){
			data += nums[score[player]][i]*power(16, player) ;
		}
		push(data);
	}
	ActualizaDisplayAux();
}

void display_countdown(int start, int delay){
	while(start >= 1){
		int i;
		const uint8_t* bits = &font[(start+48)*8];
		for (i = 0; i< NUM_COLUMN_DISPLAYAUX; i++){
			push(bits[i]);
		}
		ActualizaDisplayAux();
		Delay_xms(delay);
		start--;
	}
}

void display_icon(int icon){
	printf("%d\n", icon);
	int i;
	for (i = 0; i< NUM_COLUMN_DISPLAYAUX; i++){
		data = icons[icon][i];
		push(data);
	}
	ActualizaDisplayAux();
	
}

void display_text(char* text){
	int i = 0;
	for(i=0; i < strlen(text); i++) {
		char letter = text[i];
		
		int col; 
		const uint8_t* bits = &font[letter*8];
		for(col=0; col<NUM_COLUMN_DISPLAYAUX; col++){
			push(bits[col]);
			ActualizaDisplayAux();
			Delay_xms(80);
		}
		push(0);
		ActualizaDisplayAux();
		Delay_xms(80);

	}
	
}

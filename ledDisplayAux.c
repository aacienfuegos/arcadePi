#include "ledDisplayAux.h"


#define NUM_COLUMN_DISPLAYAUX 8

uchar *data;
uchar display[8];

/* Load letters */
#include "font-8x8l.inc"

uchar icons[3][8] = {
{0x00,0x00,0x55,0x55,0x75,0x54,0x55,0x00},//Hi!
{0xFF,0xFF,0x00,0x00,0x20,0x00,0x00,0x1C},// arkanoPi
{0x1C,0x00,0x00,0x00,0x08,0x00,0x00,0x38}// pong
};

uchar nums[10][8] = {
{0x00,0x0E,0x0A,0x0A,0x0A,0x0A,0x0E,0x00},//00
{0x00,0x02,0x06,0x0A,0x02,0x02,0x02,0x00},//01
{0x00,0x04,0x0A,0x02,0x04,0x08,0x0E,0x00},//02
{0x00,0x0E,0x02,0x0E,0x02,0x02,0x0E,0x00},//03
{0x00,0x02,0x06,0x0A,0x0F,0x02,0x02,0x00},//04
{0x00,0x0E,0x08,0x0C,0x02,0x02,0x0C,0x00},//05
{0x00,0x0E,0x08,0x0E,0x0A,0x0A,0x0E,0x00},//06
{0x00,0x0E,0x02,0x07,0x02,0x02,0x02,0x00},//07
{0x00,0x0E,0x0A,0x0E,0x0A,0x0A,0x0E,0x00},//08
{0x00,0x0E,0x0A,0x0E,0x02,0x02,0x02,0x00},//09

};

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
	Write_SPI(0x0B,0x07);
	Write_SPI(0x0A,0xFF);
	Write_SPI(0x0C,0x01);

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

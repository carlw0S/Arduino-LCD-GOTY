#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);



// game variables
int gameSpeed = 100;

// "sprites"
char player = 'o', spike = '<', clear = ' ';

// button variables
const int buttonPin = 13;
int buttonState = 0, buttonPrevState = 0;

// spike variables
int spikeColumn = 14, spikeRow = 1, spikeMoveLoad = 0, spikeMoveDelay = 5, spikeState = 1;



void setup() {

	pinMode(buttonPin, INPUT);
	lcd.begin(16, 2);
	lcd.setCursor(1, 1);
	lcd.print(player);
	lcd.setCursor(spikeColumn, spikeRow);
	lcd.print(spike);

}

void loop() {

	delay(gameSpeed);

	buttonState = digitalRead(buttonPin);

	// player position update
	if (buttonState == HIGH && buttonPrevState == LOW) {		// jump

		lcd.setCursor(1, 1);
		lcd.print(clear);
		lcd.setCursor(1, 0);
		lcd.print(player);

	}
	else if (buttonState == LOW && buttonPrevState == HIGH) {	// ground

		lcd.setCursor(1, 0);
		lcd.print(clear);
		lcd.setCursor(1, 1);
		lcd.print(player);

	}

	// spike position update
	if (spikeMoveLoad++ == spikeMoveDelay && spikeState == 1) {
		
		lcd.setCursor(spikeColumn, spikeRow);
		lcd.print(clear);
		if (spikeColumn != 0){

			lcd.setCursor(--spikeColumn, spikeRow);
			lcd.print(spike);

		}
		else		// when the spike reaches the left edge, it is disabled
			spikeState = 0;
			
		spikeMoveLoad = 0;
	}

	buttonPrevState = buttonState;		// needed in order to avoid unnecessary player position updates
}
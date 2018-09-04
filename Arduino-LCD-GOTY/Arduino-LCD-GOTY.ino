#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int buttonPin = 13;

int buttonState = 0;
int buttonPrevState = 0;

char player = 'o';
char spike = '<';
char clear = ' ';

int speed = 100;
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

	delay(speed);

	buttonState = digitalRead(buttonPin);

	if (buttonState == HIGH && buttonPrevState == LOW) {

		lcd.setCursor(1, 1);
		lcd.print(clear);
		lcd.setCursor(1, 0);
		lcd.print(player);

	}
	else if (buttonState == LOW && buttonPrevState == HIGH) {

		lcd.setCursor(1, 0);
		lcd.print(clear);
		lcd.setCursor(1, 1);
		lcd.print(player);

	}

	if (spikeMoveLoad++ == spikeMoveDelay && spikeState == 1) {
		
		lcd.setCursor(spikeColumn, spikeRow);
		lcd.print(clear);
		if (spikeColumn != 0){

			lcd.setCursor(--spikeColumn, spikeRow);
			lcd.print(spike);

		}
		else
			spikeState = 0;
			
		spikeMoveLoad = 0;
	}

	buttonPrevState = buttonState;
}
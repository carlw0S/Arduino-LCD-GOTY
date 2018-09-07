#include <LiquidCrystal.h>

void spikeMovement(int *spikeColumn, int spikeRow, int *spikeMoveLoad, int spikeMoveDelay, int *spikeState);

bool spikeCollision(int spikeColumn, int spikeRow, int playerRow);


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);



// game variables
float gameSpeed = 16.6666667;	// 60 fps??? i have no idea lmao

// "sprites"
char player = 'o', spike = '<', clear = ' ';

// button variables
const int buttonPin = 13;
int buttonState = 0, buttonPrevState = 0;

// player variables
int playerColumn = 1, playerRow = 1;

// spike variables
int spikeColumn = 14, spikeRow = 1, spikeMoveLoad = 0, spikeMoveDelay = 20, spikeState = 1;



void setup() {

	pinMode(buttonPin, INPUT);
	lcd.begin(16, 2);
	lcd.setCursor(playerRow, playerColumn);
	lcd.print(player);
	lcd.setCursor(spikeColumn, spikeRow);
	lcd.print(spike);
}

void loop() {

	delay(gameSpeed);

	buttonState = digitalRead(buttonPin);

	// player position update
	if (buttonState == HIGH && buttonPrevState == LOW) {		// jump

		lcd.setCursor(playerColumn, playerRow);
		lcd.print(clear);
		playerRow = 0;
		lcd.setCursor(playerColumn, playerRow);
		lcd.print(player);

	}
	else if (buttonState == LOW && buttonPrevState == HIGH) {	// ground

		lcd.setCursor(playerColumn, playerRow);
		lcd.print(clear);
		playerRow = 1;
		lcd.setCursor(playerColumn, playerRow);
		lcd.print(player);

	}

	// spike position update
	spikeMovement(&spikeColumn, spikeRow, &spikeMoveLoad, spikeMoveDelay, &spikeState);

	// spike collision detection
	if (spikeCollision(spikeColumn, spikeRow, playerRow))
		delay(3000);



	buttonPrevState = buttonState;		// needed in order to avoid unnecessary player position updates
}



void spikeMovement(int *spikeColumn, int spikeRow, int *spikeMoveLoad, int spikeMoveDelay, int *spikeState){

	if ((*spikeMoveLoad)++ == spikeMoveDelay && (*spikeState) == 1) {
		
		lcd.setCursor((*spikeColumn), spikeRow);
		lcd.print(clear);
		if ((*spikeColumn) != 0){

			lcd.setCursor(--(*spikeColumn), spikeRow);
			lcd.print(spike);

		}
		else		// when the spike reaches the left edge, it is disabled
			(*spikeState) = 0;
			
		(*spikeMoveLoad) = 0;
	}
}



bool spikeCollision(int spikeColumn, int spikeRow, int playerRow){

	bool collision = false;

	if (spikeColumn == 1 && spikeRow == playerRow)
		collision = true;

	return collision;
}
#include <LiquidCrystal.h>



void playerMovement(int buttonState, int buttonPrevState, int playerColumn, int *playerRow);

void spikeMovement(int *spikeColumn, int spikeRow, int *spikeMovementCounter, int spikeMovementDelay, int *spikeState);

bool spikeCollision(int spikeColumn, int spikeRow, int playerRow);

void spikeSpawn(int *spikeColumn, int *spikeRow, int *spikeState);



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
int spikeColumn = 17, spikeRow = 1, spikeMovementCounter = 0, spikeMovementDelay = 20, spikeState = 1;





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
	playerMovement(buttonState, buttonPrevState, playerColumn, &playerRow);

	// spike position update
	spikeMovement(&spikeColumn, spikeRow, &spikeMovementCounter, spikeMovementDelay, &spikeState);

	// spike collision detection
	if (spikeCollision(spikeColumn, spikeRow, playerRow))
		delay(3000);

	spikeSpawn(&spikeColumn, &spikeRow, &spikeState);

	buttonPrevState = buttonState;		// needed in order to avoid unnecessary player position updates
}





void playerMovement(int buttonState, int buttonPrevState, int playerColumn, int *playerRow){

	if (buttonState == HIGH && buttonPrevState == LOW) {		// jump

		lcd.setCursor(playerColumn, (*playerRow));
		lcd.print(clear);
		(*playerRow) = 0;
		lcd.setCursor(playerColumn, (*playerRow));
		lcd.print(player);

	}
	else if (buttonState == LOW && buttonPrevState == HIGH) {	// fall

		lcd.setCursor(playerColumn, (*playerRow));
		lcd.print(clear);
		(*playerRow) = 1;
		lcd.setCursor(playerColumn, (*playerRow));
		lcd.print(player);

	}
}



void spikeMovement(int *spikeColumn, int spikeRow, int *spikeMovementCounter, int spikeMovementDelay, int *spikeState){

	if ((*spikeMovementCounter)++ == spikeMovementDelay && (*spikeState) == 1) {		// the spike moves when its internal counter reaches a certain amount
		
		lcd.setCursor((*spikeColumn), spikeRow);
		lcd.print(clear);
		if ((*spikeColumn) != 0){		// the spike hasn't reached the left edge yet

			lcd.setCursor(--(*spikeColumn), spikeRow);
			lcd.print(spike);

		}
		else		// when the spike reaches the left edge, it is disabled
			(*spikeState) = 0;
			
		(*spikeMovementCounter) = 0;
	}
}



bool spikeCollision(int spikeColumn, int spikeRow, int playerRow){

	bool collision = false;

	if (spikeColumn == 1 && spikeRow == playerRow)
		collision = true;

	return collision;
}



void spikeSpawn(int *spikeColumn, int *spikeRow, int *spikeState){

	if ((*spikeState) == 0 && random(10) == 0){
		(*spikeState) = 1;
		(*spikeColumn) = 17;
		(*spikeRow) = random(2);
	}
}
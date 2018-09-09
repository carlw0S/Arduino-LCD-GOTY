#include <LiquidCrystal.h>



void gameOver();

void playerMovement();

void spikeMovement(int *spikeColumn, int spikeRow, int *spikeUpdateCounter, int *spikeState);

bool spikeCollision(int *spikeColumn, int spikeRow, int *spikeState);

void spikeSpawn(int *spikeColumn, int *spikeRow, int *spikeState);



// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);



// game variables
float gameSpeed = 16.6666667;	// 60 fps??? i have no idea lmao
int score = 0;
int objectUpdateCounter = 0, objectUpdateDelay = 8;

// "sprites"
char player = 'o', spike = '<', clear = ' ';

// button variables and macros
const int buttonPin = 13;
int buttonState = 0, buttonPrevState = 1;
#define BUTTON_PRESS buttonPrevState == LOW && buttonState == HIGH
#define BUTTON_RELEASE buttonPrevState == HIGH && buttonState == LOW

// player variables
int playerColumn = 1, playerRow = 1;

// spike variables
int spikeSpawned = 16, spikeMinDistance = 4;	// these variables assure that two spikes don't spawn too close to each other; each time a spike appears, spikeSpawned resets to 0, and increases with each object update; when it reaches spikeMinDistance, a new spike can spawn
int spikeColumn, spikeRow, spikeUpdateCounter = 0, spikeState = 0;
int spikeColumn2, spikeRow2, spikeUpdateCounter2 = 0, spikeState2 = 0;
int spikeColumn3, spikeRow3, spikeUpdateCounter3 = 0, spikeState3 = 0;






void setup() {

	pinMode(buttonPin, INPUT);
	lcd.begin(16, 2);
	lcd.setCursor(1, 1);
	lcd.print(player);

}

void loop() {

	delay(gameSpeed);

	// player position update
	playerMovement();

	// object update, checked every few frames determined by objectUpdateDelay
	if (objectUpdateCounter++ == objectUpdateDelay){

		objectUpdateCounter = 0;

		// spike position update
		spikeMovement(&spikeColumn, spikeRow, &spikeUpdateCounter, &spikeState);
		spikeMovement(&spikeColumn2, spikeRow2, &spikeUpdateCounter2, &spikeState2);
		spikeMovement(&spikeColumn3, spikeRow3, &spikeUpdateCounter3, &spikeState3);
		spikeSpawned++;

		// object spawning
		spikeSpawn(&spikeColumn, &spikeRow, &spikeState);
		spikeSpawn(&spikeColumn2, &spikeRow2, &spikeState2);
		spikeSpawn(&spikeColumn3, &spikeRow3, &spikeState3);

	}

	// spike collision detection
	if (spikeCollision(&spikeColumn, spikeRow, &spikeState) || spikeCollision(&spikeColumn2, spikeRow2, &spikeState2) || spikeCollision(&spikeColumn3, spikeRow3, &spikeState3))
		gameOver();

}





void gameOver(){

	int retry = 0;
	char buffer[17];



	spikeState = 0;
	spikeState2 = 0;
	spikeSpawned = 16;



	sprintf(buffer, "Score: %i", score);

	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("GAME OVER");
	lcd.setCursor(0,1);
	lcd.print(buffer);

	do{

		delay(gameSpeed);
		buttonState = digitalRead(buttonPin);
		if (BUTTON_PRESS)
			retry = 1;
		buttonPrevState = buttonState;

	}while (retry == 0);

	score = 0;
	lcd.clear();
	lcd.setCursor(1, 1);
	lcd.print(player);

}

void playerMovement(){

	buttonState = digitalRead(buttonPin);

	if (BUTTON_PRESS) {		// jump

		lcd.setCursor(playerColumn, playerRow);
		lcd.print(clear);
		playerRow = 0;
		lcd.setCursor(playerColumn, playerRow);
		lcd.print(player);

	}
	else if (BUTTON_RELEASE) {	// fall

		lcd.setCursor(playerColumn, playerRow);
		lcd.print(clear);
		playerRow = 1;
		lcd.setCursor(playerColumn, playerRow);
		lcd.print(player);

	}

	buttonPrevState = buttonState;		// needed in order to avoid unnecessary player position updates

}



void spikeMovement(int *spikeColumn, int spikeRow, int *spikeUpdateCounter, int *spikeState){

	if ((*spikeState) == 1) {		// the spike moves when it's enabled
		
		lcd.setCursor((*spikeColumn), spikeRow);
		lcd.print(clear);
		if ((*spikeColumn) != 0){		// the spike hasn't reached the left edge yet

			lcd.setCursor(--(*spikeColumn), spikeRow);
			lcd.print(spike);

		}
		else {		// when the spike reaches the left edge, it is disabled and 10 points are added to the score
			(*spikeState) = 0;
			score += 10;
		}
			
		(*spikeUpdateCounter) = 0;

	}

}



bool spikeCollision(int *spikeColumn, int spikeRow, int *spikeState){

	bool collision = false;

	if ((*spikeColumn) == 1 && spikeRow == playerRow){

		collision = true;
		(*spikeColumn) = 0;
		(*spikeState) = 0;

	}

	return collision;

}



void spikeSpawn(int *spikeColumn, int *spikeRow, int *spikeState){

	if (spikeSpawned >= spikeMinDistance && (*spikeState) == 0 && random(10) == 0){		// the spike has a 1/10 chance to appear

		(*spikeState) = 1;
		(*spikeColumn) = 16;
		(*spikeRow) = random(2);		// the row is random too
		spikeSpawned = 0;

	}

}
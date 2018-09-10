#include <LiquidCrystal.h>



// functions
void gameOver();
void playerMovement();
void objectMovement(int *objectColumn, int objectRow, int *objectState, int objectID);
bool objectCollision(int *objectColumn, int objectRow, int *objectState);
void objectSpawn(int *objectColumn, int *objectRow, int *objectState, int objectID);



// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);



// "sprites"
const char player = 'o', spike = '<', clear = ' ', coin = '*';

// game variables
float gameSpeed = 16.6666667;	// 60 fps??? i have no idea lmao
int score = 0;
int objectUpdateCounter = 0, objectUpdateDelay = 8;

// button variables and macros
const int buttonPin = 13;
int buttonState = 0, buttonPrevState = 1;
#define BUTTON_PRESS buttonPrevState == LOW && buttonState == HIGH
#define BUTTON_RELEASE buttonPrevState == HIGH && buttonState == LOW

// player variables
const int playerID = 0;
int playerColumn = 1, playerRow = 1;

// spike variables
const int spikeID = 1;
int spikeDistance = 16, spikeMinDistance = 4;	// needed not to spawn objects too close to each other; see objectSpawn() for more info
int spikeColumn, spikeRow, spikeState = 0;
int spike2Column, spike2Row, spike2State = 0;
int spike3Column, spike3Row, spike3State = 0;

// coin variables
const int coinID = 2;
int coinDistance = 16, coinMinDistance = 2;		// needed not to spawn objects too close to each other; see objectSpawn() for more info
int coinColumn, coinRow, coinState = 0;





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

		// object position update
		objectMovement(&spikeColumn, spikeRow, &spikeState, spikeID);
		objectMovement(&spike2Column, spike2Row, &spike2State, spikeID);
		objectMovement(&spike3Column, spike3Row, &spike3State, spikeID);
		objectMovement(&coinColumn, coinRow, &coinState, coinID);
		spikeDistance++;
		coinDistance++;

		// object spawning
		objectSpawn(&spikeColumn, &spikeRow, &spikeState, spikeID);
		objectSpawn(&spike2Column, &spike2Row, &spike2State, spikeID);
		objectSpawn(&spike3Column, &spike3Row, &spike3State, spikeID);
		objectSpawn(&coinColumn, &coinRow, &coinState, coinID);

	}

	// object collision detection
	if (objectCollision(&spikeColumn, spikeRow, &spikeState) || objectCollision(&spike2Column, spike2Row, &spike2State) || objectCollision(&spike3Column, spike3Row, &spike3State))

		gameOver();

	else if (objectCollision(&coinColumn, coinRow, &coinState)){

		lcd.setCursor(playerColumn, playerRow);
		lcd.print(player);
		score+=100;

	}

}





void gameOver(){

	int retry = 0;
	char buffer[17];


	// variable reset
	spikeState = 0;
	spike2State = 0;
	spike3State = 0;
	coinState = 0;
	spikeDistance = 16;
	coinDistance = 16;


	// convert score into a string in order to display it in the LCD
	sprintf(buffer, "Score: %i", score);

	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("GAME OVER");
	lcd.setCursor(0,1);
	lcd.print(buffer);

	do{		// press the button to retry

		delay(gameSpeed);
		buttonState = digitalRead(buttonPin);
		if (BUTTON_PRESS)
			retry = 1;
		buttonPrevState = buttonState;

	}while (retry == 0);


	// last things to reset
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



void objectMovement(int *objectColumn, int objectRow, int *objectState, int objectID){

	char sprite;

	switch(objectID){
		case spikeID: sprite = spike; break;
		case coinID: sprite = coin; break;
	}

	if ((*objectState) == 1) {		// the object moves when it's enabled
		
		lcd.setCursor((*objectColumn), objectRow);
		lcd.print(clear);
		if ((*objectColumn) != 0){		// the object hasn't reached the left edge yet

			lcd.setCursor(--(*objectColumn), objectRow);
			lcd.print(sprite);

		}
		else {		// when the object reaches the left edge, it is disabled and 10 points are added to the score if it was a spike
			(*objectState) = 0;
			if (objectID == spikeID)
				score += 10;
		}

	}

}



bool objectCollision(int *objectColumn, int objectRow, int *objectState){

	bool collision = false;

	if ((*objectColumn) == 1 && objectRow == playerRow){

		collision = true;
		(*objectColumn) = 0;
		(*objectState) = 0;

	}

	return collision;

}



void objectSpawn(int *objectColumn, int *objectRow, int *objectState, int objectID){

	if ( (*objectState) == 0 && random(10) == 0){		// the object has a 1/10 chance to appear and it has to be disabled

		// the following 'if' controls the distance between objects
		if ( (objectID == spikeID && spikeDistance >= spikeMinDistance && coinDistance >= coinMinDistance) || (objectID == coinID && spikeDistance >= coinMinDistance && coinDistance >= coinMinDistance) ){

			(*objectState) = 1;
			(*objectColumn) = 16;
			(*objectRow) = random(2);		// the row is random too
			
			switch(objectID){
				case spikeID: spikeDistance = 0; break;
				case coinID: coinDistance = 0; break;
			}

		}

	}

}
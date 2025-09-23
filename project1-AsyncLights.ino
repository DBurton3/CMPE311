//Variable Set ups

//LED 1
const int led1Pin = 2;
unsigned long LED1Interval = 0;
unsigned long LED1PrevTime = 0;
bool LED1State = LOW;

//LED2
const int led2Pin = 3;
unsigned long LED2Interval = 0;
unsigned long LED2PrevTime = 0;
bool LED2State = LOW;

//Dealing with the input
String inputString = "";
bool inputComplete = false;

//Other variables
String question = "What LED? (1 or 2)"; //Print statement 
int stage = 0;  // 0: Ask LED, 1: Ask interval
int selectedLED = 1;

void setup() {
  //Set up the pins
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);

  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);

  Serial.begin(9600);
  Serial.println(question);
}

void loop() {
  handleSerialInput();
  blinkLEDs();
}

void blinkLEDs() {
  unsigned long currentMillis = millis();

  // LED1
  if (LED1Interval > 0 && currentMillis - LED1PrevTime >= LED1Interval) {
    LED1PrevTime = currentMillis;
    LED1State = !LED1State;
    digitalWrite(led1Pin, LED1State);
  }

  // LED2
  if (LED2Interval > 0 && currentMillis - LED2PrevTime >= LED2Interval) {
    LED2PrevTime = currentMillis;
    LED2State = !LED2State;
    digitalWrite(led2Pin, LED2State);
  }
}

void handleSerialInput() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();

    //Deal with new lines and other spaces
    if (inChar == '\n' || inChar == '\r') {
      if (inputString.length() > 0) {
        processInput(inputString);
        inputString = "";
      }
    } else {
      inputString += inChar;
    }
  }
}

void processInput(String input) {
  input.trim();
  if (stage == 0) {
    int ledNum = input.toInt();
    if (ledNum == 1 || ledNum == 2) {
      selectedLED = ledNum;
      stage = 1;
      Serial.println("What interval (in msec)?");
    } 
  } else if (stage == 1) {
    int interval = input.toInt();
    if (interval > 0) {

      //Changes the LED interval depending on the LED
      if (selectedLED == 1){
        LED1Interval = interval;
      }
      else if(selectedLED == 2){
        LED2Interval = interval;
      }

      //Prints out the question
      Serial.println(question);
      stage = 0;
    } 
  }
}

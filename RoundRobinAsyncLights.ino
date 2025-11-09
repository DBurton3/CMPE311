//Project 2: Async lights with RoundRobin

// ---------- LED 1 ----------
const int led1Pin = 2;
unsigned long LED1Interval = 0;
unsigned long LED1PrevTime = 0;
bool LED1State = LOW;

// ---------- LED 2 ----------
const int led2Pin = 3;
unsigned long LED2Interval = 0;
unsigned long LED2PrevTime = 0;
bool LED2State = LOW;

// ---------- Serial Input ----------
String inputString = "";
bool inputComplete = false;

// ---------- State Variables ----------
String question = "What LED? (1 or 2)";
int stage = 0;          // 0: Ask LED, 1: Ask interval
int selectedLED = 1;

// ---------- Function Declarations ----------
void task_blinkLED1();
void task_blinkLED2();
void task_handleSerial();

// ---------- Function Pointer Array (Cyclic Executive) ----------
typedef void (*TaskFunction)();
TaskFunction taskList[] = {task_blinkLED1, task_blinkLED2, task_handleSerial};
const int NUM_TASKS = sizeof(taskList) / sizeof(taskList[0]);


// Setup
void setup() {
  delay(1000);  

  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);

  Serial.begin(9600);
  while (!Serial) { ; } // Wait for serial connection (some boards)

  Serial.println(question);
}

// Main Loop: Cyclic Executive (Round-Robin)
void loop() {
  static int currentTask = 0;
  // Call the current task
  taskList[currentTask]();

  // Move to next task (round robin)
  currentTask = (currentTask + 1) % NUM_TASKS;
}


//Blink LED 1
void task_blinkLED1() {
  unsigned long currentMillis = millis();
  if (LED1Interval > 0 && currentMillis - LED1PrevTime >= LED1Interval) {
    LED1PrevTime = currentMillis;
    LED1State = !LED1State;
    digitalWrite(led1Pin, LED1State);
  }
}


// Blink LED 2
void task_blinkLED2() {
  unsigned long currentMillis = millis();
  if (LED2Interval > 0 && currentMillis - LED2PrevTime >= LED2Interval) {
    LED2PrevTime = currentMillis;
    LED2State = !LED2State;
    digitalWrite(led2Pin, LED2State);
  }
}


// Task 3: Handle Serial Input (polled, not interrupt-driven)
void task_handleSerial() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();

    // Process newline or carriage return as end of input
    if (inChar == '\n' || inChar == '\r') {
      inputString.trim();
      if (inputString.length() > 0) {
        processInput(inputString);
      }
      inputString = "";
    } 
    // Only add printable characters
    else if (isPrintable(inChar)) {
      inputString += inChar;
    }
  }
}


// User Input
void processInput(String input) {
  input.trim();

  if (stage == 0) {
    int ledNum = input.toInt();
    if (ledNum == 1 || ledNum == 2) {
      selectedLED = ledNum;
      stage = 1;
      Serial.println("What interval (in msec)?");
    } else {
      Serial.println("Invalid LED. Enter 1 or 2.");
    }
  } 
  else if (stage == 1) {
    int interval = input.toInt();
    if (interval > 0) {
      if (selectedLED == 1) {
        LED1Interval = interval;
      } else if (selectedLED == 2) {
        LED2Interval = interval;
      }

      // Feedback to user
      Serial.print("LED ");
      Serial.print(selectedLED);
      Serial.print(" set to ");
      Serial.print(interval);
      Serial.println(" ms");

      Serial.println(question);
      stage = 0;
    } else {
      Serial.println("Invalid interval. Try again.");
    }
  }
}

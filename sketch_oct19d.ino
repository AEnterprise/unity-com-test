#include <SoftwareSerial.h>
#include <SerialCommand.h>
//command handler that does the heavy lifting for communication
SerialCommand sCmd;


#define PULSES 22

// sensor 1
#define PIN1A 2
#define PIN1B 3
int count1 = 0;
int current1A = LOW;
int prev1A = LOW;
int speed1 = 0;
int speed2 = 0;

//sensor 2

#define PIN2A 4
#define PIN2B 5
int count2 = 0;
int current2A = LOW;
int prev2A = LOW;

//timing
#define INTERVAL 100
#define MULTIPLIER (60000 / INTERVAL) / PULSES
long prevTime = 0;
long currentTime = 0;

//this is supposed to be faster then digitalRead
#define read1A bitRead(PIND, PIN1A)
#define read1B bitRead(PIND, PIN1B)
#define read2A bitRead(PIND, PIN2A)
#define read2B bitRead(PIND, PIN2B)


void setup() {
  //set serial clock, must mach on both ends!
  Serial.begin(9600);
  while (!Serial);

  //add ping handler
  sCmd.addCommand("PING", pingHandler);
  sCmd.addCommand("SPEEDS", speedHandler);

  //set datapins in input mode
  pinMode(PIN1A, INPUT);
  pinMode(PIN1B, INPUT);
  pinMode(PIN2A, INPUT);
  pinMode(PIN2B, INPUT);

  //init timing
  prevTime = millis();
}

void loop() {
  currentTime = millis();
  //update sensor 1

  current1A = read1A;
  if (current1A != prev1A) {
    if (read1B != current1A) {
      count1++;
    } else {
      count1--;
    }
    prev1A = current1A;
  }

  //update sensor 2

  current2A = read2A;
  if (current2A != prev2A) {
    if (read2B != current2A) {
      count2++;
    } else {
      count2--;
    }
    prev2A = current2A;
  }

  if (currentTime - prevTime >= INTERVAL) {
    //update sensor 1 speed
    speed1 = count1 * MULTIPLIER;
    count1 = 0;

    //update sensor 2 speed
    speed2 = count2 * MULTIPLIER;
    count2 = 0;
    
    prevTime = currentTime;

  
  if (Serial.available() > 0)
  sCmd.readSerial();
  }
}

void pingHandler (const char *command) {
 Serial.println("PONG");
}

void speedHandler (const char *command) {
 Serial.print(speed1);
 Serial.print("/");
 Serial.println(speed2);
}

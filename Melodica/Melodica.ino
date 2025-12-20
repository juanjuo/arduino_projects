int in1 = 10;
int in2 = 15;
int potentiometer;

void setup() {
  Serial.begin(9600);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  digitalWrite(in1, LOW);  //turn off
  digitalWrite(in2, LOW);
}

void loop() {

  potentiometer = analogRead(A3);
  potentiometer = map(potentiometer, 0, 1023, 0, 255);

  Serial.println(potentiometer);

  analogWrite(in1, potentiometer);

  digitalWrite(in2, LOW);
}

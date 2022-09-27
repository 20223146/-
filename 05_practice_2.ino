#define PIN_LED 7
unsigned int count, toggle;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(115200); // Initialize serial port
  while (!Serial) {
    ; // wait for serial port to connect.
  }
  Serial.println("Start");
  count =0;
  toggle = 1;
  digitalWrite(PIN_LED, 0); // turn off LED.
  delay(1000);
}

void loop() {
  Serial.println(++count);
  toggle = toggle_status(toggle);
  digitalWrite(PIN_LED, toggle);
  delay(100);

  while(count >= 12) {toggle = 0;}
}

int toggle_status(int toggle) {
  if(toggle == 0){
    ++toggle;}
  else{--toggle;}
  return toggle;
}

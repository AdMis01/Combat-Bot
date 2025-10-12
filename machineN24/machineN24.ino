#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

Servo esc;
int M1A = 13;
int M1B = 12;
int M2A = 14;
int M2B = 27;
int escPin = 26;
 
typedef struct struct_message {
  int x;
  int y;
  int s;
  int p;
} struct_message;

struct_message incomingData;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingDataBytes, int len) {
  memcpy(&incomingData, incomingDataBytes, sizeof(incomingData));
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Błąd inicjalizacji ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
  
  pinMode(M1A, OUTPUT);
  pinMode(M1B, OUTPUT);
  pinMode(M2A, OUTPUT);
  pinMode(M2B, OUTPUT);
  
  digitalWrite(M1A, LOW);
  digitalWrite(M1B, LOW);
  digitalWrite(M2A, LOW);
  digitalWrite(M2B, LOW);

  esc.attach(escPin, 1000, 2000);
  delay(2000);
}

void loop() {
  Serial.print("x: ");
  Serial.print(incomingData.x);
  Serial.print(" y: ");
  Serial.print(incomingData.y);
  Serial.print(" s: ");
  Serial.print(incomingData.s);
  Serial.print(" p: ");
  Serial.println(incomingData.p);
  int x = incomingData.x;
  int y = incomingData.y;
  int s = incomingData.s;
  int p = incomingData.p;

  if(s == 1 && p > 50){
    p = map(p, 0, 1023, 1000, 2000);
    esc.writeMicroseconds(p);
  } else {
    esc.writeMicroseconds(1000);
  }
  
  if (y > 3000) { 
    digitalWrite(M1A, LOW);
    digitalWrite(M1B, HIGH);
    digitalWrite(M2A, LOW);
    digitalWrite(M2B, HIGH);
  } 
  else if (y < 1500) { 
    digitalWrite(M1A, HIGH);
    digitalWrite(M1B, LOW);
    digitalWrite(M2A, HIGH);
    digitalWrite(M2B, LOW);
  } 
  else if (x > 3000) {
    digitalWrite(M1A, LOW);
    digitalWrite(M1B, HIGH);
    digitalWrite(M2A, HIGH);
    digitalWrite(M2B, LOW);
  } 
  else if (x < 1500) {
    digitalWrite(M1A, HIGH);
    digitalWrite(M1B, LOW);
    digitalWrite(M2A, LOW);
    digitalWrite(M2B, HIGH);
  } 
  else {
    digitalWrite(M1A, LOW);
    digitalWrite(M1B, LOW);
    digitalWrite(M2A, LOW);
    digitalWrite(M2B, LOW);
  }
  delay(100);
}

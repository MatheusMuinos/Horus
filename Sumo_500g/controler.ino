#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>
#include <Arduino.h>

// --- Definição dos Pinos ---
const int IN1 = 14; 
const int IN2 = 27;
const int IN3 = 26;
const int IN4 = 25;
const int ENA = 32; 
const int ENB = 33;

// --- Configurações de PWM para o ESP32 ---
const int freq = 5000;
const int canalA = 0;
const int canalB = 1;
const int resolucao = 8;

int velocidade = 200; 

void setup() {
  Serial.begin(115200);
  Dabble.begin("Neftis"); 

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  ledcSetup(canalA, freq, resolucao);
  ledcSetup(canalB, freq, resolucao);
  ledcAttachPin(ENA, canalA);
  ledcAttachPin(ENB, canalB);
}

void parar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  ledcWrite(canalA, 0);
  ledcWrite(canalB, 0);
}

void loop() {
  Dabble.processInput(); 

  if (GamePad.isUpPressed()) {
    // FRENTE (Antes ia para esquerda, agora corrigido para frente)
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
    ledcWrite(canalA, velocidade);
    ledcWrite(canalB, velocidade);
  }
  
  else if (GamePad.isDownPressed()) {
    // TRÁS (Antes girava para direita, agora corrigido para trás)
    digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    ledcWrite(canalA, velocidade);
    ledcWrite(canalB, velocidade);
  }
  
  else if (GamePad.isLeftPressed()) {
    // ESQUERDA (Antes ia para trás, agora corrigido para girar à esquerda)
    digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
    ledcWrite(canalA, velocidade);
    ledcWrite(canalB, velocidade);
  }
  
  else if (GamePad.isRightPressed()) {
    // DIREITA (Antes ia para frente, agora corrigido para girar à direita)
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    ledcWrite(canalA, velocidade);
    ledcWrite(canalB, velocidade);
  }
  
  else {
    parar(); 
  }
}
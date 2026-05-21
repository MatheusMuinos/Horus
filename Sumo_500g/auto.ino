#include <DabbleESP32.h>
#include <Arduino.h>

// ultrassonicos
const int US_FRENTE_TRIG = 18;
const int US_FRENTE_ECHO = 19;
const int US_ESQ_TRIG = 21;
const int US_ESQ_ECHO = 22;
const int US_DIR_TRIG = 23;
const int US_DIR_ECHO = 25;

// Infravermelho
const int PINO_IR_FRENTE = 26;
const int PINO_IR_TRAS = 4;
const bool IR_PULLUP = true;
const int IR_LINHA_ATIVA = LOW; // use HIGH se seu sensor retorna HIGH no preto

// motor esquerdo
const int MOTOR_E_IN3 = 15;
const int MOTOR_E_IN4 = 14;
const int MOTOR_E_PWM = 32;

// motor direito
const int MOTOR_D_IN1 = 12;
const int MOTOR_D_IN2 = 27;
const int MOTOR_D_PWM = 33;

// pwm
const int PWM_FREQUENCIA_HZ = 1000;
const int PWM_RESOLUCAO_BITS = 8;
const int PWM_MAXIMO = 255;
const int PWM_CANAL_E = 0;
const int PWM_CANAL_D = 1;

// distancias de deteccao
const int DIST_DET_FRENTE_CM = 50;
const int DIST_DET_LADO_CM = 40;

// 8000 us e aprox 137 cm para leitura
const int TEMPO_MAX_ECHO_US = 8000;

// velocidades
const int VELOCIDADE_MAX = 255;
const int VELOCIDADE_GIRO = 220;
const int VELOCIDADE_BUSCA_GIRO = 220;
const int VELOCIDADE_BUSCA_AVANCO = 200;
const int VELOCIDADE_RE = 220;

// giro menor para voltar mais rapido ao combate depois de fugir da borda
const int TEMPO_RECUO_MS = 250;
const int TEMPO_GIRO_FUGA_MS = 180;

// busca alternada por tempo + micro-avanco
const int BUSCA_TROCA_MS = 300;
const int BUSCA_AVANCO_MS = 160;
const int BUSCA_CICLOS_PARA_AVANCO = 2;

// estado de referencia
bool buscaAlternada = true;
unsigned long tempoUltimaTrocaBuscaMs = 0;
unsigned long inicioAvancoBuscaMs = 0;
int ciclosBusca = 0;
bool buscaEmAvanco = false;

// funcoes
long lerUltrassonicoCm(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracao = pulseIn(echoPin, HIGH, TEMPO_MAX_ECHO_US);

  if (duracao == 0) {
    return 999; // sem eco
  }

  return (long)(duracao * 0.0343 / 2.0);
}

bool lerIRBordaFrente() {
  int estado = digitalRead(PINO_IR_FRENTE);
  return estado == IR_LINHA_ATIVA;
}

bool lerIRBordaTras() {
  int estado = digitalRead(PINO_IR_TRAS);
  return estado == IR_LINHA_ATIVA;
}

void motorEsquerdo(int velocidade) {
  velocidade = constrain(velocidade, -PWM_MAXIMO, PWM_MAXIMO);

  if (velocidade > 0) {
    digitalWrite(MOTOR_E_IN1, HIGH);
    digitalWrite(MOTOR_E_IN2, LOW);
    ledcWrite(PWM_CANAL_E, velocidade);
  } else if (velocidade < 0) {
    digitalWrite(MOTOR_E_IN1, LOW);
    digitalWrite(MOTOR_E_IN2, HIGH);
    ledcWrite(PWM_CANAL_E, -velocidade);
  } else {
    digitalWrite(MOTOR_E_IN1, LOW);
    digitalWrite(MOTOR_E_IN2, LOW);
    ledcWrite(PWM_CANAL_E, 0);
  }
}

void motorDireito(int velocidade) {
  velocidade = constrain(velocidade, -PWM_MAXIMO, PWM_MAXIMO);

  if (velocidade > 0) {
    digitalWrite(MOTOR_D_IN1, HIGH);
    digitalWrite(MOTOR_D_IN2, LOW);
    ledcWrite(PWM_CANAL_D, velocidade);
  } else if (velocidade < 0) {
    digitalWrite(MOTOR_D_IN1, LOW);
    digitalWrite(MOTOR_D_IN2, HIGH);
    ledcWrite(PWM_CANAL_D, -velocidade);
  } else {
    digitalWrite(MOTOR_D_IN1, LOW);
    digitalWrite(MOTOR_D_IN2, LOW);
    ledcWrite(PWM_CANAL_D, 0);
  }
}

void parar() {
  motorEsquerdo(0);
  motorDireito(0);
}

void avancarMaximo() {
  motorEsquerdo(VELOCIDADE_MAX);
  motorDireito(VELOCIDADE_MAX);
}

void recuarRapido() {
  motorEsquerdo(-VELOCIDADE_RE);
  motorDireito(-VELOCIDADE_RE);
}

void girarEsquerdaRapido() {
  motorEsquerdo(-VELOCIDADE_GIRO);
  motorDireito(VELOCIDADE_GIRO);
}

void girarDireitaRapido() {
  motorEsquerdo(VELOCIDADE_GIRO);
  motorDireito(-VELOCIDADE_GIRO);
}

void buscarOponente() {
    
  unsigned long agora = millis();

  if (buscaEmAvanco) {
    motorEsquerdo(VELOCIDADE_BUSCA_AVANCO);
    motorDireito(VELOCIDADE_BUSCA_AVANCO);
    if (agora - inicioAvancoBuscaMs >= (unsigned long)BUSCA_AVANCO_MS) {
      buscaEmAvanco = false;
      tempoUltimaTrocaBuscaMs = agora;
    }
    return;
  }

  if (agora - tempoUltimaTrocaBuscaMs >= (unsigned long)BUSCA_TROCA_MS) {
    buscaAlternada = !buscaAlternada;
    tempoUltimaTrocaBuscaMs = agora;
    ciclosBusca++;
    if (ciclosBusca >= BUSCA_CICLOS_PARA_AVANCO) {
      ciclosBusca = 0;
      buscaEmAvanco = true;
      inicioAvancoBuscaMs = agora;
    }
  }

  if (buscaAlternada) {
    motorEsquerdo(-VELOCIDADE_BUSCA_GIRO);
    motorDireito(VELOCIDADE_BUSCA_GIRO);
  } else {
    motorEsquerdo(VELOCIDADE_BUSCA_GIRO);
    motorDireito(-VELOCIDADE_BUSCA_GIRO);
  }
}

void fugirDaBorda(bool bordaNaFrente) {
  parar();
  delay(20);

  if (bordaNaFrente) {
    recuarRapido();
  } else {
    motorEsquerdo(VELOCIDADE_RE);
    motorDireito(VELOCIDADE_RE);
  }

  delay(TEMPO_RECUO_MS);

  if (buscaAlternada) {
    girarDireitaRapido();
  } else {
    girarEsquerdaRapido();
  }

  delay(TEMPO_GIRO_FUGA_MS);

  parar();

  buscaAlternada = !buscaAlternada;
}

void loopDecisao() {
  // p1: não sair da arena
  bool bordaFrente = lerIRBordaFrente();
  bool bordaTras = lerIRBordaTras();
  if (bordaFrente || bordaTras) {
    fugirDaBorda(bordaFrente);
    return;
  }

  // p2: leitura dos sensores ultrassônicos
  long dFrente = lerUltrassonicoCm(US_FRENTE_TRIG, US_FRENTE_ECHO);
  long dEsq = lerUltrassonicoCm(US_ESQ_TRIG, US_ESQ_ECHO);
  long dDir = lerUltrassonicoCm(US_DIR_TRIG, US_DIR_ECHO);

  // p3: atacar diretamente
  if (dFrente <= DIST_DET_FRENTE_CM) {
    avancarMaximo();
  }

  // p4: alinhar pela esquerda
  else if (dEsq <= DIST_DET_LADO_CM) {
    girarEsquerdaRapido();
  }

  // p5: alinhar pela direita
  else if (dDir <= DIST_DET_LADO_CM) {
    girarDireitaRapido();
  }

  // p6: buscar adversário
  else {
    buscarOponente();
  }
}


void setup() {
  pinMode(US_FRENTE_TRIG, OUTPUT);
  pinMode(US_FRENTE_ECHO, INPUT);

  pinMode(US_ESQ_TRIG, OUTPUT);
  pinMode(US_ESQ_ECHO, INPUT);

  pinMode(US_DIR_TRIG, OUTPUT);
  pinMode(US_DIR_ECHO, INPUT);

  if (IR_PULLUP) {
    pinMode(PINO_IR_FRENTE, INPUT_PULLUP);
    pinMode(PINO_IR_TRAS, INPUT_PULLUP);
  } else {
    pinMode(PINO_IR_FRENTE, INPUT);
    pinMode(PINO_IR_TRAS, INPUT);
  }

  pinMode(MOTOR_E_IN3, OUTPUT);
  pinMode(MOTOR_E_IN4, OUTPUT);

  pinMode(MOTOR_D_IN1, OUTPUT);
  pinMode(MOTOR_D_IN2, OUTPUT);

  ledcSetup(PWM_CANAL_E, PWM_FREQUENCIA_HZ, PWM_RESOLUCAO_BITS);
  ledcSetup(PWM_CANAL_D, PWM_FREQUENCIA_HZ, PWM_RESOLUCAO_BITS);

  ledcAttachPin(MOTOR_E_PWM, PWM_CANAL_E);
  ledcAttachPin(MOTOR_D_PWM, PWM_CANAL_D);

  parar();
}

void loop() {
  loopDecisao();
}
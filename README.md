# Horus

Repositorio do projeto Horus, um robo sumo com foco em combate autonomo e controle manual para testes.

## Estrutura
- `Sumo_500g/`: codigo do robo na categoria 500g.
	- `V1.ino`: logica autonoma (sensores ultrassonicos, infravermelho e controle de motores).
	- `controler.ino`: controle manual via Bluetooth usando Dabble GamePad.

## Hardware (resumo)
- ESP32 Dev Kit V1
- Ponte H para motores DC
- Sensores ultrassonicos (frente e laterais)
- Sensores IR para borda da arena

## Observacoes
- Ajuste os pinos no codigo conforme a sua montagem.
- Os parametros de velocidade e tempos podem ser calibrados para o seu chassi.
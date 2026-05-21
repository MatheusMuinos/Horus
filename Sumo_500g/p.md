Quero que você atue como um programador especialista em robótica embarcada, ESP32, Arduino C++, sensores ultrassônicos, sensores infravermelhos e controle de motores DC. O codigo tem que ser em portugues

Estou programando um robô sumô para uma competição em arena circular. O robô precisa ser extremamente rápido, responsivo e funcional. O objetivo principal é detectar o robô adversário, perseguir o adversário agressivamente e evitar sair da arena.

A placa usada será um ESP32.

Componentes usados:

1. Três sensores ultrassônicos Parallax 28015
   - Um sensor ficará na frente, apontado horizontalmente para detectar o adversário diretamente à frente.
   - Dois sensores ficarão ao lado do sensor frontal, levemente inclinados para esquerda e direita.
   - O sensor esquerdo detectará adversários na diagonal esquerda.
   - O sensor direito detectará adversários na diagonal direita.

2. Um sensor infravermelho de obstáculo
   - Será usado para detectar a borda da arena.
   - A arena é circular.
   - A borda da arena possui uma linha preta circular.
   - O robô deve permanecer dentro da arena.
   - Quando o sensor IR detectar a linha preta, o robô deve recuar rapidamente e girar para voltar ao centro.

3. Dois motores DC 3V–6V com caixa de redução
   - Um motor esquerdo.
   - Um motor direito.
   - Eles serão controlados por ponte H ou driver de motor compatível com ESP32.
   - O código deve usar PWM do ESP32 por ledcWrite.
   - Não use analogWrite, pois o ESP32 usa canais PWM ledc.

Objetivo do comportamento:

1. Prioridade máxima: não sair da arena.
   - Se o sensor IR detectar a linha preta, o robô deve parar imediatamente, recuar por curto período e girar para longe da borda.
   - Essa lógica deve ter prioridade sobre qualquer leitura dos sensores ultrassônicos.

2. Se o adversário for detectado pelo sensor frontal:
   - O robô deve avançar em velocidade máxima.
   - Deve atacar diretamente.

3. Se o adversário for detectado pelo sensor ultrassônico esquerdo:
   - O robô deve girar rapidamente para a esquerda até alinhar com o adversário.
   - Depois deve avançar.

4. Se o adversário for detectado pelo sensor ultrassônico direito:
   - O robô deve girar rapidamente para a direita até alinhar com o adversário.
   - Depois deve avançar.

5. Se nenhum adversário for detectado:
   - O robô deve procurar o adversário.
   - Use uma estratégia eficiente de busca, como girar no próprio eixo com velocidade média/alta.
   - A busca deve ser rápida, mas controlada.

Requisitos técnicos do código:

- Escreva o código em Arduino C++ para ESP32.
- Use ledcSetup, ledcAttachPin e ledcWrite para PWM.
- Use constantes configuráveis no início do código.
- Evite delays longos.
- Use delays pequenos apenas em manobras críticas, como recuar da borda.
- Organize o código em funções separadas para:
  - Ler sensores ultrassônicos.
  - Ler sensor IR.
  - Controlar motor esquerdo.
  - Controlar motor direito.
  - Avançar.
  - Recuar.
  - Girar para esquerda.
  - Girar para direita.
  - Parar.
  - Buscar adversário.
  - Executar lógica principal de decisão.
- Crie uma estrutura clara de decisão por prioridades.
- Inclua comentários explicando as partes principais.
- O código deve ser fácil de calibrar.

Configuração inicial sugerida de pinos para ESP32:

Sensor ultrassônico frontal:
- trigger: GPIO 18
- echo: GPIO 19

Sensor ultrassônico esquerdo:
- trigger: GPIO 21
- echo: GPIO 22

Sensor ultrassônico direito:
- trigger: GPIO 23
- echo: GPIO 25

Sensor IR:
- pino digital: GPIO 26

Motor esquerdo:
- IN1: GPIO 27
- IN2: GPIO 14
- PWM: GPIO 32

Motor direito:
- IN1: GPIO 12
- IN2: GPIO 13
- PWM: GPIO 33

Configuração PWM:
- canal PWM motor esquerdo: 0
- canal PWM motor direito: 1
- frequência PWM: 1000 Hz
- resolução PWM: 8 bits
- velocidade máxima: 255

Atenção:
- Verifique se os pinos escolhidos são seguros para ESP32.
- Evite pinos problemáticos de boot, se possível.
- Se algum pino for inadequado, corrija automaticamente e explique a alteração.
- Não use GPIO 34, 35, 36 ou 39 para saída, pois são apenas entrada.
- Não use os pinos GPIO 6 a 11, pois normalmente são ligados à memória flash.
- Considere que o ESP32 usa lógica de 3.3V.
- Se os sensores ultrassônicos retornarem sinal Echo em 5V, explique que será necessário divisor de tensão ou level shifter para proteger o ESP32.

A lógica principal deve seguir esta prioridade:

1. Ler sensor IR.
2. Se detectou borda:
   - Parar.
   - Recuar rápido.
   - Girar para longe da borda.
   - Voltar ao loop.

3. Ler sensores ultrassônicos.
4. Se sensor frontal detectou adversário:
   - Avançar com força total.

5. Senão, se sensor esquerdo detectou adversário:
   - Girar para esquerda.

6. Senão, se sensor direito detectou adversário:
   - Girar para direita.

7. Senão:
   - Procurar adversário girando.

Quero que você gere:

1. Código completo funcional para ESP32.
2. Explicação curta da pinagem.
3. Explicação de como calibrar:
   - Distância de detecção dos sensores ultrassônicos.
   - Sensor IR para linha preta.
   - Velocidade dos motores.
   - Tempo de recuo.
   - Tempo de giro.
   - Estratégia de busca.
4. Sugestões de melhoria para competição.
5. Possíveis erros comuns e como corrigir.

O código deve ser otimizado para competição de robô sumô, priorizando velocidade de resposta, simplicidade, estabilidade e confiabilidade.
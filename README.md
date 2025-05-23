# Sistema de Monitoramento e Controle de Qualidade da Água para Reuso Doméstico com IoT

Este projeto simula um sistema IoT embarcado com ESP32, capaz de monitorar a qualidade da água por meio da leitura de temperatura, umidade e pH, utilizando sensores conectados e comunicação em tempo real via protocolo MQTT. Caso os valores estejam fora da faixa segura, uma bomba d'água (simulada por um LED) é automaticamente acionada.

## Funcionamento e Uso

O sistema é implementado no ambiente de simulação [Wokwi]([https://wokwi.com/](https://wokwi.com/projects/431744424048291841)) e é composto por:

- Um microcontrolador **ESP32 DevKit V1** com Wi-Fi integrado.
- Sensor **DHT22** para medir temperatura e umidade.
- Sensor de **pH simulado** via componente customizado (analogRead).
- LEDs que indicam:
  - Status da conexão Wi-Fi (LED azul)
  - Status da conexão MQTT (LED vermelho)
  - Acionamento da bomba (LED amarelo)

Quando a **temperatura > 30°C** ou **pH < 6.5 ou > 8.5**, a bomba é ligada automaticamente.

## Software e Código-Fonte 

O código principal está no arquivo:

- [`main.ino`](main.ino): contém a lógica de leitura dos sensores, conexão MQTT, controle da bomba e publicações dos dados.
- [`diagram.json`](diagram.json): define os componentes e conexões na simulação do Wokwi.
- [`custom_chips/soil_sensor.chip.c`](custom_chips/soil_sensor.chip.c) e [`soil_sensor.chip.json`](custom_chips/soil_sensor.chip.json): arquivos responsáveis por simular o comportamento do sensor de pH.

## Hardware Utilizado

| Componente             | Função                                      |
|------------------------|---------------------------------------------|
| ESP32 DevKit V1        | Microcontrolador principal com Wi-Fi        |
| Sensor DHT22           | Leitura de temperatura e umidade            |
| Sensor de pH (simulado)| Leitura analógica do nível de acidez        |
| LED Azul               | Indica conexão com Wi-Fi                    |
| LED Vermelho           | Indica conexão com o broker MQTT            |
| LED Amarelo            | Simula o acionamento da bomba d’água        |
| Resistores 150Ω        | Limitadores de corrente nos LEDs            |

Todos os componentes são virtuais, montados no simulador **Wokwi**. Não houve impressão 3D ou componentes físicos neste projeto.

## Comunicação, Interfaces e Protocolos

- **Interface Wi-Fi**: conexão do ESP32 à internet via rede `Wokwi-GUEST`
- **Protocolo MQTT**: troca de mensagens entre ESP32 e o broker
  - Broker utilizado: `test.mosquitto.org`
  - Porta: `1883`
  - Tópicos publicados:
    - `sensor/temperatura`
    - `sensor/umidade`
    - `sensor/ph`
- **Bibliotecas usadas**:
  - `WiFi.h`
  - `PubSubClient.h`
  - `DHTesp.h`

## Conectividade via Internet e Protocolo MQTT

O projeto utiliza protocolo **MQTT** sobre rede **TCP/IP**, realizando:

- **Publicação de dados em tópicos MQTT**
- **Conexão com broker público em tempo real**
- **Acionamento remoto de atuador (bomba) baseado em regras de sensor**

A lógica é implementada e testada em tempo real no simulador [Wokwi](https://wokwi.com/projects/431744424048291841), com suporte a MQTT real, conforme exigido.

## Licença

Este projeto foi desenvolvido exclusivamente para fins acadêmicos.

Desenvolvido por: **Julia Sampaio Cavalcanti Silva – 2025**

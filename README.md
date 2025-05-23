# Sistema de Monitoramento e Controle de Qualidade da Água para Reuso Doméstico com IoT

Este projeto simula um sistema IoT embarcado com ESP32, capaz de monitorar a qualidade da água reutilizada por meio de sensores e realizar o acionamento automático de uma bomba d'água com base nos dados recebidos, utilizando o protocolo MQTT para comunicação com a internet.

## Funcionalidades

- Conexão à internet via Wi-Fi
- Leitura de **temperatura** e **umidade** com sensor **DHT22**
- LED indicando conexão com o **Wi-Fi**
- LED indicando conexão com o **broker MQTT**
- LED simulando a **bomba d'água**, ativada automaticamente com base na **temperatura**
- Comunicação em tempo real com **broker MQTT público** (`test.mosquitto.org`)

## Componentes Utilizados

- ESP32 DevKit V1
- Sensor DHT22
- LED (simulando bomba 5V)
- Conexão via Wi-Fi (rede Wokwi-GUEST)
- Protocolo MQTT

## Software

O código principal está no arquivo [main.ino](main.ino).  
Você pode simular o projeto online no [Wokwi](https://wokwi.com/projects/431693626687148033).

## Comunicação MQTT

O sistema se conecta ao broker público `broker.hivemq.com` e publica os dados nos seguintes tópicos:

- `sensor/temperatura`
- `sensor/umidade`
- `sensor/ph`

O acionamento da bomba ocorre localmente com base nas regras:

- Temperatura > 30°C
- pH < 6.5 ou pH > 7.5

## Capturas de Tela

![Image](https://github.com/user-attachments/assets/b77ffdeb-a3a9-4bb5-af62-1c94f9c12a24) 
*Figura 1 – Diagrama da montagem no Fritzing.*

![Console MQTT](imagens/mqtt-console.png)  
*Figura 2 – Console com dados publicados via MQTT.*

---

**Desenvolvido por Julia Sampaio Cavalcanti Silva – 2025**

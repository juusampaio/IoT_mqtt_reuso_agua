
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DHTPIN 15
#define DHTTYPE DHT22
#define LEDPIN 2
#define PHPIN 33

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.begin(115200);
  Serial.println("Iniciando ESP32...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado.");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (client.connect("ESP32Client123")) {
      Serial.println("conectado");
    } else {
      Serial.print("falha, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(LEDPIN, OUTPUT);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float temp = dht.readTemperature();
  float umid = dht.readHumidity();
  int phValue = analogRead(PHPIN);
  float ph = map(phValue, 0, 4095, 0, 14);

  if (!isnan(temp) && !isnan(umid)) {
    char tempStr[8], umidStr[8], phStr[8];
    dtostrf(temp, 1, 2, tempStr);
    dtostrf(umid, 1, 2, umidStr);
    dtostrf(ph, 1, 2, phStr);

    client.publish("sensor/temperatura", tempStr);
    client.publish("sensor/umidade", umidStr);
    client.publish("sensor/ph", phStr);

    Serial.print("Temperatura: ");
    Serial.print(tempStr);
    Serial.print(" | Umidade: ");
    Serial.print(umidStr);
    Serial.print(" | pH: ");
    Serial.println(phStr);

    if (temp > 30.0 || ph < 6.5 || ph > 7.5) {
      digitalWrite(LEDPIN, HIGH);
    } else {
      digitalWrite(LEDPIN, LOW);
    }
  }

  delay(10000);
}

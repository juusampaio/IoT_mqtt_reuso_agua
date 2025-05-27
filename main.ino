#include <WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>

#define LED_WIFI 2     // LED azul (Wi-Fi)
#define LED_MQTT 4     // LED vermelho (MQTT)
#define BOMBA_PIN 5    // LED amarelo (simulando bomba)

const int DHT_PIN = 15;
const int PH_PIN = 34;

#define MODO_TESTE false  // Altere para false para usar os sensores reais

DHTesp dhtSensor;

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "test.mosquitto.org"; 

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {

  delay(10);

  Serial.println();
  Serial.println("Conectando ao WiFi...");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (client.connect("espClient")) {
      Serial.println("Conectado ao broker MQTT!");
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
  pinMode(LED_WIFI, OUTPUT);
  pinMode(LED_MQTT, OUTPUT);
  pinMode(BOMBA_PIN, OUTPUT);

  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
  Serial.println("Wi-Fi desconectado! Tentando reconectar...");
  setup_wifi();
}

  if (WiFi.status() == WL_CONNECTED) {
  digitalWrite(LED_WIFI, HIGH);
  } else {
  digitalWrite(LED_WIFI, LOW);
}

if (!client.connected()) {
  reconnect();
}
client.loop();

digitalWrite(LED_MQTT, client.connected() ? HIGH : LOW);

  float temperatura, umidade, ph;

  if (MODO_TESTE) {
    temperatura = 27.5;  // simulado
    umidade = 40.0;      // simulado
    ph = 6.8;            // simulado
  } else {
    TempAndHumidity data = dhtSensor.getTempAndHumidity();
    temperatura = data.temperature;
    umidade = data.humidity;

  int phAnalog = analogRead(PH_PIN);
  float ph = map(phAnalog, 0, 4095, 0, 1400); 
  }

  client.publish("sensor/temperatura", String(temperatura, 2).c_str());
  client.publish("sensor/umidade", String(umidade, 1).c_str());
  client.publish("sensor/ph", String(ph, 2).c_str());
  
  Serial.println("Temp: " + String(temperatura) + "°C");
  Serial.println("Umidade: " + String(umidade) + "%");
  Serial.println("pH: " + String(ph));
  Serial.println("---");

  if (temperatura > 30.0 || ph < 6.5 || ph > 8.5) {
    digitalWrite(BOMBA_PIN, HIGH);

    if (temperatura > 30.0 && (ph < 6.5 || ph > 8.5)) {
    Serial.println("Temperatura alta e pH fora da faixa! Bomba LIGADA.");
  } else if (temperatura > 30.0) {
    Serial.println("Temperatura alta! Bomba LIGADA.");
  } else if (ph < 6.5 || ph > 8.5) {
    Serial.println("pH fora da faixa segura! Bomba LIGADA.");
  }

  } else {
    digitalWrite(BOMBA_PIN, LOW);
    Serial.println("Temperatura e pH dentro dos padrões. Bomba DESLIGADA.");
  }

  Serial.println("---");
  delay(10000); 
}

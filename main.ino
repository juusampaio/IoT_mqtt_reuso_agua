#include <WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>

#define LED_WIFI 2     // LED azul (Wi-Fi)
#define LED_MQTT 4     // LED vermelho (MQTT)
#define BOMBA_PIN 5    // LED amarelo (simulando bomba)

const int DHT_PIN = 15;

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
      Serial.println(" tentando em 5 segundos");
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

TempAndHumidity data = dhtSensor.getTempAndHumidity();
  String temperatura = String(data.temperature, 2);
  String umidade = String(data.humidity, 1);

  Serial.println("Temp: " + temperatura + "°C");
  Serial.println("Umidade: " + umidade + "%");
  Serial.println("---");

  client.publish("sensor/temperatura", temperatura.c_str());
  client.publish("sensor/umidade", umidade.c_str());

  if (data.temperature > 30.0) {
    digitalWrite(BOMBA_PIN, HIGH);
    Serial.println("Temperatura alta! Bomba LIGADA.");
  } else {
    digitalWrite(BOMBA_PIN, LOW);
    Serial.println("Temperatura normal. Bomba DESLIGADA.");
  }

  delay(10000); 
}

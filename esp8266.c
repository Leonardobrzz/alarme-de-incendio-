#include <ArduinoWiFiServer.h>
#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiGratuitous.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>


// dados da rede wifi e do broker MQTT
const char* ssid = "Horley";
const char* password = "qwe12345";
const char* mqtt_server = "broker.emqx.io";

WiFiClient espClient;
PubSubClient client(espClient);

int fogo;

#define TOPICO_STATUS_FOGO "leozinho/statusFogo"

/***************************************************************************
 * conectaWiFi()
 */
void conectaWiFi() {
  // faz a conexão com a rede WiFi
  Serial.println();
  Serial.print("Conectando a rede: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);  // Wi-Fi no modo station
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi conectada");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

/***************************************************************************
 * conectaMQTT()
 */
void conectaMQTT() {
  client.setServer(mqtt_server, 1883);

  if (WiFi.status() == WL_CONNECTED) {
    // loop até conseguir conexão com o MQTT
    while (!client.connected()) {
      Serial.println("Tentativa de conexão MQTT...");
      // cria um client ID randômico
      String clientId = "ESP32Client-";
      clientId += String(random(0xffff), HEX);

      // tentantiva de conexão
      if (client.connect(clientId.c_str())) {
        Serial.println("MQTT conectado!");
        // se conectado, anuncia isso com um tópico especifico desse dispositivo
        String topicoStatusConexao = "leozinho/conexao/";
        topicoStatusConexao.concat(WiFi.macAddress());
        client.publish(topicoStatusConexao.c_str(), "1");
        // ... e se increve nos tópicos de interesse, no caso aqui apenas um
        client.subscribe(TOPICO_STATUS_FOGO);
      } else {
        Serial.println("Conexão falhou");
        Serial.println("tentando novamente em 5 segundos");
        delay(5000);
      }
    }
  }
}


/***************************************************************************
 * setup()
 */
void setup() {
  conectaWiFi();
  conectaMQTT();
  Serial.begin(38400);  // Initialize the Serial interface with baud rate
}

void loop() {
  // fica atento aos dados MQTT
  client.loop();

  // se conexão MQTT caiu, reconecta
  if (!client.connected()) {
    conectaMQTT();
  }

  // Verificando se há dados no buffer
  if (Serial.available() > 0) {  //Checks is there any data in buffer

    // Fogo
    Serial.print("Status Fogo:");
    fogo = int(Serial.read());
    Serial.println(fogo);  //Read serial data byte and send back to serial monitor
    client.publish(TOPICO_STATUS_FOGO, String(fogo).c_str());
    delay(500);
  }
}

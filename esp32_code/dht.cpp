#include "DHT.h"
#include "WiFi.h"
#include "PubSubClient.h"
#include "esp_system.h"
#include "NTPClient.h"


#define DHTPIN 23
#define DHTTYPE DHT11

//Variaveis  esp_random
uint64_t chipid = 0;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);  

DHT dht(DHTPIN, DHTTYPE);

WiFiUDP udp;
NTPClient ntp(udp, "a.st1.ntp.br",0, 60000);

void setup() 
{
  Serial.begin(115200);
  Serial.println("DHTxx test!");
  
  //inciializa o sensor dht
  dht.begin();
  
  // Conectar WiFi
  const char *SSID = "....."; 
  const char *PWD = "......";

  WiFi.begin(SSID,PWD);

  //servidoor ntp
  ntp.begin();
  ntp.forceUpdate(); 
  
  wifi_connect();

  //MQTT
  char *mqttServer = "192.168.1.108";
  int mqttPort = 1883;

  mqtt_connect();

  //Mac Address ESP
  chipid= ESP.getEfuseMac();
  ets_printf("%llu\n",chipid);

}

  
//Reconexao
void reconnect ()
{
  Serial.printf("Connecting to Mqtt Broker\n");
  while (!mqttClient.connected())
  {
    Serial.printf("Reconnecting to Mqtt Broker ..\n");

    if (mqttClient.connect("ESP32_DHT11"))
    {
      Serial.printf("Conectando");
      mqttClient.subscribe("/commands");
    }
  }

}

void wifi_connect()
{
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.printf(".");
    delay(500);
  }
  Serial.print("Connected.");
  Serial.println(WiFi.localIP());
}

void mqtt_connect(server, port)
{
  mqttClient.setServer(server, port); 
  mqttClient.setCallback(callback); 
}

//CallBack
void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Callback - Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
}

void loop() 
{ 
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(h) || isnan(t)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.printf("Umidade: %.2f  \n", humidity, "%");
  Serial.printf("Temperatura: %.2f °C \n", temperature);

  // pega a hora via ntp
  hora = ntp.getEpochTime(); 

  // Verificando conexao
  if (!mqttClient.connected())
  {
    reconnect();
  }
  mqttClient.loop();
  
  //variavel usada para receber o retorno da funcao millis
  long now = millis();

  //variavel estatica usada para receber o valor de now 
  static long last_time = 0;

  //envio de 5 em 5 segundos os valores do sensor para o broker mqtt
  if(now - last_time >5000)
  {
    //DHT11 Read
    char data [128]={0};
    snprintf(data,128,"{\"Id\":%llu,\"Temperature\":%d,\"Hora\":%ld,\"Humidity\":%d}",chipid, temperature, hora, humidity);
    mqttClient.publish("dht/temp",data);
    
    last_time=now;
  }
  delay(1000);
}  
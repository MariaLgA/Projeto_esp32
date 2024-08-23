#include "DHT.h"
#include "WiFi.h"
#include "PubSubClient.h"
#include "esp_system.h"
#include "NTPClient.h"


#define DHTPIN 23
#define DHTTYPE DHT11

//Variaveis
long last_time = 0;
uint64_t chipid = 0;
const char *SSID = "....."; 
const char *PWD = "......";


WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);  

DHT dht(DHTPIN, DHTTYPE);

WiFiUDP udp;
NTPClient ntp(udp, "a.st1.ntp.br",0, 60000);
long int hora;

void setup() 
{
  Serial.begin(115200);
  Serial.println("DHTxx test!");
  dht.begin();
    

// Conectar WiFi
  WiFi.begin(SSID,PWD);
  
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.printf(".");
    delay(500);
  }
  Serial.print("Connected.");
  Serial.println(WiFi.localIP());

  ntp.begin();  
  ntp.forceUpdate(); 
  
  //Publish

  char *mqttServer = "192.168.1.108";
  int mqttPort = 1883;
  
  mqttClient.setServer(mqttServer, mqttPort); 
  mqttClient.setCallback(callback); 

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

    if (mqttClient.connect("Duda2"))
    {
      Serial.printf("Conectando");
      mqttClient.subscribe("/commands");
    }
  }

}

//CallBack

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Callback - ");
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
}


void loop() 
{
  delay(1000);

    hora = ntp.getEpochTime(); 
    //Serial.println(hora);     /* Escreve a hora no monitor serial. */
    delay(1000);

   /*/ float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) 
    {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
      Serial.printf("Umidade: %.2f  \n",h,"%");
      Serial.printf("Temperatura: %.2f °C \n",t);
/*/

  // Verificando conexao
  if (!mqttClient.connected())
  {
    reconnect();
  }
  mqttClient.loop();
  long now = millis();
  

  if(now - last_time >5000)
  {
    //DHT11 Read
    char data [128]={0};
    snprintf(data,128,"{\"Id\":%llu,\"Temperature\":%d,\"Hora\":%ld,\"Humidity\":%d}",chipid,esp_random()%30,hora,esp_random()%100);
    mqttClient.publish("dht/temp",data);

    //Removing this comment for the working of random values gf
        
    //char data [128]={0};
    //snprintf(data,128,"{\"Id\":%llu,\"Temperature\":%.2f,\"Hora\":%ld,\"Humidity\":%.2f}",chipid,t,hora,h);
    //mqttClient.publish("dht/temp",data);

    
    last_time=now;
    
  }
}  

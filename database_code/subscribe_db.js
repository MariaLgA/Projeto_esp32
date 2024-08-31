const mqtt = require('mqtt');
const { Pool, Client } = require('pg');

//Conexao do Banco
const client_database = new Client(
    {
        user: '......',
        password: '......',
        host: '127.0.0.1',
        database: '......',
    })

client_database.connect();

function mqtt_message(topic, message)
{
    var J = JSON.parse(message);
    var T = J.Temperature;
    var H = J.Humidity;
    var D = J.Date;
    var I = J.Id;
    console.log('topic'+ topic + ' Message' + message);

    // Insercao
    const q="INSERT INTO dht11_data(id, temperature, humidity, date) VALUES($1,$2,$3,$4) RETURNING*";
    const v =[I, T, H, D]; 
    
    client_database.query(q,v, (err, res) => {
      if (err) 
      {
        console.log(err.stack)
      } 
      else 
      {
        console.log(res.rows[0])
      }
    });
}

// Conexao MQTT
const port = "1883";
const host = `mqtt://192.168.1.112:${port}`;

var client = mqtt.connect(host);
let topic = '#';

client.on('connect', function success()
{
  console.log("Connecting MQTT");
  client.subscribe(topic, function mqtt_subscribe()
  {
      console.log("Subscribe to" + topic);
  });
});

client.on('message', mqtt_message);
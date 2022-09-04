
//Conexao do Banco

const { Pool, Client } = require('pg');
const { json } = require('stream/consumers');

const c = new Client({
    user: '....',
    password: '....',
    host: '127.0.0.1',
    database: '....',
  })
  c.connect();

// Conexao MQTT

const mqtt = require('mqtt');
const host = "mqtt://192.168.1.108";
const port = "1883";

var client = mqtt.connect(host,port);
var topic = '#';

client.on('connect', mqtt_connect);
client.on('message',mqtt_message);

function mqtt_connect()
{
    console.log("Connecting MQTT");
    client.subscribe(topic,mqtt_subscribe);
}

function mqtt_subscribe(granted)
{
    console.log("Subscribe to" + topic); 
}

function mqtt_message(topic,message,packet)
{
    var J = JSON.parse(message);
    var T = J.Temperature;
    var H = J.Humidity;
    var D = J.Hora;
    var I = J.Id;
    console.log('topic'+ topic + ' Message' + message);

    // Insercao
    const q="INSERT INTO medicoes(id,data_hora,temperature, humidity) VALUES($1,$2,$3,$4) RETURNING*";
    const v =[I,D,T,H]; 
    
    c.query(q,v, (err, res) => {
        if (err) {
          console.log(err.stack)
        } else {
          console.log(res.rows[0])
        }
      })
}      
    












/*/Mostrar topicos e resultados "separadamentes":
for (x in J)
{
    console.log(x,J[x]);
}

//Mostrar somente um dos topicos e seu resultado
console.log(J.Temperature);
var J = JSON.parse(message);
/*/
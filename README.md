# O que faz ?
Leitura do sensor de temperatura e humidade - DHT11 através do microcontrolador ESP32, utilizando o broker MQTT como intermediador destes dados e a comunicação dos mesmos com o servidor. Utilizando de um script em Nodejs para visualização desses dados e armazenamento deles em um banco de dados(Postgresql) e demonstra-los em dashboards na aplicação chamada Grafana.

# Imagens:
## Imagem da Dashboard DHT11
![image](/Imagens/grafana.png)

## Imagem da Dashboard Random
![image](/Imagens/grafana_r.png)

# Notas:
O código atual somente efetua a leitura dos dados vindo do sensor dht11, porém é plausível também usar dados random possibilitando que você faça o projeto mesmo sem o sensor.

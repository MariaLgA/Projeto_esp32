# O que faz ?
Leitura do sensor de temperatura e humidade - DHT11 através do microcontrolador ESP32, utilizando o broker MQTT como intermediador destes dados e a comunicação dos mesmos com o servidor. Utilizando de um script em Nodejs para visualização desses dados e armazenamento deles em um banco de dados(Postgresql) e demonstra-los em dashboards na aplicação chamada Grafana.

#Imagens da Dashboard



#Notas:
O código atual somente efetua a leitura dos dados vindo do sensor dht11, porém retirando o comentário feito logo depois do envio dos dados do sensor, é possível habilitar o envio de dados como random, retirando então inicialmente a necessidade de um sensor ou componente similar.

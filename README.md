# Sistemas-Distribuidos-TP-Museo
================================

Compilacion:
------------

1. Desde una terminal, posicionarse en el directorio "Sistemas-Distribuidos-TP-Museo".
2. Ejecutar el programa ./autogen.sh
3. Ejecutar el programa ./configure
4. Ejecutar el comando make

Ejecución:
----------

###Server-id
1. Dirigirse con una terminal al directorio "Sistemas-Distribuidos-TP-Museo/src/id-server"
2. Iniciar el servicio rcpbind, ejecutando:
> sudo service rpcbind start
3. Iniciar el servidor de identificadores ejecutando:
> sudo ./id-server 100 200 300

###Broker
1. Dirigirse con una terminal al directorio "Sitemas-Distribuidos-TP-Museo/src/broker"
2. Iniciar el broker ejecutando
> ./broker


###Administrador
1. Dirigirse con una terminal al directorio "Sistemas-Distribuidos-TP-Museo/src/administrador".
2. Para abrir el museo ejecutar el programa ./administrador a {capacidad_museo}.
3. Para cerrar el museo ejecutar el programa ./administrador c.

###Puerta
1. Dirigirse con una terminal al directorio "Sistemas-Distribuidos-TP-Museo/src/puerta".
2. ejecutar el comando ./puerta -i {numero_puerta}, donde "numero_puerta" corresponde al
   numero de puerta local en la máquina; esto es, 1, 2, 3, etc.

###Persona

1. Dirigirse con una terminal al directorio "Sistemas-Distribuidos-TP-Museo/src/persona".
2. Para lanzar personas de a una, ejecutar el programa ./persona {n|i}, donde "n" ingresa una
   persona normal e "i" ingresa un investigador.
3. Para lanzar varias personas normales e investigadores en simultaneo, ejecutar el script
   ./lanzador.sh {cantidad}, donde "cantidad" corresponde al numero de personas e investigadores
   a lanzar concurrrentemente.


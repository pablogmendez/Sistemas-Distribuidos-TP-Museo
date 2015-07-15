rm *.o
rm broker
rm broker_entrada
rm broker_salida
rm prueba_envio
rm prueba_recep
rm server_entrada
rm server_salida

g++ -c ../sockets/*.cpp
ar rvs sockets.a *.o
rm *.o
g++ broker_entrada.cpp sockets.a -o broker_entrada
g++ broker_salida.cpp sockets.a -o broker_salida
g++ server_entrada.cpp sockets.a -o server_entrada
g++ server_salida.cpp sockets.a -o server_salida
g++ prueba_envio.cpp sockets.a -o prueba_envio
g++ prueba_recep.cpp sockets.a -o prueba_recep 

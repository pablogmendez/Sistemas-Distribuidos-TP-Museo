rm *.o
rm broker
rm broker_entrada
rm broker_salida
rm prueba_envio
rm prueba_recep
rm server_entrada
rm server_salida
rm shmem_get
rm shmem_return
rm prueba_get_shmem
rm broadcast

if [ "$1" != "clean" ]; then 
	g++ -c ../sockets/*.cpp
	ar rvs sockets.a *.o
	rm *.o
	g++ broker.cpp ../IPC/Semaforo.cpp ../utils/System.cpp -o broker
	g++ broker_entrada.cpp sockets.a -o broker_entrada
	g++ broker_salida.cpp sockets.a -o broker_salida
	g++ server_entrada.cpp sockets.a -o server_entrada
	g++ server_salida.cpp sockets.a -o server_salida
	g++ prueba_envio.cpp sockets.a -o prueba_envio
	g++ prueba_recep.cpp sockets.a -o prueba_recep 
	g++ shmem_get.cpp ../IPC/Semaforo.cpp ../utils/System.cpp -o shmem_get
	g++ shmem_return.cpp ../IPC/Semaforo.cpp ../utils/System.cpp -o shmem_return
	g++ prueba_get_shmem.cpp sockets.a -o prueba_get_shmem
	g++ broadcast.cpp sockets.a -o broadcast
fi

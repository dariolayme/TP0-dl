#include"utils.h"

//SERVIDOR

int iniciar_servidor(void)
{
	int socket_servidor;

    struct addrinfo hints, *servinfo, *p;



    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(IP, PUERTO, &hints, &servinfo);

    // Creamos el socket de escucha del servidor
    socket_servidor = socket(servinfo->ai_family,servinfo->ai_socktype,servinfo->ai_flags);

    if(socket_servidor != 0)
    	printf("[SERVER]-No se pudo conectar al socket");
    else
    	printf("[SERVER]-Se pudo conectar al socket");

    // Asociamos el socket a un puerto
    bind(socket_servidor,servinfo->ai_addr,servinfo->ai_addrlen);//listo el socket necesitamos asignarle el puerto a utilizar. Devuelve 0 si salio bien sino -1
    //(1)descriptor de archivo del socket
    //(2)puntero a sockddr, que contengan la informacion del puerto
    //(3)tamaño de la informacion del puerto del parametro (2)


    // Escuchamos las conexiones entrantes

    freeaddrinfo(servinfo);

    listen(socket_servidor,SOMAXCONN);//Para activar el socket
       //(1) recive el socket
       //(2) el numero maximo de conexiones pendientes que pueden estar en la colade conexiones antes  de q el socket comienze a rechazar nuevas conexiones. Devuelve 0 si salio bien sino -1


    log_trace(logger, "Listo para escuchar a mi cliente");

    while(1){
    	esperar_cliente(socket_servidor);//los datos se deben reiniciar
    }

    return socket_servidor;
}

int esperar_cliente(int socket_servidor)
{
	struct sockaddr_in dir_cliente;
	int tam_direccion = sizeof(struct sockaddr_in);
	int socket_cliente = 0;

	// Aceptamos un nuevo cliente
	socket_cliente = accept(socket_servidor,dir_cliente->sin_addr,dir_cliente->sin_);//devuelve numero no negativo si salio OK
	//(1) socket del servidor
	//(2) puntero a la struct de la direccion del cliente, donde la funcion alamacenará la info de la direccion del socket del cliente
	//(3) puntero al tamaño del struct (2)
	if(socket_cliente < 0)
	{
		log_info(logger, "Se conecto un cliente!");

	}
	t_paquete paquete;


	paquete->codigo_operacion = recibir_operacion(socket_cliente);
	paquete->buffer->stream = recibir_buffer(&(paquete->buffer->size),socket_cliente);




	return socket_cliente;
}

void procesar_pedido(int cod_op, int socket_cliente )
{
	int size;
	void* msg;

	switch(cod_op){
	case MENSAJE:

	case PAQUETE:
	}
}

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) != 0)
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

void* recibir_buffer(int* size, int socket_cliente)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(int socket_cliente)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	log_info(logger, "Me llego el mensaje %s", buffer);
	free(buffer);
}

//podemos usar la lista de valores para poder hablar del for y de como recorrer la lista
t_list* recibir_paquete(int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	while(desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
	//return NULL;
}

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
    for (p=servinfo; p != NULL; p = p->ai_next)
        {
            if ((socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            {
            	log_info(logger, "[SERVER]-No se pudo conectar al socket");
            	continue;
            }

            // Asociamos el socket a un puerto
                //(1)descriptor de archivo del socket
                //(2)puntero a sockddr, que contengan la informacion del puerto
                //(3)tamaño de la informacion del puerto del parametro (2)
            if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1) //listo el socket necesitamos asignarle el puerto a utilizar. Devuelve 0 si salio bien sino -1
            {
            	log_info(logger,"[SERVER]-No se pudo conectar al socket");
            	log_info(logger,"[SERVER]-Se cierra el socket");
                close(socket_servidor);
                continue;
            }
            break;
        }

    // Escuchamos las conexiones entrantes

    listen(socket_servidor,SOMAXCONN);//Para activar el socket
       //(1) recive el socket
       //(2) el numero maximo de conexiones pendientes que pueden estar en la colade conexiones antes  de q el socket comienze a rechazar nuevas conexiones. Devuelve 0 si salio bien sino -1

    freeaddrinfo(servinfo);

    log_trace(logger, "[SERVER]-Listo para escuchar a mi cliente");


    return socket_servidor;
}

int esperar_cliente(int socket_servidor)
{
	struct sockaddr_in dir_cliente;
	int tam_direccion = sizeof(struct sockaddr_in);
	int socket_cliente = 0;

	// Aceptamos un nuevo cliente
	//socket_cliente = accept(socket_servidor,&dir_cliente,&tam_direccion);//devuelve numero no negativo si salio OK

	socket_cliente = accept(socket_servidor,NULL,NULL);//devuelve numero no negativo si salio OK
	//(1) socket del servidor
	//(2) puntero a la struct de la direccion del cliente, donde la funcion alamacenará la info de la direccion del socket del cliente
	//(3) puntero al tamaño del struct (2)
	if(socket_cliente < 0)
	{
		log_info(logger, "[SERVER]-Failed to accept client socket.");
		close(socket_cliente);
        exit(EXIT_FAILURE);
	}


	return socket_cliente;
}
/*
void procesar_pedido(int cod_op, int socket_cliente )
{
	int size;
	void* msg;

	switch(cod_op){
	case MENSAJE:

	case PAQUETE:
	}
}
*/
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
	log_info(logger, "[SERVER]-Me llego el mensaje %s", buffer);
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
	return NULL;
}

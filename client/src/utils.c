#include "utils.h"

//CLIENTE

void* serializar_paquete(t_paquete* paquete, int bytes)
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

int crear_conexion(char *ip, char* puerto)
{

	struct addrinfo hints;//requisitos/configuracion  para filtrar las estructuras de socket recuperadas
	struct addrinfo *server_info; // es el puntero, donde la función asignará dinámicamente una lista enlazada de estructuras addrinfo.

	memset(&hints, 0, sizeof(hints));//?  //llena los primeros n bytes (c) del área de memoria apuntado por (1) con el byte constante (2).


	hints.ai_family = AF_UNSPEC;//especifica el tipo de direcciones Ipv4 o 6 o no especificada.
	hints.ai_socktype = SOCK_STREAM;//especifica el tipo de socket-> stream o datagram
	hints.ai_flags = AI_PASSIVE;//especifica el tipo de protocolo TCP o UDP

	getaddrinfo(ip, puerto, &hints, &server_info);//traduce la informacion de la direccion y el puerto para que la funcion Conect() la pueda entender. --> server_info
	//(1)recibe la IP de destino
	//(2)tipo de servicio/puerto
	//(3)puntero a la estructura de criterios para la traduccion de la informacion
	//(4)lista enlazada con la informacion obtenida/traducida.

	// Ahora vamos a crear el socket.
	int socket_cliente = socket(server_info->ai_family,server_info->ai_socktype,server_info->ai_protocol);

	// Ahora que tenemos el socket, vamos a conectarlo
	//(1)el socket a conectarse, (2) el puntero	que contiene la estructura de la direccion (3) es la estructura del puntero pasado en el campo(2)
	if(connect(socket_cliente,server_info->ai_addr,server_info->ai_addrlen) != 0)
	{
		printf("Error");//no hace falta ponerlo en un log?
	}

	freeaddrinfo(server_info);

	return socket_cliente;
}

void enviar_mensaje(char* mensaje, int socket_cliente)//Segun Ennciado deberia recir el tamañano (3)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(mensaje) + 1;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);//devuelve el numero de bytes enviados
	//(1) el socket donde se envia los datos
	//(2) buffer, q son los datos a enviar
	//(3) la cantidad de bytes a enviar, ya q el (2) es un puntero al comienzo
	//(4)bandera??

	free(a_enviar);
	eliminar_paquete(paquete);
}


void crear_buffer(t_paquete* paquete)
{
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}

t_paquete* crear_super_paquete(void)//modif??
{
	t_paquete* paquete=malloc(sizeof(t_paquete));

	paquete->codigo_operacion = PAQUETE;
	crear_buffer(paquete);
	return paquete;
}

t_paquete* crear_paquete(void)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = PAQUETE;
	crear_buffer(paquete);
	return paquete;
}

void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio)
{
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

	paquete->buffer->size += tamanio + sizeof(int);
}

void enviar_paquete(t_paquete* paquete, int socket_cliente)
{
	int bytes = paquete->buffer->size + 2*sizeof(int);
	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
}

void eliminar_paquete(t_paquete* paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}

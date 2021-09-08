#include "client.h"

#include<readline/readline.h>

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger,"Hola!! Soy un log");

	config = iniciar_config();

	//puts(ip);

	// Usando el config creado previamente
	// Lee las variables de IP, Puerto y Valor
	ip = config_get_string_value(config,"CLAVE");
	log_info(logger,"Lei esto %s", ip);

	//Loggear valor de config

	leer_consola(logger);


	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo porque lo necesitaremos para lo que sigue.

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	//enviar CLAVE al servirdor

	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	nuevo_logger = log_create("cliente.log","TP0",1,LOG_LEVEL_INFO);

	if(nuevo_logger == NULL)
	{
		printf("No se pudo crear el LOGGER\n");
		exit(1);
	}

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	nuevo_config = config_create("cliente.config");

	if(nuevo_config == NULL)
	{
		printf("No se pudo crear el T_config\n");
		exit(1);
	}


	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;
	char* comando = ">";
	char* centinela = "\0";

	//El primero te lo dejo de yapa
	leido = readline(comando);

	while(strcmp(leido,centinela) != 0){
		log_info(logger,leido);
		leido = readline(comando);
	}
	log_info(logger,"Terminado");

	free(1);

	// Acá la idea es que imprimas por el log lo que recibis de la consola.



}

void paquete(int conexion)
{
	//Ahora toca lo divertido!

	char* leido;
	t_paquete* paquete;


}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las
	//commons y del TP mencionadas en el enunciado
	if(logger != NULL)
	{
		log_destroy(logger);
	}
	if(config != NULL)
	{
		config_destroy(config);
	}
}

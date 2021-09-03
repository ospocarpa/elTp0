#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
	int conexion;
	char *ip;
	char *puerto;
	char *valor;

	t_log *logger;
	t_config *config;

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"  --->listo

	log_info(logger, "Hola! Soy un log");

	config = iniciar_config();

	// Usando el config creado previamente
	// Lee las variables de IP, Puerto y Valor    --->LISTO
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");
	//Loggear valor de config
	log_info(logger, valor);
	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo porque lo necesitaremos para lo que sigue.

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	//enviar CLAVE al servirdor
	enviar_mensaje(valor, conexion);
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log *iniciar_logger(void)
{
	t_log *nuevo_logger = log_create("cfg/tp0.log", "TP0", 1, LOG_LEVEL_INFO);

	if (nuevo_logger == NULL)
	{
		printf("No se pude crear el logger\n");
		exit(1);
	}

	return nuevo_logger;
}

t_config *iniciar_config(void)
{
	t_config *nuevo_config = config_create("cfg/tp0.config");
	if (nuevo_config == NULL)
	{
		printf("No puede leer del archivo config\n");
		exit(2);
	}

	return nuevo_config;
}

void leer_consola(t_log *logger)
{
	char *leido;

	//El primero te lo dejo de yapa
	leido = readline(">");
	// Acá la idea es que imprimas por el log lo que recibis de la consola.

	while (strncmp(leido, "", 1) != 0)
	{
		log_info(logger, leido);
		free(leido);
		leido = readline(">");
	}
	free(leido);
}

void paquete(int conexion)
{
	//Ahora toca lo divertido!

	char *leido;
	leido = readline(">");
	t_paquete *paquete;
	paquete = crear_paquete();
	while (strncmp(leido, "", 1) != 0)
	{
		agregar_a_paquete(paquete, leido, strlen(leido) + 1);
		free(leido);
		leido = readline(">");
	}
	free(leido);
	enviar_paquete(paquete, conexion);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log *logger, t_config *config)
{
	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
	if (logger != NULL)
	{
		log_destroy(logger);
	}

	if (config != NULL)
	{
		config_destroy(config);
	}
	liberar_conexion(conexion);
}

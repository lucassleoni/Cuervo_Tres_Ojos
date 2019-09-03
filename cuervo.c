#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "cuervo_tres_ojos.h"

// Constantes
	#define FUTURO 'F'
	#define MODO_LECTURA "r"
	#define CASA_STARK "Stark"
	#define F_VISIONES "visiones.txt"
	#define NOMBRE_CUERVO "Brynden Rivers"
	#define NOMBRE_APRENDIZ "Brandon Stark"
	#define FORMATO "%[^;];%[^;];%[^;];%c"

	#define ERROR -1
	#define ES_VALIDO 0
	#define CANT_ARGUMENTOS 4
	#define CANT_CARACTERES_MIN 30

bool es_vision_de_casa_stark(vision_t vision){
	return (strcmp(vision.casa_protagonista, CASA_STARK) == 0);
}

bool es_vision_extensa(vision_t vision){
	return (strlen(vision.descripcion) > CANT_CARACTERES_MIN);
}

bool es_vision_futura(vision_t vision){
	return (vision.epoca == FUTURO);
}

bool es_vision_con_exclamacion(vision_t vision){
	bool exclamacion = false;
	int i = 0;
	while((i < strlen(vision.descripcion)) && (!exclamacion)){
		if((vision.descripcion[i] == '!')){
			exclamacion = true;
		}
		i++;
	}

	return exclamacion;
}


int main(){
	FILE* visiones = fopen(F_VISIONES, MODO_LECTURA);
	if(!visiones){
		printf("No se pudo abrir el archivo de visiones\n");
		return ERROR;
	}

	cuervo_tres_ojos_t* cuervo = invocar_cuervo(NOMBRE_CUERVO);
	if(!cuervo){
		fclose(visiones);
		printf("No se pudo invocar a %s\n", NOMBRE_CUERVO);
		return ERROR;
	}

	cuervo_aprendiz_t* cuervo_aprendiz = invocar_aprendiz(NOMBRE_APRENDIZ);
	if(!cuervo_aprendiz){
		free(cuervo);
		fclose(visiones);
		printf("No se pudo invocar a %s\n", NOMBRE_APRENDIZ);
		return ERROR;
	}

	vision_t vision;
	int leido = fscanf(visiones, FORMATO, vision.protagonista, vision.casa_protagonista, vision.descripcion, &(vision.epoca));
	int estado_vision = agregar_vision(cuervo, vision);
	while((leido == CANT_ARGUMENTOS) && (estado_vision == ES_VALIDO)){
		estado_vision = agregar_vision(cuervo, vision);
		leido = fscanf(visiones, FORMATO, vision.protagonista, vision.casa_protagonista, vision.descripcion, &(vision.epoca));
	}

	fclose(visiones);
	
	transmitir_visiones(cuervo, cuervo_aprendiz, es_vision_de_casa_stark);
	transmitir_visiones(cuervo, cuervo_aprendiz, es_vision_extensa);
	transmitir_visiones(cuervo, cuervo_aprendiz, es_vision_futura);
	transmitir_visiones(cuervo, cuervo_aprendiz, es_vision_con_exclamacion);

	listar_visiones_transmitidas((*cuervo_aprendiz), es_vision_de_casa_stark);
	listar_visiones_transmitidas((*cuervo_aprendiz), es_vision_extensa);
	listar_visiones_transmitidas((*cuervo_aprendiz), es_vision_futura);
	listar_visiones_transmitidas((*cuervo_aprendiz), es_vision_con_exclamacion);
	
	destruir_cuervo(cuervo);
	destruir_aprendiz(cuervo_aprendiz);

	return 0;
}
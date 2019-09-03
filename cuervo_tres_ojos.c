#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "cuervo_tres_ojos.h"

// Constantes
	#define PASADO 'P'
	#define FUTURO 'F'
	#define ERROR -1
	#define ES_VALIDO 0
	#define SIN_VISIONES 0
	#define SE_COPIO 0


/*
 * Recibe el nombre del cuervo a invocar.
 * Creará la estructura cuervo_tres_ojos, reservando la memoria necesaria.
 * Devolverá la estructura con los topes de las visiones inicializados en 0.
 * En caso de no poder invocar al cuervo, devolverá NULL.
 */
cuervo_tres_ojos_t* invocar_cuervo(char* nombre){
	cuervo_tres_ojos_t* cuervo = malloc(sizeof(cuervo_tres_ojos_t));
	
	if(!cuervo){
		printf("No se pudo invocar al Cuervo solicitado\n");
		return NULL;
	}

	strcpy(cuervo->nombre, nombre);
	cuervo->visiones_del_pasado = malloc(sizeof(vision_t));
	if(!(cuervo->visiones_del_pasado)){
		printf("No se pudieron invocar las visiones pasadas\n");
		return NULL;
	}
	cuervo->visiones_del_futuro = malloc(sizeof(vision_t));
	if(!(cuervo->visiones_del_futuro)){
		printf("No se pudieron invocar las visiones futuras\n");
		return NULL;
	}
	cuervo->cantidad_visiones_pasado = SIN_VISIONES;
	cuervo->cantidad_visiones_futuro = SIN_VISIONES;

	return cuervo;
}

/*
 * Recibe el nombre del aprendiz a invocar.
 * Creará la estructura cuervo_aprendiz, reservando la memoria necesaria.
 * Devolverá la estructura con el tope de las visiones inicializado en 0.
 * En caso de no poder invocar al aprendiz, devolverá NULL.
 */
cuervo_aprendiz_t* invocar_aprendiz(char* nombre){
	cuervo_aprendiz_t* cuervo_aprendiz = malloc(sizeof(cuervo_aprendiz_t));
	
	if(!cuervo_aprendiz){
		printf("No se pudo invocar al Cuervo aprendiz solicitado\n");
		return NULL;
	}

	strcpy(cuervo_aprendiz->nombre, nombre);
	cuervo_aprendiz->visiones_adquiridas = malloc(sizeof(vision_t));
	if(!(cuervo_aprendiz->visiones_adquiridas)){
		printf("No se pudieron invocar las visiones del aprendiz\n");
		return NULL;
	}
	cuervo_aprendiz->cantidad_visiones = SIN_VISIONES;

	return cuervo_aprendiz;
}

int copiar_vision(vision_t** vision_cuervo, vision_t vision_leida, int* cantidad_visiones){
	vision_t* vision_aux = realloc((*vision_cuervo), (long unsigned int)((*cantidad_visiones)+1) * sizeof(vision_t));
	if(!(vision_aux)){
		printf("No se pudo agregar la vision '%s'\n", vision_leida.descripcion);
		return ERROR;
	}
	(*vision_cuervo) = vision_aux;
	(*vision_cuervo)[(*cantidad_visiones)] = vision_leida;

	(*cantidad_visiones)++;

	return SE_COPIO;
}

/*
 * Agrega una vision al vector de visiones correspondiente,
 * reservando la memoria necesaria para ella.
 * Devolverá 0 si pudo agregarla o -1 si no pudo.
 */
int agregar_vision(cuervo_tres_ojos_t* cuervo, vision_t vision){
	int estado_vision = ES_VALIDO;

	if(vision.epoca == PASADO){
		estado_vision = copiar_vision(&(cuervo->visiones_del_pasado), vision, &(cuervo->cantidad_visiones_pasado));
	}

	else if(vision.epoca == FUTURO){
		estado_vision = copiar_vision(&(cuervo->visiones_del_futuro), vision, &(cuervo->cantidad_visiones_futuro));
	}

	return estado_vision;
}

bool tiene_vision(cuervo_aprendiz_t* aprendiz, vision_t vision_buscada){
	int es_vision_repetida = false;
	int i = 0;

	while((i < (aprendiz->cantidad_visiones)) && (!es_vision_repetida)){
		if(strcmp(aprendiz->visiones_adquiridas[i].protagonista, vision_buscada.protagonista) == 0){
			if(strcmp(aprendiz->visiones_adquiridas[i].casa_protagonista, vision_buscada.casa_protagonista) == 0){
				if(strcmp(aprendiz->visiones_adquiridas[i].descripcion, vision_buscada.descripcion) == 0){
					if(aprendiz->visiones_adquiridas[i].epoca == vision_buscada.epoca){
						es_vision_repetida = true;
					}
				}
			}
		}
		i++;
	}

	return es_vision_repetida;
}

/*
 * Transmitirá al aprendiz las visiones del pasado y del futuro del cuervo que sean 
 * transmisibles de acuerdo a la función recibida como parámetro.
 * En caso de que la visión a transmitir ya sea conocida por el aprendiz (repetida), 
 * no se agrega a las visiones del mismo.
 * Devolverá 0 si se transmitieron con éxito, -1 en caso contrario.
 */
int transmitir_visiones(cuervo_tres_ojos_t* cuervo, cuervo_aprendiz_t* aprendiz, bool (*es_transmisible)(vision_t)){
	int estado_vision = ES_VALIDO;
	
	for(int i = 0; i < (cuervo->cantidad_visiones_pasado); i++){
		if((*es_transmisible)(cuervo->visiones_del_pasado[i]) && (!tiene_vision(aprendiz, cuervo->visiones_del_pasado[i]))){
			estado_vision = copiar_vision(&(aprendiz->visiones_adquiridas), cuervo->visiones_del_pasado[i], &(aprendiz->cantidad_visiones));
		}
	}

	for(int j = 0; j < (cuervo->cantidad_visiones_futuro); j++){
		if((*es_transmisible)(cuervo->visiones_del_futuro[j]) && (!tiene_vision(aprendiz, cuervo->visiones_del_futuro[j]))){
			estado_vision = copiar_vision(&(aprendiz->visiones_adquiridas), cuervo->visiones_del_futuro[j], &(aprendiz->cantidad_visiones));
		}
	}

	return estado_vision;
}

/*
 * Mostrará por pantalla las visiones adquiridas por el aprendiz.
 * Una visión se listará sólo si la función recibida como parámetro lo permite.
 */
void listar_visiones_transmitidas(cuervo_aprendiz_t aprendiz, bool (*es_listable)(vision_t)){
	for(int i = 0; i < aprendiz.cantidad_visiones; i++){
		if((*es_listable)(aprendiz.visiones_adquiridas[i])){
    		printf("Veo que en el %s, %s de %s dice que \"%s\".\n",
    		aprendiz.visiones_adquiridas[i].epoca=='F'?"futuro":"pasado",
    		aprendiz.visiones_adquiridas[i].protagonista,
    		aprendiz.visiones_adquiridas[i].casa_protagonista,
    		aprendiz.visiones_adquiridas[i].descripcion);
    	}
	}
}

/*
 * Destruirá la estructura del cuervo, liberando la memoria reservada para él y sus visiones.
 * Devolverá 0 si pudo liberar la memoria o -1 si no pudo.
 */
int destruir_cuervo(cuervo_tres_ojos_t* cuervo){
	if(!cuervo){
		return ERROR;
	}
	free(cuervo->visiones_del_pasado);
	free(cuervo->visiones_del_futuro);
	free(cuervo);
	return ES_VALIDO;
}

/*
 * Destruirá la estructura del aprendiz, liberando la memoria reservada para él y sus visiones.
 * Devolverá 0 si pudo liberar la memoria o -1 si no pudo.
 */
int destruir_aprendiz(cuervo_aprendiz_t* aprendiz){
	if(!aprendiz){
		return ERROR;
	}
	free(aprendiz->visiones_adquiridas);
	free(aprendiz);
	return ES_VALIDO;
}
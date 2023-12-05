#ifndef SYNCHRO_H
#define SYNCHRO_H

#include "ensitheora.h"
#include <stdbool.h>

extern bool fini;

/* Les extern des variables pour la synchro ici */
extern pthread_mutex_t mutex_add_hash;
extern pthread_mutex_t mutex_find_hash;

extern pthread_mutex_t mutex_window;
extern pthread_cond_t condition_window;
extern bool cond_window;

extern pthread_mutex_t mutex_texture;
extern pthread_cond_t condition_texture;
extern bool cond_texture;

extern pthread_mutex_t mutex_prod_cons_text;
extern pthread_cond_t condition_full;
extern pthread_cond_t condition_empty;
extern int texture_count;

/* Fonctions de synchro à implanter */

void envoiTailleFenetre(th_ycbcr_buffer buffer);
void attendreTailleFenetre();

void attendreFenetreTexture();
void signalerFenetreEtTexturePrete();

void debutConsommerTexture();
void finConsommerTexture();

void debutDeposerTexture();
void finDeposerTexture();

#endif

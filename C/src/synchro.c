#include "ensitheora.h"
#include "synchro.h"

/* les variables pour la synchro, ici */
pthread_mutex_t mutex_window = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_window = PTHREAD_COND_INITIALIZER;
bool cond_window = false;

pthread_mutex_t mutex_texture = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_texture = PTHREAD_COND_INITIALIZER;
bool cond_texture = false;

pthread_mutex_t mutex_prod_cons_text = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t condition_empty = PTHREAD_COND_INITIALIZER;
int texture_count = 0;

/* l'implantation des fonctions de synchro ici */
void envoiTailleFenetre(th_ycbcr_buffer buffer) 
{
  pthread_mutex_lock( &mutex_window );

    windowsx = buffer[0].width;
    windowsy = buffer[0].height;
    cond_window = true;

    pthread_cond_signal( &condition_window );
  
  pthread_mutex_unlock( &mutex_window );  
}

void attendreTailleFenetre() 
{
  pthread_mutex_lock( &mutex_window );

  while(!cond_window)
    pthread_cond_wait( &condition_window, &mutex_window );

  pthread_mutex_unlock( &mutex_window );
}

void signalerFenetreEtTexturePrete() 
{
  pthread_mutex_lock( &mutex_texture );

    cond_texture = true;
    pthread_cond_signal( &condition_texture );
  
  pthread_mutex_unlock( &mutex_texture );  
}

void attendreFenetreTexture() 
{
  pthread_mutex_lock( &mutex_texture );

  while(!cond_texture)
    pthread_cond_wait( &condition_texture, &mutex_texture );

  pthread_mutex_unlock( &mutex_texture );
}

void debutConsommerTexture() 
{
  pthread_mutex_lock( &mutex_prod_cons_text );

  while(texture_count == 0)
    pthread_cond_wait( &condition_empty, &mutex_prod_cons_text );
  
  pthread_mutex_unlock( &mutex_prod_cons_text );  
}

void finConsommerTexture() 
{
  pthread_mutex_lock( &mutex_prod_cons_text );

    texture_count--;
    pthread_cond_signal( &condition_full );
  
  pthread_mutex_unlock( &mutex_prod_cons_text ); 
}

void debutDeposerTexture() 
{
  pthread_mutex_lock( &mutex_prod_cons_text );

  while(texture_count == NBTEX)
    pthread_cond_wait( &condition_full, &mutex_prod_cons_text );
  
  pthread_mutex_unlock( &mutex_prod_cons_text );  
}

void finDeposerTexture() 
{
  pthread_mutex_lock( &mutex_prod_cons_text );

    texture_count++;
    pthread_cond_signal( &condition_empty );
  
  pthread_mutex_unlock( &mutex_prod_cons_text ); 
}

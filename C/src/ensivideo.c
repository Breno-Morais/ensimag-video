#include <SDL2/SDL.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "oggstream.h"
#include "stream_common.h"

pthread_t theora_th, vorbis_th, theora2sdlthread;

int main(int argc, char *argv[]) {
  int res;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s FILE", argv[0]);
    exit(EXIT_FAILURE);
  }
  assert(argc == 2);

  // Initialisation de la SDL
  res = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);
  atexit(SDL_Quit);
  assert(res == 0);

  // Your code HERE  
  pthread_create( &vorbis_th, NULL, vorbisStreamReader, (void*) argv[1]);
  pthread_create( &theora_th, NULL, theoraStreamReader, (void*) argv[1]);

  // wait for vorbis thread
  pthread_join( vorbis_th, NULL);

  // 1 seconde of sound in advance, thus wait 1 seconde
  // before leaving
  sleep(1);

  // Cancel them before
  pthread_cancel(theora_th);
  pthread_cancel(theora2sdlthread);

  // Wait for theora and theora2sdl threads
  pthread_join( theora_th, NULL);
  pthread_join( theora2sdlthread, NULL);

  exit(EXIT_SUCCESS);
}

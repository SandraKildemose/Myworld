#include "audio.h"
#include <stdio.h>

sfSound *g_clickSound = NULL;
sfMusic *g_bgMusic   = NULL;

static sfSoundBuffer *clickBuffer = NULL;


void audio_init(void)
{
   
    clickBuffer = sfSoundBuffer_createFromFile("assets/click.wav");
    if (!clickBuffer) {
        printf("Failed to load click.wav\n");
    } else {
        g_clickSound = sfSound_create();
        sfSound_setBuffer(g_clickSound, clickBuffer);
    }

  
    g_bgMusic = sfMusic_createFromFile("assets/music.ogg");
    if (!g_bgMusic) {
        printf("Failed to load music.ogg\n");
    } else {
   
        sfMusic_setLoop(g_bgMusic, sfTrue);
        sfMusic_play(g_bgMusic);
    }
}



void audio_cleanup(void)
{

    if (g_clickSound) {
        sfSound_destroy(g_clickSound);
        g_clickSound = NULL;
    }
    if (clickBuffer) {
        sfSoundBuffer_destroy(clickBuffer);
        clickBuffer = NULL;
    }

    
    if (g_bgMusic) {
        sfMusic_destroy(g_bgMusic);
        g_bgMusic = NULL;
    }
}

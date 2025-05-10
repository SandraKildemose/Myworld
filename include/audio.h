#ifndef AUDIO_H
#define AUDIO_H

#include <SFML/Audio.h>

/*
** Bu modülde, click ses efekti ve arka plan müziği için
** gerekli değişkenler ve fonksiyonlar tanımlanır.
*/

/* Global değişkenler */
extern sfSound *g_clickSound;   // Tıklama sesi
extern sfMusic *g_bgMusic;      // Arka plan müziği

/* Başlatma / temizleme fonksiyonları */
void audio_init(void);
void audio_cleanup(void);

#endif /* AUDIO_H */

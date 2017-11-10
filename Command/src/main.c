/**
 * Command.c
 *
 * Command card software for Alarm project
 *
 */

/**
 * Main.c
 *
 * Contains only main function and interruptions. For configurations, check out
 * annex files.
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

//#define AVIS_POLITIQUE GAUCHE

#include <asf.h>
#include <init.h>
#include "audio/audio.h"

#ifdef AVIS_POLITIQUE
	#include "mainGauche.c"
#else
	#include "mainCentre.c"
#endif


int main (void) {
	
	#ifdef AVIS_POLITIQUE
		mainGauche();
	#else
		mainCentre();
	#endif
	
}
	




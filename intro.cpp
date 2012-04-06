#include "graphics.h"
#include "math.h"
#include <GL/gl.h>

#define PI 3.14159265

void Graphics::introDisplay(suseconds_t cur, suseconds_t diff) {
	bool toggled = false;
	float angle = 90;

    int bass = m_sound.getBass();

	// glRotatef(90,0,0,1); // Legendary
	glRotatef(90,0,0,1);
    gluLookAt(-2 + (float)bass / 100.0f,  -6, 0, // Eye
               0, 0, 0,  // Focus
               0.0,  1.0,  0.0); // Up


	if ( cur > 2359376  && cur < 4816286) {
		glRotatef(90,1,0,0);
		glRotatef(-90,0,0,1);
	} else if ( cur > 4816286 ) {
		glRotatef(-90,0,0,1);
	}

    for (int i=0; i<sceneOffset; i++)
       glCallList(m_callLists[i]);

	return;
}


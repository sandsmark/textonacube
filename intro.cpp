#include "graphics.h"
#include <GL/gl.h>

void Graphics::introDisplay(suseconds_t cur, suseconds_t diff) {
	glRotatef(90,0,1,0);
    gluLookAt(-2,  -6, 0, // Eye
               0, 0, 0,  // Focus
               0.0,  1.0,  0.0); // Up

    for (int i=0; i<sceneOffset; i++)
       glCallList(m_callLists[i]);

	return;
}


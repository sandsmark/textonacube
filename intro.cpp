#include "graphics.h"

void Graphics::outroDisplay(suseconds_t cur, suseconds_t diff) {
    gluLookAt(-5,  5, 5, // Eye
               0, 0, 0,  // Focus
               0.0,  1.0,  0.0); // Up

    for (int i=0; i<sceneOffset; i++)
       glCallList(m_callLists[i]);

	return;
}


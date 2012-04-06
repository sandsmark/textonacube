#include "graphics.h"

void Graphics::outroDisplay(suseconds_t cur, suseconds_t diff) {
    gluLookAt(-5,  5, 5, // Eye
               0, 0, 0,  // Focus
               0.0,  1.0,  0.0); // Up

//    50000000
//    if (m_angleSpeed > 1) m_angleSpeed = 1;
    m_angle += diff / 50000;
    printf("%d\n", m_angle);
    if (m_angle > 360) m_angle = -360;

    if (cur < 52000000) {
        if (m_angle > 90) m_angle = 90;
        glRotatef(m_angle/2, 0, 0, 1);
    } else if (cur < 54000000) {
        if (m_angle > 90) m_angle = 90;
        glRotatef(m_angle/2, 0, 1, 0);
    } else if (cur < 56000000) {
        if (m_angle > 90) m_angle = 90;
        glRotatef(m_angle/2, 1, 0, 0);
    }
    for (int i=outroOffset+4; i<m_callLists.size(); i++)
       glCallList(m_callLists[i]);


	return;
}



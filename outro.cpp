#include "graphics.h"

void Graphics::outroDisplay(suseconds_t cur, suseconds_t diff) {
    gluLookAt(-5,  5, 5, // Eye
               0, 0, 0,  // Focus
               0.0,  1.0,  0.0); // Up

    int bass = m_sound.getBass();
    m_angleSpeed += bass;
    m_angleSpeed /= 1.05;
    if (m_angleSpeed > 1) m_angleSpeed = 1;
    m_angle += diff*(m_angleSpeed+1) / 5000;
    if (m_angle > 360) m_angle = -360;
    glRotatef(m_angle/2, 1, 1, 1);

    if (bass > 85) {
        m_scale += bass;
    }

    m_scale /= 1.1;
    if (m_scale > 1) m_scale = 1;
    //glScalef(m_scale+1, m_scale+1, m_scale+1);


//    50000000
//    if (m_angleSpeed > 1) m_angleSpeed = 1;
/*    m_angle += diff / 50000;
    printf("%d\n", m_angle);
    if (m_angle > 360) m_angle = -360;

    if (cur < 52000000) {
        if (m_angle > 90) m_angle = 90;
        glRotatef(m_angle/2, 0, 0, 1);
    } else if (cur < 54000000) {
        if (m_angle > 270) m_angle = 270;
        glRotatef(m_angle/2, 0, 1, 0);
    } else if (cur < 56000000) {
        if (m_angle > 180) m_angle = 180;
        glRotatef(m_angle/2, 1, 0, 0);
    }*/
    for (int i=outroOffset+4; i<m_callLists.size(); i++)
       glCallList(m_callLists[i]);


	return;
}



#include "graphics.h"

void Graphics::sceneDisplay(suseconds_t cur, suseconds_t diff)
{
    gluLookAt(-5,  5, 5, // Eye
               0, 0, 0,  // Focus
               0.0,  1.0,  0.0); // Up
    int bass = m_sound.getBass();
    m_angleSpeed += bass;
    m_angleSpeed /= 1.05;
    if (m_angleSpeed > 1) m_angleSpeed = 1;
    m_angle += diff*(m_angleSpeed+1) / 5000;
    if (m_angle > 360) m_angle = -360;
    glRotatef(m_angle/2, 1, m_angleSpeed, m_scale);

    if (bass > 85) {
        m_scale += bass;
    }

    m_scale /= 1.1;
    if (m_scale > 1) m_scale = 1;
    glScalef(m_scale+1, m_scale+1, m_scale+1);


    //for (int i=sceneOffset; i<outroOffset; i++)
    //for (int i=outroOffset; i<m_callLists.size(); i++)
    for (int i=sceneOffset; i<outroOffset; i++)
       glCallList(m_callLists[i]);
}

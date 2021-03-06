// Graphics display
//
// Copyright (C) 2010  Martin Sandsmark - martin.sandsmark@kde.org
// Copyright (C) 2010  Amund Hov - amundhov@samfundet.no
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, 51 Franklin Steet, Fifth Floor, Boston, MA 02110-1301, USA

#include "graphics.h"
#include "C3dsParser.h"
#include "chunks.def"
#include <assert.h>
#include <iostream>
#include <sys/time.h>
#include <math.h>
#include <map>
#include <GL/freeglut.h>


//fugly
float Graphics::m_angle = 0;
float Graphics::m_angleSpeed = 1;
float Graphics::m_scale = 0;
int Graphics::m_angleSpin[3] = { 1, 1, 1 };
int Graphics::sceneOffset = 0;
int Graphics::outroOffset = 0;
C3dsParser* Graphics::m_parser;
vector<GLuint> Graphics::m_callLists;
suseconds_t Graphics::m_lastUpdate;
suseconds_t Graphics::m_started;
Sound Graphics::m_sound("default");

Graphics::Graphics(int argc, char **argv)
{
    struct timeval now;
    gettimeofday(&now, NULL);
    m_started = (now.tv_sec * 1000000) + now.tv_usec;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutCreateWindow(".text on a box");

    glClearColor(0, 0, 0, 0); 

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glutKeyboardFunc(keydown);

    glutDisplayFunc(&display);
    glutIdleFunc(&display);
    glutReshapeFunc(&reshape);
    glEnable(GL_DEPTH_TEST);

	int offset;
    m_parser = new C3dsParser("intro.3ds");
    sceneOffset = compileObject(0);
	printf("Sceneoffset %i\n", sceneOffset);
	delete m_parser;
    m_parser = new C3dsParser("del-fin.3ds");
    outroOffset = compileObject(sceneOffset);
	delete m_parser;
    m_parser = new C3dsParser("outro.3ds");
    compileObject(outroOffset);

    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 0.1 };
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);


    GLfloat light_position[] = { -5.0, 5.0, 5.0, 0.0 };

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    glutFullScreen();
    m_sound.play();

    glutMainLoop();
}

void Graphics::keydown(unsigned char key, int x, int y) {
	printf("Time: %i\n", m_lastUpdate);
    switch (key) {
    case '\033': // Escape
        exit(0);
    default:
        return;
    }

    glutPostRedisplay();
}

void Graphics::display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    struct timeval now;
    gettimeofday(&now, NULL);
    suseconds_t cur = (now.tv_sec * 1000000) + now.tv_usec;
    cur -= m_started;
    suseconds_t diff = cur - m_lastUpdate;
    m_lastUpdate = cur;

    if (cur < 10000000) {
        introDisplay(cur, diff);
    } else if (cur < 50000000) {
        sceneDisplay(cur, diff);
    } else if (cur < 41487691) {
        outroDisplay(cur, diff);
    } else {
        exit(0);
    }

    glutSwapBuffers();
}

int Graphics::compileObject(int offset){
    string currentMesh, currentMaterial, mat;
    uint16_t numFaces;
    map<string, face*>    faces;
    map<string, vector3*> vertices;
    map<string, int>      vertCount;
    map<string, int>      faceCount;
    map<string, color>   faceMaterial;
    map<string, color>    colors;

    while ( !m_parser->eof() ) {
		m_parser->enterChunk();
		switch ( m_parser->getChunkId() ){
			case MAIN_CHUNK:
				break;

			case MAIN_VERSION:
				m_parser->skipChunk();
				break;

			case EDITOR_CHUNK:
				break;

		   	case OBJECT_BLOCK:
                currentMesh = m_parser->extractStrData();
		   		break;

		   	case TRIANGULAR_MESH:
		        break;

		    case VERTICES_LIST:
                assert(!currentMesh.empty());

				vertCount[currentMesh] = m_parser->extractCount();
				vertices[currentMesh] = m_parser->extractArray<vector3>(vertCount[currentMesh]);
		       	break;

			case FACES_LIST:
				faceCount[currentMesh] = m_parser->extractCount();
				faces[currentMesh] = m_parser->extractArray<face>(faceCount[currentMesh], 2);
		       	break;

			case MATERIAL_BLOCK:
				break;

			case MATERIAL_NAME:
                currentMaterial = m_parser->extractStrData();
				m_parser->skipChunk();
				break;

            case DIFFUSE_COLOR:
                break;

            case RGB1:
                colors[currentMaterial] = m_parser->extractValue<color>();
				m_parser->skipChunk();
                break;

            case FACES_MATERIAL:
                mat = m_parser->extractStrData();
                numFaces = m_parser->extractValue<uint16_t>();
                faceMaterial[currentMesh] = colors[mat];
                m_parser->skipChunk();
                break;

			default:
				printf("Unkown chunk %04x of length %u\n", m_parser->getChunkId(), m_parser->getChunkLength());
				m_parser->skipChunk();
				break;

            }
    }

    printf("Finished parsing!\n");
    vector3 *vertex;
    face *cface;
    vector3 normal;
    GLuint list;
    GLfloat color[3];

    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 0.0 };
	int i=0;
    for (map<string, vector3*>::iterator it = vertices.begin(); it != vertices.end(); it++) {
		i++;
        list = glGenLists(1);
        glNewList(list, GL_COMPILE);
        m_callLists.push_back(list);

        vertex = vertices[(*it).first];
        cface = faces[(*it).first];

        for (int i=0; i<faceCount[(*it).first]; i++) {
            normal = vector3::normal(vertex[cface[i].a], vertex[cface[i].b], vertex[cface[i].c]);
            glBegin(GL_TRIANGLES);
            color[0] = faceMaterial[(*it).first].r/255.0f;
            color[1] = faceMaterial[(*it).first].g/255.0f;
            color[2] = faceMaterial[(*it).first].b/255.0f;
            glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
            glNormal3f(normal.x, normal.y, normal.z);
            glVertex3fv((GLfloat*)&vertex[cface[i].a]);
            glVertex3fv((GLfloat*)&vertex[cface[i].b]);
            glVertex3fv((GLfloat*)&vertex[cface[i].c]);
            glEnd();
        }
        glEndList();
    }
	return i;
}

void Graphics::reshape(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f,(GLfloat)w/(GLfloat)h,0.1f,100.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


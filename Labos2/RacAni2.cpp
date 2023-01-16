#define _CRT_SECURE_NO_WARNINGS

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#include <string>
#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

class Source {
public:
	float x, y, z;
	int q;
	float cR, cG, cB;
	double size;
};

class Vertex {
public:
	float x, y, z;
	Vertex(float x1, float y1, float z1) {
		x = x1; y = y1; z = z1;
	}
};

class Particle {
public:
	float x, y, z;
	float r, g, b;
	float v;
	int t;
	float sX, sY, sZ;
	float axX, axY, axZ;
	double angle; double size;
};

vector<Particle> particles;
Source i;

GLuint window; 
GLuint sub_width = 512, sub_height = 512;

void myDisplay			();
void myIdle				();
void myReshape			(int width, int height);
void myKeyboard			(unsigned char theKey, int mouseX, int mouseY);

void drawSource		();
void drawParticle		();
void drawParticle		(Particle c);
GLuint LoadTextureRAW( const char * filename, int wrap );

double maxH = 42.0;
double pi = 3.14159;
int tempQ = -1;
vector<Particle>;
  GLuint tex;
  Vertex viewpoint(0.0, 0.0, 75.0);

int main(int argc, char ** argv)
{
  i.x = 0.0; i.y = 0; i.z = 0.0;
  i.q = 10;
  i.cB = 0.0; i.cG = 0.0; i.cR = 1.0;
  i.size = 0.4;
  srand (time(NULL));
  
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(sub_width,sub_height);
	glutInitWindowPosition(100,100);
	glutInit(&argc, argv);

	window = glutCreateWindow("Sustav Cestica");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);
	glutIdleFunc(myIdle);
  tex = LoadTextureRAW("snow.bmp", 0);

  glBlendFunc(GL_SRC_ALPHA,GL_ONE);					
	glEnable(GL_BLEND);

	
      glEnable( GL_TEXTURE_2D );
      glBindTexture( GL_TEXTURE_2D, tex );

	glutMainLoop();
    	return 0;
}

void myReshape(int width, int height)
{
	sub_width = width;                      	
    sub_height = height;						

    glViewport(0, 0, sub_width, sub_height);	
	
	glMatrixMode(GL_PROJECTION);                
    glLoadIdentity();                            
    
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,150.0f);
 
    glMatrixMode(GL_MODELVIEW);                 
    glLoadIdentity();                           
	

	glLoadIdentity();							
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );		
	glClear( GL_COLOR_BUFFER_BIT );				
	glPointSize(1.0);							
	glColor3f(0.0f, 0.0f, 0.0f);				
}

int t = 0;

void myDisplay()
{

 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	glTranslatef(viewpoint.x, viewpoint.y, -viewpoint.z);

	drawParticle();

	glutSwapBuffers();
}

int currentTime = 0; int previousTime = 0;

void myIdle() {
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	int timeInterval = currentTime - previousTime;
	if(timeInterval > 100) {
		
		if(i.q > 0) {
			int n = rand() % i.q + 1;
			for (int j = 0; j < n; j++) {
				double x, y, z, norm;
				x = (rand()%2000-1000) / (double)1000;
				y = (rand()%2000-1000) / (double)1000;
				z = (rand()%2000-1000) / (double)1000;
				norm = pow(pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0), 0.5);
				x /= norm; y /= norm; z /= norm;
				Particle c;
				c.x = i.x; c.z = i.z; c.y = i.y; 
				c.r = i.cR; c.g = i.cG; c.b = i.cB; c.v = 0.8;
				c.sX = x; c.sY = y; c.sZ = z;
				c.t = 80 + (rand()%21 - 10);
				c.size = i.size;
				particles.push_back(c);
			}
		}

		for (int j =  particles.size() - 1; j >= 0; j--) {

			Vertex s(0.0, 0.0, 1.0);
			Vertex os(0.0, 0.0, 0.0);
			Vertex e(0.0, 0.0, 0.0);
			
			e.x = particles.at(j).x - viewpoint.x;
			e.y = particles.at(j).y - viewpoint.y;
			e.z = particles.at(j).z - viewpoint.z;

			os.x = s.y*e.z - e.y*s.z;
			os.y = e.x*s.z - s.x*e.z;
			os.z = s.x*e.y - s.y*e.x;

			double apsS = pow(pow((double)s.x, 2.0) + pow((double)s.y, 2.0) + pow((double)s.z, 2.0), 0.5);
			double apsE = pow(pow((double)e.x, 2.0) + pow((double)e.y, 2.0) + pow((double)e.z, 2.0), 0.5);
			double se = s.x*e.x + s.y*e.y + s.z*e.z;
			double angle = acos(se/(apsS*apsE));
			particles.at(j).angle = angle / (2*pi) * 360;
			particles.at(j).axX = os.x; particles.at(j).axY = os.y; particles.at(j).axZ = os.z;

			particles.at(j).y += particles.at(j).v * particles.at(j).sY;
			particles.at(j).z += particles.at(j).v * particles.at(j).sZ;
			particles.at(j).x += particles.at(j).v * particles.at(j).sX;

			particles.at(j).t--;

			if (particles.at(j).r > 0) particles.at(j).r -= 0.008;
			if (particles.at(j).b < 1.0) particles.at(j).b += 0.008;
			if (particles.at(j).t <= 0) {
				particles.erase(particles.begin()+j);
			}
		}

		myDisplay();
		previousTime = currentTime;
	}
}

void drawParticle() {
	for (int j = 0; j < particles.size(); j++) {
		drawParticle(particles.at(j));
	}
}

void drawParticle(Particle c) {

	glColor3f(c.r, c.g, c.b);
	glTranslatef(c.x, c.y, c.z);
	glRotatef(c.angle, c.axX, c.axY, c.axZ);//
	glBegin(GL_QUADS);
	
	glTexCoord2d(0.0,0.0); glVertex3f(-c.size, -c.size, 0.0);
	glTexCoord2d(1.0,0.0); glVertex3f(-c.size, +c.size, 0.0);
	glTexCoord2d(1.0,1.0); glVertex3f(+c.size, +c.size, 0.0);
	glTexCoord2d(0.0,1.0); glVertex3f(+c.size, -c.size, 0.0);

	glEnd();
	glRotatef(-c.angle, c.axX, c.axY, c.axZ);//
	glTranslatef(-c.x, -c.y, -c.z);
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY) {
	if(theKey == 'a') i.x -= 0.5;
	if(theKey == 'd') i.x += 0.5;
	if(theKey == 'w') i.y -= 0.5;
	if(theKey == 's') i.y += 0.5;

	if(theKey == '+' && i.size < 4.0) i.size += 0.01;
	if(theKey == '-' && i.size > 0.02) i.size -= 0.01;
}

GLuint LoadTextureRAW( const char * filename, int wrap )
{
    GLuint texture;
    int width, height;
    BYTE * data;
    FILE * file;
    file = fopen( filename, "rb" );
    if ( file == NULL )  {
		return 0;
	}

    width = 256;
    height = 256;
    data =(BYTE*) malloc( width * height * 3 );

    fread( data, width * height * 3, 1, file );
    fclose( file );

    glGenTextures( 1, &texture );

    glBindTexture( GL_TEXTURE_2D, texture );

    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                     GL_LINEAR_MIPMAP_NEAREST );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                     wrap ? GL_REPEAT : GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                     wrap ? GL_REPEAT : GL_CLAMP );

    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,
                       GL_RGB, GL_UNSIGNED_BYTE, data );

    free( data );

    return texture;
}
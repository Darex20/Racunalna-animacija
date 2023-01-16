#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>

#include <string>
#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

int F = 0, V = 0, B = 0;
int S = 0;

class Vertex {
public:
	float x, y, z;
	Vertex(float x1, float y1, float z1) {
		x = x1; y = y1; z = z1;
	}
};
class Polygon {
public:
	Vertex * v1, * v2, * v3;
	Polygon (Vertex * vX, Vertex * vY, Vertex * vZ) {
		v1 = vX; v2 = vY; v3 = vZ;
	}
};

Polygon *polygons;
Vertex *vertices;
Vertex *vertexB;
Vertex *bSpline;
Vertex *tanB, *allTanB;
Vertex center(0.0, 0.0, 0.0);
Vertex ax(0.0, 0.0, 0.0);
Vertex s(0.0, 0.0, 1.0);
Vertex e(0.0, 0.0, 0.0);
float pi =  3.14159265;

GLuint window; 
GLuint sub_width = 512, sub_height = 512;


void myDisplay			();
void myIdle				();
void myReshape			(int width, int height);

int main(int argc, char ** argv)
{
  vector<string> text_file;
  
  ifstream ifs("porsche.obj");
  string temp;

  while( getline( ifs, temp ) )
     text_file.push_back( temp );

  for (int i = 0; i < text_file.size(); i++) {
	  if (text_file.at(i).at(0) == 'f') F++;
	  else if (text_file.at(i).at(0) == 'v') V++;
  }

  vertices = (Vertex *) malloc (V * sizeof(Vertex));
  polygons = (Polygon *) malloc (brF * sizeof(Polygon));

  int oVertices = 0, oPolygons = 0;

  for (int i = 0; i < text_file.size(); i++) {
	  if (text_file.at(i).at(0) == 'v') {
		  float d1, d2, d3;
		  char *text = new char[text_file.at(i).size()+1];
		  std::copy(text_file.at(i).begin(), text_file.at(i).end(), text);
		  sscanf_s(text, "v %f %f %f", &d1, &d2, &d3);
		  d1 *= 4; d2 *= 4; d3 *= 4;
		  center.x += d1; center.y += d2; center.z += d3;
		  Vertex v(d1, d2, d3);
		  vertices[oVertices] = v;
		  oVertices++;
	  }
  }
  center.x /= oVertices; center.y /= oVertices; center.z /= oVertices;

  for (int i = 0; i < text_file.size(); i++) {
	  if (text_file.at(i).at(0) == 'f') {
		  int d1, d2, d3;
		  char *text = new char[text_file.at(i).size()+1];
		  std::copy(text_file.at(i).begin(), text_file.at(i).end(), text);
		  sscanf_s(text, "f %d %d %d", &d1, &d2, &d3);
		  Polygon p(&vertices[d1-1], &vertices[d2-1], &vertices[d3-1]);
		  polygons[oPolygons] = p;
		  oPolygons++;
	  }
  }

  text_file.clear();
  ifstream ifs2("bSpline.txt");
  string temp2;

  while (getline(ifs2, temp2))
	 text_file.push_back(temp2);
  

  B = text_file.size();
  vertexB = (Vertex *) malloc (B * sizeof(Vertex));
  S = B - 3;
	

  for (int i = 0; i < B; i++) {
	float d1, d2, d3;
	char *text = new char[text_file.at(i).size()+1];
	std::copy(text_file.at(i).begin(), text_file.at(i).end(), text);
	sscanf_s(text, "%f %f %f", &d1, &d2, &d3);
	Vertex v(d1, d2, d3);
	vertexB[i] = v;
  }

  for (int i = 0; i < S; i++) {
	  Vertex v1 = vertexB[i]; Vertex v2 = vertexB[i+1];
	  Vertex v3 = vertexB[i+2]; Vertex v4 = vertexB[i+3];

  }

  bSpline = (Vertex *) malloc (100 * S * sizeof(Vertex));
  tanB = (Vertex *) malloc (S *8 * sizeof(Vertex));
  allTanB = (Vertex *) malloc (S * 2 * 100 * sizeof(Vertex));
  int tanNum = 0, numOfTan = 0;

  for (int i = 0; i < S; i++) {
	  Vertex v0 = vertexB[i];
	  Vertex v1 = vertexB[i+1];
	  Vertex v2 = vertexB[i+2];
	  Vertex v3 = vertexB[i+3];

	  for (int t = 0; t < 100; t++) {
		  double j = t/100.0;
		  float f1 = (- pow(j,3.0) + 3*pow(j,2.0) - 3*j + 1) / 6.0;
		  float f2 = (3*pow(j,3.0) - 6*pow(j,2.0) + 4) / 6.0;
		  float f3 = (- 3*pow(j,3.0) + 3*pow(j,2.0) + 3*j + 1) / 6.0;
		  float f4 = pow(j,3.0) / 6.0;

		  bSpline[100*i + t].x = f1*v0.x + f2*v1.x + f3*v2.x + f4*v3.x;
		  bSpline[100*i + t].y = f1*v0.y + f2*v1.y + f3*v2.y + f4*v3.y;
		  bSpline[100*i + t].z = f1*v0.z + f2*v1.z + f3*v2.z + f4*v3.z;
		  
		  if (t % 25 == 0) {
			  float t1 = 0.5 * (-pow(j, 2.0) + 2*j -1);
			  float t2 = 0.5 * (3 * pow(j, 2.0) - 4*j);
			  float t3 = 0.5 * (-3* pow(j, 2.0) + 2*j +1);
			  float t4 = 0.5 * (pow(j, 2.0));

			  tanB[tanNum].x = f1*v0.x + f2*v1.x + f3*v2.x + f4*v3.x;
			  tanB[tanNum].y = f1*v0.y + f2*v1.y + f3*v2.y + f4*v3.y;
			  tanB[tanNum].z = f1*v0.z + f2*v1.z + f3*v2.z + f4*v3.z;
			  tanNum++;

			  float vx = t1*v0.x + t2*v1.x + t3*v2.x + t4*v3.x;
			  float vy = t1*v0.y + t2*v1.y + t3*v2.y + t4*v3.y;
			  float vz = t1*v0.z + t2*v1.z + t3*v2.z + t4*v3.z;
			  
			  tanB[tanNum].x = tanB[tanNum-1].x + vx/3;
			  tanB[tanNum].y = tanB[tanNum-1].y + vy/3;
			  tanB[tanNum].z = tanB[tanNum-1].z + vz/3;
			  tanNum++;
		  }

		float t1 = 0.5 * (-pow(j, 2.0) + 2*j -1);
		float t2 = 0.5 * (3 * pow(j, 2.0) - 4*j);
		float t3 = 0.5 * (-3* pow(j, 2.0) + 2*j +1);
		float t4 = 0.5 * (pow(j, 2.0));

		allTanB[numOfTan].x = f1*v0.x + f2*v1.x + f3*v2.x + f4*v3.x;
		allTanB[numOfTan].y = f1*v0.y + f2*v1.y + f3*v2.y + f4*v3.y;
		allTanB[numOfTan].z = f1*v0.z + f2*v1.z + f3*v2.z + f4*v3.z;
		numOfTan++;

		float vx = t1*v0.x + t2*v1.x + t3*v2.x + t4*v3.x;
		float vy = t1*v0.y + t2*v1.y + t3*v2.y + t4*v3.y;
		float vz = t1*v0.z + t2*v1.z + t3*v2.z + t4*v3.z;
			  
		allTanB[numOfTan].x = allTanB[numOfTan-1].x + vx/3;
		allTanB[numOfTan].y = allTanB[numOfTan-1].y + vy/3;
		allTanB[numOfTan].z = allTanB[numOfTan-1].z + vz/3;
		numOfTan++;
	  }
	  
  }

  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(sub_width,sub_height);
	glutInitWindowPosition(100,100);
	glutInit(&argc, argv);

	window = glutCreateWindow("Animacija objekta");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutIdleFunc(myIdle);

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

    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
 
    glMatrixMode(GL_MODELVIEW);                 
    glLoadIdentity();                        

	glLoadIdentity();						
	glClearColor( 1.0f, 1.0f, 1.0f, 0.0f );		
	glClear( GL_COLOR_BUFFER_BIT );			
	glPointSize(1.0);							
	glColor3f(0.0f, 0.0f, 0.0f);			
}

int t = 0;

void myDisplay()
{
	glLoadIdentity();
 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTranslatef(-5.0, -5.0, -75.0);
	glBegin (GL_LINE_STRIP);
	for(int i = 0; i < 100*S; i++) {
		glVertex3f(bSpline[i].x, bSpline[i].y, bSpline[i].z);
	}
	glEnd();
	glBegin (GL_LINES);
	for (int i = 0; i < S*8; i+=2) {
		glVertex3f(tanB[i].x, tanB[i].y, tanB[i].z);
		glVertex3f(tanB[i+1].x, tanB[i+1].y, tanB[i+1].z);
	}
	glEnd();

	glTranslatef(bSpline[t].x, bSpline[t].y, bSpline[t].z);
	
	e.x = allTanB[2*t+1].x - allTanB[2*t].x;
	e.y = allTanB[2*t+1].y - allTanB[2*t].y;
	e.z = allTanB[2*t+1].z - allTanB[2*t].z;
	
	ax.x = s.y*e.z - e.y*s.z;
	ax.y = e.x*s.z - s.x*e.z;
	ax.z = s.x*e.y - s.y*e.x;

	double apsS = pow(pow((double)s.x, 2.0) + pow((double)s.y, 2.0) + pow((double)s.z, 2.0), 0.5);
	double apsE = pow(pow((double)e.x, 2.0) + pow((double)e.y, 2.0) + pow((double)e.z, 2.0), 0.5);
	double se = s.x*e.x + s.y*e.y + s.z*e.z;
	double angle = acos(se/(apsS*apsE));
	angle = angle / (2*pi) * 360;
	glRotatef(angle, ax.x, ax.y, ax.z);
	
	glTranslatef(-center.x, -center.y, -center.z);

	glBegin(GL_LINES);
	for (int i = 0; i < F; i++) {
		Vertex v1 = *polygons[i].v1;
		Vertex v2 = *polygons[i].v2;
		Vertex v3 = *polygons[i].v3;
		
		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v2.x, v2.y, v2.z);

		glVertex3f(v2.x, v2.y, v2.z);
		glVertex3f(v3.x, v3.y, v3.z);

		glVertex3f(v3.x, v3.y, v3.z);
		glVertex3f(v1.x, v1.y, v1.z);
	}
	
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(center.x, center.y, center.z);
	glVertex3f(center.x + 2.5, center.y, center.z);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(center.x, center.y, center.z);
	glVertex3f(center.x, center.y + 2.5, center.z);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(center.x, center.y, center.z);
	glVertex3f(center.x, center.y, center.z + 2.5);

	glColor3f(0.0, 0.0, 0.0);
	glEnd();
	
	t++;
	if (t == 100*S) t = 0;

	glFlush();
}

int currentTime = 0; int previousTime = 0;

void myIdle() {
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	int timeInterval = currentTime - previousTime;
	if(timeInterval > 10) {
		myDisplay();
		previousTime = currentTime;
	}
}
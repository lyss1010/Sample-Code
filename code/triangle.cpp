#include "triangle.h"
#include "glut.h"

triangle::triangle()
{
}


triangle::~triangle()
{
}

void triangle::draw(){
	Vector3 firstPos = tri[0]->newPosition;
	Vector3 secondPos = tri[1]->newPosition;
	Vector3 thirdPos = tri[2]->newPosition;

	Vector3 firstNormal = tri[0]->newNormal;
	Vector3 secondNormal = tri[1]->newNormal;
	Vector3 thirdNormal = tri[2]->newNormal;

	glBegin(GL_TRIANGLES);
	glNormal3f(firstNormal.x, firstNormal.y, firstNormal.z);
	if (tri[0]->texcoord[0] != -100){
		glTexCoord2f(tri[0]->texcoord[0], tri[0]->texcoord[1]);
	}
	glVertex3f(firstPos.x, firstPos.y, firstPos.z);
	
	glNormal3f(secondNormal.x, secondNormal.y, secondNormal.z);
	if (tri[0]->texcoord[0] != -100){
		glTexCoord2f(tri[1]->texcoord[0], tri[1]->texcoord[1]);
	}
	glVertex3f(secondPos.x, secondPos.y, secondPos.z);

	glNormal3f(thirdNormal.x, thirdNormal.y, thirdNormal.z);
	if (tri[0]->texcoord[0] != -100){
		glTexCoord2f(tri[2]->texcoord[0], tri[2]->texcoord[1]);
	}
	glVertex3f(thirdPos.x, thirdPos.y, thirdPos.z);

	glEnd();
}


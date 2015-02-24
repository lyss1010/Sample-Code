#include "skin.h"
#include <stdio.h>
#include <iostream>
#include "bmploader.h"
using namespace std;
GLuint texture[1];
Skin::Skin()
{
}

Skin::~Skin()
{
}


// Load Bitmaps And Convert To Textures
void LoadGLTextures()
{	
	BMPImage image1;
	// Load Texture
	if (!image1.load("head.bmp"))
		exit;

	// Create Texture
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// note that BMP images store their data in BRG order, not RGB
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image1.sizeX, image1.sizeY, 0, GL_BGR_EXT,
		GL_UNSIGNED_BYTE, image1.data);
	// Set bi-linear filtering for both minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
};



bool Skin::load(const char*filename){
	Tokenizer token;
	token.Open(filename);
	token.FindToken("positions");
	numOfVertex = token.GetInt();
	token.FindToken("{");
	float x, y, z;
	char temp[256];
		for (int i = 0; i < numOfVertex; i++){     //position
			Vertex*newVertex = new Vertex();
			x = token.GetFloat();
			y = token.GetFloat();
			z = token.GetFloat();
			Vector3 tempPosition(x, y, z);
			newVertex->position = tempPosition;
			vertexs.push_back(newVertex);
		}
		token.FindToken("}");

		token.GetToken(temp);						//normal
		numOfVertex = token.GetInt();
		token.FindToken("{");
		for (int i = 0; i < numOfVertex; i++){       
			x = token.GetFloat();
			y = token.GetFloat();
			z = token.GetFloat();
			Vector3 tempNormal(x, y, z);
			vertexs[i]->normal = tempNormal;
		}
		token.FindToken("}");

		token.GetToken(temp);						
		numOfVertex = token.GetInt();
		token.FindToken("{");
		
		if (strcmp(temp, "texcoords") == 0){
			for (int i = 0; i < numOfVertex; i++){       //texcoords
				x = token.GetFloat();
				y = token.GetFloat();
				vertexs[i]->texcoord[0] = x;
				vertexs[i]->texcoord[1] = y;
			}
			token.FindToken("}");
			token.GetToken(temp);
			numOfVertex = token.GetInt();
			token.FindToken("{");
		}
		
		for (int i = 0; i < numOfVertex; i++){       //skinweights
			int numOfBinding;
			numOfBinding = token.GetInt();
			vertexs[i]->numOfBindings = numOfBinding;
			for (int j = 0; j < numOfBinding; j++){
				x = token.GetInt();
				y = token.GetFloat();
				vertexs[i]->jointIndex.push_back(x);
				vertexs[i]->jointWeight.push_back(y);
			}
		}
		token.FindToken("}");

		token.GetToken(temp);						 
		if (strcmp(temp, "material") == 0){				 //material
			token.GetToken(temp);
			token.FindToken("{");
			token.GetToken(temp);
			token.GetToken(temp);
			//image1.load(temp);
			LoadGLTextures();
			token.FindToken("}");
			token.GetToken(temp);
		}
		numOfTriangle = token.GetInt();
		token.FindToken("{");
		for (int i = 0; i < numOfTriangle; i++){     //triangle
			triangle *newTriangle = new triangle();
			x = token.GetInt();
			y = token.GetInt();
			z = token.GetInt();
			newTriangle->tri[0] = vertexs[x];
			newTriangle->tri[1] = vertexs[y];
			newTriangle->tri[2] = vertexs[z];
			triangles.push_back(newTriangle);
		}
		token.FindToken("}");

		token.GetToken(temp);						 //bindings
		numOfBindings = token.GetInt();
		token.FindToken("{");
		//cout << numOfBindings << endl;
		for (int i = 0; i < numOfBindings; i++){    
			token.FindToken("matrix");
			token.FindToken("{");
			x = token.GetFloat();
			y = token.GetFloat();
			z = token.GetFloat();
			Vector3 A(x, y, z);
			x = token.GetFloat();
			y = token.GetFloat();
			z = token.GetFloat();
			Vector3 B(x, y, z);
			x = token.GetFloat();
			y = token.GetFloat();
			z = token.GetFloat();
			Vector3 C(x, y, z);
			x = token.GetFloat();
			y = token.GetFloat();
			z = token.GetFloat();
			Vector3 D(x, y, z);
			Matrix34 tempMatrix;
			tempMatrix.a = A;
			tempMatrix.b = B;
			tempMatrix.c = C;
			tempMatrix.d = D;
			tempMatrix.Inverse();
			bindings.push_back(tempMatrix);
			token.FindToken("}");
		}
	
	token.Close();
	return true;
}


void Skin::draw(){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	for (int i = 0; i < numOfTriangle; i++){
		triangles[i]->draw();
	}
	//glDisable(GL_TEXTURE);
}

void Skin::update(vector <joint*> &joints){
	for (size_t i=0; i < vertexs.size(); i++){
		vertexs[i]->update(bindings,joints);
	}
}
#include "morph.h"


Morph::Morph()
{
}


Morph::~Morph()
{
}


bool Morph::load(const char*filename, std::vector<Vertex*> &vertexs){
	Tokenizer token;
	token.Open(filename);
	token.FindToken("positions");
	num = token.GetInt();
	token.FindToken("{");
	int index;
	float x, y, z;
	char temp[256];
	for (int i = 0; i < num; i++){     //position

		index = token.GetInt();
		x = token.GetFloat();
		y = token.GetFloat();
		z = token.GetFloat();
		Vector3 tempPosition(x, y, z);
		vertexs[index]->position = tempPosition;
		
	}
	token.FindToken("}");

	token.GetToken(temp);						//normal
	num = token.GetInt();
	token.FindToken("{");
	for (int i = 0; i < num; i++){
		index = token.GetInt();
		x = token.GetFloat();
		y = token.GetFloat();
		z = token.GetFloat();
		Vector3 tempNormal(x, y, z);
		vertexs[index]->normal = tempNormal;
	}
	token.FindToken("}");
	token.Close();
	return true;
}
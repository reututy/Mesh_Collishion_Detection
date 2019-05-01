#define GLEW_STATIC
#include <GL\glew.h>
#include "shape.h"
#include "Log.hpp"


Shape::Shape(const Shape& shape,unsigned int mode)
{
	
	mesh = new MeshConstructor(*shape.mesh);
	//tex = shape.tex;
	isCopy = true;
	this->mode = mode;
	toRender = true;
}

Shape::Shape(const std::string& fileName, unsigned int mode){
	mesh = new MeshConstructor(fileName);
	isCopy = false;
	this->mode = mode;
	toRender = true;
}

Shape::Shape(const int SimpleShapeType,unsigned int mode)
{
	mesh = new MeshConstructor(SimpleShapeType);
	//mesh->Bind();
	this->mode = mode;
	isCopy = false;
	toRender = true;
}

Shape::Shape(Bezier1D *curve, unsigned int xResolution,unsigned int yResolution,bool is2D,unsigned int mode)
{
	mesh = new MeshConstructor(curve,is2D,xResolution,yResolution);
	this->mode = mode;
	isCopy = false;
	toRender = true;
}


void Shape::AddTexture(const std::string& textureFileName)
{
	tex = new Texture(textureFileName);
}


void Shape::Draw(const Shader& shader)
{
//	if(tex)
//		tex->Bind();

	shader.Bind();
	mesh->Bind();
	/*if(isCopy)
		glDrawArrays(GL_TRIANGLES, 0, indicesNum);
	else*/
	GLCall(glDrawElements(mode,mesh->GetIndicesNum(), GL_UNSIGNED_INT, 0));
	mesh->Unbind();
}

void Shape::change_mode(unsigned int new_mode)
{
	this->mode = new_mode;
}

MeshConstructor* Shape::GetMesh()
{
	return mesh;
}

unsigned int Shape::GetMode()
{
	return mode;
}

Shape::~Shape(void)
{
	if(!isCopy)
	{
		if(mesh)
			delete mesh;
		if(tex)
			delete tex;
	}
}

// Returns -1 if there is no collision 
int Shape::CollisionDetection(Shape* other)
{	
	//TODO: Check the orientation
	BoundingBox* box_that_collides = mesh->CollisionDetection(other->GetMesh(), 
															  glm::translate(glm::mat4(1), glm::vec3(this->getTraslate())), GetRot(),
															  glm::translate(glm::mat4(1), glm::vec3(other->getTraslate())), other->GetRot());
	if (box_that_collides != nullptr)
		return box_that_collides->GetNumOfShape();
	return -1;
}

void Shape::SetNumOfShape(int value)
{
	num_of_shape = value;
}

int Shape::GetNumOfShape()
{
	return num_of_shape;
}

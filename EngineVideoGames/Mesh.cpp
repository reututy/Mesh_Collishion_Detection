#include "Mesh.h"
#include <iostream>

#define BLUE glm::vec3(0, 0, 1)
#define GREEN glm::vec3(0, 1, 0)
#define RED glm::vec3(1, 0, 0)

#define NUM_OF_POINTS 2

void IndexedModel::CalcNormals()
{
    for(unsigned int i = 0; i < indices.size(); i += 3)
    {
        int i0 = indices[i];
        int i1 = indices[i + 1];
        int i2 = indices[i + 2];

        glm::vec3 v1 = positions[i1] - positions[i0];
        glm::vec3 v2 = positions[i2] - positions[i0];
        
        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));
            
        normals[i0] += normal;
        normals[i1] += normal;
        normals[i2] += normal;
    }
    
    for(unsigned int i = 0; i < positions.size(); i++)
	{
        normals[i] = glm::normalize(normals[i]);
		colors[i] = (glm::vec3(1,1,1) + normals[i])/2.0f;
	}
}

IndexedModel CubeTriangles()
{
	Vertex vertices[] =
	{
		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 0), glm::vec3(0, 0, -1),BLUE),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 0, -1),BLUE),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1),BLUE),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(0, 0, -1),BLUE),

		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(0, 0, 1),BLUE),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1),BLUE),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1),BLUE),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1),BLUE),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(0, 1), glm::vec3(0, -1, 0),BLUE),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 1), glm::vec3(0, -1, 0),BLUE),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(0, -1, 0),BLUE),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(0, 0), glm::vec3(0, -1, 0),BLUE),

		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 1, 0),BLUE),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(1, 1), glm::vec3(0, 1, 0),BLUE),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(0, 1, 0),BLUE),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 1, 0),BLUE),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 1), glm::vec3(-1, 0, 0),BLUE),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(-1, 0, 0),BLUE),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(-1, 0, 0),BLUE),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(-1, 0, 0),BLUE),

		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(1, 0, 0),BLUE),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(1, 0, 0),BLUE),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 0), glm::vec3(1, 0, 0),BLUE),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(1, 0, 0),BLUE)
	};

	unsigned int indices[] =	{0, 1, 2,
							  0, 2, 3,

							  6, 5, 4,
							  7, 6, 4,

							  10, 9, 8,
							  11, 10, 8,

							  12, 13, 14,
							  12, 14, 15,

							  16, 17, 18,
							  16, 18, 19,

							  22, 21, 20,
							  23, 22, 20
	                          };

	    IndexedModel model;
	
	for(unsigned int i = 0; i < 24; i++)
	{
		model.positions.push_back(*vertices[i].GetPos());
		model.colors.push_back(*vertices[i].GetColor());
		model.normals.push_back(*vertices[i].GetNormal());
		model.texCoords.push_back(*vertices[i].GetTexCoord());
	}
	for(unsigned int i = 0; i < 36; i++)
        model.indices.push_back(indices[i]);
	
	return model;
}

IndexedModel AxisGenerator()
{
	IndexedModel model;

	LineVertex axisVertices[] = 
	{
		LineVertex(glm::vec3(1,0,0),glm::vec3(1,0,0)),
		LineVertex(glm::vec3(-1,0,0),glm::vec3(1,0,0)),
		LineVertex(glm::vec3(0,1,0),glm::vec3(0,1,0)),
		LineVertex(glm::vec3(0,-1,0),glm::vec3(0,1,0)),
		LineVertex(glm::vec3(0,0,1),glm::vec3(0,0,1)),
		LineVertex(glm::vec3(0,0,-1),glm::vec3(0,0,1)),
	};

	 unsigned int axisIndices[] = 
	{
		0,1,
		2,3,
		4,5
	};

	 for(unsigned int i = 0; i < 6; i++)
	{
		model.positions.push_back(*axisVertices[i].GetPos());
		model.colors.push_back(*axisVertices[i].GetColor());

	}
	for(unsigned int i = 0; i < 6; i++)
        model.indices.push_back(axisIndices[i]);
	
	return model;
}

IndexedModel TethrahedronGenerator()
{

	Vertex vertices[] =
	{
		Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(-1, -1, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 0), glm::vec3(-1, -1, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(0, 1), glm::vec3(-1, -1, -1),glm::vec3(0, 0, 1)),

		Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(1, -1, 1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(0, 0), glm::vec3(1, -1, 1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(0, 1), glm::vec3(1, -1, 1),glm::vec3(0, 1, 1)),

		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 1), glm::vec3(1, 1, -1),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(1, 1), glm::vec3(1, 1, -1),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 0), glm::vec3(1, 1, -1),glm::vec3(0, 1, 0)),

		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(-1, -1, -1),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 1), glm::vec3(-1, -1, -1),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 0), glm::vec3(-1, -1, -1),glm::vec3(1, 1, 0)),

	};

	unsigned int indices[] =	{0, 1, 2,
							     3, 4, 5,
								 6,7,8,
								 9,10,11
						
	                          };

	    IndexedModel model;
	
	for(unsigned int i = 0; i < 12; i++)
	{
		model.positions.push_back(*vertices[i].GetPos());
		model.colors.push_back(*vertices[i].GetColor());
		model.normals.push_back(*vertices[i].GetNormal());
		model.texCoords.push_back(*vertices[i].GetTexCoord());
	}
	for(unsigned int i = 0; i < 12; i++)
        model.indices.push_back(indices[i]);
	
	return model;
}

IndexedModel OctahedronGenerator()
{
	Vertex vertices[] =
	{
		Vertex(glm::vec3(0, 0,-1), glm::vec2(1, 0), glm::vec3(1, 1, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(0, 1, 0), glm::vec2(0, 0), glm::vec3(1, 1, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(1, 0, 0), glm::vec2(0, 1), glm::vec3(1, 1, -1),glm::vec3(0, 0, 1)),
		
		Vertex(glm::vec3(0, 0, -1), glm::vec2(1, 0), glm::vec3(1, -1, -1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(0,-1, 0), glm::vec2(0, 0), glm::vec3(1, -1, -1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(1, 0, 0), glm::vec2(0, 1), glm::vec3(1, -1, -1),glm::vec3(0, 1, 1)),

		Vertex(glm::vec3(0, 0, -1), glm::vec2(0, 1), glm::vec3(-1, 1, -1),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(0, 1, 0), glm::vec2(1, 1), glm::vec3(-1, 1, -1),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(-1, 0, 0), glm::vec2(1, 0), glm::vec3(-1, 1, -1),glm::vec3(0, 1, 0)),
		
		Vertex(glm::vec3(0, 0, -1), glm::vec2(0, 1), glm::vec3(-1, -1, -1),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(0, -1, 0), glm::vec2(1, 1), glm::vec3(-1, -1, -1),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(-1, 0, 0), glm::vec2(1, 0), glm::vec3(-1, -1, -1),glm::vec3(1, 1, 0)),		

		Vertex(glm::vec3(0, 0, 1), glm::vec2(1, 1), glm::vec3(-1, -1, 1),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(0, -1, 0), glm::vec2(1, 0), glm::vec3(-1, -1, 1),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, 0, 0), glm::vec2(0, 0), glm::vec3(-1, -1, 1),glm::vec3(1, 0, 0)),
		

		Vertex(glm::vec3(0, 0, 1), glm::vec2(1, 1), glm::vec3(1, -1, 1),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(0, -1, 0), glm::vec2(1, 0), glm::vec3(1, -1, 1),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, 0, 0), glm::vec2(0, 0), glm::vec3(1, -1, 1),glm::vec3(1, 0, 1)),

		Vertex(glm::vec3(0, 0, 1), glm::vec2(1, 1), glm::vec3(1, 1, 1),glm::vec3(0, 0, 0)),
		Vertex(glm::vec3(0,1, 0), glm::vec2(1, 0), glm::vec3(1, 1, 1),glm::vec3(0, 0, 0)),
		Vertex(glm::vec3(1, 0, 0), glm::vec2(0, 0), glm::vec3(1, 1, 1),glm::vec3(0, 0, 0)),

		Vertex(glm::vec3(0, 0, 1), glm::vec2(1, 1), glm::vec3(-1, 1, 1),glm::vec3(0.7f, 0.7f, 0.7f)),
		Vertex(glm::vec3(0, 1, 0), glm::vec2(1, 0), glm::vec3(-1, 1, 1),glm::vec3(0.7f, 0.7f, 0.7f)),
		Vertex(glm::vec3(-1, 0, 0), glm::vec2(0, 0), glm::vec3(-1, 1, 1),glm::vec3(0.7f, 0.7f, 0.7f)),
		
	};

	unsigned int indices[] =	{0, 1, 2,
							     3, 4, 5,
								 6,7,8,
								 9,10,11,
								 12,13,14, 
								 15,16,17,
								 18,19,20,
								 21,22,23,
	                          };

	IndexedModel model;

	for(unsigned int i = 0; i < 24; i++)
	{
		model.positions.push_back(*vertices[i].GetPos());
		model.colors.push_back(*vertices[i].GetColor());
		model.normals.push_back(*vertices[i].GetNormal());
		model.texCoords.push_back(*vertices[i].GetTexCoord());
	}
	for(unsigned int i = 0; i < 24; i++)
        model.indices.push_back(indices[i]);

	return model;
}

// Functions for BoundingBox:

BoundingBox::BoundingBox()
{
	begin = glm::vec3(0, 0, 0);     // begin coordinates
	center = glm::vec3(0, 0, 0);     // center coordinates
	size = glm::vec3(0, 0, 0);       // distance between the center of the box to its side in each dimension 
	xInit = glm::vec3(1, 0, 0);      // x axis of the box. default value (1,0,0)		  
	yInit = glm::vec3(0, 1, 0);      // y axis of the box. default value (0,1,0)		 
	zInit = glm::vec3(0, 0, 1);	  // z axis of the box. default value (0,0,1)

	fixed_center = glm::vec3(0, 0, 0);     // fixed center coordinates
	fixed_size = glm::vec3(0, 0, 0);       // distance between the center of the box to its side in each dimension 
	fixed_xInit = glm::vec3(1, 0, 0);      // x axis of the box. default value (1,0,0)		  
	fixed_yInit = glm::vec3(0, 1, 0);      // y axis of the box. default value (0,1,0)		 
	fixed_zInit = glm::vec3(0, 0, 1);	  // z axis of the box. default value (0,0,1)
}

BoundingBox::BoundingBox(glm::vec3 begin, glm::vec3 center, glm::vec3 size)
{
	this->begin = begin;
	this->center = center;
	this->size = size;
	this->xInit = glm::vec3(1, 0, 0);      // x axis of the box. default value (1,0,0)		
	this->yInit = glm::vec3(0, 1, 0);      // y axis of the box. default value (0,1,0)		
	this->zInit = glm::vec3(0, 0, 1);	   // z axis of the box. default value (0,0,1)

	fixed_center = center;     // fixed center coordinates
	fixed_size = size;       // distance between the center of the box to its side in each dimension 
	fixed_xInit = glm::vec3(1, 0, 0);      // x axis of the box. default value (1,0,0)		  
	fixed_yInit = glm::vec3(0, 1, 0);      // y axis of the box. default value (0,1,0)		 
	fixed_zInit = glm::vec3(0, 0, 1);	  // z axis of the box. default value (0,0,1)
}

BoundingBox::~BoundingBox() {}

/*
Let A (this) and B (other) be oriented bounding boxes (OBB):

PA = coordinate position of the center of A
Ax = unit vector representing the x - axis of A
Ay = unit vector representing the y - axis of A
Az = unit vector representing the z - axis of A
WA = half width of A(corresponds with the local x - axis of A)
HA = half height ofA(corresponds with the local y - axis of A)
DA = half depth ofA(corresponds with the local z - axis of A)

PB = coordinate position of the center of B
Bx = unit vector representing the x - axis of B
By = unit vector representing the y - axis of B
Bz = unit vector representing the z - axis of B
WB = half width ofB(corresponds with the local x - axis of B)
HB = half height ofB(corresponds with the local y - axis of B)
DB = half depth ofB(corresponds with the local z - axis of B)
*/
bool BoundingBox::CheckCollision(BoundingBox* other)
{
	float left_side, right_side;
	//Parameters of A (this):
	glm::vec3 PA = this->center;
	glm::vec3 Ax = glm::normalize(this->xInit);
	glm::vec3 Ay = glm::normalize(this->yInit);
	glm::vec3 Az = glm::normalize(this->zInit);
	float WA = this->size.x;
	float HA = this->size.y;
	float DA = this->size.z;
	//Parameters of B (other):
	glm::vec3 PB = other->center;
	glm::vec3 Bx = glm::normalize(other->xInit);
	glm::vec3 By = glm::normalize(other->yInit);
	glm::vec3 Bz = glm::normalize(other->zInit);
	float WB = other->size.x;
	float HB = other->size.y;
	float DB = other->size.z;
	//Variables:
	glm::vec3 T = PB - PA;
	glm::vec3 L;
	/*
	std::cout << "CheckCollision normalize: ------------------------------" << std::endl;
	std::cout << "this_xInit: " << std::endl;
	std::cout << Ax.x << " " << Ax.y << " " << Ax.z << " " << std::endl;
	std::cout << "other_xInit: " << std::endl;
	std::cout << Bx.x << " " << Bx.y << " " << Bx.z << " " << std::endl;

	std::cout << "this_yInit: " << std::endl;
	std::cout << Ay.x << " " << Ay.y << " " << Ay.z << " " << std::endl;
	std::cout << "other_yInit: " << std::endl;
	std::cout << By.x << " " << By.y << " " << By.z << " " << std::endl;

	std::cout << "this_zInit: " << std::endl;
	std::cout << Az.x << " " << Az.y << " " << Az.z << " " << std::endl;
	std::cout << "other_zInit: " << std::endl;
	std::cout << Bz.x << " " << Bz.y << " " << Bz.z << " " << std::endl;
	*/
	//Check the 15 cases:
	for (int i = 1; i <= 15; i++)
	{
		switch (i)
		{
			case 1:
				L = Ax;
				break;
			case 2:
				L = Ay;
				break;
			case 3:
				L = Az;
				break;
			case 4:
				L = Bx;
				break;
			case 5:
				L = By;
				break;
			case 6:
				L = Bz;
				break;
			case 7:
				L = glm::cross(Ax, Bx);
				break;
			case 8:
				L = glm::cross(Ax, By);
				break;
			case 9:
				L = glm::cross(Ax, Bz);
				break;
			case 10:
				L = glm::cross(Ay, Bx);
				break;
			case 11:
				L = glm::cross(Ay, By);
				break;
			case 12:
				L = glm::cross(Ay, Bz);
				break;
			case 13:
				L = glm::cross(Az, Bx);
				break;
			case 14:
				L = glm::cross(Az, By);
				break;
			case 15:
				L = glm::cross(Az, Bz);
				break;
		}
		left_side = glm::abs(glm::dot(T, L));
		right_side = glm::abs(glm::dot(WA*Ax, L)) + glm::abs(glm::dot(HA*Ay, L)) + glm::abs(glm::dot(DA*Az, L)) +
			glm::abs(glm::dot(WB*Bx, L)) + glm::abs(glm::dot(HB*By, L)) + glm::abs(glm::dot(DB*Bz, L));
		if (left_side > right_side)
			return false;
	}
	return true;
}

void BoundingBox::UpdateDynamicVectors(glm::mat4 translate, glm::mat4 rotate)
{
	center = glm::vec3(translate * glm::vec4(fixed_center, 1));
	xInit = glm::vec3(rotate * glm::vec4(fixed_xInit, 0));
	yInit = glm::vec3(rotate * glm::vec4(fixed_yInit, 0));
	zInit = glm::vec3(rotate * glm::vec4(fixed_zInit, 0));
}

bool BoundingBox::IsSmallestBox()
{
	return num_of_points <= NUM_OF_POINTS;
}

void BoundingBox::SetBoundingBox(glm::vec3 begin, glm::vec3 center, glm::vec3 size)
{
	this->begin = begin;
	this->center = center;
	this->size = size;
	xInit = glm::vec3(1, 0, 0);      // x axis of the box. default value (1,0,0)		  
	yInit = glm::vec3(0, 1, 0);      // y axis of the box. default value (0,1,0)		 
	zInit = glm::vec3(0, 0, 1);	  // z axis of the box. default value (0,0,1)

	fixed_center = center;     // fixed center coordinates
	fixed_size = size;       // distance between the center of the box to its side in each dimension 
	fixed_xInit = glm::vec3(1, 0, 0);      // x axis of the box. default value (1,0,0)		  
	fixed_yInit = glm::vec3(0, 1, 0);      // y axis of the box. default value (0,1,0)		 
	fixed_zInit = glm::vec3(0, 0, 1);	  // z axis of the box. default value (0,0,1)
}

void BoundingBox::SetNumOfShape(int value)
{
	num_of_shape = value;
}

void BoundingBox::SetNumOfPoints(int value)
{
	num_of_points = value;
}

int BoundingBox::GetNumOfShape()
{
	return num_of_shape;
}

int BoundingBox::GetNumOfPoints()
{
	return num_of_points;
}

glm::vec3 BoundingBox::GetBegin()
{
	return begin;
}

glm::vec3 BoundingBox::GetCenter()
{
	return center;
}

glm::vec3 BoundingBox::GetSize()
{
	return size;
}

glm::vec3 BoundingBox::GetxInit()
{
	return xInit;
}

glm::vec3 BoundingBox::GetyInit()
{
	return yInit;
}

glm::vec3 BoundingBox::GetzInit()
{
	return zInit;
}

glm::vec3 BoundingBox::GetFixedCenter()
{
	return fixed_center;
}

glm::vec3 BoundingBox::GetFixedSize()
{
	return fixed_size;
}


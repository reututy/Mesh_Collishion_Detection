#define GLEW_STATIC
#include <GL\glew.h>
#include <iostream>
#include "MeshConstructor.h"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "bezier2D.h"
#include "obj_loader.h"

MeshConstructor::MeshConstructor(const int type)
{
	switch (type)
	{
	case Axis:	
		InitLine(AxisGenerator());
		break;
	case Cube:
		InitMesh(CubeTriangles());
		break;
	case Octahedron:
		InitMesh(OctahedronGenerator());
		break;
	case Tethrahedron:
		InitMesh(TethrahedronGenerator());
		break;
	default:
		break;
	}
}

MeshConstructor::MeshConstructor(const std::string& fileName)
{
	InitMesh(OBJModel(fileName).ToIndexedModel());
}

MeshConstructor::MeshConstructor(Bezier1D *curve,bool isSurface,unsigned int resT,unsigned int resS)
{
	if (isSurface)
	{
		Bezier2D surface(*curve, curve->GetAxis(), 4);
		InitMesh(surface.GetSurface(resT, resS));
	}
	else
	{
		InitLine(curve->GetLine(resT));
	}
}

MeshConstructor::MeshConstructor(const MeshConstructor &mesh)
{
	indicesNum = mesh.indicesNum;
	if(mesh.is2D)
		CopyMesh(mesh);
	else
		CopyLine(mesh);
}

MeshConstructor::~MeshConstructor(void)
{
	if(ib)
		delete ib;
	for (unsigned int i = 0; i < vbs.size(); i++)
	{
		if(vbs[i])
			delete vbs[i];
	}
}

void MeshConstructor::InitLine(IndexedModel &model){
	
	int verticesNum = model.positions.size();
	indicesNum = model.indices.size();
	
	vao.Bind();

	for (int i = 0; i < 2; i++)
	{
		vbs.push_back(new VertexBuffer(model.GetData(i),verticesNum*sizeof(model.positions[0])));	
		vao.AddBuffer(*vbs.back(),i,3,GL_FLOAT);
	}
	
	ib = new IndexBuffer((unsigned int*)model.GetData(4),indicesNum);
	
	vao.Unbind();
	is2D = false;
}

void MeshConstructor::InitMesh(IndexedModel &model){

	CreateTree(model.positions);
	int verticesNum = model.positions.size();
	indicesNum = model.indices.size();
	
	vao.Bind();

	for (int i = 0; i < 3; i++)
	{
		vbs.push_back(new VertexBuffer(model.GetData(i),verticesNum*sizeof(model.positions[0])));	
		vao.AddBuffer(*vbs.back(),i,3,GL_FLOAT);
	}
	vbs.push_back(new VertexBuffer(model.GetData(3),verticesNum*sizeof(model.texCoords[0])));
	vao.AddBuffer(*vbs.back(),vbs.size()-1,2,GL_FLOAT);
	
	ib = new IndexBuffer((unsigned int*)model.GetData(4),indicesNum);
	
	vao.Unbind();
	is2D = true;
	
}

void MeshConstructor::CopyLine(const MeshConstructor &mesh){
	
	vao.Bind();

	for (int i = 0; i < 2; i++)
	{
		vbs.push_back(new VertexBuffer(*(mesh.vbs[i])));	
		vao.AddBuffer(*vbs.back(),i,3,GL_FLOAT);
	}
	
	ib = new IndexBuffer(*mesh.ib);
	
	vao.Unbind();

	is2D = false;
	
}

void MeshConstructor::CopyMesh(const MeshConstructor &mesh){

	vao.Bind();

	for (int i = 0; i < 4; i++)
	{
		vbs.push_back(new VertexBuffer(*(mesh.vbs[i])));	
		vao.AddBuffer(*vbs.back(),i,3,GL_FLOAT);
	}
	
	
	ib = new IndexBuffer(*mesh.ib);
	//ib = mesh.ib;
	vao.Unbind();

	is2D = true;
	
}

void MeshConstructor::CreateTree(std::vector<glm::vec3> positions)
{
	std::list<glm::vec4> points;
	for (int i = 0; i < positions.size(); i++)
		points.push_back(glm::vec4(positions.at(i), 1));
	kdtree.makeTree(points);

	kdtree.printTree(kdtree.getRoot());
	//std::cout << kdtree.getRoot()->data.x << " " << kdtree.getRoot()->data.y << " " << kdtree.getRoot()->data.z << " " << std::endl;

	glm::vec3 sum = glm::vec3(0);
	glm::vec3 max = glm::vec3(0);
	for (std::list<glm::vec4>::iterator it = points.begin(); it != points.end(); it++)
	{
		//std::cout << "(*it): " << (*it).x << " " << (*it).y << " " << (*it).z << " " << std::endl;
		sum.x += (*it).x;
		sum.y += (*it).y;
		sum.z += (*it).z;
		if ((*it).x > max.x)
			max.x = (*it).x;
		if ((*it).y > max.y)
			max.y = (*it).y;
		if ((*it).z > max.z)
			max.z = (*it).z;
	}
	glm::vec3 avg = (1.0f / positions.size()) * sum;
	glm::vec3 size = max - avg; //TODO: size problem could be here
	bvh.SetBoundingBox(avg, size);

	//bvh.SetLeft(CreateBVH(root->left, (level++) % 3, root, points));
	//bvh.SetRight(CreateBVH(root->right, (level++) % 3, root, points));

	//TODO: need to create the boxes in "game" and hide them 
}

void MeshConstructor::CreateBVH(Node node, BoundingBox daddy, bool is_left, int level)
{

}

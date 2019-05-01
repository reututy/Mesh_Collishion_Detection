#define GLEW_STATIC
#include <GL\glew.h>
#include <iostream>
#include "MeshConstructor.h"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "bezier2D.h"
#include "obj_loader.h"

static void printMat(const glm::mat4 mat)
{
	std::cout << " matrix:" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout << mat[j][i] << " ";
		std::cout << std::endl;
	}
}


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

	glm::vec3 sum = glm::vec3(0);
	glm::vec3 max = glm::vec3(0);
	for (std::list<glm::vec4>::iterator it = points.begin(); it != points.end(); it++)
	{
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
	glm::vec3 size = glm::abs(max - avg);

	bvh.SetBoundingBox(glm::vec3(0, 0, 0), avg, size);

	bvh.SetLeft(CreateBVH(bvh.GetBox(), kdtree.getRoot(), 0, true));
	bvh.SetRight(CreateBVH(bvh.GetBox(), kdtree.getRoot(), 0, false));
}

BVH* MeshConstructor::CreateBVH(BoundingBox* parent, Node* curr_node, int level, bool is_left)
{
	BVH* bvh = new BVH();
	glm::vec3 begin = parent->GetBegin();
	glm::vec3 center = parent->GetCenter();
	glm::vec3 size = parent->GetSize();
	int curr_cut = level % 3;
	int sign = is_left ? 1 : -1;
	
	//TODO: Need to fix level 4,5 and scale size ?

	begin[curr_cut] = parent->GetBegin()[curr_cut] + sign * parent->GetSize()[curr_cut];
	center[curr_cut] = ((parent->GetCenter()[curr_cut] + sign * parent->GetSize()[curr_cut]) + curr_node->data[curr_cut]) / 2.0f;
	size[curr_cut] = glm::abs((parent->GetCenter()[curr_cut] + sign * parent->GetSize()[curr_cut]) - center[curr_cut]);

	bvh->SetBoundingBox(begin, center, size);

	if (curr_node->left != nullptr)
		bvh->SetLeft(CreateBVH(bvh->GetBox(), curr_node->left, level + 1, true));
	if (curr_node->right != nullptr)
		bvh->SetRight(CreateBVH(bvh->GetBox(), curr_node->right, level + 1, false));
	return bvh;
}

BVH* MeshConstructor::GetBVH()
{
	return &bvh;
}

// Checks collision between two bvh using BB CheckCollision
BoundingBox* MeshConstructor::CollisionDetection(MeshConstructor* other, glm::mat4 this_trans, glm::mat4 this_rot,
																		 glm::mat4 other_trans, glm::mat4 other_rot)
{
	//First Checks if the big boxes collides:
	BVH* this_curr = &this->bvh;
	BVH* other_curr = other->GetBVH();

	/*
	std::cout << "---BEFORE:---" << std::endl;
	std::cout << "this_center: " << std::endl;
	std::cout << this_curr->GetBox()->GetCenter().x << " " << this_curr->GetBox()->GetCenter().y << " " << this_curr->GetBox()->GetCenter().z << " " << std::endl;
	std::cout << "other_curr: " << std::endl;
	std::cout << other_curr->GetBox()->GetCenter().x << " " << other_curr->GetBox()->GetCenter().y << " " << other_curr->GetBox()->GetCenter().z << " " << std::endl;

	std::cout << "this_trans: " << std::endl;
	printMat(this_trans);
	std::cout << "this_rot: " << std::endl;
	printMat(this_rot);
	std::cout << "other_trans: " << std::endl;
	printMat(other_trans);
	std::cout << "other_rot: " << std::endl;
	printMat(other_rot);
	*/
	this_curr->GetBox()->UpdateDynamicVectors(this_trans, this_rot);
	other_curr->GetBox()->UpdateDynamicVectors(other_trans, other_rot);
	/*
	std::cout <<"---AFTER:---" << std::endl;
	std::cout << "this_center: "<< std::endl;
	std::cout << this_curr->GetBox()->GetCenter().x << " " << this_curr->GetBox()->GetCenter().y << " " << this_curr->GetBox()->GetCenter().z << " " << std::endl;
	std::cout << "other_curr: " << std::endl;
	std::cout << other_curr->GetBox()->GetCenter().x << " " << other_curr->GetBox()->GetCenter().y << " " << other_curr->GetBox()->GetCenter().z << " " << std::endl;
	std::cout << "this_xInit: " << std::endl;
	std::cout << this_curr->GetBox()->GetxInit().x << " " << this_curr->GetBox()->GetxInit().y << " " << this_curr->GetBox()->GetxInit().z << " " << std::endl;
	std::cout << "other_xInit: " << std::endl;
	std::cout << other_curr->GetBox()->GetxInit().x << " " << other_curr->GetBox()->GetxInit().y << " " << other_curr->GetBox()->GetxInit().z << " " << std::endl;
	std::cout << "this_xInit: " << std::endl;
	std::cout << this_curr->GetBox()->GetxInit().x << " " << this_curr->GetBox()->GetxInit().y << " " << this_curr->GetBox()->GetxInit().z << " " << std::endl;
	std::cout << "other_xInit: " << std::endl;
	std::cout << other_curr->GetBox()->GetxInit().x << " " << other_curr->GetBox()->GetxInit().y << " " << other_curr->GetBox()->GetxInit().z << " " << std::endl;

	std::cout << "this_yInit: " << std::endl;
	std::cout << this_curr->GetBox()->GetyInit().x << " " << this_curr->GetBox()->GetyInit().y << " " << this_curr->GetBox()->GetyInit().z << " " << std::endl;
	std::cout << "other_yInit: " << std::endl;
	std::cout << other_curr->GetBox()->GetyInit().x << " " << other_curr->GetBox()->GetyInit().y << " " << other_curr->GetBox()->GetyInit().z << " " << std::endl;
	
	std::cout << "this_zInit: " << std::endl;
	std::cout << this_curr->GetBox()->GetzInit().x << " " << this_curr->GetBox()->GetzInit().y << " " << this_curr->GetBox()->GetzInit().z << " " << std::endl;
	std::cout << "other_zInit: " << std::endl;
	std::cout << other_curr->GetBox()->GetzInit().x << " " << other_curr->GetBox()->GetzInit().y << " " << other_curr->GetBox()->GetzInit().z << " " << std::endl;
	*/

	if (this_curr->GetBox()->CheckCollision(other_curr->GetBox()))
	{
		std::cout << "They collide! "<< std::endl;
		return this_curr->GetBox();
	}

	/*
	BVH* curr = &this->bvh;
	if (curr->GetBox()->CheckCollision(other->bvh.GetBox()))
	{

	}*/

	/*
	std::queue<BVH*> queue;
	BVH* curr = &this->bvh;
	queue.push(curr);

	while (!queue.empty())
	{
		curr = queue.front();
		queue.pop();
		other->bvh.GetBox()->UpdateDynamicVectors(other_rot, other_trans);
		this->bvh.GetBox()->UpdateDynamicVectors(this_rot, this_trans);

		if (curr->GetBox()->CheckCollision(other->bvh.GetBox()) && 
			(other->CollisionDetection(this, other_trans, other_rot, this_trans, this_rot) != nullptr))

		{
			if (curr->GetLeft() != nullptr && curr->GetRight() != nullptr)
			{
				queue.push(curr->GetLeft());
				queue.push(curr->GetRight());
			}
			else if (curr->GetLeft() != nullptr)
				queue.push(curr->GetLeft());
			else if (curr->GetRight() != nullptr)
				queue.push(curr->GetRight());
			else
				return curr->GetBox();
		}
	}*/
	return nullptr;
}


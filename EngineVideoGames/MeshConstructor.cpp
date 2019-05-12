#define GLEW_STATIC
#include <GL\glew.h>
#include <iostream>
#include "MeshConstructor.h"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "bezier2D.h"
#include "obj_loader.h"

#define MINIMUM_VERTCIES_FOR_BVH 700

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

void MeshConstructor::InitMesh(IndexedModel &model)
{
	CreateTree(model.positions);
	positions = model.positions;

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
	//kdtree.printTree(kdtree.getRoot());

	this->bvh = *CreateBVH(positions, kdtree.getRoot(), 0);
}

BVH* MeshConstructor::CreateBVH(std::vector<glm::vec3> points, Node* curr_node, int level)
{
	BVH* bvh = new BVH();
	int curr_cut = level % 3;
	glm::vec3 center = glm::vec3(0);
	glm::vec3 size = glm::vec3(0);
	glm::vec3 sum = glm::vec3(0);
	glm::vec3 max = glm::vec3(0);

	//Calculates the center of the box:
	for (int i = 0; i < points.size(); i++)
		sum += points[i];
	center = (1.0f / points.size()) * sum;
	//Calculates the size of the box:
	for (int i = 0; i < points.size(); i++)
		size = glm::max(size, glm::abs(points[i] - center));
	
	bvh->SetBoundingBox(center, size);
	bvh->GetBox()->SetNumOfPoints(points.size());

	std::vector<glm::vec3> new_points;
	if (level == 5)
	{
		int hjhjgkjgcjlh=1;
	}
	if (level == 4)
	{
		int hjhjgkjgcjlh = 1;
	}
	if (curr_node->left != nullptr && points.size() >= MINIMUM_VERTCIES_FOR_BVH)
	{
		for (int i = 0; i < points.size(); i++)
		{
			if (curr_node->data[curr_cut] < points[i][curr_cut])
			{
				new_points.push_back(points[i]);
			}
		}
		bvh->SetLeft(CreateBVH(new_points, curr_node->left, level + 1));
	}
	new_points.clear();
	if (curr_node->right != nullptr && points.size() >= MINIMUM_VERTCIES_FOR_BVH)
	{
		for (int i = 0; i < points.size(); i++)
		{
			if (curr_node->data[curr_cut] >= points[i][curr_cut])
			{
				new_points.push_back(points[i]);
			}
		}
		bvh->SetRight(CreateBVH(new_points, curr_node->right, level + 1));
	}
	return bvh;
}












/*
BVH* MeshConstructor::CreateBVH(BoundingBox* parent, Node* curr_node, int level, bool is_left, int num_of_points, 
								std::list<glm::vec4> &left, std::list<glm::vec4> &right)
{
	BVH* bvh = new BVH();
	glm::vec3 center = parent->GetFixedCenter();
	glm::vec3 size = glm::vec3(0);
	int curr_cut = level % 3;
	int sign = is_left ? -1 : 1;


	glm::vec3 sum = glm::vec3(0);
	glm::vec3 max = glm::vec3(0);
	*/

	/* A try: Half of the size - does not work*/
	/*
	std::list<glm::vec4> *points_to_check = is_left ? &left : &right;

	for (std::list<glm::vec4>::iterator it = points_to_check->begin(); it != points_to_check->end(); it++)
	{
		sum.x += (*it).x;
		sum.y += (*it).y;
		sum.z += (*it).z;
	}
	center = (1.0f / points_to_check->size()) * sum;
	size[curr_cut] = parent->GetSize()[curr_cut] / 2.0f;
	*/

	/* Works partly: */
	/*
	glm::vec3 min_point_x = parent->GetSize();
	glm::vec3 max_point_x = glm::vec3(0);
	glm::vec3 min_point_y = parent->GetSize();
	glm::vec3 max_point_y = glm::vec3(0);
	glm::vec3 min_point_z = parent->GetSize();
	glm::vec3 max_point_z = glm::vec3(0);
	std::list<glm::vec4> *points_to_check = is_left ? &left : &right;

	for (std::list<glm::vec4>::iterator it = points_to_check->begin(); it != points_to_check->end(); it++)
	{
		sum.x += (*it).x;
		sum.y += (*it).y;
		sum.z += (*it).z;
		//look for max points
		if ((*it).x > max_point_x.x)
			max_point_x.x = (*it).x;
		if ((*it).y > max_point_y.y)
			max_point_y.y = (*it).y;
		if ((*it).z > max_point_z.z)
			max_point_z.z = (*it).z;
		//look for min points
		if ((*it).x < min_point_x.x)
			min_point_x.x = (*it).x;
		if ((*it).y < min_point_y.y)
			min_point_y.y = (*it).y;
		if ((*it).z < min_point_z.z)
			min_point_z.z = (*it).z;
	}

	center = (1.0f / points_to_check->size()) * sum;
	size.x = glm::abs(max_point_x.x - min_point_x.x) / 2.0f;
	size.y = glm::abs(max_point_x.y - min_point_x.y) / 2.0f;
	size.z = glm::abs(max_point_x.z - min_point_x.z) / 2.0f;
	*/

	/* Almog - what should be:*/
	/*
	glm::vec3 sum = glm::vec3(0);
	glm::vec3 max = glm::vec3(0);

	std::list<glm::vec4> *points_to_check = is_left ? &left : &right;
	
	for (std::list<glm::vec4>::iterator it = points_to_check->begin(); it != points_to_check->end(); it++)
	{
		sum.x += (*it).x;
		sum.y += (*it).y;
		sum.z += (*it).z;
	}
	center = (1.0f / points_to_check->size()) * sum;

	for (std::list<glm::vec4>::iterator it = points_to_check->begin(); it != points_to_check->end(); it++)
	{
		size = glm::max(size, glm::abs(glm::vec3(*it) - center));
	}

	bvh->SetBoundingBox(begin, center, size);
	bvh->GetBox()->SetNumOfPoints(num_of_points);

	
	std::list<glm::vec4> left_list;
	std::list<glm::vec4> right_list;

	kdtree.findMedian(curr_cut, left, left_list, right_list);
	kdtree.findMedian(curr_cut, right, left_list, right_list);

	if (curr_node->left != nullptr && num_of_points >= MINIMUM_VERTCIES_FOR_BVH)
		bvh->SetLeft(CreateBVH(bvh->GetBox(), curr_node->left, level + 1, true, num_of_points / 2, left_list, right_list));
	if (curr_node->right != nullptr && num_of_points >= MINIMUM_VERTCIES_FOR_BVH)
		bvh->SetRight(CreateBVH(bvh->GetBox(), curr_node->right, level + 1, false, num_of_points / 2, left_list, right_list));
	return bvh;
}*/






BVH* MeshConstructor::GetBVH()
{
	return &bvh;
}

// Checks collision between two bvh using BB CheckCollision
BoundingBox* MeshConstructor::CollisionDetection(MeshConstructor* other, glm::mat4 this_trans, glm::mat4 this_rot,
																		 glm::mat4 other_trans, glm::mat4 other_rot)
{
	//First Checks if the big boxes collides:
	std::vector<std::pair<BVH*, BVH*>> queue;
	BVH* this_curr = &this->bvh;
	BVH* other_curr = other->GetBVH();

	/*
	this_curr->GetBox()->UpdateDynamicVectors(this_trans, this_rot);
	other_curr->GetBox()->UpdateDynamicVectors(other_trans, other_rot);
	
	if (this_curr->GetBox()->CheckCollision(other_curr->GetBox()))
	{
		std::cout << "They collide! "<< std::endl;
		return this_curr->GetBox();
	}
	*/

	queue.emplace_back(this_curr, other_curr);
	while (!queue.empty())
	{
		this_curr = queue.back().first;
		other_curr = queue.back().second;
		queue.pop_back();
		
		if (this_curr != nullptr && other_curr != nullptr)
		{
			this_curr->GetBox()->UpdateDynamicVectors(this_trans, this_rot);
			other_curr->GetBox()->UpdateDynamicVectors(other_trans, other_rot);
			if (this_curr->GetBox()->CheckCollision(other_curr->GetBox()))
			{
				if (this_curr->IsSmallestBox() && other_curr->IsSmallestBox())
				{
					std::cout << "this_level: " << this_curr->GetLevel() << std::endl;
					std::cout << "other_curr: " << other_curr->GetLevel() << std::endl;
					//if (this_curr->GetLevel() == 4)
					return this_curr->GetBox();
				}
				//Pushes children boxes into queue
				if (this_curr->IsSmallestBox() && other_curr != nullptr)
				{
					queue.emplace_back(this_curr, other_curr->GetLeft());
					queue.emplace_back(this_curr, other_curr->GetRight());
				}
				else if (other_curr->IsSmallestBox() && this_curr != nullptr)
				{
					queue.emplace_back(other_curr->GetLeft(), other_curr);
					queue.emplace_back(other_curr->GetRight(), other_curr);
				}
				else
				{
					queue.emplace_back(this_curr->GetLeft(), other_curr->GetLeft());
					queue.emplace_back(this_curr->GetLeft(), other_curr->GetRight());
					queue.emplace_back(this_curr->GetRight(), other_curr->GetLeft());
					queue.emplace_back(this_curr->GetRight(), other_curr->GetRight());
				}
			}
		}
	}
	return nullptr;
}

/* The 'working code': */
/*
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

	bvh.SetBoundingBox(avg, size);
	bvh.GetBox()->SetNumOfPoints(positions.size());

	bvh.SetLeft(CreateBVH(bvh.GetBox(), kdtree.getRoot(), 0, true, positions.size()));
	bvh.SetRight(CreateBVH(bvh.GetBox(), kdtree.getRoot(), 0, false, positions.size()));
}

BVH* MeshConstructor::CreateBVH(BoundingBox* parent, Node* curr_node, int level, bool is_left, int num_of_points)
{
	BVH* bvh = new BVH();
	glm::vec3 center = parent->GetFixedCenter();
	glm::vec3 size = parent->GetSize();
	int curr_cut = level % 3;
	int sign = is_left ? -1 : 1;

	//TODO: Need to fix level 4,5 and scale size ?

	center[curr_cut] = ((parent->GetFixedCenter()[curr_cut] + sign * parent->GetSize()[curr_cut]) + curr_node->data[curr_cut]) / 2.0f;
	//size[curr_cut] = glm::abs(parent->GetSize()[curr_cut] / 2.0f);
	if (!is_left)
		size[curr_cut] = glm::abs((parent->GetFixedCenter()[curr_cut] + parent->GetSize()[curr_cut]) - center[curr_cut]);
	else
		size[curr_cut] = glm::abs((parent->GetFixedCenter()[curr_cut] - center[curr_cut]) - (parent->GetFixedCenter()[curr_cut] - curr_node->data[curr_cut]));

	bvh->SetBoundingBox(center, size);
	bvh->GetBox()->SetNumOfPoints(num_of_points);

	if (curr_node->left != nullptr)
		bvh->SetLeft(CreateBVH(bvh->GetBox(), curr_node->left, level + 1, true, num_of_points / 2));
	if (curr_node->right != nullptr)
		bvh->SetRight(CreateBVH(bvh->GetBox(), curr_node->right, level + 1, false, num_of_points / 2));
	return bvh;
}
*/
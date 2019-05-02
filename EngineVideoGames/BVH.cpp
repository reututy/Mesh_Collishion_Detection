#include "BVH.h"
#include <iostream>

BVH::BVH()
{
	box = nullptr;
	left = nullptr;
	right = nullptr;
}

BVH::~BVH()
{
	/*
	if (box != NULL)
		delete box;
	if (left != NULL)
		delete box;
	if (right != NULL)
		delete box;
	*/
}

bool BVH::IsLeaf()
{
	bool is = left == nullptr && right == nullptr;
	return is;
}

bool BVH::IsSmallestBox()
{
	bool is = IsLeaf() && box->IsSmallestBox();
	return is;
}

void BVH::SetBoundingBox(glm::vec3 begin, glm::vec3 center, glm::vec3 size)
{
	if (box == nullptr)
		box = new BoundingBox(begin, center, size);
	else 
		box->SetBoundingBox(begin, center, size);
}

void BVH::SetLeft(BVH* bvh)
{
	left = bvh;
}

void BVH::SetRight(BVH* bvh)
{
	right = bvh;
}

void BVH::SetLevel(int value)
{
	level = value;
}

BoundingBox* BVH::GetBox()
{
	return box;
}

BVH* BVH::GetLeft()
{
	return left;
}

BVH* BVH::GetRight()
{
	return right;
}

int BVH::GetLevel()
{
	return level;
}

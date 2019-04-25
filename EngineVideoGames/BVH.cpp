#include "BVH.h"
#include <iostream>

BVH::BVH()
{
	box = NULL;
	left = NULL;
	right = NULL;
}

BVH::~BVH()
{
	if (box != NULL)
		delete box;
	if (left != NULL)
		delete box;
	if (right != NULL)
		delete box;
}

void BVH::SetBoundingBox(glm::vec3 center, glm::vec3 size)
{
	if (box == NULL)
		box = new BoundingBox();
	box->SetBoundingBox(center, size);
}

void BVH::SetLeft(BVH* bvh)
{
	left = bvh;
}

void BVH::SetRight(BVH* bvh)
{
	right = bvh;
}

BVH* BVH::GetLeft()
{
	return left;
}

BVH* BVH::GetRight()
{
	return right;
}

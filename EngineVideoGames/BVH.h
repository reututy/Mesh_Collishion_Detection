#pragma once
#include "Mesh.h"
#include "kdtree.h"

/* Bounding Volume Hierarchy */
class BVH
{
private:
	BoundingBox* box;
	BVH* left;
	BVH* right;

public:
	BVH();
	~BVH();

	void SetBoundingBox(glm::vec3 center, glm::vec3 size);
	void SetLeft(BVH* bvh);
	void SetRight(BVH* bvh);
	
	BoundingBox* GetBox();
	BVH* GetLeft();
	BVH* GetRight();
};


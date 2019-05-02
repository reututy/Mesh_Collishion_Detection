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
	int level;

public:
	BVH();
	~BVH();

	void SetBoundingBox(glm::vec3 begin, glm::vec3 center, glm::vec3 size);
	void SetLeft(BVH* bvh);
	void SetRight(BVH* bvh);
	void SetLevel(int value);
	
	BoundingBox* GetBox();
	BVH* GetLeft();
	BVH* GetRight();
	int GetLevel();
};


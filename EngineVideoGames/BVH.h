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

	bool IsLeaf();
	bool IsSmallestBox();

	void SetBoundingBox(glm::vec3 center, glm::vec3 size);
	void SetLeft(BVH* bvh);
	void SetRight(BVH* bvh);
	void SetLevel(int value);
	
	BoundingBox* GetBox();
	BVH* GetLeft();
	BVH* GetRight();
	int GetLevel();
};
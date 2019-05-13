#pragma once
#include <vector>
#include "glm\glm.hpp"
#include "VertexArray.hpp"
#include "Mesh.h"
#include "bezier1D.h"
#include "kdtree.h"
#include "BVH.h"


class MeshConstructor
{
private:
	VertexArray vao;
	IndexBuffer *ib;

	Kdtree kdtree;
	BVH bvh;
	int type;
	
	std::vector<VertexBuffer*> vbs;
	bool is2D;
	int unsigned indicesNum;

	void InitMesh(IndexedModel &model);
	void CopyMesh(const MeshConstructor &mesh);
	void CopyLine(const MeshConstructor &mesh);

public:
	enum SimpleShapes
	{
		Axis,
		Cube,
		Octahedron,
		Tethrahedron,
		BezierLine,
		BezierSurface,
	};
	enum Axis{X, Y, Z};
	MeshConstructor(const int type);
	MeshConstructor(Bezier1D *curve, bool isSurface, unsigned int resT, unsigned int resS);
	MeshConstructor(const MeshConstructor &mesh);
	MeshConstructor(const std::string& fileName);
	~MeshConstructor(void);

	void Bind() { vao.Bind(); }
	void Unbind() { vao.Unbind(); }
	inline unsigned int GetIndicesNum() { return indicesNum; }
	inline bool Is2D() { return is2D; }
	void InitLine(IndexedModel &model);

	void CreateTree(std::vector<glm::vec3> positions);
	BVH* CreateBVH(std::vector<glm::vec3> points, Node* curr_node, int level);
	BoundingBox* MeshConstructor::CollisionDetection(MeshConstructor* other, glm::mat4 this_trans, glm::mat4 this_rot,
																			 glm::mat4 other_trans, glm::mat4 other_rot);

	BVH* GetBVH();
	int GeyType();
};

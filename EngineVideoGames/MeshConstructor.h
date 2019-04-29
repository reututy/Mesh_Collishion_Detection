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

	//TO DO: add bounding box data base and build it in the constructor:
	Kdtree kdtree;
	BVH bvh;
	
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
	BVH* CreateBVH(BoundingBox parent, Node curr_node, int level, bool is_left);
	//TO DO: add collision detection function which get other MeshConstructor and Mat4 of related transformasions. The function may return a pointer to the relevant Bounding Box when collide:
	BoundingBox* CollisionDetection(BVH* other, glm::mat4 orientation /*MeshConstructor and Mat4 ?*/);

	BVH* GetBVH();
};

#pragma once
#include "scene.h"
#include "bezier1D.h"

class Game : public Scene
{
	Bezier1D *curve;
	Bezier1D *curve_two;

public:
	Game(void);
	Game(glm::vec3 position, float angle, float hwRelation, float near, float far);
	~Game(void);
	void Init();
	int CreateCurveControlPoints(int counter, Bezier1D *curve);
	void addShape(int type, int parent, unsigned int mode);
	//void Update( glm::mat4 MVP ,glm::mat4 *jointTransforms,const int length,const int  shaderIndx);
	void Update(const glm::mat4 &MVP, const glm::mat4 &Normal, Shader *s);
	//void ControlPointUpdate();
	void WhenRotate();
	void WhenTranslate();
	void WhenTranslateForCurve(Bezier1D *curve, int min, int max, int num_of_shape);
	void Motion();
	void MoveControlCubes();
	void MoveControlCubesForCurve(Bezier1D *curve, int min, int max);
	void MoveControlCubesByVec(Bezier1D *curve, int min, int max, glm::vec3 curr_pos);
};


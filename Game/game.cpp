#include "game.h"
#include <iostream>

#define CONTROL_POINT_SCALE 0.1
#define PURGATORY -5000000
#define BB_SCALE 4.f

bool once = false;
int num_of_curve = 1;
int num_of_curve_two = 0;

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

Game::Game():Scene()
{
	curve = NULL;
	curve_two = NULL;
}

Game::Game(glm::vec3 position, float angle, float hwRelation, float near, float far) : Scene(position, angle, hwRelation, near, far)
{
	std::vector<glm::mat4> ctrlPointsVec_one;
	std::vector<glm::mat4> ctrlPointsVec_two;

	ctrlPointsVec_one.push_back(glm::mat4(glm::vec4(0.0, 0.0, 0.0, 1.0), glm::vec4(0.0, 2.0, 0.0, 1.0),
		glm::vec4(2.0, 2.0, 0.0, 1.0), glm::vec4(2.0, 0.0, 0.0, 1.0)));

	ctrlPointsVec_one.push_back(glm::mat4(glm::vec4(2.0, 0.0, 0.0, 1.0), glm::vec4(2.0, -2.0, 0.0, 1.0),
		glm::vec4(4.0, -2.0, 0.0, 1.0), glm::vec4(4.0, 0.0, 0.0, 1.0)));

	ctrlPointsVec_one.push_back(glm::mat4(glm::vec4(4.0, 0.0, 0.0, 1.0), glm::vec4(4.0, 2.0, 0.0, 1.0),
		glm::vec4(6.0, 2.0, 0.0, 1.0), glm::vec4(6.0, 0.0, 0.0, 1.0)));
	/*
	ctrlPointsVec_one.push_back(glm::mat4(glm::vec4(10.0, 0.0, 0.0, 1.0), glm::vec4(10.0, 2.0, 0.0, 1.0),
		glm::vec4(12.0, 2.0, 0.0, 1.0), glm::vec4(12.0, 0.0, 0.0, 1.0)));

	ctrlPointsVec_one.push_back(glm::mat4(glm::vec4(12.0, 0.0, 0.0, 1.0), glm::vec4(12.0, -2.0, 0.0, 1.0),
		glm::vec4(14.0, -2.0, 0.0, 1.0), glm::vec4(14.0, 0.0, 0.0, 1.0)));

	ctrlPointsVec_one.push_back(glm::mat4(glm::vec4(14.0, 0.0, 0.0, 1.0), glm::vec4(14.0, 2.0, 0.0, 1.0),
		glm::vec4(16.0, 2.0, 0.0, 1.0), glm::vec4(16.0, 0.0, 0.0, 1.0)));
		*/
	curve = new Bezier1D(ctrlPointsVec_one);
	MIN_CTRL = 3;
	MAX_CTRL = 3;
	
	ctrlPointsVec_two.push_back(glm::mat4(glm::vec4(0.0, 0.0, 0.0, 1.0), glm::vec4(0.0, 2.0, 0.0, 1.0),
		glm::vec4(-2.0, 2.0, 0.0, 1.0), glm::vec4(-2.0, 0.0, 0.0, 1.0)));

	ctrlPointsVec_two.push_back(glm::mat4(glm::vec4(-2.0, 0.0, 0.0, 1.0), glm::vec4(-2.0, -2.0, 0.0, 1.0),
		glm::vec4(-4.0, -2.0, 0.0, 1.0), glm::vec4(-4.0, 0.0, 0.0, 1.0)));

	ctrlPointsVec_two.push_back(glm::mat4(glm::vec4(-4.0, 0.0, 0.0, 1.0), glm::vec4(-4.0, 2.0, 0.0, 1.0),
		glm::vec4(-6.0, 2.0, 0.0, 1.0), glm::vec4(-6.0, 0.0, 0.0, 1.0)));

	curve_two = new Bezier1D(ctrlPointsVec_two);
	MIN_CTRL_TWO = 3;
	MAX_CTRL_TWO = 3;
}

Game::~Game(void)
{
	if(curve != NULL)
		delete curve;
	if (curve_two != NULL)
		delete curve_two;
}

void Game::addShape(int type, int parent, unsigned int mode)
{
	chainParents.push_back(parent);
	if (type != BezierLine && type != BezierSurface)
		shapes.push_back(new Shape(type, mode));
	else
	{
		if (pickedShape == 1)
		{
			if (type == BezierLine)
				shapes.push_back(new Shape(curve, 30, 30, false, mode));
			else
				shapes.push_back(new Shape(curve, 30, 30, true, mode));
		}
		else if (pickedShape == 17)
		{
			if (type == BezierLine)
				shapes.push_back(new Shape(curve_two, 30, 30, false, mode));
			else
				shapes.push_back(new Shape(curve_two, 30, 30, true, mode));
		}
	}
}

void Game::CreateBoundingBoxes(BVH* bvh, int level)
{
	addShapeCopy(2, -1, LINE_LOOP);
	pickedShape = shapes.size() - 1;

	shapeTransformation(xScale, bvh->GetBox()->GetSize().x);
	shapeTransformation(yScale, bvh->GetBox()->GetSize().y);
	shapeTransformation(zScale, bvh->GetBox()->GetSize().z);

	shapeTransformation(xGlobalTranslate, bvh->GetBox()->GetBegin().x); 
	shapeTransformation(yGlobalTranslate, bvh->GetBox()->GetBegin().y);
	shapeTransformation(zGlobalTranslate, bvh->GetBox()->GetBegin().z);

	//TODO: need to hide all the shapes
	shapes[pickedShape]->Hide();
	//chainParents[pickedShape] = parent;

	//TODO: Need to fix level 4,5
	if (level == 2)
		shapes[pickedShape]->Unhide();
	if (bvh->GetLeft() != nullptr)
		CreateBoundingBoxes(bvh->GetLeft(), level + 1);
	if (bvh->GetRight() != nullptr)
		CreateBoundingBoxes(bvh->GetRight(), level + 1);
}

void Game::Init()
{	
	/*
	//Add Axis
	addShape(Axis, -1, LINES);
	//Add curve
	pickedShape = 1;
	addShape(BezierLine, -1, LINE_STRIP);
	
	//Translate all scene away from camera
	myTranslate(glm::vec3(0, 0, -20), 0);

	//Axis scale:
	pickedShape = 0;
	shapeTransformation(yScale, 20);
	shapeTransformation(xScale, 20);
	shapeTransformation(zScale, 20);

	//Add Cube for copying
	addShape(Cube, -1, TRIANGLES);
	pickedShape = 2;
	HideShape(pickedShape);

	num_of_shapes = MIN_CTRL;
	num_of_shapes = CreateCurveControlPoints(num_of_shapes, curve);
	MAX_CTRL = num_of_shapes;

	//For not making a mess with the numbers
	addShape(Cube, -1, TRIANGLES);
	pickedShape = num_of_shapes++;
	HideShape(pickedShape);
	addShape(Cube, -1, TRIANGLES);
	pickedShape = num_of_shapes++;
	HideShape(pickedShape);
	
	pickedShape = 17;
	addShape(BezierLine, -1, LINE_STRIP);
	num_of_curve_two = num_of_shapes++;
	MIN_CTRL_TWO = num_of_shapes;
	shapes[num_of_curve_two]->GetMesh()->InitLine(curve_two->GetLine(30));

	num_of_shapes = CreateCurveControlPoints(num_of_shapes, curve_two);

	MAX_CTRL_TWO = num_of_shapes;
	pickedShape = -1;
	*/

	addShape(Axis,-1,LINES); //0 Axis
	addShape(Octahedron,-1,TRIANGLES); //1 Octahedron
	//addShapeCopy(1,-1,TRIANGLES); //2 Octahedron
	addShape(Cube,1,LINE_LOOP);  //3 Cube belong to 1 --- 2
	//addShapeCopy(3,2,LINE_LOOP); //4 Cube belong to 2

	//translate all scene away from camera
	myTranslate(glm::vec3(0,0,-20),0);

	//Scale the Axis:
	pickedShape = 0;
	shapeTransformation(yScale, 10);
	shapeTransformation(xScale, 10);
	shapeTransformation(zScale, 10);

	ReadPixel();

	//pickedShape = 2;
	//shapeTransformation(zLocalRotate, 45);
	//shapeTransformation(xGlobalTranslate, 10);

	//Scale the first Octahedron
	pickedShape = 1;
	//shapeTransformation(xGlobalTranslate, -10);
	//shapeTransformation(yScale, BB_SCALE);
	//shapeTransformation(xScale, BB_SCALE);
	//shapeTransformation(zScale, BB_SCALE);

	//Scale the first box of the Octahedron
	pickedShape = 2;
	//shapeTransformation(yScale, BB_SCALE);
	//shapeTransformation(xScale, BB_SCALE);
	//shapeTransformation(zScale, BB_SCALE);
	shapes[2]->Hide();
	
	/*my code:*/
	for (int i = 0; i < shapes.size(); i++)
	{
		if (shapes[i]->GetMode() == TRIANGLES)
		{
			CreateBoundingBoxes(shapes[i]->GetMesh()->GetBVH(), 0);
		}
	}
	
	/*
	addShapeCopy(3, -1, LINE_LOOP); //5 Cube belong to 2
	pickedShape = 5;
	shapeTransformation(xGlobalTranslate, 2);
	//shapeTransformation(xGlobalTranslate, kd.getRoot()->left->data.x);
	shapeTransformation(zScale, BB_SCALE);
	shapeTransformation(yScale, BB_SCALE);
	shapeTransformation(xScale, BB_SCALE / 2);
	
	addShapeCopy(3, -1, LINE_LOOP); //6 Cube belong to 2
	pickedShape = 6;
	shapeTransformation(xGlobalTranslate, -2);
	//shapeTransformation(xGlobalTranslate, kd.getRoot()->right->data.x*2);
	shapeTransformation(zScale, BB_SCALE);
	shapeTransformation(yScale, BB_SCALE);
	shapeTransformation(xScale, BB_SCALE / 2);
	*/

	pickedShape = -1;
	//Activate();

}

int Game::CreateCurveControlPoints(int counter, Bezier1D *curve)
{
	glm::vec3 control_point;
	int modulu = 0;
	for (int k = 0; k < curve->GetNumSegs(); k++)
	{
		modulu = 0;
		for (int i = 0; i < 4; i++)
		{
			addShapeCopy(2, -1, TRIANGLES);
			pickedShape = counter++;
			control_point = *(curve->GetControlPoint(k, i)).GetPos();
			//scaling the cube
			shapeTransformation(xScale, CONTROL_POINT_SCALE);
			shapeTransformation(yScale, CONTROL_POINT_SCALE);
			shapeTransformation(zScale, CONTROL_POINT_SCALE);
			//move the cube to the control point
			if (modulu++ != 3 || (i == 3 && k == curve->GetNumSegs() - 1))
			{
				shapeTransformation(xGlobalTranslate, control_point.x / CONTROL_POINT_SCALE);
				shapeTransformation(yGlobalTranslate, control_point.y / CONTROL_POINT_SCALE);
				shapeTransformation(zGlobalTranslate, control_point.z / CONTROL_POINT_SCALE);
			}
			else
			{
				HideShape(pickedShape);
			}
		}
	}
	return counter;
}

void Game::Update(const glm::mat4 &MVP, const glm::mat4 &Normal, Shader *s)
{
	int prev_shape = pickedShape;
	if (!once) 
		MoveControlCubes();

	int r = ((pickedShape + 1) & 0x000000FF) >> 0;
	int g = ((pickedShape + 1) & 0x0000FF00) >> 8;
	int b = ((pickedShape + 1) & 0x00FF0000) >> 16;
	s->Bind();
	s->SetUniformMat4f("MVP", MVP);
	s->SetUniformMat4f("Normal", Normal);
	s->SetUniform4f("lightDirection", 0.0f, 0.0f, -1.0f, 1.0f);
	s->SetUniform4f("lightColor", r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
}

void Game::WhenRotate()
{
	if (pickedShape >= 0)
		GetShapeTransformation();
}

void Game::WhenTranslate()
{
	WhenTranslateForCurve(curve, MIN_CTRL, MAX_CTRL, num_of_curve);
	WhenTranslateForCurve(curve_two, MIN_CTRL_TWO, MAX_CTRL_TWO, num_of_curve_two);
}

void Game::WhenTranslateForCurve(Bezier1D *curve, int min, int max, int num_of_shape)
{
	bool no_preservation = !(pickedShape <= min + 1 || pickedShape >= max - 2);
	if (pickedShape >= min && pickedShape < max) //Make sure that it only happens in the case of the cubes
	{
		glm::vec4 trans_vec = GetShapeTransformation()*glm::vec4(0, 0, 0, 1);
		//move the relevent control point
		curve->MoveControlPoint((pickedShape - min), (pickedShape - min), no_preservation, trans_vec);
		//move the relevent cube incident to that specific control point
		shapes[num_of_shape]->GetMesh()->InitLine(curve->GetLine(30));
	}
}

void Game::Motion()
{
	if (isActive)
	{
		int p = pickedShape;
		pickedShape = 2;
		shapeTransformation(zLocalRotate, 0.45);
		pickedShape = p;
	}
}

void Game::MoveControlCubes()
{
	MoveControlCubesForCurve(curve, MIN_CTRL, MAX_CTRL);
	MoveControlCubesForCurve(curve_two, MIN_CTRL_TWO, MAX_CTRL_TWO);
}

void Game::MoveControlCubesForCurve(Bezier1D *curve, int min, int max)
{
	int old_picked_shape = pickedShape;
	glm::vec3 control_point;

	for (int i = min; i < max; i++)
	{
		if (!((i - 2) % 4) == 0 && i != max - 1)
		{
			pickedShape = i;
			glm::vec4 curr_pos = GetShapeTransformation()*glm::vec4(0, 0, 0, 1);
			control_point = *(curve->GetControlPoint((i - min) / 4, (i - min) % 4)).GetPos();
			control_point = (control_point)-glm::vec3(curr_pos);
			shapeTransformation(xGlobalTranslate, control_point.x);
			shapeTransformation(yGlobalTranslate, control_point.y);
			shapeTransformation(zGlobalTranslate, control_point.z);
		}
	}
	pickedShape = old_picked_shape;
}

Bezier1D* Game::GetCurve()
{
	return curve;
}

Bezier1D* Game::GetCurveTwo()
{
	return curve_two;
}

int Game::GetNumOfShapes()
{
	return num_of_shapes;
}

void Game::SetNumOfShapes(int value)
{
	num_of_shapes = value;
}

int Game::GetMINCTRL()
{
	return MIN_CTRL;
}

int Game::GetMAXCTRL()
{
	return MAX_CTRL;
}

int Game::GetMINCTRLTWO()
{
	return MIN_CTRL_TWO;
}

int Game::GetMAXCTRLTWO()
{
	return MAX_CTRL_TWO;
}
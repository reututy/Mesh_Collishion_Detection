#pragma once   //maybe should be static class
#include "display.h"
#include "game.h"
#include <iostream>
#include <math.h>

#define NO_OF_MODES 8

int curr_mode = 7;
char modes_names[8][20] = {"POINTS", "LINES" , "LINE_STRIP", "LINE_LOOP", "TRIANGLES", "TRIANGLE_STRIP", "TRIANGLE_FAN", "QUADS" };


void mouse_callback(GLFWwindow* window,int button, int action, int mods)
{	
	if(action == GLFW_PRESS )
	{
		Game *scn = (Game*)glfwGetWindowUserPointer(window);
		double x2,y2;
		glfwGetCursorPos(window,&x2,&y2);
		scn->picking((int)x2,(int)y2);
	}
}
	
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Game *scn = (Game*)glfwGetWindowUserPointer(window);
	scn->shapeTransformation(scn->zCameraTranslate,yoffset);
}
	
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Game *scn = (Game*)glfwGetWindowUserPointer(window);
	glm::vec3 pos = *(scn->GetCurve()->GetVertex(0, 0).GetPos());
	glm::vec3 axis;
	float right = pos.x;
	float left = pos.x;
	float up = pos.y;
	float down = pos.y;
	float near = pos.y;
	float far = pos.y;
	float t = 0.0;
	int resT = scn->GetCurve()->GetResolution();
	float t_inc = (float)1 / (resT - 1);
	float alfa_rad;
	float alfa_deg;
	float axis_dist;
	float box_dist;
	float dot;
	float kefel;
	int sign;

	if(action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
			case GLFW_KEY_ESCAPE:			
				glfwSetWindowShouldClose(window,GLFW_TRUE);
			break;
			case GLFW_KEY_SPACE:
				//create 3d of curve
				scn->SetPickedShape(1);
				scn->addShape(5, -1, curr_mode);
				for (int i = scn->GetMINCTRL() - 2; i < scn->GetMAXCTRL(); i++)
				{
					if(i != 2 && i != 6 && i !=  10)
						scn->HideShape(i);
				}
				//create 3d of curve_two
				scn->SetPickedShape(17);
				scn->addShape(5, -1, curr_mode);
				scn->SetNumOfShapes(scn->GetNumOfShapes() + 1);
				for (int i = 17; i < 30; i++)
				{
					if (i != 21 && i != 25)
						scn->HideShape(i);
				}

				//Cube for curve
				scn->addShape(Scene::Shapes::Cube, -1, Scene::modes::LINE_LOOP);
				scn->SetNumOfShapes(scn->GetNumOfShapes() + 1);
				
				//create surrounding 3d of curve
				for (int seg = 0; seg < scn->GetCurve()->GetNumSegs(); seg++)
				{
					t = 0.0;
					for (int index = 0; index < resT; index++)
					{
						pos = *(scn->GetCurve()->GetVertex(seg, t).GetPos());
						if (pos.x > right)
						{
							right = pos.x;
						}
						if (pos.x < left)
						{
							left = pos.x;
						}
						if (pos.y > up)
						{
							up = pos.y;
						}
						if (pos.y < down)
						{
							down = pos.y;
						}
						if (pos.y > far)
						{
							far = pos.y;
						}
						if (pos.y < near)
						{
							near = pos.y;
						}
						t += t_inc;
					}
				}
				std::cout << "up: " << up << std::endl;
				std::cout << "down: " << down << std::endl;

				//Translate the box to where the shape is:
				scn->SetPickedShape(scn->GetNumOfShapes());
				scn->shapeTransformation(Scene::transformations::xGlobalTranslate, ((right - left) / 2) + left);
				scn->shapeTransformation(Scene::transformations::yGlobalTranslate, ((up - down) / 2) + down);
				/*
				//Rotate the box to where the shape is:
				axis = scn->GetCurve()->GetAxis();
				axis_dist = glm::length(glm::vec4(axis, 0));
				box_dist = glm::length(glm::vec4(1, 0, 0, 0));
				dot = glm::dot(glm::vec4(1, 0, 0, 0), glm::vec4(axis, 0));
				kefel = axis_dist * box_dist;
				alfa_rad = glm::acos((float) (dot / kefel));
				alfa_deg = alfa_rad * 180 / M_PI;
				sign = (axis.x > 0 && axis.y > 0) || ((axis.x < 0 && axis.y < 0)) ? 1 : -1;
				scn->shapeTransformation(Scene::transformations::zLocalRotate, sign*alfa_deg);
				
				std::cout << "scn->GetCurve()->GetAxis(): " << scn->GetCurve()->GetAxis().x << " " << scn->GetCurve()->GetAxis().y << " " << scn->GetCurve()->GetAxis().y << " " << std::endl;
				/*std::cout << "axis_dist: " << axis_dist << std::endl;
				std::cout << "box_dist: " << box_dist << std::endl;
				std::cout << "dot: " << dot << std::endl;
				std::cout << "kefel: " << kefel << std::endl;
				std::cout << "alfa_deg: " << alfa_deg << std::endl;
				std::cout << "alfa_rad: " << alfa_rad << std::endl;*/

				scn->setParent(32, 30);
				scn->SetPickedShape(scn->GetNumOfShapes());
				scn->shapeTransformation(Scene::transformations::xScale, (right - left) / 2);
				scn->shapeTransformation(Scene::transformations::yScale, (up - down) / 2);
				scn->shapeTransformation(Scene::transformations::zScale, (near - far) / 2);

				//scn->ReadPixel();
				//scn->SetPickedShape(-1);
				//scn->Activate();
				
				/*
				scn->HideShape(0);
				if(scn->IsActive())
					scn->Deactivate();
				else
					scn->Deactivate();
					*/
				break;
				case GLFW_KEY_RIGHT:
				scn->shapeTransformation(scn->xGlobalTranslate, 0.1f);
				break;
			case GLFW_KEY_LEFT:
				scn->shapeTransformation(scn->xGlobalTranslate, -0.1f);
				break;
			case GLFW_KEY_UP:
				scn->shapeTransformation(scn->yGlobalTranslate, 0.1f);
				break;
			case GLFW_KEY_DOWN:
				scn->shapeTransformation(scn->yGlobalTranslate, -0.1f);
				break;
			case GLFW_KEY_M:
				curr_mode = (curr_mode + 1) % NO_OF_MODES;
				std::cout << "Current mode is: " << modes_names[curr_mode] << std::endl;
				scn->change_shape_mode(1, curr_mode);
				scn->change_shape_mode(17, curr_mode);
				/*
				curr_mode = (curr_mode + 1) % NO_OF_MODES;
				std::cout << "Current mode is: " << modes_names[curr_mode] << std::endl;
				scn->change_shape_mode(1, curr_mode);
				scn->change_shape_mode(17, curr_mode);
				*/
				break;
			case GLFW_KEY_D:
				scn->shapeTransformation(scn->zLocalRotate, -5.f);
				break;
			case GLFW_KEY_A:
				scn->shapeTransformation(scn->zLocalRotate, 5.f);
				break;
			case GLFW_KEY_W:
				scn->shapeTransformation(scn->xGlobalRotate, -5.f);
				break;
			case GLFW_KEY_S:
				scn->shapeTransformation(scn->xLocalRotate, 5.f);
				break;
			default:
				break;
		}
	}
}
	

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Game *scn = (Game*)glfwGetWindowUserPointer(window);

	scn->updatePosition((float)xpos,(float)ypos);
	if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		scn->mouseProccessing(GLFW_MOUSE_BUTTON_RIGHT);
	}
	else if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		scn->mouseProccessing(GLFW_MOUSE_BUTTON_LEFT);
	}

}

void window_size_callback(GLFWwindow* window, int width, int height)
{
	Game *scn = (Game*)glfwGetWindowUserPointer(window);

	scn->resize(width,height);
	//relation = (float)width/(float)height;
}

void init(Display &display)
{
	display.addKeyCallBack(key_callback);
	display.addMouseCallBacks(mouse_callback,scroll_callback,cursor_position_callback);
	display.addResizeCallBack(window_size_callback);
}

#pragma once
#define _USE_MATH_DEFINES
#include "Field.h"
#include "Circle.h"
#include "WorldDrawer2d.h"
#include <cmath> 

float Field::LINE_WIDTH = 0.4f,  
	  Field::HEIGHT		= 3.0f;
Point2d Field::LEFTLCORNER(-13.0f-LINE_WIDTH, -16.0f);
Point2d Field::LEFTUCORNER(-13.0f-LINE_WIDTH, 16.0f);
Point2d Field::RIGHTLCORNER(13.0f, -16.0f); 
Point2d Field::RIGHTUCORNER(13.0f, 16.0f);
Point2d	Field::UP_MIDDLE_LEFT, Field::UP_MIDDLE_RIGHT,
	    Field::BOTTOM_MIDDLE_LEFT, Field::BOTTOM_MIDDLE_RIGHT;


Field::Field(CoordinateSystem2d* system) : m_system(system) {
	// Setting a default color for the line
	SetObjectColor(RGBcolor(1.0f, 1.0f, 1.0f));
	// Draw a simple square
	points_line.push_back(Point2d(1,0)); 
	points_line.push_back(Point2d(1,1));
	points_line.push_back(Point2d(0,1));
	points_line.push_back(Point2d(0,0));
	// Give it's topology
	topology_line.push_back(0);
	topology_line.push_back(1); 
	topology_line.push_back(2); 
	topology_line.push_back(2); 
	topology_line.push_back(3);
	topology_line.push_back(0);
	
}

Field::~Field(){
}

/* Sets the default color which is used to
 * draw a new line */ 
void Field::SetObjectColor(RGBcolor color){
	m_color.SetColor(color);
}


/* Draws a line from start to end. One of the axis must be constant
 * and the other must be drawn in the up direction if x is constant 
 * or in the right direction if y is constant */ 
void Field::DrawLine(Point2d start, Point2d end){
	Object2d *object;
	float width = LINE_WIDTH; // width of the line
	object = new Object2d(points_line, topology_line);
	m_objects.push_back(object);
	object->setcolor(m_color.GetRed(), m_color.GetGreen(), m_color.GetBlue());
	// We add the object to a vector of objects 
	m_system->objectAdd(object);
	object->translate(start.x, start.y);
	if (start.x - end.x == 0){
		// The drawn line is vertical
		object->scaleRelativeToPoint(start, width, fabs(start.y - end.y));
	}
	else if (start.y - end.y == 0){
		// The drawn line is horizontal
		object->scaleRelativeToPoint(start, fabs(start.x - end.x), width);
	}
}

/* Draws a line with a specified angle */
void Field::DrawRotatedLine(Point2d start, Point2d end, float angle){
	DrawLine(start, end);
	Object2d *object = m_objects.back();
	object->rotateSelf(angle);
}

/* Draws lines for the score. The objects of this lines
 * are kept for later to be removed when the game resets */ 
void Field::DrawScoreLine(Point2d start, Point2d end){
	DrawLine(start, end); 
	score_lines.push_back(m_objects.back());
}

/* Removes the lines which are used to draw the score */ 
void Field::RemoveScoreLines(){
	std::vector<Object2d*>::iterator it; 
	for (it = score_lines.begin(); it != score_lines.end(); ++it){
		m_system->objectRemove(*it);
	}
	score_lines.clear();
}

/* Draws the contour of the tribunes using only lines */ 
void Field::DrawTribunes(){
	SetObjectColor(RGBcolor(0.4f, 0.4f, 1.0f));
	DrawRotatedLine(Point2d(-21.0f, 15.5f), Point2d(-14.0f, 15.5f), -(float)M_PI/7.0f);
	DrawLine(Point2d(-21.0f, 0.2f), Point2d(-21.0f, 17.3f));
	DrawRotatedLine(Point2d(-21.0f, 1.6f), Point2d(-14.0f, 1.6f), (float)M_PI/7.0f);
	DrawLine(Point2d(-14.7f, 3.2f), Point2d(-14.7f, 14.3f));

	DrawRotatedLine(Point2d(-21.0f, -2.0f), Point2d(-14.0f, -2), - (float)M_PI/7.0f);
	DrawLine( Point2d(-21.0f, -17.3f), Point2d(-21.0f, -0.2f));
	DrawRotatedLine(Point2d(-21.0f, -15.8f), Point2d(-14.0f, -15.8f), (float)M_PI/7.0f); 
	DrawLine(Point2d(-14.7f, -14.5f), Point2d(-14.7f, -3.0f));
}

/* Draws the field */ 
void Field::DrawField(){
	Object2d* field;
	Circle *circle;
	RGBcolor red(0.93f, 0.19f, 0.19f);
	// The drawing of the margins
	SetObjectColor(RGBcolor(1.0f, 1.0f, 1.0f));
	// Lateral
	SetObjectColor(red);
	DrawLine(LEFTLCORNER, LEFTUCORNER);
	DrawLine(RIGHTLCORNER, RIGHTUCORNER);
	// Middle
	SetObjectColor(RGBcolor(0.61f, 0.97f, 0.56f));
	Point2d middle_left(LEFTLCORNER.x, 
						fabs(LEFTLCORNER.y-LEFTUCORNER.y + LINE_WIDTH)/2 + LEFTLCORNER.y);
	Point2d middle_right(RIGHTLCORNER.x,
						fabs(RIGHTLCORNER.y-RIGHTUCORNER.y + LINE_WIDTH)/2 + RIGHTLCORNER.y);
	DrawLine(middle_left, middle_right);
	SetObjectColor(red);
	

	// Uppers
	float up_middle_left = LEFTUCORNER.x + (RIGHTUCORNER.x - LEFTUCORNER.x)/3, 
		  up_middle_right = LEFTUCORNER.x + 2*(RIGHTUCORNER.x- LEFTUCORNER.x)/3;
	// The circles from the goal
	circle = new Circle(10, 1.0f); 
	circle->SetColor(red);
	UP_MIDDLE_LEFT.set(up_middle_left, RIGHTUCORNER.y + LINE_WIDTH/2);
	UP_MIDDLE_RIGHT.set(up_middle_right, RIGHTUCORNER.y + LINE_WIDTH/2);
	circle->DrawCircle(m_system, UP_MIDDLE_LEFT);
	circle->DrawCircle(m_system, UP_MIDDLE_RIGHT);

	// The lines
	SetObjectColor(RGBcolor(0.61f, 0.97f, 0.56f));
	DrawLine(Point2d(up_middle_left, RIGHTUCORNER.y),
			 Point2d(up_middle_right, RIGHTUCORNER.y));

	SetObjectColor(red);
	DrawLine(LEFTUCORNER, Point2d(RIGHTUCORNER.x+LINE_WIDTH,RIGHTUCORNER.y));
	// The goal
	DrawLine(Point2d(up_middle_left-LINE_WIDTH/2, LEFTUCORNER.y),
			 Point2d(up_middle_left-LINE_WIDTH/2, LEFTUCORNER.y+HEIGHT));
	DrawLine(Point2d(up_middle_left-LINE_WIDTH/2, LEFTUCORNER.y+HEIGHT),
			 Point2d(up_middle_right + LINE_WIDTH/2, LEFTUCORNER.y + HEIGHT));
	DrawLine(Point2d(up_middle_right - LINE_WIDTH/2, LEFTUCORNER.y),
			 Point2d(up_middle_right - LINE_WIDTH/2, LEFTUCORNER.y+HEIGHT));


	// Bottoms
	float bottom_middle_left = LEFTLCORNER.x + (RIGHTLCORNER.x - LEFTLCORNER.x)/3, 
		  bottom_middle_right = LEFTLCORNER.x + 2*(RIGHTLCORNER.x - LEFTLCORNER.x)/3; 
	// The circles from the goal
	BOTTOM_MIDDLE_LEFT.set(bottom_middle_left, RIGHTLCORNER.y + LINE_WIDTH/2);
	BOTTOM_MIDDLE_RIGHT.set(bottom_middle_right, RIGHTLCORNER.y + LINE_WIDTH/2);
	circle->DrawCircle(m_system, BOTTOM_MIDDLE_LEFT);
	circle->DrawCircle(m_system, BOTTOM_MIDDLE_RIGHT);
	// The lines
	SetObjectColor(RGBcolor(0.61f, 0.97f, 0.56f));
	DrawLine(Point2d(bottom_middle_left, RIGHTLCORNER.y), 
			 Point2d(bottom_middle_right, RIGHTLCORNER.y));
	SetObjectColor(red);
	DrawLine(LEFTLCORNER, RIGHTLCORNER);
	// The goal
	DrawLine(Point2d(bottom_middle_left-LINE_WIDTH/2, LEFTLCORNER.y - HEIGHT),
			 Point2d(bottom_middle_left-LINE_WIDTH/2, LEFTLCORNER.y));
	DrawLine(Point2d(bottom_middle_left-LINE_WIDTH/2, LEFTLCORNER.y - HEIGHT),
			 Point2d(bottom_middle_right + LINE_WIDTH/2, LEFTLCORNER.y - HEIGHT));
	DrawLine(Point2d(bottom_middle_right - LINE_WIDTH/2, LEFTLCORNER.y - HEIGHT),
			 Point2d(bottom_middle_right - LINE_WIDTH/2, LEFTLCORNER.y));

	DrawTribunes();

	// Here we draw the field
	points_field.push_back(RIGHTLCORNER); 
	points_field.push_back(RIGHTUCORNER);
	points_field.push_back(LEFTUCORNER);
	points_field.push_back(LEFTLCORNER);
	// The topology is identical with topology_line
	for(unsigned int i=0; i<topology_line.size(); ++i){
		topology_field.push_back(topology_line[i]);
	}
	field = new Object2d(points_field, topology_field);
	field->setcolor(0.34f, 0.81f, 0.27f);
	m_system->objectAdd(field);
	// End of actually field drawing

	
	
}

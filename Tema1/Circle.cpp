#pragma once 
#define _USE_MATH_DEFINES
#include "Circle.h"
#include "WorldDrawer2d.h"
#include <cmath>



Circle::Circle(int tris, float radius) : 
	m_tris(tris), 
	m_radius(radius), 
	has_arms(false) 
{
	// Here we will calculate the coordinates of the points
	calculatePointPositions();
}

Circle::Circle() : m_tris(10), m_radius(1.0f){
	// Here we will calculate the coordinate of the points
	calculatePointPositions();
}

Circle::~Circle(){
}


void Circle::calculatePointPositions(){
	Point2d prev_point, new_point;
	float angle = 2*(float)M_PI/m_tris;
	// We add the origin of the circle
	points.push_back(Point2d(0,0));
	// And the point (r,0)
	points.push_back(Point2d(m_radius,0));
	// We calculate the next points of the circle 
	// from angle to angle
	for(int i=0; i<m_tris; ++i){
		prev_point	= points.back();
		new_point.x	= prev_point.x*cos(angle) - prev_point.y*sin(angle);
		new_point.y	= prev_point.x*sin(angle) + prev_point.y*cos(angle);
		points.push_back(new_point);
	}
	// We also have to calculate the typology
	for(int i=0; i<=m_tris; ++i){
		topology.push_back(i); 
		topology.push_back(0);
		topology.push_back(i+1);
	}
}

void Circle::calculateArmsPositions(){
	// The points from the positive and 
	// negative x axis
	Point2d positive = Point2d(m_radius,0), 
			auxiliary;
	float angle = (float)M_PI/5, 
		  pi = (float)M_PI;
	
	// We add the origin
	points_arms.push_back(Point2d(0,0));
	// y positive, x > 0 
	auxiliary.x = positive.x*cos(angle) - positive.y*sin(angle); 
	auxiliary.y = positive.x*sin(angle) + positive.y*cos(angle);
	points_arms.push_back(auxiliary);
	points_arms.push_back(positive);
	// y nevative, x > 0 
	auxiliary.x = positive.x*cos(-angle) - positive.y*sin(-angle); 
	auxiliary.y = positive.x*sin(-angle) + positive.y*cos(-angle);
	points_arms.push_back(auxiliary);
	
	// y positive, x < 0
	auxiliary.x = positive.x*cos(pi-angle) - positive.y*sin(pi-angle); 
	auxiliary.y = positive.x*sin(pi-angle) + positive.y*cos(pi-angle);
	points_arms.push_back(auxiliary);
	points_arms.push_back(Point2d(-m_radius,0));
	// y negative, x < 0
	auxiliary.x = positive.x*cos(pi+angle) - positive.y*sin(pi+angle); 
	auxiliary.y = positive.x*sin(pi+angle) + positive.y*cos(pi+angle);
	points_arms.push_back(auxiliary);
	
	for(int i=1; i<6; ++i){
		topology_arms.push_back(0); 
		topology_arms.push_back(i);
		topology_arms.push_back(i+1);
	}
}



void Circle::DrawCircle(CoordinateSystem2d* system, Point2d position){
	object = new Object2d(points, topology);
	object->setcolor(m_color.GetRed(), m_color.GetGreen(), m_color.GetBlue());
	system->objectAdd(object);
	object->translate(position.x, position.y);
}

void Circle::DrawArms(CoordinateSystem2d* system, Point2d position){
	// Not every circle will have arms therefore is better to
	// invoke it here to avoid unecessary operations
	calculateArmsPositions();
	has_arms = true; 
	head = new Circle(10, 0.5f);
	head->SetColor(RGBcolor(m_color.GetRed(), m_color.GetGreen(), m_color.GetBlue()));
	head->DrawCircle(system, position);
	object_arms = new Object2d(points_arms, topology_arms); 
	object_arms->setcolor(m_color_arms.GetRed(), m_color_arms.GetGreen(),
				m_color_arms.GetBlue());
	system->objectAdd(object_arms); 
	object_arms->translate(position.x, position.y);
}

void Circle::RotateCircle(float angle){
	object->rotateSelf(angle); 
	if (has_arms){
		head->GetObject()->rotateSelf(angle);
		object_arms->rotateSelf(angle);
	}
}
void Circle::TranslateCircle(CoordinateSystem2d* system, float tx, float ty){
	system->objectTranslate(object, tx, ty); 
	if (has_arms){
		system->objectTranslate(head->GetObject(), tx, ty);
		system->objectTranslate(object_arms, tx, ty);
	}
}

void Circle::SetColor(RGBcolor color){
	m_color = color;
}

void Circle::SetArmsColor(RGBcolor color){
	m_color_arms = color;
}

Object2d* Circle::GetObject(){
	return object;
}


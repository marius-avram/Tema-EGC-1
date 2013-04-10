#pragma once
#include "Transform2d.h"
#include "Support2d.h"
#include <cmath>

void Transform2d::translate(Point2d *pct, float tx, float ty){
	pct->x += tx;
	pct->y += ty;
}

void Transform2d::rotate(Point2d *pct, float angleInRadians){
	Point2d copy = *pct;
	pct->x = copy.x*cos(angleInRadians) - copy.y*sin(angleInRadians);
	pct->y = copy.x*sin(angleInRadians) + copy.y*cos(angleInRadians);
}

void Transform2d::rotateRelativeToAnotherPoint(Point2d *pct, Point2d *ref, float angleInRadians){
	translate(pct, -ref->x, -ref->y);
	rotate(pct,angleInRadians);
	translate(pct, ref->x, ref->y);
}
void Transform2d::scale(Point2d *pct, float sx, float sy){
	pct->x *= sx; 
	pct->y *= sy;
}
void Transform2d::scaleRelativeToAnotherPoint(Point2d *pct, Point2d *ref, float sx, float sy){
	translate(pct, -ref->x, -ref->y);
	scale(pct, sx, sy); 
	translate(pct, ref->x, ref->y);
}

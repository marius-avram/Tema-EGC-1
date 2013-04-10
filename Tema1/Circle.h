#pragma once
#include "HeadersAndDefines.h"

/* Used to draw circles in the game for various uses.
 * It also has the capability to drawn a player with 
 * more complex elements 
 * e.g: arms */ 
class Circle{
	
	public: 
		Circle();
		Circle(int tris, float radius);
		virtual ~Circle();
		
		void calculatePointPositions();
		void calculateArmsPositions();
		void DrawCircle(CoordinateSystem2d* system, Point2d position);
		void DrawArms(CoordinateSystem2d* system, Point2d position);
		void RotateCircle(float angle);
		void TranslateCircle(CoordinateSystem2d* system, float tx, float ty);
		void SetColor(RGBcolor color);
		void SetArmsColor(RGBcolor color);
		Object2d* GetObject();
	private:
		int m_tris;
		float m_radius;
		Object2d *object, *object_arms;
		RGBcolor m_color, m_color_arms;
		bool has_arms;
		Circle* head;
	public:
		std::vector<Point2d> points;
		std::vector<int> topology;
		std::vector<Point2d> points_arms;
		std::vector<int> topology_arms;
};

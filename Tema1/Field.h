#pragma once
#include "HeadersAndDefines.h"

/* Contains only static elements of the soccer game.
 * For example: the grass, the mantle and the tribune.
 * By static we understand that the objects are not 
 * animated. */ 
class Field{
	public:
		Field(CoordinateSystem2d* system);
		virtual ~Field();
		void SetObjectColor(RGBcolor color);
		void DrawLine(Point2d start, Point2d end);
		void DrawRotatedLine(Point2d start, Point2d end, float angle);
		void DrawScoreLine(Point2d start, Point2d end); 
		void RemoveScoreLines();
		void DrawTribunes();
		void DrawField();

		static Point2d LEFTUCORNER, LEFTLCORNER, RIGHTUCORNER, RIGHTLCORNER,
			   UP_MIDDLE_LEFT, UP_MIDDLE_RIGHT, BOTTOM_MIDDLE_LEFT, BOTTOM_MIDDLE_RIGHT;
		static float LINE_WIDTH, HEIGHT;
	private:
		std::vector<Point2d> points_line;
		std::vector<int> topology_line;
		std::vector<Point2d> points_field; 
		std::vector<int> topology_field;
		std::vector<Object2d*> m_objects;
		CoordinateSystem2d* m_system;
		std::vector<Object2d*> score_lines;
		RGBcolor m_color;
};
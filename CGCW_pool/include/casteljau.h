#include <list>
#include <vector>
#include <algorithm>

#include "point.h"

point evaluate(float t, std::list<point> P)
{
	std::list<point> Q(P.begin(), P.end());
	while (Q.size() > 1) {
		std::list<point> R;

		std::list<point>::iterator p1 = Q.begin();
		std::list<point>::iterator p2 = Q.begin();
		p2++;
		for (; p1 != prev(Q.end()); p1++, p2++) {
			point p = ((1 - t) * *p1 + t * *p2);
			R.push_back(p);
		}
		Q.clear();
		Q = R;
	}

	return Q.front();
}

std::vector<point> EvaluateBezierCurve(std::vector<point>ctrl_points, int num_evaluations)
{
	std::list<point> points(ctrl_points.begin(), ctrl_points.end());
	std::vector<point> curve;

	float offset = 1.f / (float)num_evaluations;
	curve.push_back(points.front());

	for (int e = 0; e < num_evaluations; e++) {
		point p = evaluate(offset * (e + 1), points);
		curve.push_back(p);
	}

	return curve;
}

float* MakeFloatsFromVector(std::vector<point> curve, int& num_verts, int& num_floats, float r, float g, float b)
{
	if (num_verts == 0) return NULL;
	float* vertices = new float[num_floats];

	// for each p on the curve
	int i = 0;
	for (point p : curve) {
		vertices[i++] = p.x;
		vertices[i++] = p.y;
		vertices[i++] = p.z;
		vertices[i++] = r;
		vertices[i++] = g;
		vertices[i++] = b;
	}
	return vertices;
}


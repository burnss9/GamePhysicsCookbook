#include "ConstructiveSolidGeometry.h"
#include "../Code/Geometry3D.h"
#include <iostream>
#include <vector>

void ConstructiveSolidGeometry::Split(Mesh* m1, Mesh* m2)
{

	std::vector<Triangle> solvedTriangles = std::vector<Triangle>();
	std::vector<Triangle> unsolvedTriangles = std::vector<Triangle>();

	for (int i = 0; i < m1->numTriangles; i++) {
		unsolvedTriangles.push_back(m1->triangles[i]);
	}

	while (unsolvedTriangles.size()) {

		std::vector<Triangle> newUnsolvedTriangles = std::vector<Triangle>();

		for each (Triangle t1 in unsolvedTriangles) {
			bool hit = false;
			for (int index2 = 0; index2 < m2->numTriangles; index2++) {
				Triangle t2 = m2->triangles[index2];
				if (TriangleTriangle(t1, t2)) {

					vec3 n = Cross(t2.b - t2.a, t2.c - t2.a);
					float d = Dot(n, t2.a);

					Point ab, ac, bc, ba, ca, cb;

					bool crossAb = ClipToPlane(Plane(n, d), Line(t1.a, t1.b), &ab);
					bool crossAc = ClipToPlane(Plane(n, d), Line(t1.a, t1.c), &ac);
					bool crossBc = ClipToPlane(Plane(n, d), Line(t1.b, t1.c), &bc);

					ba = ab + Normalized(ab - t1.a) * 0.0001f;
					ab = ab + Normalized(ab - t1.a) * -0.0001f;

					ca = ac + Normalized(ac - t1.a) * 0.0001f;
					ac = ac + Normalized(ac - t1.a) * -0.0001f;

					cb = bc + Normalized(bc - t1.b) * 0.0001f;
					bc = bc + Normalized(bc - t1.b) * -0.0001f;

					if (crossAb && crossAc) {
						Triangle point = Triangle(t1.a, ab, ac);
						Triangle base1 = Triangle(ba, t1.b, ca);
						Triangle base2 = Triangle(ca, t1.b, t1.c);
						newUnsolvedTriangles.push_back(point);
						newUnsolvedTriangles.push_back(base1);
						newUnsolvedTriangles.push_back(base2);
						hit = true;
						break;
					}
					else if (crossAb && crossBc) {
						Triangle point = Triangle(t1.b, bc, ba);
						Triangle base1 = Triangle(cb, t1.c, ab);
						Triangle base2 = Triangle(ab, t1.c, t1.a);
						newUnsolvedTriangles.push_back(point);
						newUnsolvedTriangles.push_back(base1);
						newUnsolvedTriangles.push_back(base2);
						hit = true;
						break;
					}
					else if (crossAc && crossBc) {
						Triangle point = Triangle(t1.c, ca, cb);
						Triangle base1 = Triangle(ac, t1.a, bc);
						Triangle base2 = Triangle(bc, t1.a, t1.b);
						newUnsolvedTriangles.push_back(point);
						newUnsolvedTriangles.push_back(base1);
						newUnsolvedTriangles.push_back(base2);
						hit = true;
						break;
					}
				}
			}

			if (!hit) {
				solvedTriangles.push_back(t1);
			}
		}

		unsolvedTriangles = newUnsolvedTriangles;
	}

	m1->numTriangles = solvedTriangles.size();
	m1->triangles = new Triangle[solvedTriangles.size()];

	for (int i = 0; i < solvedTriangles.size(); i++) {
		m1->triangles[i] = solvedTriangles.at(i);
	}

}

bool ConstructiveSolidGeometry::PointInsideMesh(Point p, Mesh* m)
{

	float Closest = FLT_MAX;
	bool ClosestInside = false;

	for (int i = 0; i < m->numTriangles; i++) {
		Triangle t = m->triangles[i];

		vec3 n = Cross(t.b - t.a, t.c - t.a);
		float d = Dot(n, t.a);

		Point crossPoint;

		bool crossPlane = ClipToPlane(Plane(n, d), Line(p, Point(100000000.0f, 0, 0)), &crossPoint);
		if (crossPlane && PointInTriangle(crossPoint, t)) {
			if (Distance(p, crossPoint) < Closest) {
				Closest = Distance(p, crossPoint);
				ClosestInside = Dot(n, p - t.a) < 0;
			}
		}

	}


	return ClosestInside;
}



void ConstructiveSolidGeometry::Union(Mesh* m1, Mesh* m2)
{
}

Mesh* ConstructiveSolidGeometry::RelativeComplement(Mesh* m1, Mesh* m2)
{

	Mesh* m1Copy = new Mesh();
	m1Copy->numTriangles = m1->numTriangles;
	m1Copy->triangles = new Triangle[m1->numTriangles];

	for (int i = 0; i < m1->numTriangles; i++) {
		m1Copy->triangles[i] = m1->triangles[i];
	}

	Mesh* m2Copy = new Mesh();
	m2Copy->numTriangles = m2->numTriangles;
	m2Copy->triangles = new Triangle[m2->numTriangles];

	for (int i = 0; i < m2->numTriangles; i++) {
		m2Copy->triangles[i] = m2->triangles[i];
	}

	Split(m1Copy, m2);
	Split(m2Copy, m1);

	std::vector<Triangle> savedTriangles = std::vector<Triangle>();

	for (int i = 0; i < m1Copy->numTriangles; i++) {
		if (!PointInsideMesh(m1Copy->triangles[i].a, m2)) {
			savedTriangles.push_back(m1Copy->triangles[i]);
		}
	}
	for (int i = 0; i < m2Copy->numTriangles; i++) {
		if (PointInsideMesh(m2Copy->triangles[i].a, m1)) {
			Triangle flipped = Triangle(m2Copy->triangles[i].c, m2Copy->triangles[i].b, m2Copy->triangles[i].a);
			savedTriangles.push_back(flipped);
		}
	}

	Mesh* result = new Mesh();
	result->numTriangles = savedTriangles.size();
	result->triangles = new Triangle[savedTriangles.size()];
	for (int i = 0; i < savedTriangles.size(); i++) {
		result->triangles[i] = savedTriangles.at(i);
	}

	return result;
}

void ConstructiveSolidGeometry::Intersection(Mesh* m1, Mesh* m2)
{
}

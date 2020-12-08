#include "ConstructiveSolidGeometry.h"
#include "../Code/Geometry3D.h"
#include <iostream>
#include <vector>

void ConstructiveSolidGeometry::Split(Mesh* m1, Mesh* m2)
{

	std::vector<Triangle> triangles1 = std::vector<Triangle>();
	std::vector<Triangle> triangles2 = std::vector<Triangle>();

	for (int index1 = 0; index1 < m1->numTriangles; index1++) {
		Triangle t1 = m1->triangles[index1];
		bool hit = false;
		for (int index2 = 0; index2 < m2->numTriangles; index2++) {
			Triangle t2 = m2->triangles[index2];


			if (TriangleTriangle(t1, t2)) {
				hit = true;
			}


		}

		if (!hit) {
			triangles1.push_back(t1);
		}
	}

	m1->numTriangles = triangles1.size();
	m1->triangles = new Triangle[triangles1.size()];

	for (int i = 0; i < triangles1.size(); i++) {
		m1->triangles[i] = triangles1.at(i);
	}

}

void ConstructiveSolidGeometry::Union(Mesh* m1, Mesh* m2)
{
}

void ConstructiveSolidGeometry::RelativeComplement(Mesh* m1, Mesh* m2)
{
	Split(m1, m2);
}

void ConstructiveSolidGeometry::Intersection(Mesh* m1, Mesh* m2)
{
}

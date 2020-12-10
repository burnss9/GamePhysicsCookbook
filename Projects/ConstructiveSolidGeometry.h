#pragma once
#include "../Code/Geometry3D.h"

class ConstructiveSolidGeometry {

private: 

	void Split(Mesh* m1, Mesh* m2);
	bool PointInsideMesh(Point p, Mesh* m);

public:

	void Union(Mesh* m1, Mesh* m2);	
	Mesh* RelativeComplement(Mesh* m1, Mesh* m2);	
	void Intersection(Mesh* m1, Mesh* m2);	



};
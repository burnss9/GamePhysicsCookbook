#pragma once
#include "../Code/Geometry3D.h"

class ConstructiveSolidGeometry {

private: 

	void Split(Mesh* m1, Mesh* m2);

public:

	void Union(Mesh* m1, Mesh* m2);	
	void RelativeComplement(Mesh* m1, Mesh* m2);	
	void Intersection(Mesh* m1, Mesh* m2);	



};
#ifndef _H_CH16DEMO_
#define _H_CH16DEMO_

#include "DemoBase.h"
#include "Geometry3D.h"
#include "PhysicsSystem.h"
#include "Cloth.h"

class CH16Demo : public DemoBase {
protected:
	PhysicsSystem physicsSystem;

	bool size_imgui_window;
protected:
	void ResetDemo();
	float Random(float min, float max);
public:
	inline CH16Demo() : DemoBase(), size_imgui_window(true) { }

	void Initialize(int width, int height);
	void Render();
	void Update(float dt);
	void ImGUI();
};

#endif
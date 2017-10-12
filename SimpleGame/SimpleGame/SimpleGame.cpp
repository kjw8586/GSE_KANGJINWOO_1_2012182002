/*
Copyright 2017 Lee Taek Hee (Korea Polytech University)

This program is free software: you can redistribute it and/or modify
it under the terms of the What The Hell License. Do it plz.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"
#include <iostream>
#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"

#include "Renderer.h"
#include "Object.h"

#define WINDOWX 500
#define WINDOWY 500

Renderer *g_Renderer = NULL;

CObject Obj("ABC", 100, 10);

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);

	// x, y, z, size, r, g, b, a (왼쪽 위는 (-250, 250) 이다.)
	g_Renderer->DrawSolidRect(Obj.GetPos().fX, Obj.GetPos().fY, Obj.GetPos().fZ, Obj.GetSize(),
		Obj.GetColor().fR, Obj.GetColor().fG, Obj.GetColor().fB, Obj.GetColor().fA);

	Obj.Update();

	glutSwapBuffers();
}

void Idle(void)
{
	RenderScene();
}

//GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON
//GLUT_UP, GLUT_DOWN
void MouseInput(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		Obj.SetLeftButtonDown(true);
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		if (Obj.GetLeftButtonDown())
		{
			Obj.SetTargetPos(x - WINDOWX / 2, -y + WINDOWY / 2);
			Obj.SetLeftButtonDown(false);
		}
	}

	RenderScene();
}

void KeyInput(unsigned char key, int x, int y)
{
	RenderScene();
}

void SpecialKeyInput(int key, int x, int y)
{
	RenderScene();
}

int main(int argc, char **argv)
{
	// Initialize GL things
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Game Software Engineering KPU");

	glewInit();
	if (glewIsSupported("GL_VERSION_3_0"))
	{
		std::cout << " GLEW Version is 3.0\n ";
	}
	else
	{
		std::cout << "GLEW 3.0 not supported\n ";
	}

	// Initialize Renderer
	g_Renderer = new Renderer(WINDOWX, WINDOWY);
	if (!g_Renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}

	// Set Object 
	Obj.Init();
	Obj.SetPos(0.f, 0.f);
	Obj.SetSize(100.f);
	Obj.SetColor(1.f, 1.f, 1.f, 1.f);
	//

	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyInput);
	glutMouseFunc(MouseInput);
	glutSpecialFunc(SpecialKeyInput);

	glutMainLoop();

	delete g_Renderer;

	return 0;
}


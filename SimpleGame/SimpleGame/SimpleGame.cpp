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
#include "SceneMgr.h"

#define WINDOWX 500
#define WINDOWY 500

Renderer *g_Renderer = NULL;

CSceneMgr g_SceneMgr;

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);

	for (int i = 0; i < 50; ++i)
	{
		// x, y, z, size, r, g, b, a (왼쪽 위는 (-250, 250) 이다.)
		g_Renderer->DrawSolidRect(g_SceneMgr.GetObjects(i)->GetPos().fX, g_SceneMgr.GetObjects(i)->GetPos().fY, g_SceneMgr.GetObjects(i)->GetPos().fZ, g_SceneMgr.GetObjects(i)->GetSize(),
								  g_SceneMgr.GetObjects(i)->GetColor().fR, g_SceneMgr.GetObjects(i)->GetColor().fG, g_SceneMgr.GetObjects(i)->GetColor().fB, g_SceneMgr.GetObjects(i)->GetColor().fA);
	}
	g_SceneMgr.Update();

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
	//if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	//{
	//	Obj.SetLeftButtonDown(true);
	//}

	//if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	//{
	//	if (Obj.GetLeftButtonDown())
	//	{
	//		Obj.SetTargetPos(x - WINDOWX / 2, -y + WINDOWY / 2);
	//		Obj.SetLeftButtonDown(false);
	//	}
	//}

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

	// Init SceneMgr
	g_SceneMgr.Init();

	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyInput);
	glutMouseFunc(MouseInput);
	glutSpecialFunc(SpecialKeyInput);

	glutMainLoop();

	delete g_Renderer;

	return 0;
}


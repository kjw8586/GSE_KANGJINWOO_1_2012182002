/*
Copyright 2017 Lee Taek Hee (Korea Polytech University)

This program is free software: you can redistribute it and/or modify
it under the terms of the What The Hell License. Do it plz.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"
#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"

#include "SceneMgr.h"

CSceneMgr* g_SceneMgr = NULL;

DWORD g_prevTime = 0;
bool g_LButtonDown = false;
float g_fTimeTerm_CreateCharacter_Team2 = 7.f; // 최초에 한 번은 바로 찍힐 수 있도록 

OBJECT_TYPE g_Obj = OBJECT_CHARACTER;

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.f, 0.f, 1.0f);

	DWORD currTime = timeGetTime();
	DWORD elapsedTime = currTime - g_prevTime;
	g_prevTime = currTime;

	float fTime = elapsedTime / 1000.f;
	g_fTimeTerm_CreateCharacter_Team2 += fTime;

	g_SceneMgr->Update(float(elapsedTime));
	g_SceneMgr->Render();

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
		g_LButtonDown = true;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		if (g_LButtonDown)
		{
			if (g_SceneMgr->GetCharacterCount() < 1000)
			{
				float fX = float(x - WINCX / 2.f);
				float fY = float(-y + WINCY / 2.f);

				if (fY < 0 /*&& g_fTimeTerm_CreateCharacter_Team2 > 1.f*/)
				{
					if(g_Obj == 1)
						g_SceneMgr->CreateObjects(fX, fY, OBJECT_CHARACTER, OBJECT_TEAM2);
					else if(g_Obj == 2)
						g_SceneMgr->CreateObjects(fX, fY, OBJECT_INFANTRY, OBJECT_TEAM2);

					g_fTimeTerm_CreateCharacter_Team2 = 0.f;
				}
			}

			g_LButtonDown = false;
		}
	}

	RenderScene();
}

void KeyInput(unsigned char key, int x, int y)
{
	if (key == '1')
		g_Obj = OBJECT_CHARACTER;
	else if (key == '2')
		g_Obj = OBJECT_INFANTRY;

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
	glutInitWindowSize(WINCX, WINCY);
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

	// Initialize SceneMgr
	g_SceneMgr = new CSceneMgr();
	g_SceneMgr->Init();

	g_prevTime = timeGetTime();

	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyInput);
	glutMouseFunc(MouseInput);
	glutSpecialFunc(SpecialKeyInput);

	glutMainLoop();

	delete g_SceneMgr;

	return 0;
}


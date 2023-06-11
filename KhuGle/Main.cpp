//
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//
#include "KhuGleWin.h"
#include "KhuGle3DSprite.h"
#include <iostream>
#include <string>

#pragma warning(disable:4996)

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

class CThreeDim : public CKhuGleWin
{
public:
	CKhuGleLayer *m_pGameLayer;

	CKhuGleCamera *m_camera;
	
	CThreeDim(int nW, int nH);
	void Update();
	CKhuGle3DSprite* AddObject(KgColor24 color, CKgVector3D worldPos, char* FilePath);

	CKgPoint m_LButtonStart, m_LButtonEnd;
	int m_nLButtonStatus;
};

CThreeDim::CThreeDim(int nW, int nH) : CKhuGleWin(nW, nH)
{
	m_nLButtonStatus = 0;

	m_Gravity = CKgVector2D(0., 98.);
	m_AirResistance = CKgVector2D(0.1, 0.1);


	m_pScene = new CKhuGleScene(640, 480, KG_COLOR_24_RGB(100, 100, 150));

	m_pGameLayer = new CKhuGleLayer(600, 420, KG_COLOR_24_RGB(150, 150, 200), CKgPoint(20, 30));
	m_pScene->AddChild(m_pGameLayer);

	m_camera = new CKhuGleCamera(m_pGameLayer->m_nW, m_pGameLayer->m_nH, Pi / 2.5, 1000, 0.1, CKgVector3D(12., 0., 12.), CKgVector3D(-1., 0., -1.), CKgVector3D(0., -1., 0.));
	//m_camera = new CKhuGleCamera(m_pGameLayer->m_nW, m_pGameLayer->m_nH, Pi / 2., 1000, 0.1, CKgVector3D(0., 0., 2.), CKgVector3D(0., 0., -1.), CKgVector3D(0., 1., 0.));

	
	auto* m_p_Axis_X = new CKhuGle3DSprite(KG_COLOR_24_RGB(255, 0, 0), CKgVector3D(0., 0., 0.), m_camera, "axis.obj");
	auto* m_p_Axis_Y = new CKhuGle3DSprite(KG_COLOR_24_RGB(0, 255, 0), CKgVector3D(0., 0., 0.), m_camera, "axis.obj");
	auto* m_p_Axis_Z = new CKhuGle3DSprite(KG_COLOR_24_RGB(0, 0, 255), CKgVector3D(0., 0., 0.), m_camera, "axis.obj");
	m_p_Axis_Y->RotateBy(0, 0, Pi / 2);
	m_p_Axis_Z->RotateBy(0, -Pi/2, 0.);

	/*m_pGameLayer->AddChild(m_p_Axis_X);
	m_pGameLayer->AddChild(m_p_Axis_Y);
	m_pGameLayer->AddChild(m_p_Axis_Z);*/
}

CKhuGle3DSprite* CThreeDim::AddObject(KgColor24 color, CKgVector3D worldPos, char* FilePath)
{
	auto new_object = new CKhuGle3DSprite(color, worldPos, m_camera, FilePath);
	m_pGameLayer->AddChild(new_object);

	return new_object;
}

void CThreeDim::Update()
{
	float speed = 0.01;
	float rot_speed = Pi / 100;

	if (m_bKeyPressed[VK_LEFT])
	{
		auto left = -1 * speed * m_camera->m_forward.Cross(m_camera->m_up);
		m_camera->MoveBy(left.x, left.y, left.z);
	}
	else if (m_bKeyPressed[VK_RIGHT])
	{
		auto right = speed * m_camera->m_forward.Cross(m_camera->m_up);
		m_camera->MoveBy(right.x, right.y, right.z);
	}
	if(m_bKeyPressed[VK_UP])
	{
		auto front = speed * m_camera->m_forward;
		m_camera->MoveBy(front.x, front.y, front.z);
	}
	else if (m_bKeyPressed[VK_DOWN])
	{
		auto back = -1 * speed * m_camera->m_forward;
		m_camera->MoveBy(back.x, back.y, back.z);
	}
	if (m_bKeyPressed[VK_SPACE])
	{
		auto up = speed * m_camera->m_up;
		m_camera->MoveBy(up.x, up.y, up.z);
	}
	else if (m_bKeyPressed[VK_SHIFT])
	{
		auto down = -1 * speed * m_camera->m_up;
		m_camera->MoveBy(down.x, down.y, down.z);
	}
	if (m_bKeyPressed['Q'])
	{
		m_camera->RotateBy(0., rot_speed, 0.);
	}
	else if (m_bKeyPressed['E'])
	{
		m_camera->RotateBy(0., -rot_speed, 0.);
	}
	
	m_pScene->Render();
	DrawSceneTextPos("3D Rendering", CKgPoint(0, 0));

	if (m_bViewFps)
	{
		std::string strFps = "FPS: " + std::to_string(m_Fps);
		DrawSceneTextPos(strFps.c_str(), CKgPoint(200, 0));
	}

	CKhuGleWin::Update();
}

int main()
{
	CThreeDim *pThreeDim = new CThreeDim(640, 480);

	//pThreeDim->AddObject(KG_COLOR_24_RGB(255, 0, 255), CKgVector3D(0, 0, 0), "asd.obj");

	auto m_pShip = pThreeDim->AddObject(KG_COLOR_24_RGB(255, 0, 255), CKgVector3D(0, 0, 0), "VideoShip.obj");
	m_pShip->RotateBy(-Pi / 2, Pi/2, 0);

	//pThreeDim->AddObject(KG_COLOR_24_RGB(255, 0, 255), CKgVector3D(0, 0, 0), "asd.obj");

	/*
	auto m_IronMan = pThreeDim->AddObject(KG_COLOR_24_RGB(255, 255, 255), CKgVector3D(0, 0, 0), "IronMan.obj");
	m_IronMan->RotateBy(0, Pi / 4, 0);
	*/
	

	KhuGleWinInit(pThreeDim);
	return 0;
}

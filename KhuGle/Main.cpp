//
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//
#include "KhuGleWin.h"
#include "KhuGle3DSprite.h"
#include <iostream>
#include <string>
#include <format>

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

	CKhuGle3DSprite *m_pObject3D;

	CKhuGleCamera *m_camera;
	
	CThreeDim(int nW, int nH);
	void Update();

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

	auto* m_p_Axis_X = new CKhuGle3DSprite(KG_COLOR_24_RGB(255, 0, 0), CKgVector3D(0., 0., 0.), CKgVector3D(10, 0, 0), m_camera);
	auto* m_p_Axis_Y = new CKhuGle3DSprite(KG_COLOR_24_RGB(0, 255, 0), CKgVector3D(0., 0., 0.), CKgVector3D(0, 10, 0), m_camera);
	auto* m_p_Axis_Z = new CKhuGle3DSprite(KG_COLOR_24_RGB(0, 0, 255), CKgVector3D(0., 0., 0.), CKgVector3D(0, 0, 10), m_camera);
	m_pGameLayer->AddChild(m_p_Axis_X);
	m_pGameLayer->AddChild(m_p_Axis_Y);
	m_pGameLayer->AddChild(m_p_Axis_Z);

	//auto* m_pObject3Daxis = new CKhuGle3DSprite(m_pGameLayer->m_nW, m_pGameLayer->m_nH, Pi / 2., 1000., 0.1, KG_COLOR_24_RGB(255, 255, 255), m_camera, CKgVector3D(0, 0, 0), std::vector<CKgTriangle>{CKgTriangle(CKgVector3D(0.5, 0, 0), CKgVector3D(0., 0.5, 0.), CKgVector3D(0., 0., 0.5))});
	//m_pGameLayer->AddChild(m_pObject3Daxis);


	std::vector<CKgTriangle> SurfaceMesh;
	SurfaceMesh.push_back(CKgTriangle(CKgVector3D(-0.5, 0., -sqrt(3.) / 6), CKgVector3D(0.5, 0., -sqrt(3.) / 6), CKgVector3D(0., 0., sqrt(3.) / 3), true));
	SurfaceMesh.push_back(CKgTriangle(CKgVector3D(0., 0., sqrt(3.) / 3), CKgVector3D(0.5, 0., -sqrt(3.) / 6), CKgVector3D(0., sqrt(3.) / 3, 0.), true));
	SurfaceMesh.push_back(CKgTriangle(CKgVector3D(-0.5, 0., -sqrt(3.) / 6), CKgVector3D(0, 0., sqrt(3.) / 3), CKgVector3D(0., sqrt(3.) / 3, 0.), true));
	SurfaceMesh.push_back(CKgTriangle(CKgVector3D(0.5, 0., -sqrt(3.) / 6), CKgVector3D(-0.5, 0., -sqrt(3.) / 6), CKgVector3D(0., sqrt(3.) / 3, 0.), true));
	m_pObject3D = new CKhuGle3DSprite(KG_COLOR_24_RGB(255, 0, 255), CKgVector3D(0, 0, 0), SurfaceMesh, m_camera);

	SurfaceMesh.clear();
	SurfaceMesh.push_back(CKgTriangle(CKgVector3D(-0.5, 0., -sqrt(3.) / 6), CKgVector3D(0.5, 0., -sqrt(3.) / 6), CKgVector3D(0., 0., sqrt(3.) / 3), false));
	SurfaceMesh.push_back(CKgTriangle(CKgVector3D(0., 0., sqrt(3.) / 3), CKgVector3D(0.5, 0., -sqrt(3.) / 6), CKgVector3D(0., sqrt(3.) / 3, 0.), false));
	SurfaceMesh.push_back(CKgTriangle(CKgVector3D(-0.5, 0., -sqrt(3.) / 6), CKgVector3D(0, 0., sqrt(3.) / 3), CKgVector3D(0., sqrt(3.) / 3, 0.), false));
	SurfaceMesh.push_back(CKgTriangle(CKgVector3D(0.5, 0., -sqrt(3.) / 6), CKgVector3D(-0.5, 0., -sqrt(3.) / 6), CKgVector3D(0., sqrt(3.) / 3, 0.), false));
	auto m_pObject3DEdge = new CKhuGle3DSprite(KG_COLOR_24_RGB(0, 0, 0), CKgVector3D(0, 0, 0), SurfaceMesh, m_camera);

	auto m_pShip = new CKhuGle3DSprite(KG_COLOR_24_RGB(255, 255, 255), CKgVector3D(0, 0, 0), m_camera, "VideoShip.obj");
	m_pShip->RotateBy(0, Pi / 2, Pi / 2);
	m_pGameLayer->AddChild(m_pShip);


	//auto m_IronMan = new CKhuGle3DSprite(KG_COLOR_24_RGB(255, 255, 255), CKgVector3D(0, 0, 0), m_camera, "IronMan.obj");
	//m_IronMan->RotateBy(0, Pi / 2, Pi / 2);
	//m_pGameLayer->AddChild(m_IronMan);


	m_pGameLayer->AddChild(m_pObject3D);

	m_pGameLayer->AddChild(m_pObject3DEdge);
	//auto m_pPortal = new CKhuGlePortal(m_pGameLayer->m_nW, m_pGameLayer->m_nH, Pi / 2, 1000, 0.1, KG_COLOR_24_RGB(0, 0, 0), CKgVector3D(0, 0, 0), CKgVector3D(0, 0, 1), m_camera);
	//m_pGameLayer->AddChild(m_pPortal);
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

	KhuGleWinInit(pThreeDim);

	return 0;
}

// define std::pair<std::vector<int, int>(2), KgColor24> render_inform
// or ... std::pair<render_line = <int, int, int, int>, KgColor24>

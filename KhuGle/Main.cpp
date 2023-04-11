//
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//
#include "KhuGleWin.h"
#include "KhuGle3DSprite.h"
#include <iostream>

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

	CKgCamera *m_camera;
	
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

	m_camera = new CKgCamera(CKgVector3D(2.,0.,2.), CKgVector3D(-1.,0.,-1.), CKgVector3D(0.,-1.,0.));

	m_pScene = new CKhuGleScene(640, 480, KG_COLOR_24_RGB(100, 100, 150));

	m_pGameLayer = new CKhuGleLayer(600, 420, KG_COLOR_24_RGB(150, 150, 200), CKgPoint(20, 30));
	m_pScene->AddChild(m_pGameLayer);

	auto* m_p_Axis_X = new CKhuGle3DSprite(m_pGameLayer->m_nW, m_pGameLayer->m_nH, Pi / 2., 1000., 0.1, KG_COLOR_24_RGB(255, 0, 0), m_camera, CKgVector3D(0., 0., 0.), CKgVector3D(10, 0, 0));
	auto* m_p_Axis_Y = new CKhuGle3DSprite(m_pGameLayer->m_nW, m_pGameLayer->m_nH, Pi / 2., 1000., 0.1, KG_COLOR_24_RGB(0, 255, 0), m_camera, CKgVector3D(0., 0., 0.), CKgVector3D(0, 10, 0));
	auto* m_p_Axis_Z = new CKhuGle3DSprite(m_pGameLayer->m_nW, m_pGameLayer->m_nH, Pi / 2., 1000., 0.1, KG_COLOR_24_RGB(0, 0, 255), m_camera, CKgVector3D(0., 0., 0.), CKgVector3D(0, 0, 10));
	m_pGameLayer->AddChild(m_p_Axis_X);
	m_pGameLayer->AddChild(m_p_Axis_Y);
	m_pGameLayer->AddChild(m_p_Axis_Z);

	auto* m_pObject3Daxis = new CKhuGle3DSprite(m_pGameLayer->m_nW, m_pGameLayer->m_nH, Pi / 2., 1000., 0.1, KG_COLOR_24_RGB(255, 255, 255), m_camera, CKgVector3D(0, 0, 0), std::vector<CKgTriangle>{CKgTriangle(CKgVector3D(0.5, 0, 0), CKgVector3D(0., 0.5, 0.), CKgVector3D(0., 0., 0.5))});
	//m_pGameLayer->AddChild(m_pObject3Daxis);

	std::vector<CKgTriangle> SurfaceMesh;
	SurfaceMesh.push_back(CKgTriangle(CKgVector3D(-0.5, 0., -sqrt(3.) / 6), CKgVector3D(0.5, 0., -sqrt(3.) / 6), CKgVector3D(0., 0., sqrt(3.) / 3)));
	SurfaceMesh.push_back(CKgTriangle(CKgVector3D(0., 0., sqrt(3.) / 3), CKgVector3D(0.5, 0., -sqrt(3.) / 6), CKgVector3D(0., sqrt(3.) / 3, 0.)));
	SurfaceMesh.push_back(CKgTriangle(CKgVector3D(-0.5, 0., -sqrt(3.) / 6), CKgVector3D(0, 0., sqrt(3.) / 3), CKgVector3D(0., sqrt(3.) / 3, 0.)));
	SurfaceMesh.push_back(CKgTriangle(CKgVector3D(0.5, 0., -sqrt(3.) / 6), CKgVector3D(-0.5, 0., -sqrt(3.) / 6), CKgVector3D(0., sqrt(3.) / 3, 0.)));
	m_pObject3D = new CKhuGle3DSprite(m_pGameLayer->m_nW, m_pGameLayer->m_nH, Pi/2., 1000., 0.1, KG_COLOR_24_RGB(255, 0, 255), m_camera, CKgVector3D(0, 0, 0), SurfaceMesh);

	m_pGameLayer->AddChild(m_pObject3D);
}

void CThreeDim::Update()
{
	if (m_bKeyPressed[VK_LEFT])
	{
		m_pObject3D->localRotateBy(0, Pi / 1000, 0);
	}
	else if (m_bKeyPressed[VK_RIGHT])
	{
		m_pObject3D->localRotateBy(0, -Pi / 1000, 0);
	}
	if(m_bKeyPressed[VK_DOWN])
	{
		m_camera->MoveBy(0., -0.005, 0.);
	}
//		m_pObject3D->MoveBy(0, 0.0005, 0.);
	else if (m_bKeyPressed[VK_UP])
	{
		m_camera->MoveBy(0., 0.005, 0.);
		//m_camera->m_target += CKgVector3D(0, 0.0005, 0);
	}

	m_pScene->Render();
	DrawSceneTextPos("3D Rendering", CKgPoint(0, 0));

	CKhuGleWin::Update();
}

int main()
{
	CThreeDim *pThreeDim = new CThreeDim(640, 480);

	KhuGleWinInit(pThreeDim);

	return 0;
}
//
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//
#include "KhuGleWin.h"
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

int constexpr GP_3DTYPE_LINE = 0;
int constexpr GP_3DTYPE_TRIANGLE = 1;

struct CKgTriangle{
	CKgVector3D v0, v1, v2;

	CKgTriangle() : v0(CKgVector3D()), v1(CKgVector3D()), v2(CKgVector3D()) {}
	CKgTriangle(CKgVector3D vv0, CKgVector3D vv1, CKgVector3D vv2) : v0(vv0), v1(vv1), v2(vv2) {}
};

struct CKgCamera
{
	CKgVector3D m_position;
	CKgVector3D m_forward;
	CKgVector3D m_up;
	double** m_view_matrix;
	double** m_rotation_matrix;
	double rx, ry, rz;

	CKgCamera(const CKgVector3D& m_position, const CKgVector3D& m_forward, const CKgVector3D& m_up);

	void MoveBy(double OffsetX, double OffsetY, double OffsetZ)
	{
		m_position += CKgVector3D(OffsetX, OffsetY, OffsetZ);
		m_view_matrix = ComputeViewMatrix();
	}
	void RotateBy(double dRadianX, double dRadianY, double dRadianZ)
	{
		double c_x, c_y, c_z, s_x, s_y, s_z;
		rx += dRadianX;
		ry += dRadianY;
		rz += dRadianZ;
		c_x = cos(rx);
		c_y = cos(ry);
		c_z = cos(rz);
		s_x = sin(rx);
		s_y = sin(ry);
		s_z = sin(rz);

		m_rotation_matrix[0][0] = c_z * c_y;
		m_rotation_matrix[1][0] = s_z * c_y;
		m_rotation_matrix[2][0] = -s_y;
		m_rotation_matrix[3][0] = 0.;
		m_rotation_matrix[0][1] = -s_z * c_x + c_z * s_y * s_x;
		m_rotation_matrix[1][1] = c_z * c_x + s_z * s_y * s_x;
		m_rotation_matrix[2][1] = c_y * s_x;
		m_rotation_matrix[3][1] = 0.;
		m_rotation_matrix[0][2] = s_z * s_x + c_z * s_y * c_x;
		m_rotation_matrix[1][2] = -c_z * s_x + s_z * s_y * c_x;
		m_rotation_matrix[2][2] = c_y * c_x;
		m_rotation_matrix[3][2] = 0.;
		m_rotation_matrix[0][3] = 0.;
		m_rotation_matrix[1][3] = 0.;
		m_rotation_matrix[2][3] = 0.;
		m_rotation_matrix[3][3] = 1.;

		m_view_matrix = ComputeViewMatrix();
	}

	double** ComputeViewMatrix()
	{
		CKgVector3D camera_pos = m_position;
		//CKgVector3D Target = m_position + m_forward;
		CKgVector3D CameraUp = m_up;
		//CKgVector3D Forward = Target - camera_pos;
		CKgVector3D Forward = m_forward;
		Forward.Normalize();
		CameraUp.Normalize();
		CKgVector3D Right = CameraUp.Cross(Forward);
		CKgVector3D Up = Forward.Cross(Right);

		double** RT = dmatrix(4, 4);
		double** View = dmatrix(4, 4);

		RT[0][0] = Right.x;
		RT[1][0] = Right.y;
		RT[2][0] = Right.z;
		RT[3][0] = 0.;
		RT[0][1] = Up.x;
		RT[1][1] = Up.y;
		RT[2][1] = Up.z;
		RT[3][1] = 0.;
		RT[0][2] = Forward.x;
		RT[1][2] = Forward.y;
		RT[2][2] = Forward.z;
		RT[3][2] = 0.;
		RT[0][3] = camera_pos.x;
		RT[1][3] = camera_pos.y;
		RT[2][3] = camera_pos.z;
		RT[3][3] = 1.;

		bool bInverse = InverseMatrix(RT, View, 4);

		free_dmatrix(RT, 4, 4);

		if (bInverse)
			return View;

		return nullptr;
	}
};

CKgCamera::CKgCamera(const CKgVector3D& m_position, const CKgVector3D& m_forward, const CKgVector3D& m_up)
	: m_position(m_position), m_forward(m_forward), m_up(m_up)
{
	m_view_matrix = ComputeViewMatrix();
	m_rotation_matrix = dmatrix(4, 4);
	for (int r = 0; r < 4; ++r)
		for (int c = 0; c < 4; ++c)
			m_rotation_matrix[r][c] = (r == c) ? 1. : 0.;

}
class CKhuGle3DSprite : public CKhuGleSprite {
public:
	int m_nType;
	std::vector<CKgTriangle> SurfaceMesh;
	double **m_ProjectionMatrix;
	double** m_RotationMatrix;
	double rx, ry, rz;
	CKgCamera *m_camera;
	CKgVector3D m_Line;

	CKgVector3D m_WorldPos;

	CKhuGle3DSprite(int nW, int nH, double Fov, double Far, double Near, KgColor24 fgColor, CKgCamera* p_camera, CKgVector3D pWorldPos, CKgVector3D Line);
	CKhuGle3DSprite(int nW, int nH, double Fov, double Far, double Near, KgColor24 fgColor, CKgCamera *p_camera, CKgVector3D pWorldPos, std::vector<CKgTriangle> pTriangleMeshes);
	~CKhuGle3DSprite();

	static void DrawTriangle(unsigned char **R, unsigned char **G, unsigned char **B, int nW, int nH, 
		int x0, int y0, int x1, int y1, int x2, int y2, KgColor24 Color24);
	static void MatrixVector44(CKgVector3D &out, CKgVector3D v, double **M);
	//static double **ComputeViewMatrix(CKgCamera Camera);

	void Render();
	void MoveBy(double OffsetX, double OffsetY, double OffsetZ);
	void localRotateBy(double RadianX, double RadianY, double RadianZ);
};



CKhuGle3DSprite::CKhuGle3DSprite(int nW, int nH, double Fov, double Far, double Near, KgColor24 fgColor,
                                 CKgCamera* p_camera, CKgVector3D pWorldPos, CKgVector3D Line)
{
	m_nType = GP_3DTYPE_LINE;
	m_fgColor = fgColor;
	m_camera = p_camera;
	m_WorldPos = pWorldPos;
	rx = 0.;ry = 0.;rz = 0.;

	m_ProjectionMatrix = dmatrix(4, 4);
	for (int r = 0; r < 4; ++r)
		for (int c = 0; c < 4; ++c)
			m_ProjectionMatrix[r][c] = 0.;

	m_RotationMatrix = dmatrix(4, 4);
	for (int r = 0; r < 4; ++r)
		for (int c = 0; c < 4; ++c)
			m_RotationMatrix[r][c] = (r == c) ? 1. : 0.;

	m_ProjectionMatrix[0][0] = (double)nH / (double)nW * 1. / tan(Fov / 2.);
	m_ProjectionMatrix[1][1] = 1. / tan(Fov / 2.);
	m_ProjectionMatrix[2][2] = (-Near - Far) / (Near - Far);
	m_ProjectionMatrix[2][3] = 2. * (Far * Near) / (Near - Far);
	m_ProjectionMatrix[3][2] = 1.;
	m_ProjectionMatrix[3][3] = 0.;

	m_Line = Line;
}

CKhuGle3DSprite::CKhuGle3DSprite(int nW, int nH, double Fov, double Far, double Near, KgColor24 fgColor, 
	CKgCamera *p_camera, CKgVector3D pWorldPos, std::vector<CKgTriangle> pTriangleMeshes)
{
	m_nType = GP_3DTYPE_TRIANGLE;
	m_fgColor = fgColor;
	m_camera = p_camera;
	m_WorldPos = pWorldPos;
	rx = 0.;ry = 0.;rz = 0.;

	m_ProjectionMatrix = dmatrix(4, 4);
	for(int r = 0 ; r < 4 ; ++r)
		for(int c = 0 ; c < 4 ; ++c)
			m_ProjectionMatrix[r][c] = 0.;

	m_RotationMatrix = dmatrix(4, 4);
	for (int r = 0; r < 4; ++r)
		for (int c = 0; c < 4; ++c)
			m_RotationMatrix[r][c] = (r == c) ? 1. : 0.;

	m_ProjectionMatrix[0][0] = (double)nH/(double)nW * 1./tan(Fov/2.);
	m_ProjectionMatrix[1][1] = 1./tan(Fov/2.);
	m_ProjectionMatrix[2][2] = (-Near-Far) / (Near-Far);
	m_ProjectionMatrix[2][3] = 2.*(Far * Near) / (Near-Far);
	m_ProjectionMatrix[3][2] = 1.;
	m_ProjectionMatrix[3][3] = 0.;	
	
	for (auto& triangle : pTriangleMeshes)
	{
		SurfaceMesh.push_back(triangle);
	}
};

CKhuGle3DSprite::~CKhuGle3DSprite() {
	free_dmatrix(m_ProjectionMatrix, 4, 4);
	free_dmatrix(m_RotationMatrix, 4, 4);
};

void CKhuGle3DSprite::DrawTriangle(unsigned char **R, unsigned char **G, unsigned char **B, int nW, int nH, 
	int x0, int y0, int x1, int y1, int x2, int y2, KgColor24 Color24)
{
	CKhuGleSprite::DrawLine(R, G, B, nW, nH, x0, y0, x1, y1, Color24);
	CKhuGleSprite::DrawLine(R, G, B, nW, nH, x1, y1, x2, y2, Color24);
	CKhuGleSprite::DrawLine(R, G, B, nW, nH, x2, y2, x0, y0, Color24);	
}

void CKhuGle3DSprite::MatrixVector44(CKgVector3D &out, CKgVector3D v, double **M)
{
	out.x = v.x*M[0][0] + v.y*M[0][1] + v.z*M[0][2] + M[0][3];
	out.y = v.x*M[1][0] + v.y*M[1][1] + v.z*M[1][2] + M[1][3];
	out.z = v.x*M[2][0] + v.y*M[2][1] + v.z*M[2][2] + M[2][3];

	double w = v.x*M[3][0] + v.y*M[3][1] + v.z*M[3][2] + M[3][3];

	if(fabs(w) > 0)
		out = (1./w)*out;
}

void CKhuGle3DSprite::Render()
{
	if(!m_Parent) return;

	CKgVector3D m_camera_pos = m_camera->m_position;

	CKhuGleLayer *Parent = (CKhuGleLayer *)m_Parent;

	auto ViewMatrix = m_camera->m_view_matrix;

	if(ViewMatrix == nullptr) return;

	if (m_nType == GP_3DTYPE_LINE)
	{
		CKgVector3D worldPos = m_Line;
		MatrixVector44(worldPos, m_Line, m_RotationMatrix);
		worldPos += m_WorldPos;
		
		CKgVector3D ViewPos;
		CKgVector3D ViewLine;
		CKgVector3D ProjectedPos;
		CKgVector3D ProjectedLine;

		MatrixVector44(ViewPos, m_WorldPos, ViewMatrix);
		MatrixVector44(ViewLine, worldPos, ViewMatrix);

		MatrixVector44(ProjectedPos, ViewPos, m_ProjectionMatrix);
		MatrixVector44(ProjectedLine, ViewLine, m_ProjectionMatrix);

		ProjectedPos.x += 1.;
		ProjectedPos.y += 1.;
		ProjectedLine.x += 1.;
		ProjectedLine.y += 1.;
		ProjectedPos.x *= Parent->m_nW / 2.;
		ProjectedPos.y *= Parent->m_nH / 2.;
		ProjectedLine.x *= Parent->m_nW / 2.;
		ProjectedLine.y *= Parent->m_nH / 2.;
		
		DrawLine(Parent->m_ImageR, Parent->m_ImageG, Parent->m_ImageB,
			Parent->m_nW, Parent->m_nH, (int)ProjectedPos.x, (int)ProjectedPos.y, (int)ProjectedLine.x, (int)ProjectedLine.y, m_fgColor);
	}
	else {
		for (auto Triangle : SurfaceMesh)
		{
			CKgTriangle worldPos;
			MatrixVector44(worldPos.v0, Triangle.v0, m_RotationMatrix);
			MatrixVector44(worldPos.v1, Triangle.v1, m_RotationMatrix);
			MatrixVector44(worldPos.v2, Triangle.v2, m_RotationMatrix);

			worldPos.v0 = worldPos.v0 + m_WorldPos;
			worldPos.v1 = worldPos.v1 + m_WorldPos;
			worldPos.v2 = worldPos.v2 + m_WorldPos;
			
		
			CKgVector3D Side01, Side02, Normal;

			Side01 = worldPos.v1 - worldPos.v0;
			Side02 = worldPos.v2 - worldPos.v0;

			Normal = Side01.Cross(Side02);
			Normal.Normalize();

			CKgTriangle ViewTriangle;
			CKgTriangle Projected;

			if (Normal.Dot(worldPos.v0 - m_camera_pos) < 0.)
			{
				MatrixVector44(ViewTriangle.v0, worldPos.v0, ViewMatrix);
				MatrixVector44(ViewTriangle.v1, worldPos.v1, ViewMatrix);
				MatrixVector44(ViewTriangle.v2, worldPos.v2, ViewMatrix);

				MatrixVector44(Projected.v0, ViewTriangle.v0, m_ProjectionMatrix);
				MatrixVector44(Projected.v1, ViewTriangle.v1, m_ProjectionMatrix);
				MatrixVector44(Projected.v2, ViewTriangle.v2, m_ProjectionMatrix);
				

				/*MatrixVector44(Projected.v0, Triangle.v0, m_ProjectionMatrix);
				MatrixVector44(Projected.v1, Triangle.v1, m_ProjectionMatrix);
				MatrixVector44(Projected.v2, Triangle.v2, m_ProjectionMatrix);*/

				Projected.v0.x += 1.;
				Projected.v0.y += 1.;
				Projected.v1.x += 1.;
				Projected.v1.y += 1.;
				Projected.v2.x += 1.;
				Projected.v2.y += 1.;
				Projected.v0.x *= Parent->m_nW / 2.;
				Projected.v0.y *= Parent->m_nH / 2.;
				Projected.v1.x *= Parent->m_nW / 2.;
				Projected.v1.y *= Parent->m_nH / 2.;
				Projected.v2.x *= Parent->m_nW / 2.;
				Projected.v2.y *= Parent->m_nH / 2.;

				DrawTriangle(Parent->m_ImageR, Parent->m_ImageG, Parent->m_ImageB,
					Parent->m_nW, Parent->m_nH,
					(int)Projected.v0.x, (int)Projected.v0.y,
					(int)Projected.v1.x, (int)Projected.v1.y,
					(int)Projected.v2.x, (int)Projected.v2.y, m_fgColor);
			}
		}
	}

	//free_dmatrix(ViewMatrix, 4, 4);
}

void CKhuGle3DSprite::MoveBy(double OffsetX, double OffsetY, double OffsetZ)
{
	for(auto &Triangle: SurfaceMesh)
	{
		Triangle.v0 = Triangle.v0 + CKgVector3D(OffsetX, OffsetY, OffsetZ);
		Triangle.v1 = Triangle.v1 + CKgVector3D(OffsetX, OffsetY, OffsetZ);
		Triangle.v2 = Triangle.v2 + CKgVector3D(OffsetX, OffsetY, OffsetZ);
	}
}

void CKhuGle3DSprite::localRotateBy(double RadianX, double RadianY, double RadianZ)
{
	double c_x, c_y, c_z, s_x, s_y, s_z;
	rx += RadianX;
	ry += RadianY;
	rz += RadianZ;
	c_x = cos(rx); c_y = cos(ry); c_z = cos(rz);
	s_x = sin(rx); s_y = sin(ry); s_z = sin(rz);

	m_RotationMatrix[0][0] = c_z * c_y;
	m_RotationMatrix[1][0] = s_z * c_y;
	m_RotationMatrix[2][0] = -s_y;
	m_RotationMatrix[3][0] = 0.;
	m_RotationMatrix[0][1] = -s_z * c_x + c_z * s_y * s_x;
	m_RotationMatrix[1][1] = c_z * c_x + s_z * s_y * s_x;
	m_RotationMatrix[2][1] = c_y * s_x;
	m_RotationMatrix[3][1] = 0.;
	m_RotationMatrix[0][2] = s_z * s_x + c_z * s_y * c_x;
	m_RotationMatrix[1][2] = -c_z * s_x + s_z * s_y * c_x;
	m_RotationMatrix[2][2] = c_y * c_x;
	m_RotationMatrix[3][2] = 0.;
	m_RotationMatrix[0][3] = 0.;
	m_RotationMatrix[1][3] = 0.;
	m_RotationMatrix[2][3] = 0.;
	m_RotationMatrix[3][3] = 1.;
}

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
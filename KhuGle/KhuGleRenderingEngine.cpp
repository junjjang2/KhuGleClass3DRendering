

//
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//

#include "KhuGleRenderingEngine.h"
#include "KhuGleLayer.h"
#include "KhuGleScene.h"

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <cstring>
#include <crtdbg.h>

#include "KhuGle3DSprite.h"

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

KhuGleRenderingEngine::KhuGleRenderingEngine(int nW, int nH, KgColor24 bgColor)
{
	m_bInit = false;

	SetBackgroundImage(nW, nH, bgColor);
}

KhuGleRenderingEngine::~KhuGleRenderingEngine()
{
	ResetBackgroundImage();
}

void KhuGleRenderingEngine::SetBackgroundImage(int nW, int nH, KgColor24 bgColor)
{
	if (m_bInit) ResetBackgroundImage();

	m_nW = nW;
	m_nH = nH;

	m_bgColor = bgColor;

	m_ImageR = cmatrix(m_nH, m_nW);
	m_ImageG = cmatrix(m_nH, m_nW);
	m_ImageB = cmatrix(m_nH, m_nW);

	int x, y;
	for (y = 0; y < m_nH; y++)
		for (x = 0; x < m_nW; x++)
		{
			m_ImageR[y][x] = KgGetRed(bgColor);
			m_ImageG[y][x] = KgGetGreen(bgColor);
			m_ImageB[y][x] = KgGetBlue(bgColor);
		}

	m_bInit = true;
}

void KhuGleRenderingEngine::ResetBackgroundImage()
{
	if (m_bInit)
	{
		free_cmatrix(m_ImageR, m_nH, m_nW);
		free_cmatrix(m_ImageG, m_nH, m_nW);
		free_cmatrix(m_ImageB, m_nH, m_nW);

		m_bInit = false;
	}
}

void KhuGleRenderingEngine::SetBgColor(KgColor24 bgColor)
{
	m_bgColor = bgColor;
}

/*
void KhuGleRenderingEngine::calcVertexes()
{
	for (auto& triangle : triangles)
	{

		auto worldPosMat = dmatrix(4, 4);
		auto ProjectionMat = dmatrix(4, 4);
		auto TriangleMatrix = dmatrix(4, 4);

		TriangleMatrix[0][0] = triangle.v0.x;
		TriangleMatrix[1][0] = triangle.v0.y;
		TriangleMatrix[2][0] = triangle.v0.z;
		TriangleMatrix[3][0] = 1.;
		TriangleMatrix[0][1] = triangle.v1.x;
		TriangleMatrix[1][1] = triangle.v1.y;
		TriangleMatrix[2][1] = triangle.v1.z;
		TriangleMatrix[3][1] = 1.;
		TriangleMatrix[0][2] = triangle.v2.x;
		TriangleMatrix[1][2] = triangle.v2.y;
		TriangleMatrix[2][2] = triangle.v2.z;
		TriangleMatrix[3][2] = 1.;
		TriangleMatrix[0][3] = 0.;
		TriangleMatrix[1][3] = 0.;
		TriangleMatrix[2][3] = 0.;
		TriangleMatrix[3][3] = 0.;

		// Scale  WIP

		// Rotation
		MatrixMultiply44(worldPosMat, m_rotation_matrix, TriangleMatrix);

		// Translation
		for (int i = 0; i < 3; i++)
		{
			worldPosMat[i][0] += m_WorldPos.x;
			worldPosMat[i][1] += m_WorldPos.y;
			worldPosMat[i][2] += m_WorldPos.z;
		}

		// check condition 
		CKgVector3D Side01, Side02, Normal;

		Side01 = worldPos.v1 - worldPos.v0;
		Side02 = worldPos.v2 - worldPos.v0;

		Normal = Side01.Cross(Side02);
		Normal.Normalize();

		// Find if Target is in FoV
		auto Target = worldPos.v0 - m_camera_pos;
		Target.Normalize();

		auto normal_front = m_camera->m_forward;
		MatrixMultiply44(normal_front, m_camera->m_forward, m_camera->m_rotation_matrix);

		normal_front.Normalize();
		double angle = asin(Target.Dot(normal_front));
		// Find if Target is in FoV
		bool isRenderable = Normal.Dot(worldPos.v0 - m_camera_pos) < 0.;
		if (!isRenderable)	// && 0 <= angle && angle <= m_Fov) // render if normal vector of triangle is opposite to camera's forward vector,
			return result;	//			 and angle between forward vector and target vector is smaller than FOV


		auto ViewProjectionMatrix = m_camera->m_view_projection_matrix;
		MatrixMultiply44(ProjectionMat, ViewProjectionMatrix, worldPosMat);

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++) {
				ProjectionMat[j][i] *= (1. / ProjectionMat[3][i]);
			}
			ProjectionMat[0][i] += 1;
			ProjectionMat[1][i] += 1;
			ProjectionMat[2][i] += 1;
			ProjectionMat[0][i] *= Parent->m_nW / 2.;
			ProjectionMat[1][i] *= Parent->m_nW / 2.;
			ProjectionMat[2][i] *= Parent->m_nW / 2.;
		}

		for (int i = 0; i < 3; i++)
		{
			if (i < 2)
				result.push_back(render_inform((int)ProjectionMat[0][i], (int)ProjectionMat[1][i], (int)ProjectionMat[0][i + 1], (int)ProjectionMat[1][i + 1], m_fgColor));
			else
				result.push_back(render_inform((int)ProjectionMat[0][i], (int)ProjectionMat[1][i], (int)ProjectionMat[0][0], (int)ProjectionMat[1][0], m_fgColor));
		}
	}

	//Drawing  /// TEMP

	for (auto& r_inform : result)
	{
		//std::cout << r_inform.start_x << " " << r_inform.end_x << std::endl;
		DrawLine(Parent->m_ImageR, Parent->m_ImageG, Parent->m_ImageB, Parent->m_nW, Parent->m_nH, r_inform.start_x, r_inform.start_y, r_inform.end_x, r_inform.end_y, r_inform.m_color);
	}
}
*/

void KhuGleRenderingEngine::Render()
{
	int y;
	for (y = 0; y < m_nH; y++)
	{
		memset(m_ImageR[y], KgGetRed(m_bgColor), m_nW);
		memset(m_ImageG[y], KgGetGreen(m_bgColor), m_nW);
		memset(m_ImageB[y], KgGetBlue(m_bgColor), m_nW);
	}

	for (auto& Child : m_Children)
	{
		CKhuGleLayer* Layer = (CKhuGleLayer*)Child;

		Layer->Render();

		for (int y = 0; y < Layer->m_nH; ++y)
		{
			if (y + Layer->m_ptPos.Y >= m_nH) break;

			int nLen = std::min(Layer->m_nW, m_nW - Layer->m_ptPos.X);

			if (nLen <= 0) continue;
			memcpy(m_ImageR[y + Layer->m_ptPos.Y] + Layer->m_ptPos.X, Layer->m_ImageR[y], nLen);
			memcpy(m_ImageG[y + Layer->m_ptPos.Y] + Layer->m_ptPos.X, Layer->m_ImageG[y], nLen);
			memcpy(m_ImageB[y + Layer->m_ptPos.Y] + Layer->m_ptPos.X, Layer->m_ImageB[y], nLen);
		}
	}
}

std::vector<double> KhuGleRenderingEngine::getRenderInform()
{
	return std::vector<double>(0, 0.);

}


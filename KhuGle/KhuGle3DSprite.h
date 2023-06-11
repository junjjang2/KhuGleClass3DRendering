#pragma once

#include "KhuGleSprite.h"
#include "KhuGleBase.h"

int constexpr GP_3DTYPE_LINE = 0;
int constexpr GP_3DTYPE_TRIANGLE = 1;



struct render_inform
{
	render_inform(int start_x, int start_y, int end_x, int end_y, KgColor24 m_color)
		: start_x(start_x),
		  start_y(start_y),
		  end_x(end_x),
		  end_y(end_y),
		  m_color(m_color)
	{
	}
	int start_x, start_y, end_x, end_y;
	KgColor24 m_color;
};


class CKhuGle3DObject : public CKhuGleSprite
{
public:
	std::vector<CKgTriangle> SurfaceMesh;
	double** m_projection_matrix;
	double** m_rotation_matrix;
	double rx, ry, rz; // rotation
	double sx, sy, sz; // scale

	CKgVector3D m_WorldPos;

	CKhuGle3DObject(KgColor24 fgColor, CKgVector3D pWorldPos);
	~CKhuGle3DObject() override;

	static void Draw(unsigned char** R, unsigned char** G, unsigned char** B, int nW, int nH,
		int x0, int y0, int x1, int y1, int x2, int y2, KgColor24 Color24);

	static void MatrixVector44(CKgVector3D& out, CKgVector3D v, double** M);
	
	void MoveBy(double OffsetX, double OffsetY, double OffsetZ);
	void RotateBy(double RadianX, double RadianY, double RadianZ);
};

class CKhuGleCamera : public CKhuGle3DObject
{
public:
	CKgVector3D m_forward;
	CKgVector3D m_forward_base;
	CKgVector3D m_up;

	double** m_view_matrix;
	double** m_projection_matrix;
	
	CKhuGleCamera(int nW, int nH, double Fov, double Far, double Near, const CKgVector3D& m_position, const CKgVector3D& m_forward, const CKgVector3D& m_up);
	~CKhuGleCamera();

	double** ComputeViewMatrix();

	void MoveBy(double OffsetX, double OffsetY, double OffsetZ);
	void RotateBy(double dRadianX, double dRadianY, double dRadianZ);
};

class CKhuGle3DSprite : public CKhuGle3DObject
{
public:
	CKhuGle3DSprite(KgColor24 fgColor, CKgVector3D pWorldPos, CKhuGleCamera* c, char* FilePath);
	CKhuGle3DSprite(KgColor24 fgColor, CKgVector3D pWorldPos, std::vector<CKgTriangle>& pTriangleMeshes, CKhuGleCamera* c);
	~CKhuGle3DSprite();

	CKhuGleCamera* m_camera;

	std::vector<CKgVector2D> texture_list;
	std::vector<CKgVector3D> vertex_list;
	std::vector<std::vector<int>> triangle_list;

	void Render();
	static void DrawTriangle(unsigned char** R, unsigned char** G, unsigned char** B, int nW, int nH,
		int x0, int y0, int x1, int y1, int x2, int y2, KgColor24 Color24);

	//void MoveBy(double OffsetX, double OffsetY, double OffsetZ);
	//void RotateBy(double RadianX, double RadianY, double RadianZ);

	void ReadObj(char* FilePath, std::vector<std::vector<double>>& vertex_list, std::vector<std::vector<int>>& triangle_list);
};

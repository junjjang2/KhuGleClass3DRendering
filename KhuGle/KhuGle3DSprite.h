#pragma once

#include "KhuGleSprite.h"

int constexpr GP_3DTYPE_LINE = 0;
int constexpr GP_3DTYPE_TRIANGLE = 1;

struct CKgTriangle {
	CKgVector3D v0, v1, v2;
	bool bFill;
	CKgTriangle() : v0(CKgVector3D()), v1(CKgVector3D()), v2(CKgVector3D()), bFill(false) {}
	CKgTriangle(CKgVector3D vv0, CKgVector3D vv1, CKgVector3D vv2, bool bFill) : v0(vv0), v1(vv1), v2(vv2), bFill(bFill) {}
};
struct CKg3DLine
{
	CKgVector3D start, end;

	CKg3DLine() : start(CKgVector3D()), end(CKgVector3D()) {}
	CKg3DLine(CKgVector3D start, CKgVector3D direction) : start(start), end(start+direction) {}
};

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
//struct CKgCamera
//{
//	CKgVector3D m_position;
//	CKgVector3D m_forward;
//	CKgVector3D m_up;
//	double** m_view_matrix;
//	double** m_rotation_matrix;
//	double rx, ry, rz;
//
//	CKgCamera(const CKgVector3D& m_position, const CKgVector3D& m_forward, const CKgVector3D& m_up);
//
//	void MoveBy(double OffsetX, double OffsetY, double OffsetZ);
//
//	void RotateBy(double dRadianX, double dRadianY, double dRadianZ);
//
//	double** ComputeViewMatrix();
//	void MatrixVector44(CKgVector3D& out, CKgVector3D v, double** M);
//};

//class CKhuGle3DSprite : public CKhuGleSprite {
//public:
//	int m_nType;
//	std::vector<CKgTriangle> SurfaceMesh;
//	double** m_ProjectionMatrix;
//	double** m_RotationMatrix;
//	double rx, ry, rz;
//	double m_Fov;
//	CKgCamera* m_camera;
//	CKgVector3D m_Line;
//
//	CKgVector3D m_WorldPos;
//
//	CKhuGle3DSprite(int nW, int nH, double Fov, double Far, double Near, KgColor24 fgColor, CKgCamera* p_camera, CKgVector3D pWorldPos, CKgVector3D Line);
//	CKhuGle3DSprite(int nW, int nH, double Fov, double Far, double Near, KgColor24 fgColor, CKgCamera* p_camera, CKgVector3D pWorldPos, std::vector<CKgTriangle> pTriangleMeshes);
//	~CKhuGle3DSprite();
//
//	static void DrawTriangle(unsigned char** R, unsigned char** G, unsigned char** B, int nW, int nH,
//		int x0, int y0, int x1, int y1, int x2, int y2, KgColor24 Color24);
//	static void MatrixVector44(CKgVector3D& out, CKgVector3D v, double** M);
//	//static double **ComputeViewMatrix(CKgCamera Camera);
//
//	void Render();
//	void MoveBy(double OffsetX, double OffsetY, double OffsetZ);
//	void localRotateBy(double RadianX, double RadianY, double RadianZ);
//};

class CKhuGle3DObject : public CKhuGleSprite
{
public:
	int m_nType;
	std::vector<CKgTriangle> SurfaceMesh;
	//double** m_ProjectionMatrix;
	double** m_rotation_matrix;
	double rx, ry, rz;

	CKgVector3D m_WorldPos;

	//CKhuGle3DObject();
	CKhuGle3DObject(KgColor24 fgColor, CKgVector3D pWorldPos);
	~CKhuGle3DObject() override;

	static void Draw(unsigned char** R, unsigned char** G, unsigned char** B, int nW, int nH,
		int x0, int y0, int x1, int y1, int x2, int y2, KgColor24 Color24);

	static void MatrixVector44(CKgVector3D& out, CKgVector3D v, double** M);

	virtual std::vector<double> getRenderInform();
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
	double** m_view_projection_matrix;
	
	CKhuGleCamera(int nW, int nH, double Fov, double Far, double Near, const CKgVector3D& m_position, const CKgVector3D& m_forward, const CKgVector3D& m_up);
	~CKhuGleCamera();

	double** ComputeViewMatrix();

	//void Render();
	void MoveBy(double OffsetX, double OffsetY, double OffsetZ);
	void RotateBy(double dRadianX, double dRadianY, double dRadianZ);
};

class CKhuGle3DSprite : public CKhuGle3DObject
{
public:
	CKhuGle3DSprite(KgColor24 fgColor, CKgVector3D pWorldPos, CKhuGleCamera* c, char* FilePath);
	CKhuGle3DSprite(KgColor24 fgColor, CKgVector3D pWorldPos, CKgVector3D Line, CKhuGleCamera* c);
	CKhuGle3DSprite(KgColor24 fgColor, CKgVector3D pWorldPos, std::vector<CKgTriangle> pTriangleMeshes, CKhuGleCamera* c);
	~CKhuGle3DSprite();

	CKhuGleCamera* m_camera;

	CKgVector3D m_Line;

	std::vector<std::vector<double>> vertex_list;
	std::vector<std::vector<int>> triangle_list;

	std::vector<render_inform> Render(CKgVector3D object_position, double** ViewProjectionMatrix, CKgTriangle triangle) const;
	void Render();
	static void DrawTriangle(unsigned char** R, unsigned char** G, unsigned char** B, int nW, int nH,
		int x0, int y0, int x1, int y1, int x2, int y2, KgColor24 Color24);

	//	void Render();
	void MoveBy(double OffsetX, double OffsetY, double OffsetZ);
	void RotateBy(double RadianX, double RadianY, double RadianZ);

	void ReadObj(char* FilePath, std::vector<std::vector<double>>& vertex_list, std::vector<std::vector<int>>& triangle_list);
};

//class CKhuGlePortal : public CKhuGle3DObject
//{
//public:
//	CKgVector3D secondWorldPos;
//	CKhuGleCamera* first_camera;
//	CKhuGleCamera* second_camera;
//
//	CKhuGlePortal(int nW, int nH, double Fov, double Far, double Near, KgColor24 fgColor, const CKgVector3D& pWorldPos, const CKgVector3D psecondWorldPos, CKhuGleCamera *firstCamera)
//		: CKhuGle3DObject(fgColor, pWorldPos), secondWorldPos(psecondWorldPos)
//	{
//		first_camera = firstCamera;
//		second_camera = new CKhuGleCamera(nW,nH,Fov,Far,Near,secondWorldPos, firstCamera->m_forward, firstCamera->m_up);
//	}
//	CKhuGlePortal();
//
//	void Render();
//};
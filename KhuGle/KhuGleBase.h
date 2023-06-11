//
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//


#pragma once

#include <algorithm>
#include <cmath>
#include <vector>

#define Pi	3.14159

typedef unsigned long KgColor24;
#define KG_COLOR_24_RGB(R, G, B)		((unsigned long)(((unsigned char)(R)|((unsigned short)((unsigned char)(G))<<8))|(((unsigned long)(unsigned char)(B))<<16)))

#define KgGetRed(RGB)			((RGB)& 0xff)
#define KgGetGreen(RGB)			((((unsigned short)(RGB)) >> 8)& 0xff)
#define KgGetBlue(RGB)			(((RGB)>>16)& 0xff)

struct CKgPoint {
	int X, Y;

	CKgPoint() {}
	CKgPoint(int x, int y) : X(x), Y(y) {}

	CKgPoint operator+ (CKgPoint p1);
	CKgPoint &operator+= (CKgPoint p1);
};

struct CKgLine {
	CKgPoint Start, End;

	CKgLine() {}
	CKgLine(CKgPoint s, CKgPoint e) : Start(s), End(e) {}
	CKgLine(int sx, int sy, int ex, int ey) : Start(CKgPoint(sx, sy)), End(CKgPoint(ex, ey)) {}
};

struct CKgRect {
	int Left, Top, Right, Bottom;

	CKgRect() : Left(0), Top(0), Right(0), Bottom(0) {}
	CKgRect(int l, int t, int r, int b) : Left(l), Top(t), Right(r), Bottom(b) {}

	bool IsRect();
	int Width();
	int Height();
	CKgPoint Center();
	void Move(int x, int y);
	void Intersect(CKgRect rt);
	void Union(CKgRect rt);
	void Expanded(int e);
};

class CKgVector2D 
{
public:
	double x, y;

	CKgVector2D() : x(0.), y(0.) {}
	CKgVector2D(double xx, double yy) : x(xx), y(yy) {}
	CKgVector2D(CKgPoint pt) : x(pt.X), y(pt.Y) {}

	static double abs(CKgVector2D v);
	void Normalize();
	double Dot(CKgVector2D v1);
	CKgVector2D operator+ (CKgVector2D v);
	CKgVector2D operator- (CKgVector2D v);
	CKgVector2D operator- ();
	CKgVector2D &operator+= (CKgVector2D v);
};
CKgVector2D operator*(double s, CKgVector2D v);

class CKgVector3D 
{
public:
	double x, y, z, w;

	CKgVector3D() : x(0.), y(0.), z(0.), w(1.) {}
	CKgVector3D(double xx, double yy, double zz) : x(xx), y(yy), z(zz), w(1.) {}
	CKgVector3D(double xx, double yy, double zz, double ww) : x(xx), y(yy), z(zz), w(ww) {}

	static double abs(CKgVector3D v);
	void Normalize();
	double Dot(CKgVector3D v1);
	CKgVector3D Cross(CKgVector3D v);
	CKgVector3D operator+ (CKgVector3D v);
	CKgVector3D operator- (CKgVector3D v);
	CKgVector3D operator- ();
	CKgVector3D &operator+= (CKgVector3D v);
};
CKgVector3D operator*(double s, CKgVector3D v);

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
	CKg3DLine(CKgVector3D start, CKgVector3D direction) : start(start), end(start + direction) {}
};


unsigned char **cmatrix(int nH, int nW);
void free_cmatrix(unsigned char **Image, int nH, int nW);
double **dmatrix(int nH, int nW);
void free_dmatrix(double **Image, int nH, int nW);

void DrawLine(unsigned char **ImageGray, int nW, int nH, int x0, int y0, int x1, int y1, unsigned char Color);

bool InverseMatrix(double  **a, double **y, int nN);

void MatrixMultiply44(double** result, double** mat1, double** mat2);

void DrawTriangle_Raw(unsigned char** R, unsigned char** G, unsigned char** B, double** depth, int nW, int nH, int x0, int y0, double z0, int x1, int y1, double z1, int x2, int y2, double z2, unsigned char Color, bool bFill);

bool linePlaneIntersection(CKgVector3D& contact, CKgVector3D ray, CKgVector3D rayOrigin, CKgVector3D normal, CKgVector3D coord);

int Triangle_ClipAgainstPlain(CKgVector3D plane_pos, CKgVector3D plane_normal, CKgTriangle triangle, std::vector<CKgTriangle>& new_tris);

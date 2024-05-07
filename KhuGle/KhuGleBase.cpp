//
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//
#include "KhuGleBase.h"
#include <cmath>
#include <vector>

#pragma warning(disable:4996)

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#include <iostream>
#include <fstream>
#include <string>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

CKgPoint CKgPoint::operator+ (CKgPoint p1) 
{
	return CKgPoint(X+p1.X, Y+p1.Y);
}

CKgPoint &CKgPoint::operator+= (CKgPoint p1) 
{
	*this = *this + p1;
	return *this;
}

bool CKgRect::IsRect() 
{
	if(Width() <= 0) return false;
	if(Height() <= 0) return false;
	return true;
}

int CKgRect::Width()
{
	return Right-Left;
}

int CKgRect::Height()
{
	return Bottom-Top;
}

CKgPoint CKgRect::Center()
{
	return CKgPoint((Left+Right)/2, (Top+Bottom)/2);
}

void CKgRect::Move(int x, int y)
{
	Left += x;
	Top += y;
	Right += x;
	Bottom += y;
}

void CKgRect::Intersect(CKgRect rt)
{
	Left = std::max(Left, rt.Left);
	Top = std::max(Top, rt.Top);
	Right = std::min(Right, rt.Right);
	Bottom = std::min(Bottom, rt.Bottom);
}

void CKgRect::Union(CKgRect rt)
{
	Left = std::min(Left, rt.Left);
	Top = std::min(Top, rt.Top);
	Right = std::max(Right, rt.Right);
	Bottom = std::max(Bottom, rt.Bottom);
}

void CKgRect::Expanded(int e)
{
	Left -= e;
	Top -= e;
	Right += e;
	Bottom += e;
}

double CKgVector2D::abs(CKgVector2D v) {
	return sqrt(v.x*v.x + v.y*v.y);
}

void CKgVector2D::Normalize() {
	double Magnitude = abs(*this);

	if(Magnitude == 0) return;

	x /= Magnitude;
	y /= Magnitude;
}

double CKgVector2D::Dot(CKgVector2D v)
{
	return x*v.x + y*v.y;
}

CKgVector2D CKgVector2D::operator+ (CKgVector2D v) 
{
	return CKgVector2D(x+v.x, y+v.y);
}

CKgVector2D CKgVector2D::operator- (CKgVector2D v) 
{
	return CKgVector2D(x-v.x, y-v.y);
}

CKgVector2D CKgVector2D::operator- () 
{
	return CKgVector2D(-x, -y);
}

CKgVector2D &CKgVector2D::operator+= (CKgVector2D v) 
{
	*this = *this + v;
	return *this;
}

CKgVector2D operator*(double s, CKgVector2D v)
{
	return CKgVector2D(s*v.x, s*v.y);
}

double CKgVector3D::abs(CKgVector3D v)
{
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

void CKgVector3D::Normalize()
{
	double Magnitude = abs(*this);

	if(Magnitude == 0) return;

	x /= Magnitude;
	y /= Magnitude;
	z /= Magnitude;
}

double CKgVector3D::Dot(CKgVector3D v)
{
	return x*v.x + y*v.y + z*v.z;
}

CKgVector3D CKgVector3D::Cross(CKgVector3D v)
{
	CKgVector3D NewV;

	NewV.x = y*v.z - z*v.y;
	NewV.y = z*v.x - x*v.z;
	NewV.z = x*v.y - y*v.x;

	return NewV;
}

CKgVector3D CKgVector3D::operator+ (CKgVector3D v)
{
	return CKgVector3D(x+v.x, y+v.y, z+v.z);
}

CKgVector3D CKgVector3D::operator- (CKgVector3D v)
{
	return CKgVector3D(x-v.x, y-v.y, z-v.z);
}

CKgVector3D CKgVector3D::operator- ()
{
	return CKgVector3D(-x, -y, -z);
}

CKgVector3D &CKgVector3D::operator+= (CKgVector3D v)
{
	*this = *this + v;
	return *this;
}

CKgVector3D operator*(double s, CKgVector3D v)
{
	return CKgVector3D(s*v.x, s*v.y, s*v.z, s*v.w);
}

ColorMap::ColorMap(char* FilePath)
{
	std::ifstream f_s;
	f_s.open(FilePath);

	std::string LineString = "";
	std::string Delimeter = " ";
	
	f_s >> n_W >> n_H;

	color_map = new unsigned long* [n_H];
	for(int y=0; y < n_H; y++)
	{
		color_map[y] = new unsigned long[n_W];
		std::vector<int> RowInt;

		unsigned int nPos = 0, nFindPos;
		std::string type;
		unsigned int npos = 4294967295;

		unsigned c;

		for(int x=0; x < n_W; x++)
		{
			f_s >> std::hex >> c;
			color_map[y][x] = c;
		}
	}

	std::cout << "sprite FIle load success : " << FilePath << std::endl;

}

ColorMap::~ColorMap()
{
	for (int y = 0; y < n_H; y++) {
		delete[] color_map[y];
	}
	delete[] color_map;
}


unsigned long ColorMap::getColor(double u, double v)
{
	int x = std::lround(u * (n_W-1));
	int y = std::lround(v * (n_H-1));
	return color_map[x][y];
}

unsigned char **cmatrix(int nH, int nW) {
	unsigned char **Temp;

	Temp = new unsigned char *[nH];
	for(int y = 0 ; y < nH ; y++)
		Temp[y] = new unsigned char[nW];

	return Temp;
}

void free_cmatrix(unsigned char **Image, int nH, int nW) {
	for(int y = 0 ; y < nH ; y++)
		delete [] Image[y];

	delete [] Image;
}

double **dmatrix(int nH, int nW) {
	double **Temp;

	Temp = new double *[nH];
	for(int y = 0 ; y < nH ; y++)
		Temp[y] = new double[nW];

	return Temp;
}

void free_dmatrix(double **Image, int nH, int nW) {
	for(int y = 0 ; y < nH ; y++)
		delete [] Image[y];

	delete [] Image;
}

void DrawLine(unsigned char** ImageGray, int nW, int nH, int x0, int y0, int x1, int y1, unsigned char Color)
{
	int nDiffX = abs(x0 - x1);
	int nDiffY = abs(y0 - y1);

	int x, y;
	int nFrom, nTo;
	

	if (nDiffY == 0 && nDiffX == 0)
	{
		y = y0;
		x = x0;

		if (!(x < 0 || x >= nW || y < 0 || y >= nH)) {
				ImageGray[y][x] = Color;
			
		}
	}
	else if (nDiffX == 0)
	{
		x = x0;

		nFrom = (y0 < y1 ? y0 : y1);
		if (nFrom < 0) nFrom = 0;
		nTo = (y0 < y1 ? y1 : y0);
		if (nTo >= nH) nTo = nH - 1;
		

		for (y = nFrom; y <= nTo; y++)
		{
			if (x < 0 || x >= nW || y < 0 || y >= nH) continue;
			ImageGray[y][x] = Color;
		}
	}
	else if (nDiffY == 0)
	{
		y = y0;

		nFrom = (x0 < x1 ? x0 : x1);
		if (nFrom < 0) nFrom = 0;
		nTo = (x0 < x1 ? x1 : x0);
		if (nTo >= nW) nTo = nW - 1;
		
		for (x = nFrom; x <= nTo; x++)
		{
			if (x < 0 || x >= nW || y < 0 || y >= nH) continue;
				ImageGray[y][x] = Color;
			
		}
	}
	else if (nDiffY > nDiffX)
	{
		nFrom = (y0 < y1 ? y0 : y1);
		if (nFrom < 0) nFrom = 0;
		nTo = (y0 < y1 ? y1 : y0);
		if (nTo >= nH) nTo = nH - 1;
		
		for (y = nFrom; y <= nTo; y++)
		{
			x = (y - y0) * (x0 - x1) / (y0 - y1) + x0;

			if (x < 0 || x >= nW || y < 0 || y >= nH) continue;
			
			ImageGray[y][x] = Color;
			
		}
	}
	else
	{
		nFrom = (x0 < x1 ? x0 : x1);
		if (nFrom < 0) nFrom = 0;
		nTo = (x0 < x1 ? x1 : x0);
		if (nTo >= nW) nTo = nW - 1;

		for (x = nFrom; x <= nTo; x++)
		{
			y = (x - x0) * (y0 - y1) / (x0 - x1) + y0;
			if (x < 0 || x >= nW || y < 0 || y >= nH) continue;
				ImageGray[y][x] = Color;
		}
	}
}

void DrawLine(unsigned char** ImageGray, double** depth, int nW, int nH, int x0, int y0, double z0, int x1, int y1, double z1, unsigned char Color)
{
	int nDiffX = abs(x0-x1);
	int nDiffY = abs(y0-y1);
	double  dDiffZ;

	int x, y;
	int nFrom, nTo;

	double z;
	double zFrom, zTo;

	if(nDiffY == 0 && nDiffX == 0)
	{
		y = y0;
		x = x0;
		z = z0;

		if (!(x < 0 || x >= nW || y < 0 || y >= nH)) {
			if (0 <= z && z <= 1.0 && depth[y][x] >= z)
			{
				depth[y][x] = z;
				ImageGray[y][x] = Color;
			}
		}
	}
	else if(nDiffX == 0)
	{
		x = x0;

		nFrom = (y0 < y1 ? y0 : y1);
		if(nFrom < 0) nFrom = 0;
		nTo = (y0 < y1 ? y1 : y0);
		if(nTo >= nH) nTo = nH-1;

		zFrom = (nFrom == y0 ? z0 : z1);
		zTo = (nFrom == y0 ? z1 : z0);
		dDiffZ = (zTo - zFrom) / (nTo - nFrom);
		z = zFrom - dDiffZ;


		for(y = nFrom ; y <= nTo ; y++)
		{
			z += dDiffZ;

			if(x < 0 || x >= nW || y < 0 || y >= nH) continue;
			if (0 <= z && z <= 1.0 && depth[y][x] >= z)
			{
				depth[y][x] = z;
				ImageGray[y][x] = Color;
			}
		}
	}
	else if(nDiffY == 0)
	{
		y = y0;

		nFrom = (x0 < x1 ? x0 : x1);
		if(nFrom < 0) nFrom = 0;
		nTo = (x0 < x1 ? x1 : x0);
		if(nTo >= nW) nTo = nW-1;

		zFrom = (nFrom == x0 ? z0 : z1);
		zTo = (nFrom == x0 ? z1 : z0);
		dDiffZ = (zTo - zFrom) / (nTo - nFrom);
		z = zFrom - dDiffZ;

		for(x = nFrom ; x <= nTo ; x++)
		{
			z += dDiffZ;

			if(x < 0 || x >= nW || y < 0 || y >= nH) continue;
			if (0 <= z && z <= 1.0 && depth[y][x] >= z)
			{
				depth[y][x] = z;
				ImageGray[y][x] = Color;
			}
		}
	}
	else if(nDiffY > nDiffX)
	{
		nFrom = (y0 < y1 ? y0 : y1);
		if(nFrom < 0) nFrom = 0;
		nTo = (y0 < y1 ? y1 : y0);
		if(nTo >= nH) nTo = nH-1;

		zFrom = (nFrom == y0 ? z0 : z1);
		zTo = (nFrom == y0 ? z1 : z0);
		dDiffZ = (zTo - zFrom) / (nTo - nFrom);
		z = zFrom - dDiffZ;

		for(y = nFrom ; y <= nTo ; y++)
		{
			x = (y-y0)*(x0-x1)/(y0-y1) + x0;
			z += dDiffZ;

			if(x < 0 || x >= nW || y < 0 || y >= nH) continue;
			if (0 <= z && z <= 1.0 && depth[y][x] >= z)
			{
				depth[y][x] = z;
				ImageGray[y][x] = Color;
			}
		}
	}
	else
	{
		nFrom = (x0 < x1 ? x0 : x1);
		if(nFrom < 0) nFrom = 0;
		nTo = (x0 < x1 ? x1 : x0);
		if(nTo >= nW) nTo = nW-1;

		zFrom = (nFrom == x0 ? z0 : z1);
		zTo = (nFrom == x0 ? z1 : z0);
		dDiffZ = (zTo - zFrom) / (nTo - nFrom);
		z = zFrom - dDiffZ;

		for(x = nFrom ; x <= nTo ; x++)
		{
			y = (x-x0)*(y0-y1)/(x0-x1) + y0;
			z += dDiffZ;

			if(x < 0 || x >= nW || y < 0 || y >= nH) continue;
			if (0 <= z && z <= 1.0 && depth[y][x] >= z)
			{
				depth[y][x] = z;
				ImageGray[y][x] = Color;
			}
		}
	}
}

void Bresenham(unsigned char** R, unsigned char** G, unsigned char** B, double** depth, int nW, int nH, int x0, int y0, double z0, int x1, int y1, double z1, int x2, int y2, double z2, unsigned long Color, bool bFill)
{
	int x_min = std::max(0, std::min({ x0, x1, x2 }));
	int x_max = std::min(nW - 1, std::max({ x0, x1, x2 }));
	int y_min = std::max(0, std::min({ y0, y1, y2 }));
	int y_max = std::min(nH - 1, std::max({ y0, y1, y2 }));

	if (x_min > x_max || y_min > y_max)
		return;

	std::vector<std::pair<int, int>> minmax(y_max - y_min + 1, { x_max, x_min });
	std::vector<std::pair<double, double>> depth_minmax(y_max - y_min + 1, { 1.0, 0. });

	int nDiffX;
	int nDiffY;
	double dDiffZ;

	int x, y;
	int nFrom, nTo;
	double z;
	double zFrom, zTo;

	std::vector<std::vector<int>> pair_list = { {x0, x1, y0, y1}, { x1, x2, y1, y2 }, { x2, x0, y2, y0 } };
	std::vector<std::vector<double>> dpair_list = { {z0, z1}, {z1, z2}, {z2, z0} };

	for (int i = 0; i < 3; i++)
	{
		auto& params = pair_list[i];
		auto& dparams = dpair_list[i];

		x0 = params[0];
		x1 = params[1];
		y0 = params[2];
		y1 = params[3];

		z0 = dparams[0];
		z1 = dparams[1]; 

		nDiffX = abs(x0 - x1);
		nDiffY = abs(y0 - y1);

		if (nDiffY == 0 && nDiffX == 0)
		{
			x = x0;
			y = y0;
			z = z0;
			if (!(x < 0 || x >= nW || y < 0 || y >= nH))
			{
				auto& x_left = minmax[y - y_min].first;
				auto& x_right = minmax[y - y_min].second;
				auto& z_left = depth_minmax[y - y_min].first;
				auto& z_right = depth_minmax[y - y_min].second;
				
				if (x < x_left)
				{
					x_left = x;
					z_left = z;
				}
				if (x > x_right)
				{
					x_right = x;
					z_right = z;
				}

				minmax[y - y_min] = {x_left, x_right };
				depth_minmax[y - y_min] = {z_left, z_right};
			}
		}
		else if (nDiffX == 0)
		{
			x = x0;

			nFrom = (y0 < y1 ? y0 : y1);
			if (nFrom < 0) nFrom = 0;
			nTo = (y0 < y1 ? y1 : y0);
			if (nTo >= nH) nTo = nH - 1;

			zFrom = (nFrom == y0 ? z0 : z1);
			zTo = (nFrom == y0 ? z1 : z0);
			dDiffZ = (zTo - zFrom) / (nTo - nFrom);
			z = zFrom - dDiffZ;

			for (y = nFrom; y <= nTo; y++)
			{
				z += dDiffZ;

				if (x < 0 || x >= nW || y < 0 || y >= nH) continue;

				auto& x_left = minmax[y - y_min].first;
				auto& x_right = minmax[y - y_min].second;
				auto& z_left = depth_minmax[y - y_min].first;
				auto& z_right = depth_minmax[y - y_min].second;

				if (x < x_left)
				{
					x_left = x;
					z_left = z;
				}
				if (x > x_right)
				{
					x_right = x;
					z_right = z;
				}
				minmax[y - y_min] = { x_left, x_right };
				depth_minmax[y - y_min] = { z_left, z_right };
			}
		}
		else if (nDiffY == 0)
		{
			y = y0;

			nFrom = (x0 < x1 ? x0 : x1);
			if (nFrom < 0) nFrom = 0;
			nTo = (x0 < x1 ? x1 : x0);
			if (nTo >= nW) nTo = nW - 1;

			zFrom = (nFrom == x0 ? z0 : z1);
			zTo = (nFrom == x0 ? z1 : z0);
			dDiffZ = (zTo - zFrom) / (nTo - nFrom);
			z = zFrom - dDiffZ;

			for (x = nFrom; x <= nTo; x++)
			{
				z += dDiffZ;

				if (x < 0 || x >= nW || y < 0 || y >= nH) continue;
				auto& x_left = minmax[y - y_min].first;
				auto& x_right = minmax[y - y_min].second;
				auto& z_left = depth_minmax[y - y_min].first;
				auto& z_right = depth_minmax[y - y_min].second;

				if (x < x_left)
				{
					x_left = x;
					z_left = z;
				}
				if (x > x_right)
				{
					x_right = x;
					z_right = z;
				}

				minmax[y - y_min] = { x_left, x_right };
				depth_minmax[y - y_min] = { z_left, z_right };
			}
		}
		else if (nDiffY > nDiffX)
		{
			nFrom = (y0 < y1 ? y0 : y1);
			if (nFrom < 0) nFrom = 0;
			nTo = (y0 < y1 ? y1 : y0);
			if (nTo >= nH) nTo = nH - 1;

			zFrom = (nFrom == y0 ? z0 : z1);
			zTo = (nFrom == y0 ? z1 : z0);
			dDiffZ = (zTo - zFrom) / (nTo - nFrom);
			z = zFrom - dDiffZ;

			for (y = nFrom; y <= nTo; y++)
			{
				x = (y - y0) * (x0 - x1) / (y0 - y1) + x0;
				z += dDiffZ;

				if (x < 0 || x >= nW || y < 0 || y >= nH) continue;
				auto& x_left = minmax[y - y_min].first;
				auto& x_right = minmax[y - y_min].second;
				auto& z_left = depth_minmax[y - y_min].first;
				auto& z_right = depth_minmax[y - y_min].second;

				if (x < x_left)
				{
					x_left = x;
					z_left = z;
				}
				if (x > x_right)
				{
					x_right = x;
					z_right = z;
				}

				minmax[y - y_min] = { x_left, x_right };
				depth_minmax[y - y_min] = { z_left, z_right };

			}
		}
		else
		{
			nFrom = (x0 < x1 ? x0 : x1);
			if (nFrom < 0) nFrom = 0;
			nTo = (x0 < x1 ? x1 : x0);
			if (nTo >= nW) nTo = nW - 1;

			zFrom = (nFrom == x0 ? z0 : z1);
			zTo = (nFrom == x0 ? z1 : z0);
			dDiffZ = (zTo - zFrom) / (nTo - nFrom);
			z = zFrom - dDiffZ;

			for (x = nFrom; x <= nTo; x++)
			{
				z += dDiffZ;
				y = (x - x0) * (y0 - y1) / (x0 - x1) + y0;
				if (x < 0 || x >= nW || y < 0 || y >= nH) continue;
				auto& x_left = minmax[y - y_min].first;
				auto& x_right = minmax[y - y_min].second;
				auto& z_left = depth_minmax[y - y_min].first;
				auto& z_right = depth_minmax[y - y_min].second;

				if (x < x_left)
				{
					x_left = x;
					z_left = z;
				}
				if (x > x_right)
				{
					x_right = x;
					z_right = z;
				}

				minmax[y - y_min] = { x_left, x_right };
				depth_minmax[y - y_min] = { z_left, z_right };
			}
		}
	}

	for (int y = 0; y < y_max - y_min; y++)
	{
		auto& x_left = minmax[y].first;
		auto& x_right = minmax[y].second;

		auto& z_left = depth_minmax[y].first;
		auto& z_right = depth_minmax[y].second;

		double zDiff = (z_right - z_left) / (x_right - x_left + 1);
		double z = z_left - zDiff;
		for (int x = x_left; x <= x_right; x++)
		{
		    z += zDiff;

			if (0<=z && z<=1.0 && depth[y+y_min][x] > z)
			{
				depth[y + y_min][x] = z;

				R[y + y_min][x] = KgGetRed(Color);
				G[y + y_min][x] = KgGetGreen(Color);
				B[y + y_min][x] = KgGetBlue(Color);

			}
		}
	}
}

void DrawSprite(unsigned char** R, unsigned char** G, unsigned char** B, double** depth, int nW, int nH, int x0, int y0, double z0, double u0, double v0, int x1, int y1, double z1, double u1, double v1, int x2, int y2, double z2, double u2, double v2, ColorMap* c_map)
{
	int x_min = std::max(0, std::min({ x0, x1, x2 }));
	int x_max = std::min(nW - 1, std::max({ x0, x1, x2 }));
	int y_min = std::max(0, std::min({ y0, y1, y2 }));
	int y_max = std::min(nH - 1, std::max({ y0, y1, y2 }));

	if (x_min > x_max || y_min > y_max)
		return;

	std::vector<std::pair<int, int>> minmax(y_max - y_min + 1, { x_max, x_min });
	std::vector<std::pair<double, double>> depth_minmax(y_max - y_min + 1, { 1.0, 0. });
	std::vector<std::pair<double, double>> u_minmax(y_max - y_min + 1, { 1.0, 0. });
	std::vector<std::pair<double, double>> v_minmax(y_max - y_min + 1, { 1.0, 0. });


	int nDiffX;
	int nDiffY;
	double dDiffZ;
	double dDiffU;
	double dDiffV;

	int x, y;
	int nFrom, nTo;

	double z, u, v;
	double zFrom, zTo;

	double uFrom, uTo;

	double vFrom, vTo;


	std::vector<std::vector<int>> pair_list = { {x0, x1, y0, y1}, { x1, x2, y1, y2 }, { x2, x0, y2, y0 } };
	std::vector<std::vector<double>> dpair_list = { {z0, z1, u0, u1, v0, v1}, {z1, z2, u1, u2, v1, v2}, {z2, z0, u2, u0, v2, v0} };


	for (int i=0; i<3; i++)
	{
		auto& params = pair_list[i];
		auto& dparams = dpair_list[i];


		x0 = params[0];
		x1 = params[1];
		y0 = params[2];
		y1 = params[3];

		z0 = dparams[0];
		z1 = dparams[1];
		u0 = dparams[2];
		u1 = dparams[3];
		v0 = dparams[4];
		v1 = dparams[5];

		nDiffX = abs(x0 - x1);
		nDiffY = abs(y0 - y1);

		if (nDiffY == 0 && nDiffX == 0)
		{
			x = x0;
			y = y0;
			z = z0;
			u = u0;
			v = v0;
			if (!(x < 0 || x >= nW || y < 0 || y >= nH))
			{
				auto& x_left = minmax[y - y_min].first;
				auto& x_right = minmax[y - y_min].second;
				auto& z_left = depth_minmax[y - y_min].first;
				auto& z_right = depth_minmax[y - y_min].second;
				auto& u_left = u_minmax[y - y_min].first;
				auto& u_right = u_minmax[y - y_min].second;
				auto& v_left = v_minmax[y - y_min].first;
				auto& v_right = v_minmax[y - y_min].second;

				if (x < x_left)
				{
					x_left = x;
					z_left = z;
					u_left = u;
					v_left = v;
				}
				if (x > x_right)
				{
					x_right = x;
					z_right = z;
					u_right = u;
					v_right = v;
				}

				minmax[y - y_min] = { x_left, x_right };
				depth_minmax[y - y_min] = { z_left, z_right };
				u_minmax[y - y_min] = {u_left, u_right };
				v_minmax[y - y_min] = {v_left, v_right };
			}
		}
		else if (nDiffX == 0)
		{
			x = x0;


			nFrom = (y0 < y1 ? y0 : y1);
			if (nFrom < 0) nFrom = 0;
			nTo = (y0 < y1 ? y1 : y0);
			if (nTo >= nH) nTo = nH - 1;

			zFrom = (nFrom == y0 ? z0 : z1);
			zTo = (nFrom == y0 ? z1 : z0);
			dDiffZ = (zTo - zFrom) / (nTo - nFrom);
			z = zFrom - dDiffZ;

			uFrom = (nFrom == y0 ? u0 : u1);
			uTo = (nFrom == y0 ? u1 : u0);
			dDiffU = (uTo - uFrom) / (nTo - nFrom);
			u = uFrom - dDiffU;

			vFrom = (nFrom == y0 ? v0 : v1);
			vTo = (nFrom == y0 ? v1 : v0);
			dDiffV = (vTo - vFrom) / (nTo - nFrom);
			v = vFrom - dDiffV;

			for (y = nFrom; y <= nTo; y++)
			{
				z += dDiffZ;
				u += dDiffU;
				v += dDiffV;

				if (x < 0 || x >= nW || y < 0 || y >= nH) continue;
				auto& x_left = minmax[y - y_min].first;
				auto& x_right = minmax[y - y_min].second;
				auto& z_left = depth_minmax[y - y_min].first;
				auto& z_right = depth_minmax[y - y_min].second;
				auto& u_left = u_minmax[y - y_min].first;
				auto& u_right = u_minmax[y - y_min].second;
				auto& v_left = v_minmax[y - y_min].first;
				auto& v_right = v_minmax[y - y_min].second;

				if (x < x_left)
				{
					x_left = x;
					z_left = z;
					u_left = u;
					v_left = v;
				}
				if (x > x_right)
				{
					x_right = x;
					z_right = z;
					u_right = u;
					v_right = v;
				}

				minmax[y - y_min] = { x_left, x_right };
				depth_minmax[y - y_min] = { z_left, z_right };
				u_minmax[y - y_min] = { u_left, u_right };
				v_minmax[y - y_min] = { v_left, v_right };
			}
		}
		else if (nDiffY == 0)
		{
			y = y0;

			nFrom = (x0 < x1 ? x0 : x1);
			if (nFrom < 0) nFrom = 0;
			nTo = (x0 < x1 ? x1 : x0);
			if (nTo >= nW) nTo = nW - 1;

			zFrom = (nFrom == x0 ? z0 : z1);
			zTo = (nFrom == x0 ? z1 : z0);
			dDiffZ = (zTo - zFrom) / (nTo - nFrom);
			z = zFrom - dDiffZ;

			uFrom = (nFrom == y0 ? u0 : u1);
			uTo = (nFrom == y0 ? u1 : u0);
			dDiffU = (uTo - uFrom) / (nTo - nFrom);
			u = uFrom - dDiffU;

			vFrom = (nFrom == y0 ? v0 : v1);
			vTo = (nFrom == y0 ? v1 : v0);
			dDiffV = (vTo - vFrom) / (nTo - nFrom);
			v = vFrom - dDiffV;

			for (x = nFrom; x <= nTo; x++)
			{
				z += dDiffZ;
				u += dDiffU;
				v += dDiffV;

				if (x < 0 || x >= nW || y < 0 || y >= nH) continue;
				auto& x_left = minmax[y - y_min].first;
				auto& x_right = minmax[y - y_min].second;
				auto& z_left = depth_minmax[y - y_min].first;
				auto& z_right = depth_minmax[y - y_min].second;
				auto& u_left = u_minmax[y - y_min].first;
				auto& u_right = u_minmax[y - y_min].second;
				auto& v_left = v_minmax[y - y_min].first;
				auto& v_right = v_minmax[y - y_min].second;

				if (x < x_left)
				{
					x_left = x;
					z_left = z;
					u_left = u;
					v_left = v;
				}
				if (x > x_right)
				{
					x_right = x;
					z_right = z;
					u_right = u;
					v_right = v;
				}

				minmax[y - y_min] = { x_left, x_right };
				depth_minmax[y - y_min] = { z_left, z_right };
				u_minmax[y - y_min] = { u_left, u_right };
				v_minmax[y - y_min] = { v_left, v_right };
			}
		}
		else if (nDiffY > nDiffX)
		{
			nFrom = (y0 < y1 ? y0 : y1);
			if (nFrom < 0) nFrom = 0;
			nTo = (y0 < y1 ? y1 : y0);
			if (nTo >= nH) nTo = nH - 1;

			zFrom = (nFrom == y0 ? z0 : z1);
			zTo = (nFrom == y0 ? z1 : z0);
			dDiffZ = (zTo - zFrom) / (nTo - nFrom);
			z = zFrom - dDiffZ;

			uFrom = (nFrom == y0 ? u0 : u1);
			uTo = (nFrom == y0 ? u1 : u0);
			dDiffU = (uTo - uFrom) / (nTo - nFrom);
			u = uFrom - dDiffU;

			vFrom = (nFrom == y0 ? v0 : v1);
			vTo = (nFrom == y0 ? v1 : v0);
			dDiffV = (vTo - vFrom) / (nTo - nFrom);
			v = vFrom - dDiffV;

			for (y = nFrom; y <= nTo; y++)
			{
				x = (y - y0) * (x0 - x1) / (y0 - y1) + x0;

				z += dDiffZ;
				u += dDiffU;
				v += dDiffV;

				if (x < 0 || x >= nW || y < 0 || y >= nH) continue;
				auto& x_left = minmax[y - y_min].first;
				auto& x_right = minmax[y - y_min].second;
				auto& z_left = depth_minmax[y - y_min].first;
				auto& z_right = depth_minmax[y - y_min].second;
				auto& u_left = u_minmax[y - y_min].first;
				auto& u_right = u_minmax[y - y_min].second;
				auto& v_left = v_minmax[y - y_min].first;
				auto& v_right = v_minmax[y - y_min].second;

				if (x < x_left)
				{
					x_left = x;
					z_left = z;
					u_left = u;
					v_left = v;
				}
				if (x > x_right)
				{
					x_right = x;
					z_right = z;
					u_right = u;
					v_right = v;
				}

				minmax[y - y_min] = { x_left, x_right };
				depth_minmax[y - y_min] = { z_left, z_right };
				u_minmax[y - y_min] = { u_left, u_right };
				v_minmax[y - y_min] = { v_left, v_right };
			}
		}
		else
		{
			nFrom = (x0 < x1 ? x0 : x1);
			if (nFrom < 0) nFrom = 0;
			nTo = (x0 < x1 ? x1 : x0);
			if (nTo >= nW) nTo = nW - 1;

			zFrom = (nFrom == x0 ? z0 : z1);
			zTo = (nFrom == x0 ? z1 : z0);
			dDiffZ = (zTo - zFrom) / (nTo - nFrom);
			z = zFrom - dDiffZ;

			uFrom = (nFrom == y0 ? u0 : u1);
			uTo = (nFrom == y0 ? u1 : u0);
			dDiffU = (uTo - uFrom) / (nTo - nFrom);
			u = uFrom - dDiffU;

			vFrom = (nFrom == y0 ? v0 : v1);
			vTo = (nFrom == y0 ? v1 : v0);
			dDiffV = (vTo - vFrom) / (nTo - nFrom);
			v = vFrom - dDiffV;

			for (x = nFrom; x <= nTo; x++)
			{
				y = (x - x0) * (y0 - y1) / (x0 - x1) + y0;

				z += dDiffZ;
				u += dDiffU;
				v += dDiffV;

				if (x < 0 || x >= nW || y < 0 || y >= nH) continue;
				auto& x_left = minmax[y - y_min].first;
				auto& x_right = minmax[y - y_min].second;
				auto& z_left = depth_minmax[y - y_min].first;
				auto& z_right = depth_minmax[y - y_min].second;
				auto& u_left = u_minmax[y - y_min].first;
				auto& u_right = u_minmax[y - y_min].second;
				auto& v_left = v_minmax[y - y_min].first;
				auto& v_right = v_minmax[y - y_min].second;

				if (x < x_left)
				{
					x_left = x;
					z_left = z;
					u_left = u;
					v_left = v;
				}
				if (x > x_right)
				{
					x_right = x;
					z_right = z;
					u_right = u;
					v_right = v;
				}

				minmax[y - y_min] = { x_left, x_right };
				depth_minmax[y - y_min] = { z_left, z_right };
				u_minmax[y - y_min] = { u_left, u_right };
				v_minmax[y - y_min] = { v_left, v_right };
			}
		}
	}

	for (int y = 0; y < y_max - y_min; y++)
	{
		auto& x_left = minmax[y].first;
		auto& x_right = minmax[y].second;

		auto& z_left = depth_minmax[y].first;
		auto& z_right = depth_minmax[y].second;

		auto& u_left = u_minmax[y].first;
		auto& u_right = u_minmax[y].second;
		
		auto& v_left = v_minmax[y].first;
		auto& v_right = v_minmax[y].second;

		double zDiff = (z_right - z_left) / (x_right - x_left + 1);
		double z = z_left - zDiff;

		double uDiff = (u_right - u_left) / (x_right - x_left + 1);
		float u = u_left - uDiff;

		double vDiff = (v_right - v_left) / (x_right - x_left + 1);
		float v = v_left - vDiff;

		for (int x = x_left; x <= x_right; x++)
		{
			z += zDiff;
			u += uDiff;
			v += vDiff;
			if (0 <= z && z <= 1.0 && depth[y + y_min][x] > z)
			{
				depth[y + y_min][x] = z;
				unsigned long Color = c_map->getColor(u, v);

				R[y + y_min][x] = KgGetRed(Color);
				G[y + y_min][x] = KgGetGreen(Color);
				B[y + y_min][x] = KgGetBlue(Color);
			}
		}
	}
}

void DrawTriangle_Raw(unsigned char** R, unsigned char** G, unsigned char** B, double** depth, int nW, int nH, int x0, int y0, double z0, int x1, int y1, double z1, int x2, int y2, double z2, unsigned long Color, bool bFill)
{
	// Draw Line if not bFill
	if (bFill == false)
	{
		DrawLine(R, depth, nW, nH, x0, y0, z0, x1, y1, z1, KgGetRed(Color));
		DrawLine(G, depth, nW, nH, x0, y0, z0, x1, y1, z1, KgGetGreen(Color));
		DrawLine(B, depth, nW, nH, x0, y0, z0, x1, y1, z1, KgGetBlue(Color));

		DrawLine(R, depth, nW, nH, x1, y1, z1, x2, y2, z2, KgGetRed(Color));
		DrawLine(G, depth, nW, nH, x1, y1, z1, x2, y2, z2, KgGetGreen(Color));
		DrawLine(B, depth, nW, nH, x1, y1, z1, x2, y2, z2, KgGetBlue(Color));

		DrawLine(R, depth, nW, nH, x0, y0, z0, x2, y2, z2, KgGetRed(Color));
		DrawLine(G, depth, nW, nH, x0, y0, z0, x2, y2, z2, KgGetGreen(Color));
		DrawLine(B, depth, nW, nH, x0, y0, z0, x2, y2, z2, KgGetBlue(Color));

		return;
	}
	// For test
	if (true)
	{
		Bresenham(R, G, B, depth, nW, nH, x0, y0, z0, x1, y1, z1, x2, y2, z2, Color, bFill);
		return;
	}

	// vector cross product
	
	
	int x_min = std::max(0, std::min({ x0, x1, x2 }));
	int x_max = std::min(nW-1, std::max({ x0, x1, x2 }));
	int y_min = std::max(0, std::min({ y0, y1, y2 }));
	int y_max = std::min(nH-1, std::max({ y0, y1, y2 }));

	for (int y = y_min; y <= y_max; y++)
	{
		for (int x = x_min; x <= x_max; x++)
		{
			

			CKgVector3D AB{ (double)(x1 - x0), (double)(y1 - y0), 0. };
			CKgVector3D BC{ (double)(x2 - x1), (double)(y2 - y1), 0. };
			CKgVector3D CA{ (double)(x0 - x2), (double)(y0 - y2), 0. };

			CKgVector3D AP{ (double)(x - x0), (double)(y - y0), 0. };
			CKgVector3D BP{ (double)(x - x1), (double)(y - y1), 0. };
			CKgVector3D CP{ (double)(x - x2), (double)(y - y2), 0. };

			double cross1 = AB.Cross(AP).z;
			double cross2 = BC.Cross(BP).z;
			double cross3 = CA.Cross(CP).z;

			if (bFill && (cross1 >= 0 && cross2 >= 0 && cross3 >= 0))
			{

				R[y][x] = KgGetRed(Color);
				G[y][x] = KgGetGreen(Color);
				B[y][x] = KgGetBlue(Color);
			}
		}
	}
}

// LU decomposition
bool ludcmp(double** a, int nN, int* indx, double* d)
{
	int i, imax, j, k;
	double big, dum, sum, temp;
	double* vv = new double[nN];
	const double TinyValue = 1.0e-20;

	*d = 1.0;
	for (i = 0; i < nN; i++)
	{
		big = 0.0;
		for (j = 0; j < nN; j++)
			if ((temp = fabs(a[i][j])) > big)
				big = temp;

		if (big == 0.0)
		{
			delete[] vv;
			return false; // Singular
		}

		vv[i] = 1.0 / big;
	}

	for (j = 0; j < nN; j++)
	{
		for (i = 0; i < j; i++)
		{
			sum = a[i][j];
			for (k = 0; k < i; k++)
				sum -= a[i][k] * a[k][j];

			a[i][j] = sum;
		}

		big = 0.0;
		for (i = j; i < nN; i++)
		{
			sum = a[i][j];
			for (k = 0; k < j; k++)
				sum -= a[i][k] * a[k][j];

			a[i][j] = sum;
			if ((dum = vv[i] * fabs(sum)) >= big)
			{
				big = dum;
				imax = i;
			}
		}

		if (j != imax)
		{
			for (k = 0; k < nN; k++)
			{
				dum = a[imax][k];
				a[imax][k] = a[j][k];
				a[j][k] = dum;
			}
			*d = -(*d);
			vv[imax] = vv[j];
		}

		indx[j] = imax;
		if (a[j][j] == 0.0) a[j][j] = TinyValue;

		if (j != nN - 1)
		{
			dum = 1.0 / (a[j][j]);
			for (i = j + 1; i < nN; i++)
				a[i][j] *= dum;
		}
	}
	delete[] vv;

	return true;
}



void lubksb(double **a, int nN, int *indx, double *b)
{
	int i, ii = -1, ip, j;
	double sum;

	for(i = 0; i < nN; i++) {
		ip = indx[i];
		sum = b[ip];
		b[ip] = b[i];

		if(ii >= 0)
		{
			for(j = ii; j <= i - 1; j++)
			{
				sum -= a[i][j] * b[j];
			}
		}
		else if(sum)
			ii = i;

		b[i] = sum;
	}
	for(i = nN - 1; i >= 0; i--)
	{
		sum = b[i];
		for (j = i + 1; j < nN; j++)
			sum -= a[i][j] * b[j];
		b[i] = sum / a[i][i];
	}
}

// Orginal code: NUMERICAL RECIPES IN C 
bool InverseMatrix(double  **a, double **y, int nN)
{
	double **CopyA = dmatrix(nN, nN);

	double *col = new double[nN];
	int *indx = new int[nN];
	double d;

	for(int r = 0; r < nN; r++)
		for(int c = 0; c < nN; c++)
			CopyA[r][c] = a[r][c];

	if(!ludcmp(CopyA, nN, indx, &d))
	{
		free_dmatrix(CopyA, nN, nN);

		delete[] indx;
		delete[] col;

		return false;
	}

	for(int j = 0; j < nN; j++) {
		for(int i = 0; i < nN; i++)
			col[i] = 0.0;
		col[j] = 1.0;

		lubksb(CopyA, nN, indx, col);

		for(int i = 0; i < nN; i++)
			y[i][j] = col[i];
	}

	free_dmatrix(CopyA, nN, nN);

	delete[] indx;
	delete[] col;

	return true;
}


void MatrixMultiply44(double** result, double**mat1, double** mat2)
{
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			double sum = 0.;
			for(int k=0; k<4; k++)
			{
				sum += mat1[i][k] * mat2[k][j];
			}
			result[i][j] = sum;
		}
	}
}

bool  linePlaneIntersection(CKgVector3D& contact, CKgVector3D ray, CKgVector3D rayOrigin,
	CKgVector3D normal, CKgVector3D coord, double& x) {
	// get d value

	double dist = sqrt(ray.Dot(ray));
	ray.Normalize();

	if (normal.Dot(ray) == 0) {
		return false; // No intersection, the line is parallel to the plane
	}

	// Compute the X value for the directed line ray intersecting the plane
	x = (normal.Dot(rayOrigin - coord)) / normal.Dot(ray);

	// output contact point
	ray.x *= x;
	ray.y *= x;
	ray.z *= x;
	contact = rayOrigin - ray; //Make sure your ray vector is normalized

	x /= dist;
	return true;
}


int Triangle_ClipAgainstPlain(CKgVector3D plane_pos, CKgVector3D plane_normal, CKgTriangle triangle, std::vector<CKgTriangle>& new_tris){

	CKgVector3D p0 = triangle.v0;
	CKgVector3D p1 = triangle.v1;
	CKgVector3D p2 = triangle.v2;
	CKgVector2D t0 = triangle.t0;
	CKgVector2D t1 = triangle.t1;
	CKgVector2D t2 = triangle.t2;

	std::vector<CKgVector3D> inside_points(3);
	std::vector<CKgVector3D> outside_points(3);
	std::vector<CKgVector2D> inside_texture_points(3);
	std::vector<CKgVector2D> outside_texture_points(3);
	int inside_points_cnt = 0;
	int outside_points_cnt = 0;

	double r1, r2;

	plane_normal.Normalize();

	if (p0.Dot(plane_normal) - plane_pos.Dot(plane_normal) >= 0)
	{
		inside_texture_points[inside_points_cnt] = t0;
		inside_points[inside_points_cnt++] = p0;
	}
	else
	{
		outside_texture_points[outside_points_cnt] = t0;
		outside_points[outside_points_cnt++] = p0;
	}
	
	if (p1.Dot(plane_normal) - plane_pos.Dot(plane_normal) >= 0)
	{
		inside_texture_points[inside_points_cnt] = t1;
		inside_points[inside_points_cnt++] = p1;
	}
	else
	{
		outside_texture_points[outside_points_cnt] = t1;
		outside_points[outside_points_cnt++] = p1;
	}

	if (p2.Dot(plane_normal) - plane_pos.Dot(plane_normal) >= 0)
	{
		inside_texture_points[inside_points_cnt] = t2;
		inside_points[inside_points_cnt++] = p2;
	}
	else
	{
		outside_texture_points[outside_points_cnt] = t2;
		outside_points[outside_points_cnt++] = p2;
	}


	if (outside_points_cnt == 0) {
		new_tris.push_back(triangle);
		return 1;
	}
	if (outside_points_cnt == 1) {

		CKgVector3D clip1, clip2;
		linePlaneIntersection(clip1, inside_points[0] - outside_points[0], outside_points[0], plane_normal, plane_pos, r1);
		linePlaneIntersection(clip2, inside_points[1] - outside_points[0], outside_points[0], plane_normal, plane_pos, r2);


		CKgVector2D clip_t1 = outside_texture_points[0] - r1 * (inside_texture_points[0] - outside_texture_points[0]);
		CKgVector2D clip_t2 = outside_texture_points[0] - r2 * (inside_texture_points[1] - outside_texture_points[0]) ;


		new_tris.push_back(CKgTriangle(clip1, inside_points[0], inside_points[1], clip_t1, inside_texture_points[0], inside_texture_points[1], triangle.bFill));
		new_tris.push_back(CKgTriangle(clip2, inside_points[1], clip1, clip_t2, inside_texture_points[1], clip_t1, triangle.bFill));

		return 2;
	}

	if (outside_points_cnt == 2) {
		CKgVector3D clip1, clip2;
		linePlaneIntersection(clip1, inside_points[0] - outside_points[0], outside_points[0], plane_normal, plane_pos, r1);
		linePlaneIntersection(clip2, inside_points[0] - outside_points[1], outside_points[1], plane_normal, plane_pos, r2);


		CKgVector2D clip_t1 = outside_texture_points[0] - r1 * (inside_texture_points[0] - outside_texture_points[0]);
		CKgVector2D clip_t2 = outside_texture_points[0] - r2 * (inside_texture_points[1] - outside_texture_points[0]);

		
		new_tris.push_back(CKgTriangle(clip1, inside_points[0], clip2, clip_t1, inside_texture_points[0], clip_t2, triangle.bFill));

		return 1;
	}
	if (outside_points_cnt == 3) {
		return 0;
	}

	return 0;
}
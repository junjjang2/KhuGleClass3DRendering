#include "KhuGle3DSprite.h"
#include "KhuGleBase.h"
#include <iostream>
#include <fstream>
#include <list>
#include <queue>
#include <string>

CKhuGle3DObject::CKhuGle3DObject(KgColor24 fgColor, CKgVector3D pWorldPos)
{
	m_fgColor = fgColor;
	m_WorldPos = pWorldPos;

	rx = 0.; ry = 0.; rz = 0.;
	sx = 1.; sy = 1.; sz = 1.;

	m_rotation_matrix = dmatrix(4, 4);
	for (int r = 0; r < 4; ++r)
		for (int c = 0; c < 4; ++c)
			m_rotation_matrix[r][c] = (r == c) ? 1. : 0.;
}

CKhuGle3DObject::~CKhuGle3DObject() {
	free_dmatrix(m_projection_matrix, 4, 4);
	free_dmatrix(m_rotation_matrix, 4, 4);
};

void CKhuGle3DObject::MoveBy(double OffsetX, double OffsetY, double OffsetZ)
{
	m_WorldPos += CKgVector3D(OffsetX, OffsetY, OffsetZ);
}

void CKhuGle3DObject::RotateBy(double dRadianX, double dRadianY, double dRadianZ)
{
	double c_x, c_y, c_z, s_x, s_y, s_z;
	rx = std::fmod((rx + dRadianX), 2 * Pi);
	ry = std::fmod((ry + dRadianY), 2 * Pi);
	rz = std::fmod((rz + dRadianZ), 2 * Pi);
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

}


CKhuGleCamera::CKhuGleCamera(int nW, int nH, double Fov, double Far, double Near, const CKgVector3D& m_position, const CKgVector3D& m_forward, const CKgVector3D& m_up)
	: CKhuGle3DObject(KG_COLOR_24_RGB(255, 255, 255), m_position), m_forward_base(m_forward), m_forward(m_forward), m_up(m_up)
{
	this->m_forward.Normalize();
	this->m_up.Normalize();

	m_projection_matrix = dmatrix(4, 4);
	for (int r = 0; r < 4; ++r)
		for (int c = 0; c < 4; ++c)
			m_projection_matrix[r][c] = 0.;

	m_projection_matrix[0][0] = (double)nH / (double)nW * 1. / tan(Fov / 2.);
	m_projection_matrix[1][1] = 1. / tan(Fov / 2.);
	m_projection_matrix[2][2] = (-Near - Far) / (Near - Far);
	m_projection_matrix[2][3] = 2. * (Far * Near) / (Near - Far);
	m_projection_matrix[3][2] = 1.;
	m_projection_matrix[3][3] = 0.;

	m_view_matrix = ComputeViewMatrix();
}


CKhuGleCamera::~CKhuGleCamera()
{
	free_dmatrix(m_view_matrix, 4, 4);
	free_dmatrix(m_projection_matrix, 4, 4);
}


void CKhuGleCamera::MoveBy(double OffsetX, double OffsetY, double OffsetZ)
{
	CKhuGle3DObject::MoveBy(OffsetX, OffsetY, OffsetZ);
	m_view_matrix = ComputeViewMatrix();
}

void CKhuGleCamera::RotateBy(double dRadianX, double dRadianY, double dRadianZ)
{
	CKhuGle3DObject::RotateBy(dRadianX, dRadianY, dRadianZ);

	MatrixVector44(m_forward, m_forward_base, m_rotation_matrix);
	m_forward.Normalize();

	m_view_matrix = ComputeViewMatrix();
}


double** CKhuGleCamera::ComputeViewMatrix()
{
	CKgVector3D camera_pos = m_WorldPos;
	CKgVector3D CameraUp = m_up;
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

void CKhuGle3DObject::MatrixVector44(CKgVector3D& out, CKgVector3D v, double** M)
{
	out.x = v.x * M[0][0] + v.y * M[0][1] + v.z * M[0][2] + v.w * M[0][3];
	out.y = v.x * M[1][0] + v.y * M[1][1] + v.z * M[1][2] + v.w * M[1][3];
	out.z = v.x * M[2][0] + v.y * M[2][1] + v.z * M[2][2] + v.w * M[2][3];
	out.w = v.x * M[3][0] + v.y * M[3][1] + v.z * M[3][2] + v.w * M[3][3];

	/*if (fabs(out.w) > 0)
		out = (1. / out.w) * out;*/
}

CKhuGle3DSprite::CKhuGle3DSprite(KgColor24 fgColor, CKgVector3D pWorldPos, CKhuGleCamera* c, char* FilePath) : m_camera(c), CKhuGle3DObject(fgColor, pWorldPos)
{
	std::vector<std::vector<double>> local_locations;
	ReadObj(FilePath, local_locations, triangle_list);

	for(auto& locations : local_locations)
	{
		vertex_list.push_back(CKgVector3D(locations[0], locations[1], locations[2]));
	}
}

CKhuGle3DSprite::CKhuGle3DSprite(KgColor24 fgColor, CKgVector3D pWorldPos, std::vector<CKgTriangle>& pTriangleMeshes, CKhuGleCamera* c) : m_camera(c), CKhuGle3DObject(fgColor, pWorldPos)
{
	for (auto& triangle : pTriangleMeshes)
	{
		SurfaceMesh.push_back(triangle);
	}
};

CKhuGle3DSprite::~CKhuGle3DSprite() {
	//free_dmatrix(m_projection_matrix, 4, 4);
	free_dmatrix(m_rotation_matrix, 4, 4);
};

void CKhuGle3DSprite::ReadObj(char* FilePath, std::vector<std::vector<double>>& vertex_list, std::vector<std::vector<int>>& triangle_list)
{
	std::ifstream f_s;
	f_s.open(FilePath);

	std::string LineString = "";
	std::string Delimeter = " ";

	while (std::getline(f_s, LineString))
	{
		std::vector<double> RowDouble;
		std::vector<int> RowInt;
		std::vector<std::string> RowData;

		unsigned int nPos = 0, nFindPos;
		std::string type;
		unsigned int npos = 4294967295;
		do
		{
			nFindPos = LineString.find(Delimeter, nPos);
			if (nFindPos == std::string::npos || nFindPos == npos)
				nFindPos = LineString.length();

			if (type.empty()) type = LineString.substr(nPos, nFindPos - nPos);
			else
			{
				if (type == "#") break;
				else if (type == "v") RowDouble.push_back(std::stod(LineString.substr(nPos, nFindPos - nPos)));
				else if (type == "f") RowInt.push_back(std::stoi(LineString.substr(nPos, nFindPos - nPos)) - 1); // vertex index starts from 1;
				else if (type == "s") RowData.push_back(LineString.substr(nPos, nFindPos - nPos));
			}

			nPos = nFindPos + 1;

		} while (nFindPos < LineString.length());

		if (type == "#") continue;
		else if (type == "v") vertex_list.push_back(RowDouble);
		else if (type == "f") triangle_list.push_back(RowInt);
		else if (type == "s") continue;
	}
	std::cout <<"Obj FIle load success : " << FilePath << std::endl;
}

void CKhuGle3DSprite::DrawTriangle(unsigned char** R, unsigned char** G, unsigned char** B, int nW, int nH,
	int x0, int y0, int x1, int y1, int x2, int y2, KgColor24 Color24)
{
	CKhuGleSprite::DrawLine(R, G, B, nW, nH, x0, y0, x1, y1, Color24);
	CKhuGleSprite::DrawLine(R, G, B, nW, nH, x1, y1, x2, y2, Color24);
	CKhuGleSprite::DrawLine(R, G, B, nW, nH, x2, y2, x0, y0, Color24);
}

void CKhuGle3DSprite::Render()
{
	std::vector<render_inform> result;

	CKhuGleLayer* Parent = (CKhuGleLayer*)m_Parent;

	// local position --(transform matrix)-> worldPos --(condition check, View, Projection)-> drawLine
	int non_rendered = 0;
	int z_clipped = 0;
	int rendered = 0;
	int clipped_rendered = 0;

	for (auto& vertex_indexs : triangle_list)
	{
		CKgVector3D v0 = vertex_list[vertex_indexs[0]];
		CKgVector3D v1 = vertex_list[vertex_indexs[1]];
		CKgVector3D v2 = vertex_list[vertex_indexs[2]];

		// Scale 
		v0.x *= sx;
		v0.y *= sy;
		v0.z *= sz;

		v1.x *= sx;
		v1.y *= sy;
		v1.z *= sz;

		v2.x *= sx;
		v2.y *= sy;
		v2.z *= sz;

		// Rotation
		CKgVector3D world_v0, world_v1, world_v2;
		MatrixVector44(world_v0, v0, m_rotation_matrix);
		MatrixVector44(world_v1, v1, m_rotation_matrix);
		MatrixVector44(world_v2, v2, m_rotation_matrix);

		// Translation
		world_v0 += m_WorldPos;
		world_v1 += m_WorldPos;
		world_v2 += m_WorldPos;

		//// check condition
		CKgVector3D Side1, Side2, Normal;
		Side1 = world_v1 - world_v0;
		Side2 = world_v2 - world_v0;

		Normal = Side2.Cross(Side1);
		Normal.Normalize();

		// culling 
		if (Normal.Dot(world_v0 - m_camera->m_WorldPos) > 0)
		{
			non_rendered++;
			continue;

		}

		//// end check condition

		// World -> View
		MatrixVector44(v0, world_v0, m_camera->m_view_matrix);
		MatrixVector44(v1, world_v1, m_camera->m_view_matrix);
		MatrixVector44(v2, world_v2, m_camera->m_view_matrix);

		// Depth Clipping 
		if (v0.z < 0 || v1.z < 0 || v2.z < 0) // < m_camera->near
		{
			z_clipped++;
			continue;
		}

		// View -> Projection
		CKgVector3D projected_v0, projected_v1, projected_v2;

		MatrixVector44(projected_v0, v0, m_camera->m_projection_matrix);
		MatrixVector44(projected_v1, v1, m_camera->m_projection_matrix);
		MatrixVector44(projected_v2, v2, m_camera->m_projection_matrix);


		
		projected_v0 = (1. / projected_v0.w) * projected_v0;
		projected_v1 = (1. / projected_v1.w) * projected_v1;
		projected_v2 = (1. / projected_v2.w) * projected_v2;

		// [-1, 1] -> [0, m_nH or m_nW]
		projected_v0.x += 1;
		projected_v0.y += 1;
		projected_v0.x *= Parent->m_nW / 2.;
		projected_v0.y *= Parent->m_nW / 2.;

		projected_v1.x += 1;
		projected_v1.y += 1;
		projected_v1.x *= Parent->m_nW / 2.;
		projected_v1.y *= Parent->m_nW / 2.;

		projected_v2.x += 1;
		projected_v2.y += 1;
		projected_v2.x *= Parent->m_nW / 2.;
		projected_v2.y *= Parent->m_nW / 2.;


		// Triangle clipping
		if (!(projected_v0.x < 0 || projected_v0.x > Parent->m_nW - 1 || projected_v1.x < 0 || projected_v1.x > Parent->m_nW - 1 || projected_v2.x < 0 || projected_v2.x > Parent->m_nW - 1 ||
			projected_v0.y < 0 || projected_v0.y > Parent->m_nH - 1 || projected_v1.y < 0 || projected_v1.y > Parent->m_nH - 1 || projected_v2.y < 0 || projected_v2.y > Parent->m_nH - 1
			)) {
			DrawTriangle_Raw(Parent->m_ImageR, Parent->m_ImageG, Parent->m_ImageB, Parent->m_Depth, Parent->m_nW, Parent->m_nH, (int)projected_v0.x, (int)projected_v0.y, projected_v0.z, (int)projected_v1.x, (int)projected_v1.y, projected_v1.z, (int)projected_v2.x, (int)projected_v2.y, projected_v2.z, m_fgColor, true);
			rendered++;
		}
		else
		{
			CKgTriangle original_t(projected_v0, projected_v1, projected_v2, true);

			std::vector<CKgTriangle> clipped;
			std::list<CKgTriangle> listTriangle;

			listTriangle.push_back(original_t);
			int nNewTriangles = 1;

			// 여기가 가장 오래 걸림
			for (int i = 0; i < 4; i++)
			{
				int nTrisToAdd = 0;
				while (nNewTriangles > 0)
				{
					clipped.clear();

					CKgTriangle temp = listTriangle.front();
					listTriangle.pop_front();
					nNewTriangles--;

					switch (i)
					{
					case 0: nTrisToAdd = Triangle_ClipAgainstPlain(CKgVector3D(0, 0, 0), CKgVector3D(0, 1, 0), temp, clipped); break;
					case 1: nTrisToAdd = Triangle_ClipAgainstPlain(CKgVector3D(0, Parent->m_nH - 1, 0), CKgVector3D(0, -1, 0), temp, clipped); break;
					case 2: nTrisToAdd = Triangle_ClipAgainstPlain(CKgVector3D(0, 0, 0), CKgVector3D(1, 0, 0), temp, clipped); break;
					case 3: nTrisToAdd = Triangle_ClipAgainstPlain(CKgVector3D(Parent->m_nW - 1, 0, 0), CKgVector3D(-1, 0, 0), temp, clipped); break;
					}

					for (auto& tri : clipped)
						listTriangle.push_back(tri);
				}
				nNewTriangles = listTriangle.size();
			}


			for (auto& tri : listTriangle)
			{
				clipped_rendered++;
				DrawTriangle_Raw(Parent->m_ImageR, Parent->m_ImageG, Parent->m_ImageB, Parent->m_Depth, Parent->m_nW, Parent->m_nH, (int)tri.v0.x, (int)tri.v0.y, tri.v0.z, (int)tri.v1.x, (int)tri.v1.y, tri.v1.z, (int)tri.v2.x, (int)tri.v2.y, tri.v2.z, m_fgColor, original_t.bFill);
			}
		}
	}
	std::cout << "non-rendered: " <<  non_rendered << " z-clipped: " << z_clipped<< " rendered: " << rendered << " clipped-rendered: " << clipped_rendered << std::endl;

}


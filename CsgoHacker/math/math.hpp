#ifndef _FROSTBITE_HPP
#define _FROSTBITE_HPP

#include <Windows.h>
#include "../sdk/sdk.hpp"

inline bool WorldToScreen(float* vIn, float* flOut)
{

	CViewMatrix* cViewMatrix = (CViewMatrix*)((DWORD)m_pHack->m_hclient + offsets::view_matrix);
	ViewMatrix view_projection = cViewMatrix->view_matrix;
	//printf("%x ", &view_projection);

	float w = 0.0f;

	w = view_projection.m[3][0] * vIn[0] + view_projection.m[3][1] * vIn[1] + view_projection.m[3][2] * vIn[1] + view_projection.m[3][3];

	if (w < 0.01)
		return false;

	flOut[0] = view_projection.m[0][0] * vIn[0] + view_projection.m[0][1] * vIn[1] + view_projection.m[0][2] * vIn[2] + view_projection.m[0][3];
	//根据目标在世界空间的xyz轴，得出在该目标在矩阵中的y轴
	flOut[1] = view_projection.m[1][0] * vIn[0] + view_projection.m[1][1] * vIn[1] + view_projection.m[1][2] * vIn[2] + view_projection.m[1][3];

	float invw = 1.0f / w;

	flOut[0] *= invw;
	flOut[1] *= invw;

	int width, height;

	auto io = ImGui::GetIO();
	width = io.DisplaySize.x;
	height = io.DisplaySize.y;


	float x = (float)width / 2;
	float y = (float)height / 2;

	x += 0.5 * flOut[0] * (float)width + 0.5;
	y -= 0.5 * flOut[1] * (float)height + 0.5;

	flOut[0] = x;
	flOut[1] = y;

	//float x = (float)width / 2;
	//float y = (float)height / 2;

	//x = (width * 0.5f * flOut[0]) + (flOut[0] + width * 0.5f);
	//y = -(height * 0.5f * flOut[1]) + (flOut[1] + height * 0.5f);

	//flOut[0] = x;
	//flOut[1] = y;

	return true;
}


inline void CalcAngle(float* src, float* dst, float* angles)
{
	double delta[3] = { (src[0] - dst[0]), (src[1] - dst[1]), (src[2] - dst[2]) };
	double hyp = sqrt(delta[0] * delta[0] + delta[1] * delta[1]);
	angles[0] = (float)(asinf(delta[2] / hyp) * 57.295779513082f);
	angles[1] = (float)(atanf(delta[1] / delta[0]) * 57.295779513082f);
	angles[2] = 0.0f;

	// Normalize angle
	if (delta[0] >= 0.0)
	{
		angles[1] += 180.0f;
	}
}

#endif // _FROSTBITE_HPP
#pragma once

#include <string>

struct sVertex_RGBA_XYZ_N_UV_T_BiN_Bones
{
	float r, g, b, a;
	float x, y, z, w;
	float nx, ny, nz, nw;
	float u0, v0, u1, v1;
	float tx, ty, yz, tw;
	float bx, by, bz, bw;
	float vBoneID[4];
	float vBoneWeight[4];
};

struct sModelDrawInfo
{
	sModelDrawInfo();

	std::string meshName;

	unsigned int VAO_ID;

	unsigned int VertexBufferID;
	unsigned int VertexBuffer_Start_Index;
	unsigned int numberOfVertices;

	unsigned int IndexBufferID;
	unsigned int IndexBuffer_Start_Index;
	unsigned int numberOfIndices;
	unsigned int numberOfTriangles;

	void CalculateExtents(void);
	float maxX, maxY, maxZ;
	float minX, minY, minZ;
	float extentX, extentY, extentZ;
	float maxExtent;
	sVertex_RGBA_XYZ_N_UV_T_BiN_Bones* pVertices;
	unsigned int* pIndices;
};

#ifndef CObjNew_H
#define CObjNew_H
#include "../header/Angel.h"
#include "CShape.h"

#define QUAD_NUM 6		// 2 faces, 2 triangles/face 

class CObjNew : public CShape
{
private:

	//vec4 m_Points[QUAD_NUM];
	//vec3 m_Normal[QUAD_NUM];	// 用 vec3 來宣告是為了節省計算, 如果要讓程式寫起來更方便，可改用 vec4 來宣告
	//vec4 m_Colors[QUAD_NUM];

public:
	CObjNew(char *);

	void Update(float dt, const LightSource &lights);
	void Update(float dt); // 不計算光源的照明

	GLuint GetShaderHandle() { return m_uiProgram;} 
	void RenderWithFlatShading(const LightSource &lights);//  vLightI: Light Intensity
	void RenderWithGouraudShading(const LightSource &lights);//  vLightI: Light Intensity
	void SetVtxColors(vec4 vLFColor, vec4 vLRColor, vec4 vTRColor, vec4 vTLColor); // four Vertices' Color
	void SetVtxColors(vec4 vFColor, vec4 vSColor);	// three Vertices' Color with idx as the first 

	void Draw();
	void DrawW();

	//	讀檔
	char lineHeader[1000];//一行讀檔暫存空間
	int indices[10];
	float vertex[3];
	FILE* objfile;
	char* adrFile;
};




#endif
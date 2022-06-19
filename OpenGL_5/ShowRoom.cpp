// 
// Example 5 : Cubic mapping (Environment Mapping)
// 
// ����e���ǳƤu�@
// ���� CShape.h ���� #define PERVERTEX_LIGHTING�A�ϥ� PERPIXEL_LIGHTING �~�|���@��
// �]�w #define MULTITEXTURE  (DIFFUSE_MAP|LIGHT_MAP|NORMAL_MAP)
// �}�� #define CUBIC_MAP 1

#include "header/Angel.h"
#include "Common/CQuad.h"
#include "Common/CSolidCube.h"
#include "Common/CSolidSphere.h"
#include "Common/CWireSphere.h"
#include "Common/CWireCube.h"
#include "Common/CChecker.h"
#include "Common/CCamera.h"
#include "Common/CTexturePool.h"
#include "png_loader.h"
#include"Common\CObjNew.h"
#define SPACE_KEY 32
#define SCREEN_SIZE 800
#define HALF_SIZE SCREEN_SIZE /2 
#define VP_HALFWIDTH  20.0f
#define VP_HALFHEIGHT 20.0f
#define GRID_SIZE 20 // must be an even number


// For Model View and Projection Matrix
mat4 g_mxModelView(1.0f);
mat4 g_mxProjection;

// For Objects
CQuad* g_pFloor[6];
CSolidCube* g_pCube;
CSolidSphere* g_pSphere;
// wall
CQuad* g_pLeftWall[6];
CQuad* g_pRightWall[6];
CQuad* g_pFrontWall[7];
// For View Point
GLfloat g_fRadius = 8.0;
GLfloat g_fTheta = 45.0f * DegreesToRadians;
GLfloat g_fPhi = 45.0f * DegreesToRadians;

//for animals
CQuad* g_pAimal[3];
GLuint g_uiAimalTexID[4];
GLuint g_uiAimalNormalTexID[3];
//for objs
CObjNew* g_pObj1, * g_pObj2;
GLuint g_uiCheeseNormalTexID;
GLuint g_uiDianosourNormalTexID;
//----------------------------------------------------------------------------
// Part 2 : for single light source
bool g_bAutoRotating = false;
float g_fElapsedTime = 0;
float g_fLightRadius = 6;
float g_fLightTheta = 0;

float g_fLightR = 0.85f;
float g_fLightG = 0.85f;
float g_fLightB = 0.85f;

LightSource g_Light1 = {
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // diffuse
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	point4(6.0f, 5.0f, 0.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(0.0f, 0.0f, 0.0f),			  //spotDirection
	2.0f,	// spotExponent(parameter e); cos^(e)(phi) 
	45.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot ���ө��d��
	0.707f,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), �ө���V�P�Q�ө��I���������ר� cos ��, cut off ����
	1,	// constantAttenuation	(a + bd + cd^2)^-1 ���� a, d ��������Q�ө��I���Z��
	0,	// linearAttenuation	    (a + bd + cd^2)^-1 ���� b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 ���� c
};
LightSource g_Light2 = {
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // diffuse
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	point4(25.0f, 20.0f, 0.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(0.0f, 0.0f, 0.0f),			  //spotDirection
	2.0f,	// spotExponent(parameter e); cos^(e)(phi) 
	45.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot ���ө��d��
	0.707f,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), �ө���V�P�Q�ө��I���������ר� cos ��, cut off ����
	1,	// constantAttenuation	(a + bd + cd^2)^-1 ���� a, d ��������Q�ө��I���Z��
	0,	// linearAttenuation	    (a + bd + cd^2)^-1 ���� b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 ���� c
};
LightSource g_Light3 = {
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // diffuse
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	point4(50.0f, 5.0f, 0.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(0.0f, 0.0f, 0.0f),			  //spotDirection
	2.0f,	// spotExponent(parameter e); cos^(e)(phi) 
	60.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot ���ө��d��
	0.707f,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), �ө���V�P�Q�ө��I���������ר� cos ��, cut off ����
	1,	// constantAttenuation	(a + bd + cd^2)^-1 ���� a, d ��������Q�ө��I���Z��
	0,	// linearAttenuation	    (a + bd + cd^2)^-1 ���� b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 ���� c
};
LightSource g_Light4 = {
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // diffuse
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	point4(75.0f, 5.0f, 0.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(0.0f, 0.0f, 0.0f),			  //spotDirection
	2.0f,	// spotExponent(parameter e); cos^(e)(phi) 
	45.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot ���ө��d��
	0.707f,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), �ө���V�P�Q�ө��I���������ר� cos ��, cut off ����
	1,	// constantAttenuation	(a + bd + cd^2)^-1 ���� a, d ��������Q�ө��I���Z��
	0,	// linearAttenuation	    (a + bd + cd^2)^-1 ���� b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 ���� c
};
LightSource g_Light5 = {
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // diffuse
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	point4(100.0f, 5.0f, 0.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(0.0f, 0.0f, 0.0f),			  //spotDirection
	2.0f,	// spotExponent(parameter e); cos^(e)(phi) 
	45.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot ���ө��d��
	0.707f,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), �ө���V�P�Q�ө��I���������ר� cos ��, cut off ����
	1,	// constantAttenuation	(a + bd + cd^2)^-1 ���� a, d ��������Q�ө��I���Z��
	0,	// linearAttenuation	    (a + bd + cd^2)^-1 ���� b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 ���� c
};
LightSource g_Light6 = {
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // diffuse
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	point4(125.0f, 5.0f, 0.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(0.0f, 0.0f, 0.0f),			  //spotDirection
	2.0f,	// spotExponent(parameter e); cos^(e)(phi) 
	45.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot ���ө��d��
	0.707f,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), �ө���V�P�Q�ө��I���������ר� cos ��, cut off ����
	1,	// constantAttenuation	(a + bd + cd^2)^-1 ���� a, d ��������Q�ө��I���Z��
	0,	// linearAttenuation	    (a + bd + cd^2)^-1 ���� b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 ���� c
};
CWireSphere* g_pLight[6];
//----------------------------------------------------------------------------

// Texture 
GLuint g_uiFTexID[10]; // �T�Ӫ�����O�����P���K��
int g_iTexWidth, g_iTexHeight;
GLuint g_uiSphereCubeMap; // for Cubic Texture
//CAMERA
point4 eye;
point4 at;
//----------------------------------------------------------------------------
// �禡���쫬�ŧi
extern void IdleProcess();

void init(void)
{
	mat4 mxT;
	vec4 vT, vColor;
	// ���ͩһݤ� Model View �P Projection Matrix
	// ���ͩһݤ� Model View �P Projection Matrix
	at = point4(sin(g_fTheta) * sin(g_fPhi), cos(g_fTheta), sin(g_fTheta) * cos(g_fPhi), 1.0f);
	eye = point4(0.0f, 5.0f, 0.0f, 1.0f);
	auto camera = CCamera::create();
	camera->updateViewLookAt(eye, at);
	camera->updatePerspective(60.0, (GLfloat)SCREEN_SIZE / (GLfloat)SCREEN_SIZE, 1.0, 1000.0);

	auto texturepool = CTexturePool::create();
	g_uiFTexID[0] = texturepool->AddTexture("texture/checker.png");
	g_uiFTexID[1] = texturepool->AddTexture("texture/Masonry.Brick.png");
	g_uiFTexID[3] = texturepool->AddTexture("texture/metal.png");
	g_uiFTexID[4] = texturepool->AddTexture("texture/Masonry.Brick.normal.png");
	//
	g_uiFTexID[5] = texturepool->AddTexture("texture/cheese.png");
	g_uiFTexID[6] = texturepool->AddTexture("texture/raptor.png");
#ifdef MULTITEXTURE
	g_uiFTexID[2] = texturepool->AddTexture("texture/lightMap1.png");
#endif
	g_uiSphereCubeMap = CubeMap_load_SOIL();
	//animals test
	g_uiAimalTexID[0] = texturepool->AddTexture("texture/wi_tata.png");
	g_uiAimalTexID[1] = texturepool->AddTexture("texture/wi_sma.png");
	g_uiAimalTexID[2] = texturepool->AddTexture("texture/wi_tree.png");

	g_uiAimalNormalTexID[0] = texturepool->AddTexture("texture/wi_tata_normal.png");
	g_uiAimalNormalTexID[1] = texturepool->AddTexture("texture/wi_sma_normal.png");
	g_uiAimalNormalTexID[2] = texturepool->AddTexture("texture/wi_tree_normal.png");
	g_uiCheeseNormalTexID = texturepool->AddTexture("texture/CheeseNormalMap.png");
	g_uiDianosourNormalTexID = texturepool->AddTexture("texture/raptor_normal.png");

	// ���ͪ��󪺹���	
	for (int i = 0; i < 6; i++) {
		g_pFloor[i] = new CQuad;
#ifdef MULTITEXTURE
		g_pFloor[i]->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP);
#endif
		g_pFloor[i]->SetShader();
		vT.x = i * 25; vT.z = 0; vT.z = 0;
		mxT = Translate(vT);
		g_pFloor[i]->SetTRSMatrix(mxT * Scale(25, 1, 25));
		g_pFloor[i]->SetShadingMode(GOURAUD_SHADING);
		g_pFloor[i]->SetTiling(4, 4);
		// �]�w�K��
		g_pFloor[i]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
		g_pFloor[i]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	}
	for (int i = 0; i < 6; i++) {
		g_pLeftWall[i] = new CQuad;
#ifdef MULTITEXTURE
		g_pLeftWall[i]->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP);
#endif
		g_pLeftWall[i]->SetShader();
		vT.x = i * 25; vT.y = 0; vT.z = -10.0f;
		mxT = Translate(vT);
		g_pLeftWall[i]->SetTRSMatrix(mxT * RotateX(90.0f) * Scale(25, 1, 25));
		g_pLeftWall[i]->SetShadingMode(GOURAUD_SHADING);
		g_pLeftWall[i]->SetTiling(4, 4);
		// �]�w�K��
		g_pLeftWall[i]->SetMaterials(vec4(0), vec4(0.85f, 0.15f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
		g_pLeftWall[i]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	}
	for (int i = 0; i < 6; i++) {
		g_pRightWall[i] = new CQuad;
#ifdef MULTITEXTURE
		g_pRightWall[i]->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP);
#endif
		g_pRightWall[i]->SetShader();
		vT.x = i * 25; vT.y = 0; vT.z = 10.0f;
		mxT = Translate(vT);
		g_pRightWall[i]->SetTRSMatrix(mxT * RotateX(270.0f) * Scale(25, 1, 25));
		g_pRightWall[i]->SetShadingMode(GOURAUD_SHADING);
		g_pRightWall[i]->SetTiling(4, 4);
		// �]�w�K��
		g_pRightWall[i]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.15f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
		g_pRightWall[i]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	}
	for (int i = 0; i < 7; i++) {
		g_pFrontWall[i] = new CQuad;
#ifdef MULTITEXTURE
		g_pFrontWall[i]->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP);
#endif
		g_pFrontWall[i]->SetShader();
		vT.x = i * 25 - 10.0f; vT.y = 0; vT.z = 0.0f;
		mxT = Translate(vT);
		if (i == 0)g_pFrontWall[i]->SetTRSMatrix(mxT * RotateZ(270.0f) * Scale(25, 1, 25));
		else g_pFrontWall[i]->SetTRSMatrix(mxT * RotateZ(90.0f) * Scale(25, 1, 25));
		g_pFrontWall[i]->SetShadingMode(GOURAUD_SHADING);
		g_pFrontWall[i]->SetTiling(4, 4);
		// �]�w�K��
		g_pFrontWall[i]->SetMaterials(vec4(0), vec4(0.15f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
		g_pFrontWall[i]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	}
	g_pCube = new CSolidCube;
	g_pCube->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pCube->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
	g_pCube->SetShader();
	// �]�w Cube
	vT.x = 4.0f; vT.y = 1.0f; vT.z = -0.5f;
	mxT = Translate(vT);
	mxT._m[0][0] = 2.0f; mxT._m[1][1] = 2.0f; mxT._m[2][2] = 2.0f;
	g_pCube->SetTRSMatrix(mxT);
	g_pCube->SetShadingMode(GOURAUD_SHADING);
	// materials
	g_pCube->SetMaterials(vec4(0.35f, 0.35f, 0.35f, 1), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pCube->SetKaKdKsShini(0.25f, 0.8f, 0.2f, 2);

	// For Reflecting Sphere
	g_pSphere = new CSolidSphere(1.0f, 24, 12);
	g_pSphere->SetTextureLayer(DIFFUSE_MAP);  // �ϥ� 
	g_pSphere->SetCubeMapTexName(1);
	g_pSphere->SetViewPosition(eye);
	g_pSphere->SetShaderName("vsCubeMapping.glsl", "fsCubeMapping.glsl");//�ϥ�CUBE MAPPING
	g_pSphere->SetShader();
	vT.x = 25.0f; vT.y = 2.0f; vT.z = 0.0f;
	mxT = Translate(vT);
	mxT._m[0][0] = 2.0f; mxT._m[1][1] = 2.0f; mxT._m[2][2] = 2.0f;
	g_pSphere->SetTRSMatrix(mxT * RotateX(90.0f));
	g_pSphere->SetShadingMode(GOURAUD_SHADING);
	// �]�w�K��
	g_pSphere->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pSphere->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_pSphere->SetColor(vec4(0.9f, 0.9f, 0.9f, 1.0f));


	g_pObj1 = new CObjNew("./Common/cheese.obj");
	g_pObj1->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pObj1->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
	g_pObj1->SetShader();
	vT.x = 50.5; vT.y = 0.5; vT.z = 1;
	mxT = Translate(vT);
	g_pObj1->SetTRSMatrix(mxT);
	g_pObj1->SetShadingMode(GOURAUD_SHADING);
	g_pObj1->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0.85f, 0.8, 0.9f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pObj1->SetKaKdKsShini(0.15f, 0.8f, 0.2f, 2);
	//obj2
	g_pObj2 = new CObjNew("./Common/Raptor.obj");
	g_pObj2->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pObj2->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
	g_pObj2->SetShader();
	vT.x = 50.5; vT.y = 0.5; vT.z = -2;
	mxT = Translate(vT);
	g_pObj2->SetTRSMatrix(mxT * Scale(0.05f, 0.05f, 0.05f));
	g_pObj2->SetShadingMode(GOURAUD_SHADING);
	g_pObj2->SetMaterials(vec4(0.55f, 0.55f, 0.55f, 1.0f), vec4(0.85f, 0.5, 0.5f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pObj2->SetKaKdKsShini(0.85f, 0.8f, 0.8f, 2);
	//animal test
	// For g_pAimal3
	g_pAimal[2] = new CQuad;
	//
#if MULTITEXTURE >= DIFFUSE_MAP | NORMAL_MAP 
	g_pAimal[2]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pAimal[2]->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif

	g_pAimal[2]->SetShader();
	mxT = Translate(80, 1.0f, -5.0f) * RotateX(45) * Scale(2, 2, 2);
	g_pAimal[2]->SetTRSMatrix(mxT);
	g_pAimal[2]->SetShadingMode(GOURAUD_SHADING);
	// �]�w�K��
	g_pAimal[2]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pAimal[2]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	// For g_pAimal2
	g_pAimal[1] = new CQuad;

#if MULTITEXTURE >= DIFFUSE_MAP | NORMAL_MAP
	g_pAimal[1]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pAimal[1]->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif 

	g_pAimal[1]->SetShader();
	mxT = Translate(100, 1.0f, -5.0f) * RotateX(90) * Scale(2, 2, 2);
	g_pAimal[1]->SetTRSMatrix(mxT);
	g_pAimal[1]->SetShadingMode(GOURAUD_SHADING);
	// �]�w�K��
	g_pAimal[1]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pAimal[1]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	// For g_pAimal1
	g_pAimal[0] = new CQuad;

#if MULTITEXTURE >= (DIFFUSE_MAP | NORMAL_MAP)
	g_pAimal[0]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pAimal[0]->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif

	g_pAimal[0]->SetShader();
	mxT = Translate(125, 1.0f, -3.0f) * RotateX(90) * Scale(2, 2, 2);
	g_pAimal[0]->SetTRSMatrix(mxT);
	g_pAimal[0]->SetShadingMode(GOURAUD_SHADING);
	// �]�w�K��
	g_pAimal[0]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pAimal[0]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);


	// �]�w �N�� Light �� WireSphere
	for (int i = 0; i < 6; i++) {
		g_pLight[i] = new CWireSphere(0.25f, 6, 3);
		g_pLight[i]->SetLightingDisable();
		g_pLight[i]->SetTextureLayer(NONE_MAP);	// �S���K��
		g_pLight[i]->SetShader();
		vT.x = i * 25; vT.y = 6.0f; vT.z = 0;
		mxT = Translate(vT);
		g_pLight[i]->SetTRSMatrix(mxT);
		g_pLight[0]->SetColor(g_Light1.diffuse);
	}

	// �]�����d�Ҥ��|�ʨ� Projection Matrix �ҥH�b�o�̳]�w�@���Y�i
	// �N���g�b OnFrameMove ���C���� Check
	bool bPDirty;
	mat4 mpx = camera->getProjectionMatrix(bPDirty);
	for (int i = 0; i < 6; i++) {
		g_pFloor[i]->SetProjectionMatrix(mpx);
		g_pLeftWall[i]->SetProjectionMatrix(mpx);
		g_pRightWall[i]->SetProjectionMatrix(mpx);
	}
	for (int i = 0; i < 7; i++) {
		g_pFrontWall[i]->SetProjectionMatrix(mpx);
	}
	g_pCube->SetProjectionMatrix(mpx);
	g_pSphere->SetProjectionMatrix(mpx);
	//obj1
	g_pObj1->SetProjectionMatrix(mpx);
	g_pObj2->SetProjectionMatrix(mpx);
	//animals
	g_pAimal[0]->SetProjectionMatrix(mpx);
	g_pAimal[1]->SetProjectionMatrix(mpx);
	g_pAimal[2]->SetProjectionMatrix(mpx);

	for (int i = 0; i < 6; i++) {
		g_pLight[i]->SetProjectionMatrix(mpx);
	}
}

//----------------------------------------------------------------------------
void GL_Display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window

	glEnable(GL_BLEND);  // �]�w2D Texure Mapping ���@��
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


#ifndef  MULTITEXTURE
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[0]);
	g_pFloor->Draw();
#else 
	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[0]); // �P Diffuse Map ���X
	glActiveTexture(GL_TEXTURE1); // select active texture 1
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[2]); // �P Light Map ���X
	for (int i = 0; i < 6; i++) {
		g_pFloor[i]->Draw();
		g_pLeftWall[i]->Draw();
		g_pRightWall[i]->Draw();
	}
	for (int i = 0; i < 7; i++) {
		g_pFrontWall[i]->Draw();
	}

	glActiveTexture(GL_TEXTURE0);
	//  glBindTexture(GL_TEXTURE_2D, 0);
#endif
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[1]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[4]);
	g_pCube->Draw();

	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[3]); // �P Diffuse Map ���X
	glActiveTexture(GL_TEXTURE1); // select active texture 1
	glBindTexture(GL_TEXTURE_CUBE_MAP, g_uiSphereCubeMap); // �P Light Map ���X
	g_pSphere->Draw();



	glBindTexture(GL_TEXTURE_2D, 0);
	for (int i = 0; i < 6; i++) {
		g_pLight[i]->Draw();
	}


	//animal
	glDepthMask(GL_FALSE);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiAimalTexID[0]);
	glActiveTexture(GL_TEXTURE2); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiAimalNormalTexID[0]);
	g_pAimal[0]->Draw();

	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiAimalTexID[1]);
	glActiveTexture(GL_TEXTURE2); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiAimalNormalTexID[1]);
	g_pAimal[1]->Draw();

	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiAimalTexID[2]);
	glActiveTexture(GL_TEXTURE2); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiAimalNormalTexID[2]);
	g_pAimal[2]->Draw();

	glDisable(GL_BLEND);	// ���� Blending
	glDepthMask(GL_TRUE);	// �}�ҹ� Z-Buffer ���g�J�ާ@

							//obj1
	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[5]); // �P Diffuse Map ���X
	glActiveTexture(GL_TEXTURE2); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiCheeseNormalTexID); // �P Diffuse Map ���X
	g_pObj1->Draw();

	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[6]); // �P Diffuse Map ���X
	glActiveTexture(GL_TEXTURE2); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiDianosourNormalTexID); // �P Diffuse Map ���X
	g_pObj2->Draw();
	glutSwapBuffers();	// �洫 Frame Buffer
}

//----------------------------------------------------------------------------
// Part 2 : for single light source
void UpdateLightPosition(float dt)
{
	mat4 mxT;
	// �C��¶ Y �b�� 90 ��
	g_fElapsedTime += dt;
	g_fLightTheta = g_fElapsedTime * (float)M_PI_2;
	if (g_fLightTheta >= (float)M_PI * 2.0f) {
		g_fLightTheta -= (float)M_PI * 2.0f;
		g_fElapsedTime -= 4.0f;
	}

	g_Light1.position.x = g_fLightRadius * cosf(g_fLightTheta);
	g_Light1.position.z = g_fLightRadius * sinf(g_fLightTheta);

	mxT = Translate(g_Light1.position);
	g_pLight[0]->SetTRSMatrix(mxT);

}
//----------------------------------------------------------------------------

void onFrameMove(float delta)
{

	if (g_bAutoRotating) { // Part 2 : ���s�p�� Light ����m
		UpdateLightPosition(delta);
	}

	mat4 mvx;	// view matrix & projection matrix
	bool bVDirty;	// view �P projection matrix �O�_�ݭn��s������
	auto camera = CCamera::getInstance();
	mvx = camera->getViewMatrix(bVDirty);
	if (bVDirty) {
		for (int i = 0; i < 6; i++) {
			g_pFloor[i]->SetViewMatrix(mvx);
			g_pLeftWall[i]->SetViewMatrix(mvx);
			g_pRightWall[i]->SetViewMatrix(mvx);

		}
		for (int i = 0; i < 7; i++) {
			g_pFrontWall[i]->SetViewMatrix(mvx);
		}
		g_pCube->SetViewMatrix(mvx);
		g_pSphere->SetViewMatrix(mvx);
		g_pSphere->SetViewPosition(camera->getViewPosition());
		//obj1
		g_pObj1->SetViewMatrix(mvx);
		g_pObj2->SetViewMatrix(mvx);
		//g_pObj2->SetTRSMatrix(RotateY(eye-at));

		//animals
		g_pAimal[0]->SetViewMatrix(mvx);
		g_pAimal[1]->SetViewMatrix(mvx);
		g_pAimal[2]->SetViewMatrix(mvx);

		for (int i = 0; i < 6; i++) {
			g_pLight[i]->SetViewMatrix(mvx);
		}

	}

	// �p�G�ݭn���s�p��ɡA�b�o��p��C�@�Ӫ����C��
	//for (int i = 0; i < 6; i++) {
	g_pFloor[0]->Update(delta, g_Light1);
	g_pLeftWall[0]->Update(delta, g_Light1);
	g_pRightWall[0]->Update(delta, g_Light1);
	g_pFrontWall[0]->Update(delta, g_Light1);
	g_pFloor[1]->Update(delta, g_Light2);
	g_pLeftWall[1]->Update(delta, g_Light2);
	g_pRightWall[1]->Update(delta, g_Light2);
	g_pFrontWall[1]->Update(delta, g_Light1);
	g_pFloor[2]->Update(delta, g_Light3);
	g_pLeftWall[2]->Update(delta, g_Light3);
	g_pRightWall[2]->Update(delta, g_Light3);
	g_pFrontWall[2]->Update(delta, g_Light2);
	g_pFloor[3]->Update(delta, g_Light4);
	g_pLeftWall[3]->Update(delta, g_Light4);
	g_pRightWall[3]->Update(delta, g_Light4);
	g_pFrontWall[3]->Update(delta, g_Light3);
	g_pFloor[4]->Update(delta, g_Light5);
	g_pLeftWall[4]->Update(delta, g_Light5);
	g_pRightWall[4]->Update(delta, g_Light5);
	g_pFrontWall[4]->Update(delta, g_Light4);
	g_pFloor[5]->Update(delta, g_Light6);
	g_pLeftWall[5]->Update(delta, g_Light6);
	g_pRightWall[5]->Update(delta, g_Light6);
	g_pFrontWall[5]->Update(delta, g_Light5);
	g_pFrontWall[6]->Update(delta, g_Light6);
	//}

	g_pCube->Update(delta, g_Light1);
	g_pSphere->Update(delta, g_Light2);
	g_pObj1->Update(delta, g_Light3);
	g_pObj2->Update(delta, g_Light3);
	g_pAimal[0]->Update(delta, g_Light6);
	g_pAimal[1]->Update(delta, g_Light5);
	g_pAimal[2]->Update(delta, g_Light4);
	for (int i = 0; i < 6; i++) {
		g_pLight[i]->Update(delta);
	}


	GL_Display();
}

//----------------------------------------------------------------------------

void Win_Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case  SPACE_KEY:

		break;
		//----------------------------------------------------------------------------
		// Part 2 : for single light source
	case 65: // A key
	case 97: // a key
		g_bAutoRotating = !g_bAutoRotating;
		break;
	case 82: // R key
		if (g_fLightR <= 0.95f) g_fLightR += 0.05f;
		g_Light1.diffuse.x = g_fLightR;
		g_pLight[0]->SetColor(g_Light1.diffuse);
		break;
	case 114: // r key
		if (g_fLightR >= 0.05f) g_fLightR -= 0.05f;
		g_Light1.diffuse.x = g_fLightR;
		g_pLight[0]->SetColor(g_Light1.diffuse);
		break;
	case 71: // G key
		if (g_fLightG <= 0.95f) g_fLightG += 0.05f;
		g_Light1.diffuse.y = g_fLightG;
		g_pLight[0]->SetColor(g_Light1.diffuse);
		break;
	case 103: // g key
		if (g_fLightG >= 0.05f) g_fLightG -= 0.05f;
		g_Light1.diffuse.y = g_fLightG;
		g_pLight[0]->SetColor(g_Light1.diffuse);
		break;
	case 66: // B key
		if (g_fLightB <= 0.95f) g_fLightB += 0.05f;
		g_Light1.diffuse.z = g_fLightB;
		g_pLight[0]->SetColor(g_Light1.diffuse);
		break;
	case 98: // b key
		if (g_fLightB >= 0.05f) g_fLightB -= 0.05f;
		g_Light1.diffuse.z = g_fLightB;
		g_pLight[0]->SetColor(g_Light1.diffuse);
		break;
		//----------------------------------------------------------------------------
	case 033:
		glutIdleFunc(NULL);
		delete g_pCube;
		//delete [] g_pFloor;
		//delete g_pLight;
		delete g_pAimal[0];
		delete g_pAimal[1];
		delete g_pAimal[2];
		CCamera::getInstance()->destroyInstance();
		CTexturePool::getInstance()->destroyInstance();
		exit(EXIT_SUCCESS);
		break;
	}
}

//----------------------------------------------------------------------------
void Win_Mouse(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:   // �ثe���U���O�ƹ�����
		//if ( state == GLUT_DOWN ) ; 
		break;
	case GLUT_MIDDLE_BUTTON:  // �ثe���U���O�ƹ����� �A���� Y �b
		//if ( state == GLUT_DOWN ) ; 
		break;
	case GLUT_RIGHT_BUTTON:   // �ثe���U���O�ƹ��k��
		//if ( state == GLUT_DOWN ) ;
		break;
	default:
		break;
	}
}
//----------------------------------------------------------------------------
void Win_SpecialKeyboard(int key, int x, int y) {
	auto camera = CCamera::getInstance();
	vec4 camPos = camera->getViewPos();
	float cameraSpeed = 0.5f;
	vec4 localAt = camera->getAt();

	vec4 up = camera->getUp();
	switch (key) {
	case GLUT_KEY_UP:		// �ثe���U���O�V����V��
		//printf("eye:%f\n", eye.x);
		if (eye.x > 3.0f && eye.x < 5.0f && eye.z > -1.5f && eye.z < 0.5f) {
			g_bAutoRotating = !g_bAutoRotating;
		}
		if (eye.x > -8.0f && eye.x < 133.0f && eye.z > -8.0f && eye.z < 8.0f) {
			eye = eye + cameraSpeed * normalize(vec4(at.x, 0, at.z, 0));
		}
		else {
			eye = eye - 2 * cameraSpeed * normalize(vec4(at.x, 0, at.z, 0));
		}
		camera->updateViewLookAt(eye, eye + at);

		break;
	case GLUT_KEY_DOWN:	// �ثe���U���O�V�k��V��
		if (eye.x > 3.0f && eye.x < 5.0f && eye.z > -1.5f && eye.z < 0.5f) {
			g_bAutoRotating = !g_bAutoRotating;
		}
		if (eye.x > -8.0f && eye.x < 133.0f && eye.z > -8.0f && eye.z < 8.0f) {

			eye = eye - cameraSpeed * normalize(vec4(at.x, 0, at.z, 0));
		}
		else {
			eye = eye + 2 * cameraSpeed * normalize(vec4(at.x, 0, at.z, 0));
		}
		camera->updateViewLookAt(eye, eye + at);
		break;
	case GLUT_KEY_LEFT:		// �ثe���U���O�V����V��
		if (eye.x > 3.0f && eye.x < 5.0f && eye.z > -1.5f && eye.z < 0.5f) {
			g_bAutoRotating = !g_bAutoRotating;
		}
		if (eye.x > -8.0f && eye.x < 133.0f && eye.z > -8.0f && eye.z < 8.0f) {

			eye = eye + cameraSpeed * normalize(vec4(sin(g_fTheta) * sin(g_fPhi + M_PI / 2), 0, sin(g_fTheta) * cos(g_fPhi + M_PI / 2), 0));
		}
		else {
			eye = eye - 2 * cameraSpeed * normalize(vec4(sin(g_fTheta) * sin(g_fPhi + M_PI / 2), 0, sin(g_fTheta) * cos(g_fPhi + M_PI / 2), 0));

		}
		camera->updateViewLookAt(eye, eye + at);

		break;
	case GLUT_KEY_RIGHT:		// �ثe���U���O�V����V��
		if (eye.x > 3.0f && eye.x < 5.0f && eye.z > -1.5f && eye.z < 0.5f) {
			g_bAutoRotating = !g_bAutoRotating;
		}
		if (eye.x > -8.0f && eye.x < 133.0f && eye.z > -8.0f && eye.z < 8.0f) {

			eye = eye - cameraSpeed * normalize(vec4(sin(g_fTheta) * sin(g_fPhi + M_PI / 2), 0, sin(g_fTheta) * cos(g_fPhi + M_PI / 2), 0));
		}
		else {
			eye = eye + 2 * cameraSpeed * normalize(vec4(sin(g_fTheta) * sin(g_fPhi + M_PI / 2), 0, sin(g_fTheta) * cos(g_fPhi + M_PI / 2), 0));

		}
		camera->updateViewLookAt(eye, eye + at);

		break;
	default:
		break;
	}
}

//----------------------------------------------------------------------------
// The passive motion callback for a window is called when the mouse moves within the window while no mouse buttons are pressed.
void Win_PassiveMotion(int x, int y) {

	g_fPhi = (float)-M_PI * (x - HALF_SIZE) / (HALF_SIZE); // �ഫ�� g_fPhi ���� -PI �� PI ���� (-180 ~ 180 ����)
	g_fTheta = (float)M_PI * (float)y / SCREEN_SIZE;
	auto camera = CCamera::getInstance();
	point4 _at;
	_at = point4(g_fRadius * sin(g_fTheta) * sin(g_fPhi), g_fRadius * cos(g_fTheta), g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.0f);
	at = _at;
	camera->updateViewLookAt(eye, eye + _at);
	//
	vec3 vBill = vec3(eye.x - 50.5f, 0.0, eye.z + 2.0);
	vBill = normalize(vBill);

	mat4 mxT = Translate(50.5f, 0.5f, -2.0f) * RotateY((float)atan2(vBill.x, vBill.z) / DegreesToRadians) * Scale(0.05f, 0.05f, 0.05f);
	g_pObj2->SetTRSMatrix(mxT);
}

// The motion callback for a window is called when the mouse moves within the window while one or more mouse buttons are pressed.
void Win_MouseMotion(int x, int y) {
	g_fPhi = (float)-M_PI * (x - HALF_SIZE) / (HALF_SIZE);  // �ഫ�� g_fPhi ���� -PI �� PI ���� (-180 ~ 180 ����)
	g_fTheta = (float)M_PI * (float)y / SCREEN_SIZE;;
	point4  eye(g_fRadius * sin(g_fTheta) * sin(g_fPhi), g_fRadius * cos(g_fTheta), g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.0f);
	point4  at(0.0f, 0.0f, 0.0f, 1.0f);
	CCamera::getInstance()->updateViewLookAt(eye, at);
}
//----------------------------------------------------------------------------
void GL_Reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	glClearColor(0.0, 0.0, 0.0, 1.0); // black background
	glEnable(GL_DEPTH_TEST);
}

//----------------------------------------------------------------------------

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_SIZE, SCREEN_SIZE);

	// If you use freeglut the two lines of code can be added to your application 
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("Compositing and Blending");

	// The glewExperimental global switch can be turned on by setting it to GL_TRUE before calling glewInit(), 
	// which ensures that all extensions with valid entry points will be exposed.
	glewExperimental = GL_TRUE;
	glewInit();

	init();

	glutMouseFunc(Win_Mouse);
	glutMotionFunc(Win_MouseMotion);
	glutPassiveMotionFunc(Win_PassiveMotion);
	glutKeyboardFunc(Win_Keyboard);	// �B�z ASCI ����p A�Ba�BESC ��...����
	glutSpecialFunc(Win_SpecialKeyboard);	// �B�z NON-ASCI ����p F1�BHome�B��V��...����
	glutDisplayFunc(GL_Display);
	glutReshapeFunc(GL_Reshape);
	glutIdleFunc(IdleProcess);

	glutMainLoop();
	return 0;
}
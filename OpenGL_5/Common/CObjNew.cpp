#include "CObjNew.h"
// Example 4 開始
// 面同 Example 3 朝上(Y軸)
// 每一個 Vertex 增加 Normal ，改成繼曾自 CShape，一併處理相關的設定需求


CObjNew::CObjNew(char *adrFile)
{
	m_iNumVtx = QUAD_NUM;
	m_pPoints = NULL; m_pNormals = NULL; m_pTex = NULL;// m_pTex有紅線要去CShape新增

	//引數暫存變數
	int vindex = 0;
	int nindex = 0;
	int tindex = 0;
	int mindex = 0;

	objfile = fopen(adrFile, "r");
	if (objfile == NULL) {
		printf("fail to open the file !\n");
	}
	else {
		while (fscanf(objfile, "%s", lineHeader) != EOF) {
			if (strcmp(lineHeader, "f") == 0) { vindex++; } //計算點總數
		}
		fclose(objfile);
		m_iNumVtx = vindex * 3;
		m_pPoints = new vec4[m_iNumVtx + 1];
		vindex = 0; mindex = 0;//另當後續點的暫存

							   //依照實際頂點數決定陣列大小
		m_pPoints = new vec4[m_iNumVtx];
		m_pNormals = new vec3[m_iNumVtx];
		m_pColors = new vec4[m_iNumVtx];
		m_pTex = new vec2[m_iNumVtx];
		vec4 *temp_vertices = new vec4[m_iNumVtx];
		vec3 *temp_pNormals = new vec3[m_iNumVtx];
		vec4 *temp_pColors = new vec4[m_iNumVtx];
		vec2 *temp_pTex = new vec2[m_iNumVtx];

		objfile = fopen(adrFile, "r");

		while (fscanf(objfile, "%s", lineHeader) != EOF) {
			if (strcmp(lineHeader, "v") == 0) {
				fscanf(objfile, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
				temp_vertices[vindex] = vec4(vertex[0], vertex[1], vertex[2], 1.0f);
				vindex++;
			}
			else if (strcmp(lineHeader, "vn") == 0) {
				fscanf(objfile, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
				temp_pNormals[nindex] = vec3(vertex[0], vertex[1], vertex[2]);
				nindex++;
			}

			else if (strcmp(lineHeader, "vt") == 0) {
				fscanf(objfile, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
				temp_pTex[tindex] = vec2(vertex[0], vertex[1]);
				//m_pTex[tindex] = temp_pTex[tindex];
				tindex++;
			}
			else if (strcmp(lineHeader, "f") == 0) {
				//請依照自己的OBJ檔決定讀取方式
				//"f 頂點索引/uv點索引/法線索引"
				fscanf(objfile, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &indices[0], &indices[1], &indices[2], &indices[3], &indices[4], &indices[5], &indices[6], &indices[7], &indices[8]);
				m_pPoints[mindex] = temp_vertices[indices[0] - 1];
				m_pPoints[mindex + 1] = temp_vertices[indices[3] - 1];
				m_pPoints[mindex + 2] = temp_vertices[indices[6] - 1];
				m_pNormals[mindex] = temp_pNormals[indices[2] - 1];
				m_pNormals[mindex + 1] = temp_pNormals[indices[5] - 1];
				m_pNormals[mindex + 2] = temp_pNormals[indices[8] - 1];
				m_pTex[mindex] = temp_pTex[indices[1] - 1];//我家的
				m_pTex[mindex + 1] = temp_pTex[indices[4] - 1];//我家的
				m_pTex[mindex + 2] = temp_pTex[indices[7] - 1];//我家的
				mindex += 3;
			}
		}
	}
	fclose(objfile);
	// 預設將所有的面都設定成灰色
	for( int i = 0 ; i < m_iNumVtx ; i++ ) m_pColors[i] = vec4(-1.0f,-1.0f,-1.0f,1.0f);


	m_pTex1 = new vec2[m_iNumVtx];		// 產生所需的貼圖座標
#if MULTITEXTURE >= LIGHT_MAP
	m_pTex2 = new vec2[m_iNumVtx];  // 產生 light map 所需的貼圖座標
#endif
#if MULTITEXTURE >= NORMAL_MAP
	m_pTex3 = new vec2[m_iNumVtx];	// 產生 normal map 所需的貼圖座標
	// 產生所需要的 Tanget vector for each vertex
	m_pTangentV = new vec3[m_iNumVtx];
#endif

	for (int i = 0; i < m_iNumVtx; i++) {//我+的 然後把原本設定點的地方刪掉
		m_pTex1[i] = m_pTex[i];
	}

	
#if MULTITEXTURE >= LIGHT_MAP

	for (int i = 0; i < m_iNumVtx; i++) {//我+的 然後把原本設定點的地方刪掉
		m_pTex2[i] = m_pTex[i];
	}

#endif
#if MULTITEXTURE >= NORMAL_MAP

	for (int i = 0; i < m_iNumVtx; i++) {//我+的 然後把原本設定點的地方刪掉
		m_pTex3[i] = m_pTex[i];
	}

	// 計算 tangent vector
	for (int i = 0; i < m_iNumVtx; i += 3) { // i < 6 要改成 i < m_iNumVtx (點個數)
		float dU1 = m_pTex1[i + 1].x - m_pTex1[i].x;
		float dV1 = m_pTex1[i + 1].y - m_pTex1[i].y;
		float dU2 = m_pTex1[i + 2].x - m_pTex1[i].x;
		float dV2 = m_pTex1[i + 2].y - m_pTex1[i].y;
		float f = 1.0f/(dU1 * dV2 - dU2*dV1);
		vec4 E1 = m_pPoints[i + 1] - m_pPoints[i];
		vec4 E2 = m_pPoints[i + 2] - m_pPoints[i];

		vec3 tangent;
		tangent.x = f*(dV2 * E1.x + E2.x * (-dV1));
		tangent.y = f*(dV2 * E1.y + E2.y * (-dV1));
		tangent.z = f*(dV2 * E1.z + E2.z * (-dV1));

		m_pTangentV[i] += tangent;
		m_pTangentV[i + 1] += tangent;
		m_pTangentV[i + 2] = tangent;
	}
	for (int i = 0; i < m_iNumVtx; i++)
		m_pTangentV[i] = normalize(m_pTangentV[i]);
#endif

#ifdef PERVERTEX_LIGHTING
	SetShaderName("vsPerVtxLighting.glsl", "fsPerVtxLighting.glsl");
#else
	SetShaderName("vsPerPixelLighting.glsl", "fsPerPixelLighting.glsl");
#endif

}

void CObjNew::RenderWithFlatShading(const LightSource &lights)
{
	// 以每一個面的三個頂點計算其重心，以該重心作為顏色計算的點頂
	// 根據 Phong lighting model 計算相對應的顏色，並將顏色儲存到此三個點頂
	// 因為每一個平面的頂點的 Normal 都相同，所以此處並沒有計算此三個頂點的平均 Normal
	vec4 vCentroidP;
	for (int i = 0; i < m_iNumVtx; i += 3) {
		// 計算三角形的重心
		vCentroidP = (m_pPoints[i] + m_pPoints[i + 1] + m_pPoints[i + 2]) / 3.0f;
		m_pColors[i] = m_pColors[i + 1] = m_pColors[i + 2] = PhongReflectionModel(vCentroidP, m_pNormals[i], lights);
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx + sizeof(vec3)*m_iNumVtx, sizeof(vec4)*m_iNumVtx, m_pColors); // vertcies' Color
}


void CObjNew::RenderWithGouraudShading(const LightSource &lights)
{
	// 以 vertex 為單位，根據該點的位置與其法向量，利用 Phong lighting model 計算相對應的顏色
	// 將該顏色儲存回該頂點

	// 第六個點頂構成的兩個三角形
	// 計算 0 1 2 5 四個頂點的顏色即可，0 與 3、2 與 4 的顏色相同

	m_pColors[0] = m_pColors[3] = PhongReflectionModel(m_pPoints[0], m_pNormals[0], lights);
	m_pColors[2] = m_pColors[4] = PhongReflectionModel(m_pPoints[2], m_pNormals[2], lights);
	m_pColors[1] = PhongReflectionModel(m_pPoints[1], m_pNormals[1], lights);
	m_pColors[5] = PhongReflectionModel(m_pPoints[5], m_pNormals[5], lights);

	glBindBuffer(GL_ARRAY_BUFFER, m_uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx + sizeof(vec3)*m_iNumVtx, sizeof(vec4)*m_iNumVtx, m_pColors); // vertcies' Color
}



void CObjNew::Update(float dt, const LightSource &lights)
{
#ifdef LIGHTING_WITHCPU
	if( m_bViewUpdated || m_bTRSUpdated  ) { // Model View 的相關矩陣內容有更動
		m_mxMVFinal = m_mxView * m_mxTRS;
		m_mxMV3X3Final = mat3(
			m_mxMVFinal._m[0].x,  m_mxMVFinal._m[1].x, m_mxMVFinal._m[2].x,
			m_mxMVFinal._m[0].y,  m_mxMVFinal._m[1].y, m_mxMVFinal._m[2].y,
			m_mxMVFinal._m[0].z,  m_mxMVFinal._m[1].z, m_mxMVFinal._m[2].z);
#ifdef GENERAL_CASE
		m_mxITMV = InverseTransposeMatrix(m_mxMVFinal); 
#endif
		m_bViewUpdated = m_bTRSUpdated = false;
	}
	if (m_iMode == FLAT_SHADING) RenderWithFlatShading(lights);
	else RenderWithGouraudShading(lights);

#else // Lighting With GPU
	if (m_bViewUpdated || m_bTRSUpdated) {
		m_mxMVFinal = m_mxView * m_mxTRS;
		m_bViewUpdated = m_bTRSUpdated = false;
	}
	m_vLightInView = m_mxView * lights.position;		// 將 Light 轉換到鏡頭座標再傳入
	// 算出 AmbientProduct DiffuseProduct 與 SpecularProduct 的內容
	m_AmbientProduct = m_Material.ka * m_Material.ambient  * lights.ambient;
	m_AmbientProduct.w = m_Material.ambient.w;
	m_DiffuseProduct = m_Material.kd * m_Material.diffuse  * lights.diffuse;
	m_DiffuseProduct.w = m_Material.diffuse.w;
	m_SpecularProduct = m_Material.ks * m_Material.specular * lights.specular;
	m_SpecularProduct.w = m_Material.specular.w;
#endif

}


void CObjNew::Update(float dt)
{
	if (m_bViewUpdated || m_bTRSUpdated) { // Model View 的相關矩陣內容有更動
		m_mxMVFinal = m_mxView * m_mxTRS;
		m_mxITMV = InverseTransposeMatrix(m_mxMVFinal); 
		m_bViewUpdated = m_bTRSUpdated = false;
	}
}

void CObjNew::SetVtxColors(vec4 vLFColor, vec4 vLRColor, vec4 vTRColor, vec4 vTLColor)
{
	m_pColors[3] = m_pColors[0] = vLFColor;
	m_pColors[1] = vLRColor;
	m_pColors[4] = m_pColors[2] = vTRColor;
	m_pColors[5] = vTLColor;

	// 將所有 vertices 顏色更新到 VBO 中
	glBindBuffer( GL_ARRAY_BUFFER, m_uiBuffer );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx+sizeof(vec3)*m_iNumVtx, sizeof(vec4)*m_iNumVtx, m_pColors ); // vertcies' Color
}

void CObjNew::SetVtxColors(vec4 vFColor, vec4 vSColor) // 設定兩個三角形的顏色
{
	m_pColors[0] = m_pColors[1] = m_pColors[2] = vFColor;
	m_pColors[3] = m_pColors[4] = m_pColors[5] = vSColor;

	// 將所有 vertices 顏色更新到 VBO 中
	glBindBuffer( GL_ARRAY_BUFFER, m_uiBuffer );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx+sizeof(vec3)*m_iNumVtx, sizeof(vec4)*m_iNumVtx, m_pColors ); // vertcies' Color
}

void CObjNew::Draw()
{
	DrawingSetShader();
//	glBindTexture(GL_TEXTURE_2D, m_uiTexObject[0]); 
	glDrawArrays( GL_TRIANGLES, 0, m_iNumVtx);
//	glBindTexture(GL_TEXTURE_2D, 0);  // 釋放現有的貼圖設定
}

void CObjNew::DrawW()
{
	DrawingWithoutSetShader();
//	glBindTexture(GL_TEXTURE_2D, m_uiTexObject[0]); 
	glDrawArrays( GL_TRIANGLES, 0, m_iNumVtx);
//	glBindTexture(GL_TEXTURE_2D, 0);  // 釋放現有的貼圖設定
}

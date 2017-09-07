#include "CommonMeshes.h"
#include <nclgl\OBJMesh.h>

Mesh* CommonMeshes::m_pPlane	= NULL;
Mesh* CommonMeshes::m_pCube		= NULL;
Mesh* CommonMeshes::m_pSphere	= NULL;

GLuint CommonMeshes::m_CheckerboardTex = 0;
GLuint CommonMeshes::m_SunTex = 1;
GLuint CommonMeshes::m_MercTex = 2;
GLuint CommonMeshes::m_VenTex = 3;
GLuint CommonMeshes::m_EarthTex = 4;
GLuint CommonMeshes::m_MarsTex = 5;
GLuint CommonMeshes::m_JupiTex = 6;
GLuint CommonMeshes::m_RoidTex = 7;
GLuint CommonMeshes::m_MoonTex = 8;
GLuint CommonMeshes::m_TargetTex = 9;

void CommonMeshes::InitializeMeshes()
{
	if (m_pPlane == NULL)
	{
		m_CheckerboardTex = SOIL_load_OGL_texture(TEXTUREDIR"checkerboard.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		m_SunTex = SOIL_load_OGL_texture(TEXTUREDIR"texture_sun.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		m_MercTex = SOIL_load_OGL_texture(TEXTUREDIR"mercury.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		m_VenTex = SOIL_load_OGL_texture(TEXTUREDIR"venus.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		m_EarthTex = SOIL_load_OGL_texture(TEXTUREDIR"earth.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		m_MarsTex = SOIL_load_OGL_texture(TEXTUREDIR"mars.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		m_JupiTex = SOIL_load_OGL_texture(TEXTUREDIR"jupiter.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		m_RoidTex = SOIL_load_OGL_texture(TEXTUREDIR"checkerboard.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		m_MoonTex = SOIL_load_OGL_texture(TEXTUREDIR"moon.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		m_MoonTex = SOIL_load_OGL_texture(TEXTUREDIR"target.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

		glBindTexture(GL_TEXTURE_2D, m_CheckerboardTex);
		glBindTexture(GL_TEXTURE_2D, m_SunTex);
		glBindTexture(GL_TEXTURE_2D, m_MercTex);
		glBindTexture(GL_TEXTURE_2D, m_VenTex);
		glBindTexture(GL_TEXTURE_2D, m_EarthTex);
		glBindTexture(GL_TEXTURE_2D, m_MarsTex);
		glBindTexture(GL_TEXTURE_2D, m_JupiTex);
		glBindTexture(GL_TEXTURE_2D, m_RoidTex);
		glBindTexture(GL_TEXTURE_2D, m_MoonTex);
		glBindTexture(GL_TEXTURE_2D, m_TargetTex);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); //No linear interpolation to get crisp checkerboard no matter the scalling
		
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, 1);
		glBindTexture(GL_TEXTURE_2D, 2);
		glBindTexture(GL_TEXTURE_2D, 3);
		glBindTexture(GL_TEXTURE_2D, 4);
		glBindTexture(GL_TEXTURE_2D, 5);
		glBindTexture(GL_TEXTURE_2D, 6);
		glBindTexture(GL_TEXTURE_2D, 7);
		glBindTexture(GL_TEXTURE_2D, 8);
		glBindTexture(GL_TEXTURE_2D, 9);

		m_pPlane = Mesh::GenerateQuadTexCoordCol(Vector2(1.f, 1.f), Vector2(0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		m_pCube = new OBJMesh(MESHDIR"cube.obj");
		m_pSphere = new OBJMesh(MESHDIR"sphere.obj");

		m_pPlane->SetTexture(m_CheckerboardTex);
		m_pCube->SetTexture(m_CheckerboardTex);
		m_pSphere->SetTexture(m_CheckerboardTex);
	}
}

void CommonMeshes::ReleaseMeshes()
{
	if (m_pPlane != NULL)
	{
		glDeleteTextures(0, &m_CheckerboardTex);
		delete m_pPlane;
		delete m_pCube;
		delete m_pSphere;
	}

	m_pPlane = NULL;
}
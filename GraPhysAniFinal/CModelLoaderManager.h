#ifndef _CModelLoaderManager_HG_
#define _CModelLoaderManager_HG_

// This AMAZING class will do the following:
// * Load models from files
// * Place the model information into the sexy OpenGL VBOs+Buffers
// * Keep track (for looking up) model to drawing data

#include <vector>
#include <string>
#include <map>

#include "CPly/CPlyFile5nt.h"
// NOTE: I took these out as the CFactory was including this file, and I didn't want OpenGL stuff all over the place
//#include <GL\glxew.h>		// From the OpenGL site
//#include <GL\glew.h>
//#include <GL\freeglut.h>	



class CModelLoaderManager
{
public:
	CModelLoaderManager();
	~CModelLoaderManager();
	// Used in the LoadModels
	// If bScaleToBoundingBox==true, then scale_or_bounding_box is bounding box size
	// (will be scaled to fit into that sized bounding box)
	struct CLoadInfo
	{
		CLoadInfo();
		CLoadInfo( std::string modelFileName );
		CLoadInfo( std::string modelFileName, float scale );
		CLoadInfo( std::string modelFileName, float boundingBox, bool bScaleToBoundingBox );
		std::string modelFileName;
		float scale_or_bounding_box;
		bool bScaleToBoundingBox;
		bool bNormalizeIfNormalsNotPresent;
		bool bForceNormalRecalc;
		// Added October 17, 2014
		bool bGenerateUVCoordsIfNotPresent;
		float TexUVScale;		// Used to generate the spherical coords (default is 1.0)

		// Called by the constructors
		void Reset(void);
	};

	void SetRootDirectory( std::string rootDirectory );
	std::string GetRootDirectory(void);

	//bool LoadModels( std::vector< std::string > vecFileNames );
	bool LoadModels( std::vector< CLoadInfo > vecFileNames );

	bool GetRenderingInfoByModelFileName( std::string modelName, 
		                                  CPlyInfo &theInfo );
	// Deletes any buffers
	bool ShutDown(void);

	std::wstring ASCII_to_Unicode( std::string ASCIIstring );
	std::string Unicode_to_ASCII( std::wstring UnicodeString );

	// ********************************
	// ADDED for the physics demo
	// Returns 0 if can't find it
	CPlyFile5nt* getOriginalModelInfo( std::string modelName );

private:
	std::vector< CPlyInfo > m_VecModels;

	std::string m_RootDirectoryName;

	// To keep the original ply information around
	std::map< std::string, CPlyFile5nt* > m_mapNameToCPlyFile;

	// SECRET functions only this class my call...
	// Update: took out the GLuint, replaced with "unsigned int"
	bool m_LoadPlyFileIntoGLBuffers( CLoadInfo plyFileLoadInfo,
							         unsigned int &vertexBufferObjectID,			/* GLuint &vertexBufferID, */
							         unsigned int &vertexAttribBufferID,	/* GLuint &vertexColourBufferID, */
									 //unsigned int &normalBufferID,
							         unsigned int &indexBufferID,			/* GLuint &indexBufferID, */
							         int &numberOfVerticesLoaded, 
							         int &numberOfTrianglesLoaded, 
									 float &maxExtent,
									 CVector3f &minExtentX,
									 CVector3f &maxExtentX,
									 CVector3f &minExtentY,
									 CVector3f &maxExtentY,
									 CVector3f &minExtentZ,
									 CVector3f &maxExtentZ);

	//bool m_LoadPlyFile(std::string plyFileName, 
	//			 std::vector<CVertex> &vecVertices, 
	//			 std::vector<CTriangle> &vecTriangles );
};

#endif

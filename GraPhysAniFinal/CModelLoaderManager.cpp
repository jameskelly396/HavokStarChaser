#include "CModelLoaderManager.h"

//#include "CTriangle.h"		// also includes CVertex
#include <fstream>			// For loading the files ifstream

#include <sstream>			

//#include "Utils.h"			// No longer needed
#include "global.h"				// Only for ExitOnGLError()

#include "VertexTypes.h"	// the "vertex" classes

// Called by the constructors
void CModelLoaderManager::CLoadInfo::Reset(void)
{
	this->scale_or_bounding_box = 1.0f;
	this->bScaleToBoundingBox = false;
	this->bNormalizeIfNormalsNotPresent = true;
	this->bForceNormalRecalc = false;
	this->bGenerateUVCoordsIfNotPresent = true;
	this->TexUVScale = 1.0f;	// Default. Higher numbers basically repeat the texture
	return;
}

CModelLoaderManager::CLoadInfo::CLoadInfo()
{
	this->Reset();
	return;
}

CModelLoaderManager::CLoadInfo::CLoadInfo( std::string modelFileName )
{
	this->Reset();
	this->modelFileName = modelFileName;
	return;
}

CModelLoaderManager::CLoadInfo::CLoadInfo( std::string modelFileName, float scale )
{
	this->Reset();
	this->scale_or_bounding_box = scale;
	this->modelFileName = modelFileName;
	return;
}


CModelLoaderManager::CLoadInfo::CLoadInfo( std::string modelFileName, float boundingBox, bool bScaleToBoundingBox )
{
	this->Reset();
	this->modelFileName = modelFileName;
	this->bScaleToBoundingBox = bScaleToBoundingBox;
	this->scale_or_bounding_box = boundingBox;
	return;
};

bool CModelLoaderManager::m_LoadPlyFileIntoGLBuffers( CLoadInfo plyFileLoadInfo,
							   unsigned int &vertexBufferObjectID,					/* GLuint &vertexBufferID, */
							   unsigned int &vertexAttribBufferID,				/* GLuint &vertexColourBufferID, */
							   //unsigned int &normalBufferID,
							   unsigned int &indexBufferID,						/* GLuint &indexBufferID, */
							   int &numberOfVerticesLoaded, 
							   int &numberOfTrianglesLoaded, 
							   float &maxExtent,
							   CVector3f &minExtentX,
							   CVector3f &maxExtentX,
							   CVector3f &minExtentY,
							   CVector3f &maxExtentY,
							   CVector3f &minExtentZ,
							   CVector3f &maxExtentZ)
{

	// Append the root directory to the file name
	std::stringstream ss;
	ss << this->m_RootDirectoryName << "/" << plyFileLoadInfo.modelFileName;

	std::wstring error;	// Returns error info in this string

	// ********************************************* //
	// Experimental
	//CPlyFile5nt* pPlyLoaderFast = new CPlyFile5nt();
	//{
	//	return false;
	//}

	// ********************************************* //

	CPlyFile5nt* pPlyLoader = new CPlyFile5nt();

	//if ( ! pPlyLoader->OpenPLYFile( this->ASCII_to_Unicode( ss.str() ), error ) )
	if ( ! pPlyLoader->OpenPLYFile2( this->ASCII_to_Unicode( ss.str() ), error ) )
	{
		return false;
	}

	// Calculate normals?
	if ( plyFileLoadInfo.bForceNormalRecalc )
	{
		pPlyLoader->normalizeTheModelBaby();
	}
	else if ( !pPlyLoader->bHadNormalsInFile() && plyFileLoadInfo.bNormalizeIfNormalsNotPresent )
	{
		pPlyLoader->normalizeTheModelBaby();
	}

	pPlyLoader->normalizeTheModelTriangle();

	// Are we supposed to scale (i.e. Scale != 1.0)
	static const float scaleOf1 = 1.0f;
	if ( ( plyFileLoadInfo.scale_or_bounding_box != scaleOf1 ) || plyFileLoadInfo.bScaleToBoundingBox )
	{	// The scale isn't 1, so you want scaling...
		
		float scale = plyFileLoadInfo.scale_or_bounding_box;

		// Scale or scale to bounding box? 
		if ( plyFileLoadInfo.bScaleToBoundingBox )
		{
			scale = scale / pPlyLoader->getMaxExtent();
		}

		// Scale it, baby
		pPlyLoader->scaleVertices( scale );
		pPlyLoader->calcualteExtents();
	}

	
	maxExtent = pPlyLoader->getMaxExtent();

	// ADDED: October 17, 2014
	if ( !pPlyLoader->bHadTextureCoordinatesInFile() )
	{
		if ( plyFileLoadInfo.bGenerateUVCoordsIfNotPresent )
		{
			pPlyLoader->GenTextureCoordsSpherical( CPlyFile5nt::POSITIVE_X, 
			                                       CPlyFile5nt::POSITIVE_Y, 
												   true,		/* based on normals */
												   1.0f, // plyFileLoadInfo.TexUVScale, 
												   false );		/* do fast "quick-n-dirty" calculation... which is hardly must faster */
		}
	}


	// Create a dynamic array to hold vertices...
	numberOfVerticesLoaded = pPlyLoader->GetNumberOfVerticies();
	
	CVertex_fXYZW* p_arrayBunnyVertPositions = new CVertex_fXYZW[ numberOfVerticesLoaded ];
	CVertex_fXYZW* p_arrayBunnyVertColours = new CVertex_fXYZW[ numberOfVerticesLoaded ];
	CVertex_fXYZW* p_arrayBunnyVertNormals = new CVertex_fXYZW[ numberOfVerticesLoaded ];
	// ADDED: October 17, 2014
	CVertex_fXYZW* p_arrayBunnyVertTexUV_x2 = new CVertex_fXYZW[ numberOfVerticesLoaded ];

	// Copy the data from the bunny vector to this array
	for ( int vertIndex = 0; vertIndex != numberOfVerticesLoaded; vertIndex++ )
	{
		PLYVERTEX tempVertex = pPlyLoader->getVertex_at( vertIndex );		

		p_arrayBunnyVertPositions[vertIndex].x = tempVertex.xyz.x;
		if (minExtentX.x > tempVertex.xyz.x)
			minExtentX = tempVertex.xyz;
		else if (maxExtentX.x < tempVertex.xyz.x)
			maxExtentX = tempVertex.xyz;
		p_arrayBunnyVertPositions[vertIndex].y = tempVertex.xyz.y;
		if (minExtentY.y > tempVertex.xyz.y) minExtentY = tempVertex.xyz;
		else if (maxExtentY.y < tempVertex.xyz.y) maxExtentY = tempVertex.xyz;
		p_arrayBunnyVertPositions[vertIndex].z = tempVertex.xyz.z;
		if (minExtentZ.z > tempVertex.xyz.z) minExtentZ = tempVertex.xyz;
		else if (maxExtentZ.z < tempVertex.xyz.z) maxExtentZ = tempVertex.xyz;
		p_arrayBunnyVertPositions[vertIndex].w = 1.0f;

		p_arrayBunnyVertColours[vertIndex].x = 1.0f;	// 0.25f;	//tempVertex.red;		// Red
		p_arrayBunnyVertColours[vertIndex].y = 1.0f;	// 0.75f;	//tempVertex.green;		// Green
		p_arrayBunnyVertColours[vertIndex].z = 1.0f;	// 0.65f;	//tempVertex.blue;		// Blue
		p_arrayBunnyVertColours[vertIndex].w = 1.0f;				// Alpha

		p_arrayBunnyVertNormals[vertIndex].x = tempVertex.nx;	
		p_arrayBunnyVertNormals[vertIndex].y = tempVertex.ny;	
		p_arrayBunnyVertNormals[vertIndex].z = tempVertex.nz;	
		p_arrayBunnyVertNormals[vertIndex].w = 1.0f;	

		// ADDED: October 17, 2014
		// NOTE: There are TWO sets of texture coords here, but is pretty unusual
		//       In fact, we likely -won't- use the second set.
		p_arrayBunnyVertTexUV_x2[vertIndex].x = tempVertex.tex0u;
		p_arrayBunnyVertTexUV_x2[vertIndex].y = tempVertex.tex0v;
		p_arrayBunnyVertTexUV_x2[vertIndex].z = tempVertex.tex1u;
		p_arrayBunnyVertTexUV_x2[vertIndex].w = tempVertex.tex1v;

	}


	// Add the model to the ply models
	this->m_mapNameToCPlyFile[plyFileLoadInfo.modelFileName] = pPlyLoader;


	// Now set up the OpenGL buffers and copy the information over

	// Get an ID (aka "name") for the two-part vertex buffer
	glGenVertexArrays(1, &vertexBufferObjectID );
	glBindVertexArray( vertexBufferObjectID );
	ExitOnGLError("ERROR: Could not generate the VAO");
  
	// "Bind" them, which means they are ready to be loaded with data
	glGenBuffers(1, &vertexAttribBufferID );
	glBindBuffer(GL_ARRAY_BUFFER, vertexAttribBufferID);	
	ExitOnGLError("ERROR: Could not bind the VAO");

	//// "Bind" them, which means they are ready to be loaded with data
	//glGenBuffers(1, &normalBufferID );
	//glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);	
	//ExitOnGLError("ERROR: Could not bind the VAO");


	// Figure out how big (in "system unit" aka "bytes") the local 
	//	array of vertex information is (i.e. how big is each vertex X number of vertices)
	//int bytesInVertexArray = numberOfVerticesLoaded * sizeof(Vertex);
	//int bytesInVertexArray = numberOfVerticesLoaded * sizeof(CVertex_fXYZ_fRGBA_fNxNyNzW);

	// This does the actual copy of the date from the "client" (CPU side)
	//	to the GPU ("video card") side. 
	int totalBufferSize = sizeof(CVertex_fXYZW) * numberOfVerticesLoaded				/* Position */
		                  + sizeof(CVertex_fXYZW) * numberOfVerticesLoaded				/* Colour */
						  + sizeof(CVertex_fXYZW) * numberOfVerticesLoaded				/* Normal */
						  + sizeof(CVertex_fXYZW) * numberOfVerticesLoaded;				/* Texture UVs X 2 */

	// Don't copy the data right now, so pass NULL
	glBufferData(GL_ARRAY_BUFFER, totalBufferSize, NULL, GL_STATIC_DRAW);
	// Now copy the data
	unsigned int offsetToPositions = 0;
	unsigned int offsetToColours = offsetToPositions + sizeof(CVertex_fXYZW) * numberOfVerticesLoaded;
	unsigned int offsetToNormals = offsetToColours + sizeof(CVertex_fXYZW) * numberOfVerticesLoaded;
	// ADDED: October 17, 2014
	unsigned int offsetToTextures = offsetToNormals + sizeof(CVertex_fXYZW) * numberOfVerticesLoaded;

	glBufferSubData( GL_ARRAY_BUFFER, offsetToPositions, sizeof(CVertex_fXYZW) * numberOfVerticesLoaded, p_arrayBunnyVertPositions );
	glBufferSubData( GL_ARRAY_BUFFER, offsetToColours, sizeof(CVertex_fXYZW) * numberOfVerticesLoaded, p_arrayBunnyVertColours );
	glBufferSubData( GL_ARRAY_BUFFER, offsetToNormals, sizeof(CVertex_fXYZW) * numberOfVerticesLoaded, p_arrayBunnyVertNormals );
	// ADDED: October 17, 2014
	glBufferSubData( GL_ARRAY_BUFFER, offsetToTextures, sizeof(CVertex_fXYZW) * numberOfVerticesLoaded, p_arrayBunnyVertTexUV_x2 );

	ExitOnGLError("ERROR: Could not bind the VBO to the VAO");

	glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>( offsetToPositions ) );
	glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>( offsetToColours ) );
	glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>( offsetToNormals ) );
	// ADDED: October 17, 2014
	glVertexAttribPointer( 3, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>( offsetToTextures ) );
	ExitOnGLError("ERROR: Could not bind the VBO to the VAO");


	glEnableVertexAttribArray(0);			// Position
	glEnableVertexAttribArray(1);			// Colour
	glEnableVertexAttribArray(2);			// Normal;			
	glEnableVertexAttribArray(3);			// Texture coords;			// ADDED: October 17, 2014
	ExitOnGLError("ERROR: Could not enable vertex attributes");


	//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VERTICES[0]), (GLvoid*)0);
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VERTICES[0]), (GLvoid*)sizeof(VERTICES[0].Position));
	ExitOnGLError("ERROR: Could not set VAO attributes");




	// ___           _             ___         __   __           _ 
	//|_ _| _ _   __| | ___ __ __ | _ ) _  _  / _| / _| ___  _ _(_)
	// | | | ' \ / _` |/ -_)\ \ / | _ \| || ||  _||  _|/ -_)| '_|_ 
	//|___||_||_|\__,_|\___|/_\_\ |___/ \_,_||_|  |_|  \___||_| (_)
	//                                                             
	// Now do the same for the index buffer
	// Each triangle has three corners, so we need 3 X number of triangles == number of indices
	//numberOfTrianglesLoaded = static_cast<int>( vecTriangles.size() );
	numberOfTrianglesLoaded = pPlyLoader->GetNumberOfElements();
	int numberOfIndices = numberOfTrianglesLoaded * 3;

	// **************************************************************************************
	GLuint* p_bunnyIndices = new GLuint[ numberOfIndices ];
	//GLuint p_bunnyIndices[3851 * 3];

	for ( int triIndex = 0; triIndex < numberOfTrianglesLoaded; triIndex++ )	// Step by three
	{
		//CTriangle tempTri = vecTriangles[triIndex];
		PLYELEMENT tempTri = pPlyLoader->getElement_at(triIndex);
	  
		int index1 = (triIndex * 3);
		int index2 = (triIndex * 3) + 1;
		int index3 = (triIndex * 3) + 2;

		//p_bunnyIndices[index1] = (GLuint)tempTri.V1;
		//p_bunnyIndices[index2] = (GLuint)tempTri.V2;
		//p_bunnyIndices[index3] = (GLuint)tempTri.V3;
		p_bunnyIndices[index1] = (GLuint)tempTri.vertex_index_1;
		p_bunnyIndices[index2] = (GLuint)tempTri.vertex_index_2;
		p_bunnyIndices[index3] = (GLuint)tempTri.vertex_index_3;
		//p_bunnyIndices[(triIndex * 3) + 0] = (GLuint)g_vecTriangles[triIndex].V1;
		//p_bunnyIndices[(triIndex * 3) + 1] = (GLuint)g_vecTriangles[triIndex].V2;
		//p_bunnyIndices[(triIndex * 3) + 2] = (GLuint)g_vecTriangles[triIndex].V3;
	}

	glGenBuffers(1, &indexBufferID );			// Get an ID (aka "name")

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIds[2]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);		
	ExitOnGLError("ERROR: Could not set VAO attributes");

	// Figure out how many "system units" (i.e. "bytes") the index buffer is
	// Or, how many bytes is GLuint (on this system) X number of indices
	int bytesInIndexArray = numberOfIndices * sizeof(GLuint);
	// Copy the data from the CPU RAM to the Video card RAM
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bytesInIndexArray, p_bunnyIndices, GL_STATIC_DRAW);
	ExitOnGLError("ERROR: Could not bind the IBO to the VAO");




	// Release the "connection" to any vertex buffers
	// Release the binding on all buffers
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);	// Texture coords

	glBindVertexArray(0);		
	//glBindBuffer(GL_ARRAY_BUFFER, vertexColourBufferID );
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 

	// Get rid of the local arrays
	//delete [] p_arrayBunnyVertices;
	delete [] p_arrayBunnyVertPositions;
	delete [] p_arrayBunnyVertColours;
	delete [] p_arrayBunnyVertNormals;
	delete [] p_bunnyIndices;


	ExitOnGLError("ERROR: Could not set VAO attributes");


	return true;
}



CModelLoaderManager::CModelLoaderManager()
{
	return;
}

CModelLoaderManager::~CModelLoaderManager()
{
	return;
}




bool CModelLoaderManager::LoadModels( std::vector< CLoadInfo > vecFileNames )
{
	// Go through the vector of file names, loading each one, 
	//	then storing the information into the private vector...
	for ( int index = 0; index != vecFileNames.size(); index++ )
	{

		CPlyInfo tempModelInfo;
		if ( ! this->m_LoadPlyFileIntoGLBuffers( vecFileNames[index],
										tempModelInfo.vertexBufferObjectID, 
										tempModelInfo.vertexAttribBufferID, 
										/*tempModelInfo.vertexNormalBufferID,*/
										tempModelInfo.indexBufferID, 
										tempModelInfo.numberOfVertices, 
										tempModelInfo.numberOfElements, 
										tempModelInfo.extent,
										tempModelInfo.minExtentX,
										tempModelInfo.maxExtentX,
										tempModelInfo.minExtentY,
										tempModelInfo.maxExtentY,
										tempModelInfo.minExtentZ,
										tempModelInfo.maxExtentZ) )
		{	// Something is very wrong... 
			return false;
		}
		// DID work, so:
		// * OpenGL Buffer have been created and are loaded
		// So we will save this information
		tempModelInfo.plyFileASCII = vecFileNames[index].modelFileName;	
		this->m_VecModels.push_back( tempModelInfo );
	}

	return true;
}




bool CModelLoaderManager::GetRenderingInfoByModelFileName( std::string modelName, 
		                                                   CPlyInfo &theInfo )
{
	// Search through the vector until we find the model we're looking for
	for ( int index = 0; index != (int)this->m_VecModels.size(); index++ )
	{	// Is this The One?
		if ( this->m_VecModels[index].plyFileASCII == modelName )
		{	// Yup, it is
			theInfo = this->m_VecModels[index];
			return true;
		}
	}
	// Didn't find it.
	return true;
}


bool CModelLoaderManager::ShutDown(void)
{
  //glDeleteBuffers( 1, &g_IndexBufferID[0] );
  //glDeleteBuffers( 1, &g_VertexColourBufferID[0] );
  //glDeleteVertexArrays( 1, &g_VertexPositionBufferID[0] );

	for ( std::vector< CPlyInfo >::iterator itModel = this->m_VecModels.begin();
		  itModel != this->m_VecModels.end(); ++itModel )
	{
		CPlyInfo tempInfo = *itModel;	// So we can debug. Also, the syntax below is a little easier to understand.

		glDeleteBuffers( 1, &(tempInfo.indexBufferID) );
		glDeleteBuffers( 1, &(tempInfo.vertexAttribBufferID) );
		glDeleteBuffers( 1, &(tempInfo.vertexBufferObjectID) );

		// Set to zero, just in case
		itModel->vertexBufferObjectID = 0;
		itModel->vertexAttribBufferID = 0;
		itModel->indexBufferID =0;
	}

	// Clear the vector as all the buffers are invalid
	this->m_VecModels.clear();

	return true;
}


// NOTE: There are WAY better (i.e. safer!) ways to do this. See MultiByteToWideChar(), etc. in windows
// But this is "good enough" for now
std::wstring CModelLoaderManager::ASCII_to_Unicode( std::string ASCIIstring )
{
	std::wstringstream ssReturnUNICODE;
	for ( std::string::iterator itChar = ASCIIstring.begin(); itChar != ASCIIstring.end(); itChar++ )
	{
		wchar_t theChar = static_cast<wchar_t>( *itChar );
		ssReturnUNICODE << theChar;
	}
	return ssReturnUNICODE.str();
}

std::string CModelLoaderManager::Unicode_to_ASCII( std::wstring UnicodeString )
{
	std::stringstream ssReturnASCII;
	for ( std::wstring::iterator itChar = UnicodeString.begin(); itChar != UnicodeString.end(); itChar++ )
	{
		char theChar = static_cast<char>( *itChar );
		ssReturnASCII << theChar;
	}
	return ssReturnASCII.str();
}


// ***************************
// ** For the Physic demo ****
CPlyFile5nt* CModelLoaderManager::getOriginalModelInfo( std::string modelName )
{
	std::map< std::string, CPlyFile5nt* >::iterator it_pPly = this->m_mapNameToCPlyFile.find(modelName);
	// Find it? 
	if ( it_pPly == this->m_mapNameToCPlyFile.end() )
	{	// Nope
		return 0;
	}
	// Return pointer to object
	return it_pPly->second;
}

void CModelLoaderManager::SetRootDirectory( std::string rootDirectory )
{
	this->m_RootDirectoryName = rootDirectory;
	return;
}

std::string CModelLoaderManager::GetRootDirectory(void)
{
	return this->m_RootDirectoryName;
}

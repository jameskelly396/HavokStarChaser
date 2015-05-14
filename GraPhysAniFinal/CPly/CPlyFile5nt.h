#ifndef _CPlyFile5nt_DX11_HG_
#define _CPlyFile5nt_DX11_HG_

// Written by Michael Feeney, Fanshawe College, 2009-2010
// mfeeney@fanshawec.ca
// It may be distributed under the terms of the General Public License:
// http://www.fsf.org/licenses/gpl.html
// Use this code at your own risk. It is indented only as a learning aid.

// Fairly simple PLY file loader that also calculates the normal for the model.
//  Note that it assumes a certain winding order...
//	Can load the Stanford Bunny and most (?) other files from the 
//	The Stanford 3D Scanning Repository
//	http://graphics.stanford.edu/data/3Dscanrep/
// (Well, I haven't tested them all, but it loads the bunny, dragon, and armadillo...)
//
// Very simply ply reader that also calculates the normal for the model.
//
// See Greg Turk's paper and details of the bunny here:
// http://www.cc.gatech.edu/~turk/bunny/bunny.html

// Written by Michael Feeney, Fanshawe College, 2010
// mfeeney@fanshawec.on.ca
// It may be distributed under the terms of the General Public License:
// http://www.fsf.org/licenses/gpl.html
// Use this code at your own risk. It is indented only as a learning aid.
//
#include <string>
#include <vector>
#include "../CCommonTypes/CVector3f.h"
#include "CPlyInfo.h"


// This structure holds the vertex information 
// as listed in the bunny file
struct PLYVERTEX
{
	PLYVERTEX(): nx(0.0f), ny(0.0f), nz(0.0f), 
				 tex0u(0.0f), tex0v(0.0f), tex1u(0.0f), tex1v(0.0f),
				 red(0), green(0), blue(0) {}
	CVector3f xyz; 
	//float x, y, z;
	float nx, ny, nz;
	float tex0u, tex0v;
	float tex1u, tex1v;
	// *********************
	float red, green, blue;
};

// This structure holds the elements, which 
// refer to the vertex indicies
struct PLYELEMENT
{
	PLYELEMENT(): vertex_index_1(0), vertex_index_2(0), vertex_index_3(0) {}
	int vertex_index_1;
	int vertex_index_2;
	int vertex_index_3;
	CVector3f normal;
};

struct CFileInfo
{
public:
	std::wstring fileName;
	enum enumFileType
	{
		MODEL_FILE_TYPE_PLY,
		MODEL_FILE_TYPE_GDP,
		MODEL_FILE_TYPE_UNKNOWN
	};	
	enumFileType fileType;
	CFileInfo() : fileType(MODEL_FILE_TYPE_UNKNOWN){};
	CFileInfo( std::wstring fileName, enumFileType fileType ) : fileName(fileName), fileType(fileType) {};
};

class CPlyFile5nt
{
public:
	CPlyFile5nt();
	~CPlyFile5nt();
	CPlyFile5nt( const CPlyFile5nt &rhs );	// Copy constructor
	CPlyFile5nt& operator=( const CPlyFile5nt &rhs );

	CFileInfo m_fileInformation;

	// error returns the reason for failure
	bool OpenPLYFile(std::wstring fileName, std::wstring &error);
	// Added November 1, 2014
	bool OpenPLYFile2(std::wstring fileName, std::wstring &error);
	bool SavePlyFileASCII(std::wstring fileName, bool bOverwrite, bool bIncludeNormals, bool bIncludeVertTex0, bool bIncludeVertTex1 );
	bool SaveGDPFile(std::wstring fileName, bool bOverwrite, std::wstring &error);
	bool OpenGDPFile(std::wstring fileName, std::wstring &error);	// GDP model format (basically a binary PLY version)
	// reads only the header information (number of vertices, etc.)
	bool ReadPLYFileHeader(std::wstring fileName, std::wstring &error);
	bool ReadGDPFileHeader(std::wstring fileName, std::wstring &error);
	
	int GetNumberOfVerticies(void);
	int GetNumberOfElements(void);
	PLYVERTEX getVertex_at(std::vector<PLYVERTEX>::size_type index);
	PLYELEMENT getElement_at(std::vector<PLYELEMENT>::size_type index);
	float getMaxX(void); float getMinX(void); float getDeltaX(void);
	float getMaxY(void); float getMinY(void); float getDeltaY(void);
	float getMaxZ(void); float getMinZ(void); float getDeltaZ(void);
	float getMaxExtent(void);
	float getMaxExtent(bool bRecalculateExtents);	
	// *********************
	void normalizeTheModelBaby_Rock_n_Roll(void);
	//void normalizeTheModelBabyDirectXVersion_Rock_n_Roll(void);
	void normalizeTheModelBaby(void);
	void normalizeTheModelBabyDirectXVersion(void);
	void normalizeTheModelTriangle(void);

	// Manipulation operations
	void Scale( float scale );
	void Scale( float scale, CVector3f origin );
	void Transform( CVector3f trans );
	void ScaleToFit( float boundingBoxSize );
	void AlignMinXToPlane( float xMinAxisPlane );
	void AlignMinYToPlane( float yMinAxisPlane );
	void AlignMinZToPlane( float zMinAxisPlane );
	void AlignMaxXToPlane( float xMaxAxisPlane );
	void AlignMaxYToPlane( float yMaxAxisPlane );
	void AlignMaxZToPlane( float zMaxAxisPlane );
	void ShiftToCentreOfVertices(void);


	enum enumTEXCOORDBIAS
	{
		POSITIVE_X, POSITIVE_Y, POSITIVE_Z
	};

	void GenTextureCoordsSpherical( enumTEXCOORDBIAS uBias, enumTEXCOORDBIAS vBias, bool basedOnNormals, float scale, bool fast );
	
	unsigned int GetID(void);
	// Added: March 14, 2012
	bool bHadTextureCoordinatesInFile(void);
	bool bHadNormalsInFile(void);
	// Added: November 2, 2014
	bool bIsThisMachineIsBigEndian(void);
	// Added: June 8, 2013
	void normlizeExistingNomrals(void);	
	// End of Added

	void scaleVertices( float scaleFactor );
	void calcualteExtents(void);

private:
	std::vector<PLYVERTEX> m_verticies;
	std::vector<PLYELEMENT> m_elements;
	float m_minX, m_maxX, m_deltaX;
	float m_minY, m_maxY, m_deltaY;
	float m_minZ, m_maxZ, m_deltaZ;
	// *********************
	float m_centreX, m_centreY, m_centreZ;
	float m_maxExtent;

	// This is used internally to decode the files. 
	// It's a separate class so that we can determine the loading interfaces easily
	class CPlyHeaderDescription
	{
	public:
		CPlyHeaderDescription();

		enum enumPlyHeaderLayout
		{
			HEADER_LAYOUT_IS_ASCII_XYZ = (INT_MAX - 5000),		// So it doesn't start at 0 or 1 (or whatever)
			HEADER_LAYOUT_IS_ASCII_XYZ_nXYZ,
			HEADER_LAYOUT_IS_ASCII_XYZ_nXYZ_vertUV,
			HEADER_LAYOUT_IS_ASCII_XYZ_vertUV,

			HEADER_LAYOUT_IS_BINARY_XYZ,
			HEADER_LAYOUT_IS_BINARY_XYZ_nXYZ,
			HEADER_LAYOUT_IS_BINARY_XYZ_nXYZ_vertUV,

			HEADER_LAYOUT_UNKNOWN
		};
		enumPlyHeaderLayout plyHeaderLayout;

		enum enumFormatASCIIorBinary
		{
			FORMAT_IS_ASCII = (CPlyHeaderDescription::HEADER_LAYOUT_UNKNOWN + 1),	// Start off after the last enum
			FORMAT_IS_BINARY_BIG_ENDIAN,			// Motorola, PowerPC often big endian
			FORMAT_IS_BINARY_LITTLE_ENDIAN,			// everyone else (Intel) is little
			FORMAT_UNKNOWN
		};

		enumFormatASCIIorBinary plyFormatASCIIorBinary; 

		// Based on the information contained inside, determine the header type
		void DeterminePlyFileType(void);
		bool bIsHeader_XYZ(void);
		bool bIsHeader_XYZ_nXYZ(void);
		bool bIsHeader_XYZ_nXYZ_vertUV(void);
		bool bIsHeader_XYZ_vertUV(void);


		inline bool bIsThisMachineIsBigEndian(void);	// Motorola, PowerPC often big endian - everyone else (Intel) is little

		int totalProperties;
		int x_propertyIndex;	int y_propertyIndex;	int z_propertyIndex;
		int normx_propertyIndex;	int normy_propertyIndex;	int normz_propertyIndex;
		int red_propertyIndex;	int green_propertyIndex;	int blue_propertyIndex;

		int tex0u_propertyIndex;		// "u"
		int tex0v_propertyIndex;		// "v"
		int tex1u_propertyIndex;		// "tex1u"
		int tex1v_propertyIndex;		// "tex1v"

		// Added: March 14, 2012
		bool bHadTextureCoordinatesInFile;
		bool bHadNormalsInFile;

		int numberOfElements;
		int numberOfVertices;
	};

	CPlyHeaderDescription m_PlyHeaderInfo;

	//int m_numberOfVertices;		// now in CPlyHeaderDescription
	//int m_numberOfElements;		// now in CPlyHeaderDescription

	// **********************************************************
	// **** Because the ply files can be of various formats, ****
	// ****	 this code is to allow loading 'most' ply files  ****
	// ****  not just the ones that are the 'Stanford bunny' ****
	// ****  format.                                         ****
	// **********************************************************
	//int m_totalProperties;		// now in CPlyHeaderDescription
	//int m_x_propertyIndex;		// now in CPlyHeaderDescription
	//int m_y_propertyIndex;		// now in CPlyHeaderDescription
	//int m_z_propertyIndex;		// now in CPlyHeaderDescription
	//int m_normx_propertyIndex;	// now in CPlyHeaderDescription
	//int m_normy_propertyIndex;	// now in CPlyHeaderDescription
	//int m_normz_propertyIndex;	// now in CPlyHeaderDescription
	//int m_red_propertyIndex;		// now in CPlyHeaderDescription
	//int m_green_propertyIndex;	// now in CPlyHeaderDescription
	//int m_blue_propertyIndex;		// now in CPlyHeaderDescription

	// Added: March 14, 2012
	//bool m_bHadTextureCoordinatesInFile;		// now in CPlyHeaderDescription
	//bool m_bHadNormalsInFile;					// now in CPlyHeaderDescription
	//int m_tex0u_propertyIndex;		// "u"			// now in CPlyHeaderDescription
	//int m_tex0v_propertyIndex;		// "v"			// now in CPlyHeaderDescription
	//int m_tex1u_propertyIndex;		// "tex1u"		// now in CPlyHeaderDescription
	//int m_tex1v_propertyIndex;		// "tex1v"		// now in CPlyHeaderDescription
	int m_UNKNOWN_IGNORE_propertyIndex;		// This is used when we don't know what they heck this index is.
	// End of Added

	// 
	void m_setIndexBasedOnPropertyName(int curIndex, std::wstring propName);
	void m_setIndexBasedOnPropertyNameASCII(int curIndex, std::string propName);	// For OpenPLY2()
	bool m_placeItemInAppropriateVertexLocation( std::wstring tempString, int propertyIndex, PLYVERTEX &tempVertex );

	unsigned int m_ID;
	static unsigned int statNextID;

	// *************************************************************************************************************
	// These are used for the various adapter. 
	class CDataReader
	{
	public:
		// Eventually, these may be inline, but not for now
		inline std::string ASCIIReadNextString( char* pData, unsigned int &curIndex, const unsigned int &arraySize );
		inline float ASCIIReadNextFloat( char* pData, unsigned int &curIndex, const unsigned int &arraySize );
		inline int ASCIIReadNextInt( char* pData, unsigned int &curIndex, const unsigned int &arraySize );

		//inline float BINReadNextFloat( char* pData, unsigned int &curIndex );
		//inline int BINReadNextInt( char* pData, unsigned int &curIndex );
	};


	class IVertexReader
	{
	public:
		virtual ~IVertexReader() {};
		virtual bool IsReaderValid( CPlyHeaderDescription::enumPlyHeaderLayout plyFileType ) = 0;
		virtual bool ProcessNextVertex( PLYVERTEX &vertex, char* pData, unsigned int &curIndex, const unsigned int &arraySize ) = 0;
	};


	// These are used for OpenPLYFile2()
	class CVertexReader_ASCII_fXYZ : public IVertexReader
	{
	public:
		virtual bool IsReaderValid( CPlyHeaderDescription::enumPlyHeaderLayout plyFileType );
		virtual bool ProcessNextVertex( PLYVERTEX &vertex, char* pData, unsigned int &curIndex, const unsigned int &arraySize );
		CPlyFile5nt::CDataReader reader;
	};

	class CVertexReader_ASCII_fXYZ_nXYZ : public IVertexReader
	{
	public:
		virtual bool IsReaderValid( CPlyHeaderDescription::enumPlyHeaderLayout plyFileType );
		virtual bool ProcessNextVertex( PLYVERTEX &vertex, char* pData, unsigned int &curIndex, const unsigned int &arraySize );
		CPlyFile5nt::CDataReader reader;
	};	
	
	// These are used for OpenPLYFile2()
	class CVertexReader_ASCII_fXYZ_nXYZ_vertUV : public IVertexReader
	{
	public:
		virtual bool IsReaderValid( CPlyHeaderDescription::enumPlyHeaderLayout plyFileType );
		virtual bool ProcessNextVertex( PLYVERTEX &vertex, char* pData, unsigned int &curIndex, const unsigned int &arraySize );
		CPlyFile5nt::CDataReader reader;
	};
	// These are used for OpenPLYFile2()
	class CVertexReader_ASCII_fXYZ_vertUV : public IVertexReader
	{
	public:
		virtual bool IsReaderValid( CPlyHeaderDescription::enumPlyHeaderLayout plyFileType );
		virtual bool ProcessNextVertex( PLYVERTEX &vertex, char* pData, unsigned int &curIndex, const unsigned int &arraySize );
		CPlyFile5nt::CDataReader reader;
	};

	class IElementReader
	{
	public:
		virtual ~IElementReader() {};
		virtual bool ProcessNextElement( PLYELEMENT &element, char* pData, unsigned int &curIndex, const unsigned int &arraySize ) = 0;
	};


	// This is the run-of-the-mill "property list uchar int vertex_indices" element reader
	// Specifically for triangles (example: "3 2070 2072 2073")
	class CElementReader_3intVert : public IElementReader
	{
	public:
		virtual bool ProcessNextElement( PLYELEMENT &element, char* pData, unsigned int &curIndex, const unsigned int &arraySize );
		CPlyFile5nt::CDataReader reader;
	};
};


#endif

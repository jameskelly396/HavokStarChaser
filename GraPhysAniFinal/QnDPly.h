#ifndef _QnDPly
#define _QnDPly

// Super "quick-n-dirty" ply loader. 
// Will load the following properties: 
//	x,y,z,confidence,intensity (i.e. the original Stanford bunny format)
//	x,y,z
//	x,y,z,nx,ny,nz

#include "CTriangle.h"		// Also includes CVertex
#include <string>

class QnDPly
{
public:
	QnDPly();
	~QnDPly();

	enum enumPlyType
	{
		XYZ,
		XYZ_CONF_INTENSITY,
		XYZ_NXNYNZ,
		UNKNOWN
	};

	enumPlyType getLoadedFileType(void);
	bool LoadPlyFile( std::wstring fileName, std::wstring &error );
};

#endif

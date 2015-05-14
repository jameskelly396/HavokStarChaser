#include "CPlyFile5nt.h"

#include <cstdlib>	// for atof, atoi, atol, e
#include <cctype>	// for isspace()
// For the "C" locale, white-space characters are any of:
//	' '	(0x20)	space (SPC)
//	'\t'	(0x09)	horizontal tab (TAB)
//	'\n'	(0x0a)	newline (LF)
//	'\v'	(0x0b)	vertical tab (VT)
//	'\f'	(0x0c)	feed (FF)
//	'\r'	(0x0d)	carriage return (CR)tc.

#include <fstream>



// Reads utill the next 
std::string CPlyFile5nt::CDataReader::ASCIIReadNextString( char* pData, unsigned int &curIndex, const unsigned int &arraySize )
{
	std::string returnString; char curChar = 0;
	// Gone to far in main array?
	while ( curIndex < arraySize )
	{
		curChar = static_cast<char>( pData[curIndex] );
		if ( isspace(curChar) )
		{
			curIndex++;			
			// There could be additional whitespace (like CR + LF or something)
			// Keep reading white space until we come to a non-whitespace character
			while ( (curIndex < arraySize) && isspace(pData[curIndex]) )
			{
				// "consume" this whitespace character
				curIndex++;
			}
			// Either we're off the end of the array (perhaps that's bad)...
			// ...or we've hit the first non-white space
			return returnString;
		}
		returnString += curChar;
		curIndex++;
	};
	return returnString;
}

int CPlyFile5nt::CDataReader::ASCIIReadNextInt( char* pData, unsigned int &curIndex, const unsigned int &arraySize )
{
	std::string tempString = this->ASCIIReadNextString( pData, curIndex, arraySize );
	int returnInt = atoi( tempString.c_str() );
	return returnInt;
}

float CPlyFile5nt::CDataReader::ASCIIReadNextFloat( char* pData, unsigned int &curIndex, const unsigned int &arraySize )
{
	std::string tempString = this->ASCIIReadNextString( pData, curIndex, arraySize );
	float returnFloat = static_cast<float>( atof( tempString.c_str() ) );
	return returnFloat;
}




bool CPlyFile5nt::OpenPLYFile2(std::wstring fileName, std::wstring &error)
{
	std::ifstream thePlyFile(fileName.c_str(), std::ios::binary );
	if (!thePlyFile.is_open())
	{
		error = L"Can't open the file. Sorry it didn't work out.";
		return false;
	}

	thePlyFile.seekg(0, std::ios::end);
	// reutrns "std::ios::pos_type" type (which seems to be int in VC++)
	unsigned int fileSize = static_cast<unsigned int>( thePlyFile.tellg() );	
	// Return to start
	thePlyFile.seekg(0, std::ios::beg);
		
	//std::vector<char> data( static_cast<unsigned int>(fileSize), 0);
	//this->m_vecRawData.resize( static_cast<unsigned int>(fileSize), 0);

	char* pRawData = new char[ fileSize ];
	unsigned int curIndex = 0;				// Location in the array
	
	// read the file
	//std::vector<char> data;
	//thePlyFile.read( &data[0], fileSize);
	//thePlyFile.read( &(this->m_vecRawData[0]), fileSize);
	thePlyFile.read( pRawData, fileSize );
	thePlyFile.close();

	// *****************************************************************************
	// Process the header information
	CPlyFile5nt::CDataReader reader;

	std::string tempString;
	this->m_PlyHeaderInfo.numberOfVertices = 0;
	this->m_PlyHeaderInfo.numberOfElements = 0;


	try
	{
		// ply
		tempString = reader.ASCIIReadNextString( pRawData, curIndex, fileSize );	
		if ( tempString != "ply")		{ error = L"No a ply file."; throw; }
		
		// format ascii 1.0
		tempString = reader.ASCIIReadNextString( pRawData, curIndex, fileSize );	if ( tempString != "format")	{ throw; }
		
		tempString = reader.ASCIIReadNextString( pRawData, curIndex, fileSize );

		// Added: November 2, 2014
		if ( tempString == "ascii" )
		{
			this->m_PlyHeaderInfo.plyFormatASCIIorBinary = CPlyHeaderDescription::FORMAT_IS_ASCII;
		}
		else if ( tempString == "binary_little_endian" )
		{
			this->m_PlyHeaderInfo.plyFormatASCIIorBinary = CPlyHeaderDescription::FORMAT_IS_BINARY_LITTLE_ENDIAN;
		}
		else if ( tempString == "binary_big_endian" )
		{
			this->m_PlyHeaderInfo.plyFormatASCIIorBinary = CPlyHeaderDescription::FORMAT_IS_BINARY_BIG_ENDIAN;
		}
		else 
		{
			error = L"Format must be ascii, binary_little_endian, or binary_big_endian."; 
			throw; 
		}
		// End of: Added: November 2, 2014		
		
		tempString = reader.ASCIIReadNextString( pRawData, curIndex, fileSize );	if ( tempString != "1.0") { throw; }

		// Find the number of verticies...
		// Item after "vertex" is number of verticies
		do
		{
			tempString = reader.ASCIIReadNextString( pRawData, curIndex, fileSize );
		} while ( tempString != "vertex" );


		this->m_PlyHeaderInfo.numberOfVertices = reader.ASCIIReadNextInt( pRawData, curIndex, fileSize );

		// Read the properties and note the index locations of them...
		int currentIndex = 0;
		while ( true )
		{
			tempString = reader.ASCIIReadNextString( pRawData, curIndex, fileSize );
			if ( tempString == "element" ) { break; /* on to next part of file */ }
			// property float x
			if ( tempString == "property") 
			{
				tempString = reader.ASCIIReadNextString( pRawData, curIndex, fileSize );
				if ( ( tempString == "float") | ( tempString == "uchar") )	
				{	// Figure out which index to set
					tempString = reader.ASCIIReadNextString( pRawData, curIndex, fileSize );
					this->m_setIndexBasedOnPropertyNameASCII( currentIndex, tempString );
				}
				else 
				{ 
					error = L"expected property type missing"; throw; 
				} // if ( tempString == L"float")	
			}//if ( tempString == "property") 
			else
			{ 
				error = L"expected property"; throw; 
			} // if ( tempString == L"property") 
			currentIndex++;
		}

		this->m_PlyHeaderInfo.totalProperties = currentIndex;
		 
		tempString = reader.ASCIIReadNextString( pRawData, curIndex, fileSize );
		if ( tempString != "face")	{ error = L"expected element face."; throw; }

		this->m_PlyHeaderInfo.numberOfElements = reader.ASCIIReadNextInt( pRawData, curIndex, fileSize );

		do
		{
			tempString = reader.ASCIIReadNextString( pRawData, curIndex, fileSize );
		} while ( tempString != "end_header" );




		// The header is read, so determine what "type" of header it is. 
		// Note: this isn't really used for this method (but IS used for OpenPLYFile2), but hey, 
		//	why not live a little, right? 
		this->m_PlyHeaderInfo.DeterminePlyFileType();
	}
	catch (...)
	{
		// Something is wrong
		delete [] pRawData;
		return false;
	}

	// *****************************************************************************

	// Determine the type of file... so we can find the right Vertex adapter
	this->m_PlyHeaderInfo.DeterminePlyFileType();

	// Find an appropriate vertex reader
	IVertexReader* pVertReader;
	// 
	switch ( this->m_PlyHeaderInfo.plyHeaderLayout )
	{
	case CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ:
		pVertReader = new CVertexReader_ASCII_fXYZ();
		break;
	case CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ_nXYZ:
		pVertReader = new CVertexReader_ASCII_fXYZ_nXYZ();
		break;
	case CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ_nXYZ_vertUV:
		pVertReader = new CVertexReader_ASCII_fXYZ_nXYZ_vertUV();
		break;
	case CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ_vertUV:
		pVertReader = new CVertexReader_ASCII_fXYZ_vertUV();
		break;
	};

	// Double-check if we got a valid reader
	if ( ( pVertReader == 0 ) && !pVertReader->IsReaderValid( this->m_PlyHeaderInfo.plyHeaderLayout ) )
	{
		// No good
		delete [] pRawData;
		return false;
	}

	// We have a valid vertex reader, so read the vertices
	this->m_verticies.reserve( this->m_PlyHeaderInfo.numberOfVertices );
	for ( unsigned int vertCount = 0; vertCount != this->m_PlyHeaderInfo.numberOfVertices; vertCount++ )
	{
		PLYVERTEX tempVertex;
		pVertReader->ProcessNextVertex( tempVertex, pRawData, curIndex, fileSize );
		this->m_verticies.push_back( tempVertex );
	}

	this->calcualteExtents();

	// Now read the elements...
	IElementReader* pElementReader = new CPlyFile5nt::CElementReader_3intVert();

	this->m_elements.reserve( this->m_PlyHeaderInfo.numberOfElements );
	for ( unsigned int elementCount = 0; elementCount != this->m_PlyHeaderInfo.numberOfElements; elementCount++ )
	{	
		PLYELEMENT tempElement;
		pElementReader->ProcessNextElement( tempElement, pRawData, curIndex, fileSize );
		this->m_elements.push_back( tempElement );
	}
		
	this->m_fileInformation.fileName = fileName;
	this->m_fileInformation.fileType = CFileInfo::MODEL_FILE_TYPE_PLY;

	delete [] pRawData;

	return true;
}

CPlyFile5nt::CPlyHeaderDescription::CPlyHeaderDescription()
{
	this->bHadNormalsInFile = false;
	this->bHadTextureCoordinatesInFile = false;
	this->totalProperties = INT_MAX; 	
	this->x_propertyIndex = INT_MAX; 	this->y_propertyIndex = INT_MAX; 	this->z_propertyIndex = INT_MAX;	
	this->normx_propertyIndex = INT_MAX; this->normy_propertyIndex = INT_MAX; this->normz_propertyIndex = INT_MAX; 
	this->red_propertyIndex = INT_MAX; this->green_propertyIndex = INT_MAX; this->blue_propertyIndex = INT_MAX; 
	this->tex0u_propertyIndex = INT_MAX; this->tex0v_propertyIndex = INT_MAX; 
	this->tex1u_propertyIndex = INT_MAX; this->tex1v_propertyIndex = INT_MAX; 
	this->plyHeaderLayout = CPlyHeaderDescription::HEADER_LAYOUT_UNKNOWN;
	this->numberOfElements = 0;
	this->numberOfVertices = 0;
	this->plyFormatASCIIorBinary = CPlyHeaderDescription::FORMAT_UNKNOWN;

	return;
}

bool CPlyFile5nt::CPlyHeaderDescription::bIsThisMachineIsBigEndian(void)
{
	// from: http://www.ibm.com/developerworks/aix/library/au-endianc/
	// [Code] tests the first byte of the multi-byte integer i to determine if it is 0 or 1. 
	// If it is 1, the running platform is assumed to be little-endian. 
	// If it is 0, it is assumed to be big-endian.
	const int i = 1;
	//#define is_bigendian() ( (*(char*)&i) == 0 )
	return ( (*(char*)&i) == 0 );
}

bool CPlyFile5nt::bIsThisMachineIsBigEndian(void)
{
	return this->m_PlyHeaderInfo.bIsThisMachineIsBigEndian();
}

bool CPlyFile5nt::CPlyHeaderDescription::bIsHeader_XYZ(void)
{
	// ASCII_XYZ: 
	// property float x		<-- at location 0
	// property float y		<-- at location 1
	// property float z		<-- at location 2
	// All other properties aren't defined (INT_MAX)
	if ( this->x_propertyIndex != 0 ) { return false; }
	if ( this->y_propertyIndex != 1 ) { return false; }
	if ( this->z_propertyIndex != 2 ) { return false; }
	// 
	// All the rest shouldn't be defined
	if ( this->normx_propertyIndex != INT_MAX ) { return false; }
	if ( this->normy_propertyIndex != INT_MAX ) { return false; }
	if ( this->normz_propertyIndex != INT_MAX ) { return false; }
	if ( this->red_propertyIndex != INT_MAX ) { return false; }
	if ( this->green_propertyIndex != INT_MAX ) { return false; }
	if ( this->blue_propertyIndex != INT_MAX ) { return false; }
	if ( this->tex0u_propertyIndex != INT_MAX ) { return false; }
	if ( this->tex0v_propertyIndex != INT_MAX ) { return false; }
	if ( this->tex1u_propertyIndex != INT_MAX ) { return false; }
	if ( this->tex1v_propertyIndex != INT_MAX ) { return false; }

	// So we're good to go; this has to be the interface
	return true;
}

bool CPlyFile5nt::CPlyHeaderDescription::bIsHeader_XYZ_nXYZ(void)
{
	// ASCII_XYZ_nXYZ: 
	// property float x		<-- at location 0
	// property float y		<-- at location 1
	// property float z		<-- at location 2
	// property float nx	<-- at location 3
	// property float ny	<-- at location 4
	// property float nz	<-- at location 5
	// All other properties aren't defined (INT_MAX)
	if ( this->x_propertyIndex != 0 ) { return false; }
	if ( this->y_propertyIndex != 1 ) { return false; }
	if ( this->z_propertyIndex != 2 ) { return false; }
	if ( this->normx_propertyIndex != 3 ) { return false; }
	if ( this->normy_propertyIndex != 4 ) { return false; }
	if ( this->normz_propertyIndex != 5 ) { return false; }
	// 
	// All the rest shouldn't be defined
	if ( this->red_propertyIndex != INT_MAX ) { return false; }
	if ( this->green_propertyIndex != INT_MAX ) { return false; }
	if ( this->blue_propertyIndex != INT_MAX ) { return false; }
	if ( this->tex0u_propertyIndex != INT_MAX ) { return false; }
	if ( this->tex0v_propertyIndex != INT_MAX ) { return false; }
	if ( this->tex1u_propertyIndex != INT_MAX ) { return false; }
	if ( this->tex1v_propertyIndex != INT_MAX ) { return false; }

	// So we're good to go; this has to be the interface
	return true;
}

bool CPlyFile5nt::CPlyHeaderDescription::bIsHeader_XYZ_nXYZ_vertUV(void)
{
	// ASCII_XYZ_nXYZ: 
	// property float x		<-- at location 0
	// property float y		<-- at location 1
	// property float z		<-- at location 2
	// property float nx	<-- at location 3
	// property float ny	<-- at location 4
	// property float nz	<-- at location 5
	// property float texture_u		<-- at location 6
	// property float texture_v		<-- at location 7

	// All other properties aren't defined (INT_MAX)
	if ( this->x_propertyIndex != 0 ) { return false; }
	if ( this->y_propertyIndex != 1 ) { return false; }
	if ( this->z_propertyIndex != 2 ) { return false; }
	if ( this->normx_propertyIndex != 3 ) { return false; }
	if ( this->normy_propertyIndex != 4 ) { return false; }
	if ( this->normz_propertyIndex != 5 ) { return false; }
	if ( this->tex0u_propertyIndex != 6 ) { return false; }
	if ( this->tex0v_propertyIndex != 7 ) { return false; }
	// 
	// All the rest shouldn't be defined
	if ( this->red_propertyIndex != INT_MAX ) { return false; }
	if ( this->green_propertyIndex != INT_MAX ) { return false; }
	if ( this->blue_propertyIndex != INT_MAX ) { return false; }
	if ( this->tex1u_propertyIndex != INT_MAX ) { return false; }
	if ( this->tex1v_propertyIndex != INT_MAX ) { return false; }

	// So we're good to go; this has to be the interface
	return true;
}

bool CPlyFile5nt::CPlyHeaderDescription::bIsHeader_XYZ_vertUV(void)
{
	// ASCII_XYZ_nXYZ: 
	// property float x		<-- at location 0
	// property float y		<-- at location 1
	// property float z		<-- at location 2
	// property float texture_u		<-- at location 3
	// property float texture_v		<-- at location 4
	// All other properties aren't defined (INT_MAX)
	if ( this->x_propertyIndex != 0 ) { return false; }
	if ( this->y_propertyIndex != 1 ) { return false; }
	if ( this->z_propertyIndex != 2 ) { return false; }
	if ( this->tex0u_propertyIndex != 3 ) { return false; }
	if ( this->tex0v_propertyIndex != 4 ) { return false; }
	// 
	// All the rest shouldn't be defined
	if ( this->normx_propertyIndex != INT_MAX ) { return false; }
	if ( this->normy_propertyIndex != INT_MAX ) { return false; }
	if ( this->normz_propertyIndex != INT_MAX ) { return false; }
	if ( this->red_propertyIndex != INT_MAX ) { return false; }
	if ( this->green_propertyIndex != INT_MAX ) { return false; }
	if ( this->blue_propertyIndex != INT_MAX ) { return false; }
	if ( this->tex1u_propertyIndex != INT_MAX ) { return false; }
	if ( this->tex1v_propertyIndex != INT_MAX ) { return false; }

	// So we're good to go; this has to be the interface
	return true;
}


// Based on the information contained inside, determine the header type
void CPlyFile5nt::CPlyHeaderDescription::DeterminePlyFileType(void)
{
	// Assume it's unknown
	this->plyHeaderLayout = CPlyHeaderDescription::HEADER_LAYOUT_UNKNOWN;

	// ASCII_XYZ: 
	if ( this->bIsHeader_XYZ() && ( this->plyFormatASCIIorBinary == CPlyHeaderDescription::FORMAT_IS_ASCII ) )
	{
		this->plyHeaderLayout = CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ;
	}
	else if ( this->bIsHeader_XYZ_nXYZ() && ( this->plyFormatASCIIorBinary == CPlyHeaderDescription::FORMAT_IS_ASCII ) )
	{
		this->plyHeaderLayout = CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ_nXYZ;
	}
	else if ( this->bIsHeader_XYZ_nXYZ_vertUV() && ( this->plyFormatASCIIorBinary == CPlyHeaderDescription::FORMAT_IS_ASCII ) )
	{
		this->plyHeaderLayout = CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ_nXYZ_vertUV;
	}
	else if ( this->bIsHeader_XYZ_vertUV() && ( this->plyFormatASCIIorBinary == CPlyHeaderDescription::FORMAT_IS_ASCII ) )
	{
		this->plyHeaderLayout = CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ_vertUV;
	}
	// else, we dunno what the heck the layout is (well, we can't process it, anyway)
	
	return;
}
		






 //__      __          _                             _                _                   
 //\ \    / /         | |                  /\       | |              | |                  
 // \ \  / /___  _ __ | |_  ___ __  __    /  \    __| |  __ _  _ __  | |_  ___  _ __  ___ 
 //  \ \/ // _ \| '__|| __|/ _ \\ \/ /   / /\ \  / _` | / _` || '_ \ | __|/ _ \| '__|/ __|
 //   \  /|  __/| |   | |_|  __/ >  <   / ____ \| (_| || (_| || |_) || |_|  __/| |   \__ \
 //    \/  \___||_|    \__|\___|/_/\_\ /_/    \_\\__,_| \__,_|| .__/  \__|\___||_|   |___/
 //                                                           | |                         
 //                                                           |_|                         



 //   _    ___   ___  ___  ___      __ __  ____   __ ____
 //  /_\  / __| / __||_ _||_ _|    / _|\ \/ /\ \ / /|_  /
 // / _ \ \__ \| (__  | |  | |    |  _| >  <  \ V /  / / 
 ///_/ \_\|___/ \___||___||___|___|_|  /_/\_\  |_|  /___|
 //                           |___|                      
bool CPlyFile5nt::CVertexReader_ASCII_fXYZ::IsReaderValid( CPlyHeaderDescription::enumPlyHeaderLayout plyFileType )
{
	if ( plyFileType == CPlyFile5nt::CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ )
	{
		return true;
	}	
	return false;
}

bool CPlyFile5nt::CVertexReader_ASCII_fXYZ::ProcessNextVertex( PLYVERTEX &vertex, char* pData, 
															   unsigned int &curIndex, const unsigned int &arraySize )
{
	//property float x
	//property float y
	//property float z
	vertex.xyz.x = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.xyz.y = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.xyz.z = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	return true;
}

 //  __ __  ____   __ ____         __  ____   __ ____                  _    _   _ __   __
 // / _|\ \/ /\ \ / /|_  /    _ _  \ \/ /\ \ / /|_  /  __ __ ___  _ _ | |_ | | | |\ \ / /
 //|  _| >  <  \ V /  / /    | ' \  >  <  \ V /  / /   \ V // -_)| '_||  _|| |_| | \ V / 
 //|_|  /_/\_\  |_|  /___|___|_||_|/_/\_\  |_|  /___|___\_/ \___||_|   \__| \___/   \_/  
 //                      |___|                      |___|                                
bool CPlyFile5nt::CVertexReader_ASCII_fXYZ_nXYZ_vertUV::IsReaderValid( CPlyHeaderDescription::enumPlyHeaderLayout plyFileType )
{
	if ( plyFileType == CPlyFile5nt::CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ_nXYZ_vertUV )
	{
		return true;
	}
	return false;
}

bool CPlyFile5nt::CVertexReader_ASCII_fXYZ_nXYZ_vertUV::ProcessNextVertex( PLYVERTEX &vertex, char* pData, 
															   unsigned int &curIndex, const unsigned int &arraySize )
{
	//property float x
	//property float y
	//property float z
	//property float nx
	//property float ny
	//property float nz
	//property float texture_u
	//property float texture_v
	vertex.xyz.x = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.xyz.y = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.xyz.z = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.nx = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.ny = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.nz = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.tex0u = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.tex0v = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	return true;
}

 //  __ __  ____   __ ____                  _    _   _ __   __
 // / _|\ \/ /\ \ / /|_  /  __ __ ___  _ _ | |_ | | | |\ \ / /
 //|  _| >  <  \ V /  / /   \ V // -_)| '_||  _|| |_| | \ V / 
 //|_|  /_/\_\  |_|  /___|___\_/ \___||_|   \__| \___/   \_/  
 //                      |___|                                
bool CPlyFile5nt::CVertexReader_ASCII_fXYZ_vertUV::IsReaderValid( CPlyHeaderDescription::enumPlyHeaderLayout plyFileType )
{
	if ( plyFileType == CPlyFile5nt::CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ_vertUV )
	{
		return true;
	}
	return false;
}

bool CPlyFile5nt::CVertexReader_ASCII_fXYZ_vertUV::ProcessNextVertex( PLYVERTEX &vertex, char* pData, 
															   unsigned int &curIndex, const unsigned int &arraySize )
{
	//property float x
	//property float y
	//property float z
	//property float texture_u
	//property float texture_v
	vertex.xyz.x = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.xyz.y = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.xyz.z = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.tex0u = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.tex0v = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	return true;
}

 //  __ __  ____   __ ____         __  ____   __ ____
 // / _|\ \/ /\ \ / /|_  /    _ _  \ \/ /\ \ / /|_  /
 //|  _| >  <  \ V /  / /    | ' \  >  <  \ V /  / / 
 //|_|  /_/\_\  |_|  /___|___|_||_|/_/\_\  |_|  /___|
 //                      |___|                       
bool CPlyFile5nt::CVertexReader_ASCII_fXYZ_nXYZ::IsReaderValid( CPlyHeaderDescription::enumPlyHeaderLayout plyFileType )
{
	if ( plyFileType == CPlyFile5nt::CPlyHeaderDescription::HEADER_LAYOUT_IS_ASCII_XYZ_nXYZ )
	{
		return true;
	}
	return false;
}

bool CPlyFile5nt::CVertexReader_ASCII_fXYZ_nXYZ::ProcessNextVertex( PLYVERTEX &vertex, char* pData, 
															   unsigned int &curIndex, const unsigned int &arraySize )
{
	//property float x
	//property float y
	//property float z
	//property float nx
	//property float ny
	//property float nz
	vertex.xyz.x = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.xyz.y = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.xyz.z = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.nx = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.ny = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	vertex.nz = this->reader.ASCIIReadNextFloat( pData, curIndex, arraySize );
	return true;
}





 //  ___  ___  _                        _    ___                _             ____ _       _  __   __          _   
 // / __|| __|| | ___  _ __   ___  _ _ | |_ | _ \ ___  __ _  __| | ___  _ _  |__ /(_) _ _ | |_\ \ / /___  _ _ | |_ 
 //| (__ | _| | |/ -_)| '  \ / -_)| ' \|  _||   // -_)/ _` |/ _` |/ -_)| '_|  |_ \| || ' \|  _|\ V // -_)| '_||  _|
 // \___||___||_|\___||_|_|_|\___||_||_|\__||_|_\\___|\__,_|\__,_|\___||_|___|___/|_||_||_|\__| \_/ \___||_|   \__|
 //                                                                      |___|                                     
bool CPlyFile5nt::CElementReader_3intVert::ProcessNextElement( PLYELEMENT &element, char* pData, 
																 unsigned int &curIndex, const unsigned int &arraySize )
{
	// Discard first value (will be "3" for triangles
	this->reader.ASCIIReadNextInt( pData, curIndex, arraySize );
	element.vertex_index_1 = this->reader.ASCIIReadNextInt( pData, curIndex, arraySize );
	element.vertex_index_2 = this->reader.ASCIIReadNextInt( pData, curIndex, arraySize );
	element.vertex_index_3 = this->reader.ASCIIReadNextInt( pData, curIndex, arraySize );
	return true;
}

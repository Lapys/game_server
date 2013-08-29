/////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:		Pak.cpp
//
// Author:		Raymond Wilson - ray.wilson@blueyonder.co.uk
//
// Date:		26th May 2003
//
// Notice:		Copyright (C) 2003, Raymond Wilson
//
/////////////////////////////////////////////////////////////////////////////////////////////

#include "pak.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>
#include <string>
/////////////////////////////////////////////////////////////////////////////////////////////
using namespace std;

const char* getRelativePath(const char* root,const char* path)
{
    static string rooStr;
    static string pathStr;
    rooStr = root;
    pathStr = path;
    return pathStr.substr(rooStr.size()).c_str();
}

void CPakFile::SearchDirectory(const char* name)
{
    //printf("searching:%s\n",name);
    std::string tmpName = name;
    if (tmpName[tmpName.size() - 1] != '/') {
        tmpName += "/";
    }
    
    DIR *dir = opendir(tmpName.c_str());                //Assuming absolute pathname here.
    if(dir)
    {
        char Path[256], *EndPtr = Path;
        struct dirent *e;
        strcpy(Path, tmpName.c_str());                  //Copies the current path to the 'Path' variable.
        EndPtr += tmpName.size();             //Moves the EndPtr to the ending position.
        while((e = readdir(dir)) != NULL)
        {
            if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0)
            {
                continue;
            }
            //Iterates through the entire directory.
            struct stat info;                //Helps us know about stuff
           
            if (strlen(e->d_name) >= FILE_NAME_LEN) {
                printf("ERROR:file name longer than %d,%s\n",FILE_NAME_LEN,e->d_name);
                _hasErr = true;
                return;
            }
             strcpy(EndPtr, e->d_name);       //Copies the current filename to the end of the path, overwriting it with each loop.
            if(!stat(Path, &info))
            {         //stat returns zero on success.
                if(S_ISDIR(info.st_mode))
                {  //Are we dealing with a directory?
                    //Make corresponding directory in the target folder here.
                    std::string tmp = Path ;
                    if (tmp[tmp.size() - 1] != '/') {
                        tmp += "/";
                    }
                    
                    sFileTableEntry* tmpFT = new sFileTableEntry();
                    tmpFT->dwFileSize = 0;
                    tmpFT->dwOffset = 0;
                    const char* p = getRelativePath(m_szFolderPath, tmp.c_str());
                    if (strlen(p) >= FILE_NAME_LEN) {
                        printf("ERROR:file name longer than %d,%s\n",FILE_NAME_LEN,p);
                        _hasErr = true;
                        return;
                    }
                    strcpy(tmpFT->szFileName,p);
                    ALL_FILES.push_back(*tmpFT);
                    
                    //printf("%s -> :%s\n",tmp.c_str(),getRelativePath("/Users/lingyun/projects/osx/Pak/test/", tmp.c_str()));
                    SearchDirectory(tmp.c_str());   
                } else if(S_ISREG(info.st_mode))
                { 
                    sFileTableEntry* tmp = new sFileTableEntry();
                    tmp->dwFileSize = (u32)info.st_size;
                    tmp->dwOffset = 0;
                    const char* p = getRelativePath(m_szFolderPath, Path);
                    if (strlen(p) >= FILE_NAME_LEN) {
                        printf("ERROR:file name longer than %d,%s\n",FILE_NAME_LEN,p);
                        _hasErr = true;
                        return;
                    }
                    strcpy(tmp->szFileName,p);
                    ALL_FILES.push_back(*tmp);
                    std::string kk = tmp->szFileName;
                }
                else
                {
                    printf("%s fail2",Path);
                }
            }
            else
            {
                printf("%s fail\n",Path);
            }
        }
        ///必须要关闭!!否则errno=24:)
        closedir(dir);
    }
}


CPakFile::CPakFile()
{
	// Default the header
	memset( &m_Header, 0, sizeof(sPakHeader) );

	// Default other variables
	memset( m_szFolderPath, 0, FILE_PATH_LEN );
	memset( m_szPakName, 0, FILE_PATH_LEN );

	// Add a blank dummy node to the file table
	sFileTableEntry* Dummy = new sFileTableEntry();
	memset( Dummy, 0, sizeof(sFileTableEntry) );
	m_FileTable = Dummy;
    _hasErr = false;
}

CPakFile::~CPakFile()
{
	// Default the header
	memset( &m_Header, 0, sizeof(sPakHeader) );

	// Default other variables
	memset( m_szFolderPath, 0, FILE_PATH_LEN );
	memset( m_szPakName, 0, FILE_PATH_LEN );

	// Clear the file table

	m_FileTable = NULL;
}

bool CPakFile::GenerateHFT()
{
	// Declare local variables
	int					iRandom = 0;			// Recieves a random number
	u32				dwFileCount = 0;		// Number of files in compile directory
	
	char				Buffer[FILE_PATH_LEN] = {0};		// All purpose buffer
    
	// Seed the timer with the clock
	srand( (unsigned)time(NULL) );

	// Create the header signature
	memcpy( m_Header.szSignature, "pak", 3 );

	// Set the file version
	m_Header.fVersion = 1.0;	

	// Get a random 1 or 0 (TRUE or FALSE) and set cypher direction
	iRandom = rand()%2;
	m_Header.bCypherAddition = (u8)iRandom;

	// Get the caesar cypher value
	iRandom = rand()%256;
	m_Header.iCypherValue = (u8)iRandom;

	// Create a unique ID
	for( int i = 0; i < 10; i++ )
	{
		iRandom = rand()%256;
		m_Header.szUniqueID[i] = (char)iRandom;
	}

	// Get a local copy of the compilation folder name so that the one
	// in the class isn't altered
	if( strlen(m_szFolderPath) == 0 ){ return false; }
	memcpy( Buffer, m_szFolderPath, FILE_PATH_LEN );
    
    for (int i = 0; i < ALL_FILES.size(); i++) {
        sFileTableEntry& tmp = ALL_FILES.at(i);
       // tmp.Next = m_FileTable;
        m_FileTable = &tmp;
    }
    
//    closedir(dirp);
    
	// Mark the number of files added in the header
	m_Header.dwNumFTEntries = (u32)ALL_FILES.size();

    return true;
}

bool CPakFile::WorkOutOffsets()
{
	// Declare local variables
	u32	dwFileHFTData = 0;			// Size of header and file table
	u32	dwOffset = 0;				// Individual files offset in to the PAK

	// Work out the size, in bytes, of the header and FT
	dwFileHFTData = sizeof(sPakHeader) + (m_Header.dwNumFTEntries * sizeof(sFileTableEntry));

	// Create a temporary node and make it the head of the linked list
	sFileTableEntry* Current;
	Current = m_FileTable;

	// Get the first offset
	dwOffset = dwFileHFTData + 1;

    for (int i = 0; i < ALL_FILES.size(); i++) {
        ALL_FILES.at(i).dwOffset = dwOffset;
        dwOffset += ALL_FILES.at(i).dwFileSize;
    }

	return true;
}

#include <errno.h> 

bool CPakFile::CreatePak( const char* Path, const char* Output )
{
	// Declare local variables
	FILE*	PAKStream;					// File pointer for writing to the PAK
	FILE*	InputStream;				// For reading in each file to be added
	fpos_t	Pos = 0;					// Position in the PAK file
	char	Buffer[FILE_PATH_LEN] = {0};			// General purpose buffer
	int		iStringLength = 0;			// String length

	// Error check
	if( (Path == NULL) || (Output == NULL) ){ return false; }

	// Store the function paramaters in the class
	iStringLength = strlen( Path );
    if (iStringLength >= FILE_PATH_LEN ) {
        printf("file name too long:%s,len=%d",Path,iStringLength);
        return false;
    }
	memcpy( m_szFolderPath, Path, iStringLength );
	iStringLength = strlen( Output );
    if (iStringLength >= FILE_PATH_LEN ) {
        printf("file name too long:%s,len=%d",Path,iStringLength);
        return false;
    }
	memcpy( m_szPakName, Output, iStringLength );
    
    SearchDirectory(Path);
    if (_hasErr) {
        return false;
    }
    printf("pak %d files...\n",ALL_FILES.size());
	// Create the header and file table
	if( !GenerateHFT() ){ return false; }

	// Work out the offsets
	if( !WorkOutOffsets() ){ return false; }

	// Open the file stream for writing the PAK
	PAKStream = fopen( Output, "wb" );
	if( !PAKStream ){
        printf("can't write:%s,errno=%d\n",Output,errno);
        return false;
    }

	// Write the PAK header
    int len = sizeof(sPakHeader);
	fwrite( &m_Header, len, 1, PAKStream );

    u8* Ptr = new u8[sizeof(sFileTableEntry)];
    for (int i = 0; i < ALL_FILES.size(); i++) {
        memcpy(Ptr, &ALL_FILES.at(i),sizeof(sFileTableEntry));
        for (int j = 0; j < sizeof(sFileTableEntry); j++) {
            u8 temp = Ptr[j];
            fwrite(&temp, sizeof(u8), 1, PAKStream);
        }
    }
    delete [] Ptr;
     fclose( PAKStream );

    u8 buffer[4*1024];
    for (int i = 0; i < ALL_FILES.size(); i++) {
        
        PAKStream = fopen( Output, "r+b" );
		if( !PAKStream )
        {
            printf("can't write with r+b, disk full? : %s",Output);
            return false;
        }
        Pos = ALL_FILES.at(i).dwOffset;
        fsetpos(PAKStream, &Pos);
        
        memcpy( Buffer, m_szFolderPath, FILE_PATH_LEN );
		strcat( Buffer, "/");
		strcat( Buffer, ALL_FILES.at(i).szFileName );
        InputStream = fopen(Buffer, "rb");
        if( !InputStream )
		{
            printf("can't read:%s",Buffer);
			fclose( PAKStream );
			return false;
		}
        for (u32 j = 0; j < ALL_FILES.at(i).dwFileSize;) {
            size_t size = fread(buffer, 1, sizeof(buffer), InputStream);
            fwrite(buffer, 1, size, PAKStream);
            j += size;
            if (size == 0) {
                break;
            }
        }
        fclose(InputStream);
        fclose(PAKStream);
    }
    
    fclose( PAKStream );

	return true;
}

	

/////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:		Pak.h
//
// Author:		Raymond Wilson - ray.wilson@blueyonder.co.uk
//
// Date:		25th May 2003
//
// Notice:		Copyright (C) 2003, Raymond Wilson
//
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PAK_H
#define PAK_H

// INCLUDES
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <vector>

typedef unsigned int		u32;
typedef unsigned char		u8;

#define FILE_PATH_LEN 1024
#define FILE_NAME_LEN 128
using namespace std;
// PAK file header
struct sPakHeader									
{
	char		szSignature[3];						// PAK Signature should be 'RWPACK'
	float		fVersion;							// Version of PAK file
	u32		dwNumFTEntries;						// Number of file table entries
	u8		bCypherAddition;					// Should the caesar cypher add (or subtract)
	u8		iCypherValue;						// Random value used for the caesar cypher between 0-255
	char	szUniqueID[10];						// A unique ID for the file (other programs can check it)
	u32		dwReserved;							// Reserved Value
};

// Table Entry per file in the PAK file
struct sFileTableEntry								
{
	char			szFileName[FILE_NAME_LEN];				// Name of one file in the PAK
	u32				dwFileSize;					// The size of the file in bytes
	u32				dwOffset;					// Offset of file in the PAK
	//sFileTableEntry*	Next;						// Next file table entry (is a linked list)
};

/////////////////////////////////////////////////////////////////////////////////////////////

class CPakFile
{
	private:
       
		// Private Variables
		char					m_szFolderPath[FILE_PATH_LEN];		// Folder to compile in to PAK
		char					m_szPakName[FILE_PATH_LEN];			// Output PAK file path and name

		sPakHeader				m_Header;					// The header of the PAK file
		sFileTableEntry*		m_FileTable;				// The master file table for the PAK

		// Private Functions
		bool		GenerateHFT();							// Create a Header and File Table
		bool		WorkOutOffsets();						// Work out the file offsets in the PAK

        vector<sFileTableEntry> ALL_FILES;
        bool _hasErr;
        void SearchDirectory(const char* name);
    
	public:
		CPakFile();											// Constructor
		~CPakFile();										// Deconstructor

		bool		CreatePak( const char* Path, const char* Output);	// Create the PAK file
};

#endif
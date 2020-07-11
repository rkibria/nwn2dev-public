/*++

Copyright (c) Ken Johnson (Skywing). All rights reserved.

Module Name:

	ZipFileReader.h

Abstract:

	This module defines the interface to the zip file reader, which allows
	resource load requests to be serviced against a .zip archive instead of an
	ERF file.

--*/

#ifndef _PROGRAMS_NWN2DATALIB_ZIPFILEREADER_H
#define _PROGRAMS_NWN2DATALIB_ZIPFILEREADER_H

#ifdef _MSC_VER
#pragma once
#endif

#include "ResourceAccessor.h"

//
// Define the directory file reader object, used to access directory files.
//

template< typename ResRefT >
class ZipFileReader : public IResourceAccessor< ResRefT >
{

public:

	//
	// Constructor.  Raises an std::exception on catastrophic failure.
	//

	ZipFileReader(
		nwn2dev__in const std::string & ArchiveName
		);

	//
	// Destructor.
	//

	virtual
	~ZipFileReader(
		);

	//
	// Open an encapsulated file by resref.
	//

	virtual
	FileHandle
	OpenFile(
		nwn2dev__in const ResRefT & ResRef,
		nwn2dev__in ResType Type
		);

	//
	// Open an encapsulated file by file index.
	//

	virtual
	FileHandle
	OpenFileByIndex(
		nwn2dev__in FileId FileIndex
		);

	//
	// Close an encapsulated file.
	//

	virtual
	bool
	CloseFile(
		nwn2dev__in FileHandle File
		);

	//
	// Read an encapsulated file by file handle.  The routine is optimized to
	// operate for sequential file reads.
	//

	virtual
	bool
	ReadEncapsulatedFile(
		nwn2dev__in FileHandle File,
		nwn2dev__in size_t Offset,
		nwn2dev__in size_t BytesToRead,
		nwn2dev__out size_t * BytesRead,
		__out_bcount( BytesToRead ) void * Buffer
		);

	//
	// Return the size of a file.
	//

	virtual
	size_t
	GetEncapsulatedFileSize(
		nwn2dev__in FileHandle File
		);

	//
	// Return the resource type of a file.
	//

	virtual
	ResType
	GetEncapsulatedFileType(
		nwn2dev__in FileHandle File
		);

	//
	// Iterate through resources in this resource accessor.  The routine
	// returns false on failure.
	//

	virtual
	bool
	GetEncapsulatedFileEntry(
		nwn2dev__in FileId FileIndex,
		nwn2dev__out typename ResRefT & ResRef,
		nwn2dev__out ResType & Type
		);

	//
	// Return the count of encapsulated files in this accessor.
	//

	virtual
	FileId
	GetEncapsulatedFileCount(
		);

	//
	// Get the logical name of this accessor.
	//

	virtual
	AccessorType
	GetResourceAccessorName(
		nwn2dev__in FileHandle File,
		nwn2dev__out std::string & AccessorName
		);

private:

	typedef void * ZipArchive;

	struct DirectoryEntry
	{
		FileHandle  FileHandleToReturn;
		ResRefT     Name;
		ResType     Type;
	};

	typedef std::vector< DirectoryEntry > DirectoryEntryVec;

	//
	// Open a new zip archive.
	//

	ZipArchive
	OpenArchive(
		nwn2dev__in const std::string & ArchiveName
		);

	//
	// Close an opened archive.
	//

	void
	CloseArchive(
		nwn2dev__in ZipArchive Archive
		);

	//
	// Scan directories to create directory file entries.
	//

	void
	ScanArchive(
		nwn2dev__in ZipArchive Archive
		);

	//
	// Locate a file in the zip archive.
	//

	const DirectoryEntry *
	LocateFileByName(
		nwn2dev__in const ResRefT & ResRef,
		nwn2dev__in ResType Type
		);

	DirectoryEntryVec m_DirectoryEntries;
	ZipArchive        m_Archive;
	bool              m_FileOpened;     // Is m_OpenFileHandle valid?
	FileHandle        m_OpenFileHandle; // Currently open file handle
	unsigned __int64  m_CurrentOffset;  // Offset into currently opened file
	std::string       m_FileName;

};

typedef ZipFileReader< NWN::ResRef32 > ZipFileReader32;
typedef ZipFileReader< NWN::ResRef16 > ZipFileReader16;


#endif


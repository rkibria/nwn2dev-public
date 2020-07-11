/*++

Copyright (c) Ken Johnson (Skywing). All rights reserved.

Module Name:

	DirectoryFileReader.h

Abstract:

	This module defines the interface to the directory file reader, which
	allows resource load requests to be serviced against a directory instead of
	an ERF file.

--*/

#ifndef _PROGRAMS_NWN2DATALIB_DIRECTORYFILEREADER_H
#define _PROGRAMS_NWN2DATALIB_DIRECTORYFILEREADER_H

#ifdef _MSC_VER
#pragma once
#endif

#include "ResourceAccessor.h"

//
// Define the directory file reader object, used to access directory files.
//

template< typename ResRefT >
class DirectoryFileReader : public IResourceAccessor< ResRefT >
{

public:

	//
	// Constructor.  Raises an std::exception on catastrophic failure.
	//

	DirectoryFileReader(
		nwn2dev__in const std::string & DirectoryName
		);

	//
	// Destructor.
	//

	virtual
	~DirectoryFileReader(
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

	//
	// Return the path name of the directory as provided to the constructor.
	//

	inline
	const std::string &
	GetDirectoryName(
		) const
	{
		return m_DirectoryName;
	}

	//
	// Return the full path name to a file by directory entry.
	//

	inline
	const std::string &
	GetRealFileName(
		nwn2dev__in FileId FileIndex
		)
	{
		if ((size_t) FileIndex >= m_DirectoryEntries.size( ))
			throw std::runtime_error( "Illegal FileIndex" );

		return m_DirectoryEntries[ (size_t) FileIndex ].RealFileName;
	}

private:

	//
	// Scan directories to create directory file entries.
	//

	void
	ScanDirectory(
		nwn2dev__in const std::string & Directory,
		nwn2dev__in size_t RecursionLevel
		);

	struct DirectoryEntry
	{
		std::string RealFileName;
		ResRefT     Name;
		ResType     Type;
	};

	typedef std::vector< DirectoryEntry > DirectoryEntryVec;

	DirectoryEntryVec m_DirectoryEntries;
	std::string       m_DirectoryName;

};

typedef DirectoryFileReader< NWN::ResRef32 > DirectoryFileReader32;
typedef DirectoryFileReader< NWN::ResRef16 > DirectoryFileReader16;


#endif

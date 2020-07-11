/*++

Copyright (c) Ken Johnson (Skywing). All rights reserved.

Module Name:

	2DAFileReader.h

Abstract:

	This module defines the interface to the Two Dimensional Array (2DA) file
	reader.  2DA files are used to provide arrays of textural data to the game,
	allowing for easy external editing.

--*/

#ifndef _PROGRAMS_NWN2DATALIB_2DAFILEREADER_H
#define _PROGRAMS_NWN2DATALIB_2DAFILEREADER_H

#ifdef _MSC_VER
#pragma once
#endif

//
// Define the 2DA file reader object, used to access 2DA files.
//

class TwoDAFileReader
{

public:

	//
	// Constructor.  Raises an std::exception on parse failure.
	//

	TwoDAFileReader(
		nwn2dev__in const std::string & FileName
		);

	//
	// Destructor.
	//

	virtual
	~TwoDAFileReader(
		);

	//
	// Look up the value of a particular column at a given row index.
	//
	// The routine returns false if no such column or row existed, or if the
	// column value was the empty value ("****").
	//

	bool
	Get2DAString(
		nwn2dev__in const std::string & Column,
		nwn2dev__in size_t Row,
		nwn2dev__out std::string & Value
		) const;

	//
	// Various datatype wrappers around Get2DAString.
	//

	inline
	bool
	Get2DAInt(
		nwn2dev__in const std::string & Column,
		nwn2dev__in size_t Row,
		nwn2dev__out int & Value,
		nwn2dev__in int Radix = 0
		) const
	{
		std::string V;

		if (!Get2DAString( Column, Row, V ))
			return false;

		Value = (int) strtol( V.c_str( ), NULL, Radix );

		return true;
	}

	inline
	bool
	Get2DAUlong(
		nwn2dev__in const std::string & Column,
		nwn2dev__in size_t Row,
		nwn2dev__out unsigned long & Value,
		nwn2dev__in int Radix = 0
		) const
	{
		std::string V;

		if (!Get2DAString( Column, Row, V ))
			return false;

		Value = strtoul( V.c_str( ), NULL, Radix );

		return true;
	}

	inline
	bool
	Get2DABool(
		nwn2dev__in const std::string & Column,
		nwn2dev__in size_t Row,
		nwn2dev__out bool & Value
		) const
	{
		std::string V;

		if (!Get2DAString( Column, Row, V ))
			return false;

		if (V.empty( ))
			return false;

		if ((V[ 0 ] == 't') || (V[ 0 ] == 'T') || (V[ 0 ] == '1'))
			Value = true;
		else
			Value = false;

		return true;
	}

	inline
	bool
	Get2DAResRef(
		nwn2dev__in const std::string & Column,
		nwn2dev__in size_t Row,
		nwn2dev__out NWN::ResRef32 & Value
		) const
	{
		std::string V;

		if (!Get2DAString( Column, Row, V ))
			return false;

		if (V.empty( ))
			return false;

		ZeroMemory( &Value, sizeof( Value ) );
		memcpy( &Value, V.data( ), std::min( V.size( ), sizeof( Value ) ) );

		return true;
	}

	inline
	bool
	Get2DAResRef(
		nwn2dev__in const std::string & Column,
		nwn2dev__in size_t Row,
		nwn2dev__out NWN::ResRef16 & Value
		) const
	{
		std::string V;

		if (!Get2DAString( Column, Row, V ))
			return false;

		if (V.empty( ))
			return false;

		ZeroMemory( &Value, sizeof( Value ) );
		memcpy( &Value, V.data( ), std::min( V.size( ), sizeof( Value ) ) );

		return true;
	}

	inline
	bool
	Get2DAFloat(
		nwn2dev__in const std::string & Column,
		nwn2dev__in size_t Row,
		nwn2dev__out float & Value
		) const
	{
		std::string V;

		if (!Get2DAString( Column, Row, V ))
			return false;

		Value = (float) atof( V.c_str( ) );

		return true;
	}
	//
	// Return the count of valid rows in the .2DA.
	//

	inline
	size_t
	GetRowCount(
		) const
	{
		return m_Rows.size( );
	}

	//
	// Return the count of valid columns in the .2DA.
	//

	inline
	size_t
	GetColumnCount(
		) const
	{
		return m_Columns.size( );
	}

	//
	// Determine whether the .2DA supports a particular column or not.
	//

	inline
	bool
	HasColumn(
		nwn2dev__in const std::string & ColumnName
		) const
	{
		for (ColumnNameVec::const_iterator it = m_Columns.begin( );
		     it != m_Columns.end( );
		     ++it)
		{
			if (*it == ColumnName)
				return true;
		}

		return false;
	}

private:

	typedef std::vector< std::string > ColumnNameVec;
	typedef std::vector< std::string > RowValueVec;
	typedef std::vector< RowValueVec > RowsVec;

	//
	// Parse the on-disk format and read the base column listing in.
	//

	void
	Parse2DAFile(
		nwn2dev__in const std::string & FileName
		);

	//
	// Look up a column index by column name.
	//

	inline
	size_t
	GetColumnIndex(
		nwn2dev__in const std::string & Column
		) const
	{
		for (size_t i = 0; i < m_Columns.size( ); i += 1)
		{
			if (m_Columns[ i ] == Column)
				return i;
		}

		try
		{
			std::string ErrorStr;

			ErrorStr  = "Illegal 2DA column reference: '";
			ErrorStr += Column;
			ErrorStr += "'.";

			throw std::runtime_error( ErrorStr );
		}
		catch (std::bad_alloc)
		{
			throw std::runtime_error( "Illegal 2DA column reference." );
		}
	}

	//
	// Resource list data.
	//

	ColumnNameVec m_Columns; // Column names
	RowsVec       m_Rows;    // Row contents

};

#endif

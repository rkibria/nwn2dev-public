/*++

Copyright (c) Ken Johnson (Skywing). All rights reserved.

Module Name:

	NWNX4Plugin.h

Abstract:

	This module defines interfaces related to the NWNX4 plugin API.

--*/

#ifndef _SOURCE_PROGRAMS_AURORASERVERNWSCRIPT_NWNX4PLUGIN_H
#define _SOURCE_PROGRAMS_AURORASERVERNWSCRIPT_NWNX4PLUGIN_H

#ifdef _MSC_VER
#pragma once
#endif

#define NWNXAPI __thiscall

class NWNX4PluginBase
{

public:

	inline
	NWNX4PluginBase(
		nwn2dev__in const char * ClassName,
		nwn2dev__in const char * Subclass,
		nwn2dev__in const char * Version,
		nwn2dev__in const char * Description
		)
	: m_ClassName( ClassName ),
	  m_Subclass( Subclass ),
	  m_Version( Version ),
	  m_Description( Description )
	{
	}

	inline
	virtual
	NWNXAPI
	~NWNX4PluginBase(
		)
	{
	}

	inline
	virtual
	bool
	NWNXAPI
	Init(
		nwn2dev__in const char * NWNXHome
		)
	{
		UNREFERENCED_PARAMETER( NWNXHome );

		return true;
	}

	inline
	virtual
	int
	NWNXAPI
	GetInt(
		nwn2dev__in const char * Function,
		nwn2dev__in const char * Param1,
		nwn2dev__in int Param2
		)
	{
		UNREFERENCED_PARAMETER( Function );
		UNREFERENCED_PARAMETER( Param1 );
		UNREFERENCED_PARAMETER( Param2 );

		return 0;
	}

	inline
	virtual
	void
	NWNXAPI
	SetInt(
		nwn2dev__in const char * Function,
		nwn2dev__in const char * Param1,
		nwn2dev__in int Param2,
		nwn2dev__in int Value
		)
	{
		UNREFERENCED_PARAMETER( Function );
		UNREFERENCED_PARAMETER( Param1 );
		UNREFERENCED_PARAMETER( Param2 );
		UNREFERENCED_PARAMETER( Value );
	}

	inline
	virtual
	float
	NWNXAPI
	GetFloat(
		nwn2dev__in const char * Function,
		nwn2dev__in const char * Param1,
		nwn2dev__in int Param2
		)
	{
		UNREFERENCED_PARAMETER( Function );
		UNREFERENCED_PARAMETER( Param1 );
		UNREFERENCED_PARAMETER( Param2 );

		return 0.0f;
	}

	inline
	virtual
	void
	NWNXAPI
	SetFloat(
		nwn2dev__in const char * Function,
		nwn2dev__in const char * Param1,
		nwn2dev__in int Param2,
		nwn2dev__in float Value
		)
	{
		UNREFERENCED_PARAMETER( Function );
		UNREFERENCED_PARAMETER( Param1 );
		UNREFERENCED_PARAMETER( Param2 );
		UNREFERENCED_PARAMETER( Value );
	}

	inline
	virtual
	const char *
	NWNXAPI
	GetString(
		nwn2dev__in const char * Function,
		nwn2dev__in const char * Param1,
		nwn2dev__in int Param2
		)
	{
		UNREFERENCED_PARAMETER( Param1 );
		UNREFERENCED_PARAMETER( Param2 );

		return ProcessQueryFunction( Function );
	}

	inline
	virtual
	void
	NWNXAPI
	SetString(
		nwn2dev__in const char * Function,
		nwn2dev__in const char * Param1,
		nwn2dev__in int Param2,
		nwn2dev__in const char * Value
		)
	{
		UNREFERENCED_PARAMETER( Function );
		UNREFERENCED_PARAMETER( Param1 );
		UNREFERENCED_PARAMETER( Param2 );
		UNREFERENCED_PARAMETER( Value );
	}

	inline
	virtual
	void
	NWNXAPI
	GetFunctionClass(
		__out_ecount( 128 ) char * ClassName
		)
	{
		StringCbCopyA( ClassName, 128, m_ClassName );
	}

protected:

	inline
	const char *
	ProcessQueryFunction(
		nwn2dev__in const char * QueryFunction
		)
	{
		if (!strcmp( QueryFunction, "GET SUBCLASS" ))
			return m_Subclass;
		else if (!strcmp( QueryFunction, "GET VERSION" ))
			return m_Version;
		else if (!strcmp( QueryFunction, "GET DESCRIPTION" ))
			return m_Description;
		else
			return "";
	}

private:

	const char * m_ClassName;
	const char * m_Subclass;
	const char * m_Version;
	const char * m_Description;

};

NWNX4PluginBase *
WINAPI
GetPluginPointerV2(
	);

#endif


/*++

Copyright (c) Ken Johnson (Skywing). All rights reserved.

Module Name:

	GrannyNative.h

Abstract:

	This module houses type definitions for the native file layout of a *.gr2
	file before GrannyConvertToRaw has transformed it.

--*/

#ifndef _PROGRAMS_GRANNY2LIB_GRANNYNATIVE_H
#define _PROGRAMS_GRANNY2LIB_GRANNYNATIVE_H

#ifdef _MSC_VER
#pragma once
#endif

namespace GrannyNative
{
#include <pshpack1.h>

	struct GrnFileBaseHeader
	{
		ULONG Magic[ 4 ];
		ULONG HeaderSize;
		ULONG HeaderFormat;
		ULONG Reserved[ 2  ];
	};

	static_assert( sizeof( GrnFileBaseHeader ) == 0x20 , "compile time assert failed" );

	struct GrnReference
	{
		ULONG SectionIndex;
		ULONG Offset;
	};

	static_assert( sizeof( GrnReference ) == 0x8 , "compile time assert failed" );

	struct GrnSection
	{
		ULONG Format;
		ULONG DataOffset;
		ULONG DataSize;
		ULONG ExpandedDataSize;
		ULONG First16Bit;
		ULONG First8Bit;
		ULONG PointerFixupArrayOffset;
		ULONG PointerFixupArrayCount;
		ULONG MixedMarshallingFixupArrayOffset;
		ULONG MixedMarshallingFixupArrayCount;
	};

	static_assert( sizeof( GrnSection ) == 0x2C , "compile time assert failed" );

	struct GrnPointerFixup
	{
		ULONG        FromOffset;
		GrnReference To;
	};

	static_assert( sizeof( GrnPointerFixup ) == 0xC , "compile time assert failed" );

	struct GrnMixedMarshallingFixup
	{
		ULONG        Count;
		ULONG        Offset;
		GrnReference Type[ 2 ];
	};

	static_assert( sizeof( GrnMixedMarshallingFixup ) == 0x10 , "compile time assert failed" );

	struct GrnFileHeader
	{
		GrnFileBaseHeader BaseHeader;
		ULONG             Version;
		ULONG             TotalSize;
		ULONG             CRC;
		ULONG             SectionArrayOffset;
		ULONG             SectionArrayCount;
		GrnReference      RootObjectTypeDefinition;
		GrnReference      RootObject;
		ULONG             TypeTag;
		ULONG             ExtraTags;
	};

	static_assert( sizeof( GrnFileHeader ) == 0x38 + sizeof( GrnFileBaseHeader ) , "compile time assert failed" );

#include <poppack.h>
}

#endif

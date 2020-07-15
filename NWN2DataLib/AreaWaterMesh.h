/*++

Copyright (c) Ken Johnson (Skywing). All rights reserved.

Module Name:

	AreaWaterMesh.h

Abstract:

	This module defines the AreaWaterMesh class, which stores mesh mapping and
	texture lookup data related to a water patch in an area.

	Major portions of the TRX on-disk layout courtesy Tero Kivinen
	<kivinen@iki.fi>.

--*/

#ifndef _PROGRAMS_NWN2DATALIB_AREAWATERMESH_H
#define _PROGRAMS_NWN2DATALIB_AREAWATERMESH_H

#ifdef _MSC_VER
#pragma once
#endif

#include "DdsImage.h"
#include "MeshLinkage.h"

class MeshManager;

extern const MeshLinkageTraits MLT_AreaWaterMesh;

//
// Define the AreaWaterMesh which contains water display data.
//

class AreaWaterMesh
{

public:

	inline
	AreaWaterMesh(
		)
	: m_MeshLinkage( &MLT_AreaWaterMesh )
	{
		Clear( );
	}

	inline
	~AreaWaterMesh(
		)
	{
	}

#include <pshpack1.h>

	struct WaterVertex // Vertex
	{
		NWN::Vector3 p;    // Position
		NWN::Vector2 uv_0; // XY5
		NWN::Vector2 uv_1; // XY1
	};

	static_assert( sizeof( WaterVertex ) == 28 , "compile time assert failed" );

	typedef unsigned short WaterFaceVertexIndex;

	struct WaterFace // Corner
	{
		unsigned short Vertex[ 3 ]; // Must match WaterFaceVertexIndex
	};

	static_assert( sizeof( WaterFace ) == 6 , "compile time assert failed" );

	struct WaterBitmap
	{
		unsigned long Bits[ 24 ][ 48 ];
	};

	static_assert( sizeof( WaterBitmap ) == 1152 * 4 , "compile time assert failed" );

#include <poppack.h>

	typedef std::vector< WaterVertex > WaterVertexVec;
	typedef std::vector< WaterFace > WaterFaceVec;

	inline
	const WaterVertexVec &
	GetWaterVerticies(
		) const
	{
		return m_WaterVerticies;
	}

	inline
	const WaterFaceVec &
	GetWaterFaces(
		) const
	{
		return m_WaterFaces;
	}

	inline
	void
	Clear(
		)
	{
		m_WaterColor.r = 0.0f;
		m_WaterColor.g = 0.0f;
		m_WaterColor.b = 0.0f;

		m_WaterVerticies.clear( );
		m_WaterFaces.clear( );

		ZeroMemory( &m_WaterBitmap, sizeof( m_WaterBitmap ) );

		m_Image.Clear( );
	}

	inline
	void
	AddWaterVertex(
		nwn2dev__in const WaterVertex & Vertex
		)
	{
		m_WaterVerticies.push_back( Vertex );
	}

	inline
	void
	AddWaterFace(
		nwn2dev__in const WaterFace & Face
		)
	{
		m_WaterFaces.push_back( Face );
	}

	inline
	const NWN::NWNRGB &
	GetWaterColor(
		) const
	{
		return m_WaterColor;
	}

	inline
	void
	SetWaterColor(
		nwn2dev__in const NWN::NWNRGB & WaterColor
		)
	{
		m_WaterColor = WaterColor;
	}

	inline
	const WaterBitmap &
	GetWaterBitmap(
		) const
	{
		return m_WaterBitmap;
	}

	inline
	void
	SetWaterBitmap(
		nwn2dev__in const WaterBitmap & Bitmap
		)
	{
		m_WaterBitmap = Bitmap;
	}

	inline
	unsigned long
	GetWaterX(
		) const
	{
		return m_WaterX;
	}

	inline
	void
	SetWaterX(
		nwn2dev__in unsigned long WaterX
		)
	{
		m_WaterX = WaterX;
	}

	inline
	unsigned long
	GetWaterY(
		) const
	{
		return m_WaterY;
	}

	inline
	void
	SetWaterY(	
		nwn2dev__in unsigned long WaterY
		)
	{
		m_WaterY = WaterY;
	}

	inline
	const DdsImage &
	GetImage(
		) const
	{
		return m_Image;
	}

	inline
	DdsImage &
	GetImage(
		)
	{
		return m_Image;
	}

	//
	// Validate the watermesh, ensuring that all indicies are legal.
	//

	inline
	void
	Validate(
		) const
	{
		for (WaterFaceVec::const_iterator it = m_WaterFaces.begin( );
		     it != m_WaterFaces.end( );
		     ++it)
		{
			for (unsigned long i = 0; i < 3; i += 1)
			{
				if (it->Vertex[ i ] >= m_WaterVerticies.size( ))
					throw std::runtime_error( "Illegal WaterFace.Vertex" );
			}
		}
	}

private:

	NWN::NWNRGB    m_WaterColor;
	WaterVertexVec m_WaterVerticies;
	WaterFaceVec   m_WaterFaces;
	WaterBitmap    m_WaterBitmap;
	DdsImage       m_Image;
	unsigned long  m_WaterX;
	unsigned long  m_WaterY;

public: // For internal use only

	DECLARE_MESH_LINKAGE;

private:

	friend class MeshManager;

};

typedef std::vector< AreaWaterMesh > AreaWaterMeshVec;

#endif

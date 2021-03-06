/*++

Copyright (c) Ken Johnson (Skywing). All rights reserved.

Module Name:

	RigidMesh.h

Abstract:

	This module defines the RigidMesh class, which contains a RIGD-typed mesh
	that is stored in local coordinate form and forms the basis of a model that
	does not carry its own skeleton.  For example, objects that do not have
	smootly translating animations are built with a series of RigidMeshes.

--*/

#ifndef _PROGRAMS_NWN2DATALIB_RIGIDMESH_H
#define _PROGRAMS_NWN2DATALIB_RIGIDMESH_H

#ifdef _MSC_VER
#pragma once
#endif

#include "SimpleMesh.h"
#include "MDBFormat.h"

//
// Define the rigid mesh, which is used to render structure for placeables and
// other immobile objects.
//

//
// Define vertex and face types for rigid mesh.
//

#include <pshpack1.h>
struct RMVertexFile
{
	NWN::Vector3 Position;
	NWN::Vector3 Normal;
	NWN::Vector3 Tangent;
	NWN::Vector3 Binormal;
	NWN::Vector3 uvw; // Texture vertex
};

static_assert( sizeof( RMVertexFile ) == 15 * 4 , "compile time assert failed" );

struct RMFaceFile
{
	unsigned short Corners[ 3 ];
};

static_assert( sizeof( RMFaceFile ) == 3 * 2 , "compile time assert failed" );

#include <poppack.h>

struct RMVertex
{
	NWN::Vector3 LocalPos;
	NWN::Vector3 Normal;
	NWN::Vector3 Tangent;
	NWN::Vector3 Binormal;
	NWN::Vector3 uvw; // Texture vertex
//	NWN::Vector3 Pos; // Current position, calculated (not in file)
};

struct RMFace
{
	unsigned long  Corners[ 3 ];
//	NWN::Vector3   Normal;
};

extern const SimpleMeshTypeDescriptor SMTD_RigidMesh;

//
// Define the rigid mesh core itself.
//

class RigidMesh : public SimpleMesh< RMVertex, RMFace, &SMTD_RigidMesh >
{

public:

	typedef ::RMVertex Vertex;
	typedef ::RMVertexFile VertexFile;
	typedef ::RMFace Face;
	typedef ::RMFaceFile FaceFile;

	typedef unsigned long FaceVertexIndex; // Must match RMFace::Corners

	typedef SimpleMesh< Vertex, Face, &SMTD_RigidMesh > BaseMesh;

#include <pshpack1.h>

	struct RigidHeader
	{
		NWN::ResRef32  Name;
		MODEL_MATERIAL Material;
		unsigned long  NumVerts;
		unsigned long  NumFaces;
	};

	static_assert( sizeof( RigidHeader ) == 32 + sizeof( MODEL_MATERIAL ) + 8 , "compile time assert failed" );

#include <poppack.h>

	typedef RigidHeader Header;

	inline
	RigidMesh(
		)
	: SimpleMesh< RMVertex, RMFace, &SMTD_RigidMesh >( )
	{
		ZeroMemory( &m_Header, sizeof( m_Header ) );
	}

	inline
	virtual
	~RigidMesh(
		)
	{
	}

	inline
	Header &
	GetHeader(
		)
	{
		return m_Header;
	}

	inline
	const Header &
	GetHeader(
		) const
	{
		return m_Header;
	}

	static
	inline
	void
	CopyFileVertex(
		nwn2dev__in const VertexFile & FVert,
		nwn2dev__out Vertex & Vert
		)
	{
		Vert.LocalPos  = FVert.Position;
		Vert.Normal    = FVert.Normal;
		Vert.Tangent   = FVert.Tangent;
		Vert.Binormal  = FVert.Binormal;
		Vert.uvw       = FVert.uvw;
	}

	static
	inline
	void
	CopyFileFace(
		nwn2dev__in const FaceFile & FFace,
		nwn2dev__out Face & Face
		)
	{
		for (size_t i = 0; i < 3; i += 1)
			Face.Corners[ i ] = FFace.Corners[ i ];
	}

private:

	Header    m_Header;

};

#endif

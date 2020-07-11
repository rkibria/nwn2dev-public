/*++

Copyright (c) Ken Johnson (Skywing). All rights reserved.

Module Name:

	WorldObject.h

Abstract:

	This module houses the definition of the WorldView object, which
	represents a 3D object in the world.

--*/

#ifndef _PROGRAMS_MODELRENDERER_WORLDOBJECT_H
#define _PROGRAMS_MODELRENDERER_WORLDOBJECT_H

#ifdef _MSC_VER
#pragma once
#endif

class ModelInstance;
class ModelSkeleton;
class ModelCollider;
class ResourceManager;
struct IDebugTextOut;

class WorldObject
{

public:

	typedef swutil::SharedPtr< ModelCollider > ModelColliderPtr;
 	typedef std::vector< ModelColliderPtr > ModelPartVec;

	typedef swutil::SharedPtr< WorldObject > Ptr;

	WorldObject(
		nwn2dev__in ResourceManager & ResMan,
		nwn2dev__in IDebugTextOut * TextWriter,
		nwn2dev__in const std::vector< std::string > & MDBResRefs,
		nwn2dev__in const std::string & GR2ResRef
		);

	virtual
	~WorldObject(
		);

	//
	// Intersection support.
	//

	bool
	IntersectRay(
		nwn2dev__in const NWN::Vector3 & Origin,
		nwn2dev__in const NWN::Vector3 & NormDir,
		nwn2dev__out NWN::Vector3 & IntersectNormal,
		__out_opt float * IntersectDistance
		) const;

	//
	// Coordinate space transformation.
	//

	NWN::Vector3
	LocalToWorld(
		nwn2dev__in const NWN::Vector3 & v,
		nwn2dev__in NWN::NWN2_BodyPieceSlot Slot = NWN::BPS_DEFAULT
		) const;

	//
	// Model part access.
	//

	inline
	const ModelPartVec &
	GetModelParts(
		) const
	{
		return m_ModelParts;
	}

	inline
	const ModelCollider *
	GetModel(
		nwn2dev__in NWN::NWN2_BodyPieceSlot Slot = NWN::BPS_DEFAULT
		) const
	{
		if ((size_t) Slot >= m_ModelParts.size( ))
			return NULL;

		return m_ModelParts[ Slot ].get( );
	}

	//
	// Skeleton access.
	//

	inline
	const ModelSkeleton *
	GetSkeleton(
		nwn2dev__in NWN::NWN2_AnimatableSlot Slot = NWN::AS_DEFAULT
		) const
	{
		if (Slot != NWN::AS_DEFAULT)
			return NULL;

		return m_Skeleton.get( );
	}

	//
	// Return the skeleton for a body piece slot.
	//

	inline
	const ModelSkeleton *
	GetBodyPieceSkeleton(
		nwn2dev__in NWN::NWN2_BodyPieceSlot Slot = NWN::BPS_DEFAULT
		) const
	{
		if (Slot == NWN::BPS_DEFAULT)
			return GetSkeleton( NWN::AS_DEFAULT );

		if (GetModel( Slot ) == NULL)
			return NULL;

		//
		// Right now, just the single skeleton is supported.
		//
		// Normally, we would look up which skeleton the model part is attached
		// to, but for simplicity, we assume that they are all attached to the
		// same (first and only) skeleton.
		//

		return GetSkeleton( );
	}

	//
	// Simple transform component accessors.
	//

	inline
	const NWN::Vector3 &
	GetHeading(
		) const
	{
		return m_Heading;
	}

	inline
	void
	SetHeading(
		nwn2dev__in const NWN::Vector3 & v
		)
	{
		m_Heading = v;

		UpdateWorldTransformForHeading( v, m_Up, m_Scale );
	}

	inline
	void
	SetRotation(
		nwn2dev__in const NWN::Quaternion & Q
		)
	{
		UpdateWorldTransformForRotation( Q );
	}

	inline
	void
	SetFacing(
		nwn2dev__in float f
		)
	{
		m_Facing = f;

		UpdateWorldTransformForFacing( f );
	}

	inline
	float
	GetFacing(
		) const
	{
		return m_Facing;
	}

	inline
	const NWN::Vector3 &
	GetPosition(
		) const
	{
		return m_Position;
	}

	inline
	void
	SetPosition(
		nwn2dev__in NWN::Vector3 & v
		)
	{
		m_Position = v;

		UpdateWorldTransformForPosition( v );
	}

	inline
	const NWN::Vector3 &
	GetScale(
		) const
	{
		return m_Scale;
	}

	inline
	void
	SetScale(
		nwn2dev__in const NWN::Vector3 & v
		)
	{
		const float Epsilon = 1e-7f;

		NWN::Vector3 vs = v;

		if ((vs.x > -Epsilon) && (vs.x < Epsilon))
			vs.x = 1.0f;
		if ((vs.y > -Epsilon) && (vs.y < Epsilon))
			vs.y = 1.0f;
		if ((vs.z > -Epsilon) && (vs.z < Epsilon))
			vs.z = 1.0f;

		UpdateWorldTransformForScale( vs, m_Scale );

		m_Scale = vs;
	}

	//
	// World 'up' access.
	//

	inline
	const NWN::Vector3 &
	GetWorldUp(
		) const
	{
		return m_Up;
	}

private:

	typedef swutil::SharedPtr< ModelCollider > ModelColliderPtr;
	typedef swutil::SharedPtr< ModelSkeleton > ModelSkeletonPtr;


	//
	// Loading constructs.
	//

	ModelColliderPtr
	LoadModel(
		nwn2dev__in const NWN::ResRef32 & ModelFile
		);

	ModelSkeletonPtr
	LoadSkeleton(
		nwn2dev__in const NWN::ResRef32 & SkeletonFile
		);


	//
	// World transformation update callbacks.
	//

	void
	UpdateWorldTransformForHeading(
		nwn2dev__in const NWN::Vector3 & Heading,
		nwn2dev__in const NWN::Vector3 & Up,
		nwn2dev__in const NWN::Vector3 & Scale
		);

	inline
	void
	UpdateWorldTransformForFacing(
		nwn2dev__in float Facing
		)
	{
		NWN::Vector3 Heading;

		//
		// Translate to a two-dimensional facing and update.  These objects
		// have no z-axis orientation and are assumed be aligned along the
		// z-axis.
		//

		Heading.x = cos( Facing );
		Heading.y = sin( Facing );
		Heading.z = 0.0f;

		UpdateWorldTransformForHeading( Heading, m_Up, m_Scale );
	}

	void
	UpdateWorldTransformForRotation(
		nwn2dev__in const NWN::Quaternion & Rotation
		);

	inline
	void
	UpdateWorldTransformForPosition(
		nwn2dev__in const NWN::Vector3 & Position
		)
	{
		m_WorldTrans._30 = Position.x;
		m_WorldTrans._31 = Position.y;
		m_WorldTrans._32 = Position.z;

		OnUpdateWorldTransform( );
	}

	void
	UpdateWorldTransformForScale(
		nwn2dev__in const NWN::Vector3 & Scale,
		nwn2dev__in const NWN::Vector3 & PrevScale
		);

	//
	// Event handler invoked when the world transformation has been updated.
	//

	void
	OnUpdateWorldTransform(
		);

	ResourceManager  & m_ResMan;
	IDebugTextOut    * m_TextWriter;
	ModelPartVec       m_ModelParts;
	ModelSkeletonPtr   m_Skeleton;
	NWN::Matrix44      m_WorldTrans;
	NWN::Vector3       m_Heading;
	float              m_Facing;
	NWN::Vector3       m_Up;
	NWN::Vector3       m_Position;
	NWN::Vector3       m_Scale;
};

#endif

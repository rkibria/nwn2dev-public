/*++

Copyright (c) Ken Johnson (Skywing). All rights reserved.

Module Name:

	WorldView.h

Abstract:

	This module defines the simple world viewer class.

--*/

#ifndef _PROGRAMS_MODELRENDERER_WORLDVIEW_H
#define _PROGRAMS_MODELRENDERER_WORLDVIEW_H

#ifdef _MSC_VER
#pragma once
#endif

class WorldObject;
class ResourceManager;
class Camera;
struct IDebugTextOut;

class WorldView
{

public:
	
	typedef swutil::SharedPtr< WorldObject > WorldObjectPtr;

private:

	typedef std::vector< WorldObjectPtr > WorldObjectVec;
	typedef swutil::SharedPtr< Camera > CameraPtr;

public:

	WorldView(
		nwn2dev__in ResourceManager & ResMan,
		nwn2dev__in IDebugTextOut * TextWriter,
		__in_opt HWND Parent = NULL
		);

	~WorldView(
		);

	//
	// Add an object to the world, with the model and skeleton data loaded.
	//
	// The caller may position the object using the returned pointer.
	//

	WorldObjectPtr
	CreateWorldObject(
		nwn2dev__in const std::vector< std::string > & MDBResRefs,
		nwn2dev__in const std::string & GR2ResRef
		);
	//
	// Show or hide the window.
	//

	inline
	void
	Show(
		nwn2dev__in bool Show
		)
	{
		if (Show)
			ShowWindow( m_WorldWindow, SW_SHOW );
		else
			ShowWindow( m_WorldWindow, SW_HIDE );
	}

	//
	// Register or deregister window classes.
	//

	static
	bool
	RegisterClasses(
		nwn2dev__in bool Register
		);

private:

	//
	// Draw a wireframe for an object.
	//

	bool
	DrawWireframeMesh(
		nwn2dev__in HDC hdc,
		nwn2dev__in COLORREF Color,
		nwn2dev__in const WorldObject * Object
		);

	//
	// Draw a SimpleMesh-compatible wireframe mesh.
	//

	template< typename T >
	bool
	DrawSimpleWireframeMesh(
		nwn2dev__in HDC hdc,
		nwn2dev__in COLORREF Color,
		nwn2dev__in const T & Mesh,
		nwn2dev__in const WorldObject * Object,
		nwn2dev__in NWN::NWN2_BodyPieceSlot Slot = NWN::BPS_DEFAULT
		);

	//
	// Draw a SimpleMesh-compatible wireframe mesh.  The color and fill
	// attributes are assumed to come from the device context.
	//

	template< typename T >
	bool
	DrawSimpleWireframeMesh2(
		nwn2dev__in HDC hdc,
		nwn2dev__in const T & Mesh,
		nwn2dev__in const WorldObject * Object,
		nwn2dev__in NWN::NWN2_BodyPieceSlot Slot
		);

	//
	// Draw a SimpleMesh-compatible wireframe mesh.  The routine appends
	// points and polygon definitions into a point and polygon array; no
	// drawing is performed, only the collection of the mesh into a central
	// uniform location.
	//

	template< typename T >
	bool
	DrawSimpleWireframeMesh3(
		__inout std::vector< POINT > & Points,
		__inout std::vector< INT > & PolyCounts,
		nwn2dev__in const T & Mesh,
		nwn2dev__in const WorldObject * Object,
		nwn2dev__in NWN::NWN2_BodyPieceSlot Slot,
		nwn2dev__in bool Reserve
		);

	//
	// Draw the bones for an object.
	//

	bool
	DrawBones(
		nwn2dev__in HDC hdc,
		nwn2dev__in COLORREF Color,
		nwn2dev__in const WorldObject * Object,
		nwn2dev__in NWN::NWN2_BodyPieceSlot Slot = NWN::BPS_DEFAULT
		);

	//
	// Draw the bones for an object.  The color and fill attributes are
	// assumed to come from the device context.
	//

	bool
	DrawBones2(
		nwn2dev__in HDC hdc,
		nwn2dev__in const WorldObject * Object,
		nwn2dev__in NWN::NWN2_BodyPieceSlot Slot
		);

	//
	// Draw the bone for an object.  The routine appends points and polygon
	// definitions into a point and polygon array; no drawing is performed,
	// only the collection of the bones into a central uniform location.
	//

	bool
	DrawBones3(
		__inout std::vector< POINT > & Points,
		__inout std::vector< BYTE > & Types,
		nwn2dev__in const ModelSkeleton * Skeleton,
		nwn2dev__in const WorldObject * Object,
		nwn2dev__in NWN::NWN2_BodyPieceSlot Slot,
		nwn2dev__in bool Reserve
		);

	//
	// Convert game to client coordinates.
	//

	bool
	ClientToGame(
		nwn2dev__in const POINT &pt,
		nwn2dev__out NWN::Vector3 &v,
		nwn2dev__in bool Clip = true
		);

	//
	// Convert client to game coordinates (approximate).
	//

	bool
	GameToClient(
		nwn2dev__in const NWN::Vector3 &v,
		nwn2dev__out POINT &pt,
		nwn2dev__in bool Clip = true
		);

	//
	// Convert game coordinates to client coordinates.
	//

	bool
	GameToClient(
		__inout RECT & rc,
		nwn2dev__in bool Clip = true
		);

	//
	// Convert client to game coordinates (approximate).  This routine is
	// used to specify the correct clipping behavior for mesh drawing.
	//

	bool
	GameToClientMesh(
		nwn2dev__in const NWN::Vector3 &v,
		nwn2dev__out POINT &pt
		);

	//
	// Create (and initialize) the default camera.
	//

	void
	CreateCamera(
		);

	//
	// Set default camera parameters.
	//

	void
	SetDefaultCameraParameters(
		nwn2dev__in Camera * C
		);

	//
	// Calculate whether there exists a clear line of sight from a ray to the
	// edge of the map.
	//

	bool
	CalcLineOfSightRay(
		nwn2dev__in const NWN::Vector3 & Origin,
		nwn2dev__in const NWN::Vector3 & NormDir,
		nwn2dev__out float & Distance
		);

	//
	// Recalculate the map drawing region rectangle.
	//

	void
	RecalculateMapRect(
		nwn2dev__in const RECT * ClientRect
		);

	//
	// Control readjustment after a move.
	//

	void
	MoveControls(
		);

	//
	// Redraw just the world window.
	//

	inline
	void
	RedrawWorldWindowOnly(
		nwn2dev__in bool DrawNow
		)
	{
		UINT Flags;

		Flags = RDW_ERASE | RDW_INVALIDATE;

		if (DrawNow)
			Flags |= RDW_UPDATENOW;

		(VOID) RedrawWindow( m_WorldWindow, NULL, NULL, Flags );
	}

	inline
	bool
	IsCamera3D(
		) const
	{
		return m_Camera.get( ) != NULL;
	}

	//
	// Check if a key is down.
	//

	inline
	bool
	IsKeyDown(
		nwn2dev__in UINT vk
		) const
	{
		return GetKeyState( vk ) & 0x8000 ? true : false;
	}

	//
	// Drawing logic.
	//

	void
	DrawWorldWindow(
		nwn2dev__in HWND hwnd,
		nwn2dev__in PAINTSTRUCT & ps,
		nwn2dev__in const RECT & UpdateRect
		);

	//
	// Window procedure dispatchers.
	//

	static
	LRESULT
	CALLBACK
	WorldViewWindowProc_s(
		nwn2dev__in HWND hwnd,
		nwn2dev__in UINT uMsg,
		nwn2dev__in WPARAM wParam,
		nwn2dev__in LPARAM lParam
		);

	LRESULT
	WorldViewWindowProc(
		nwn2dev__in HWND hwnd,
		nwn2dev__in UINT uMsg,
		nwn2dev__in WPARAM wParam,
		nwn2dev__in LPARAM lParam
		);

	//
	// Window message handlers.
	//

	// WM_CLOSE
	void
	OnClose(
		nwn2dev__in HWND hwnd
		);

	// WM_DESTROY
	void
	OnDestroy(
		nwn2dev__in HWND hwnd
		);

	// WM_GETMINMAXINFO
	void
	OnGetMinMaxInfo(
		nwn2dev__in HWND hwnd,
		__inout LPMINMAXINFO lpMinMaxInfo
		);

	// WM_PAINT
	void
	OnPaint(
		nwn2dev__in HWND hwnd
		);

	// WM_PRINTCLIENT
	void
	OnPrintClient(
		nwn2dev__in HWND hwnd,
		nwn2dev__in HDC hdc,
		nwn2dev__in UINT DrawFlags
		);

	// WM_SIZE
	void
	OnSize(
		nwn2dev__in HWND hwnd,
		nwn2dev__in UINT state,
		nwn2dev__in int cx,
		nwn2dev__in int cy
		);

	// WM_MOVE
	void
	OnMove(
		nwn2dev__in HWND hwnd,
		nwn2dev__in int x,
		nwn2dev__in int y
		);

	// WM_KEYDOWN & WM_KEYUP
	void
	OnKey(
		nwn2dev__in HWND hwnd,
		nwn2dev__in UINT vk,
		nwn2dev__in BOOL fDown,
		nwn2dev__in int cRepeat,
		nwn2dev__in UINT flags
		);

	// WM_MOUSEMOVE
	void
	OnMouseMove(
		nwn2dev__in HWND hwnd,
		nwn2dev__in int x,
		nwn2dev__in int y,
		nwn2dev__in UINT keyFlags
		);

	// WM_MOUSEWHEEL
	void
	OnMouseWheel(
		nwn2dev__in HWND hwnd,
		nwn2dev__in int xPos,
		nwn2dev__in int yPos,
		nwn2dev__in int zDelta,
		nwn2dev__in UINT fwKeys
		);

	// WM_NOTIFY
	LRESULT
	OnNotify(
		nwn2dev__in HWND hwnd,
		nwn2dev__in int idFrom,
		__inout NMHDR * pnmhdr
		);

	enum
	{
		PIXELS_X_PER_OBJ = 1, // Pixel multiplication
		PIXELS_Y_PER_OBJ = 1,

		EXTRA_X_PIXELS   = 4, // Extra pixels around the target.
		EXTRA_Y_PIXELS   = 4,

		HIT_TEST_DISTANCE = 8 // Hit test distance for object clicks.
	};

	//
	// Instance variables.
	//

	ResourceManager & m_ResMan;
	IDebugTextOut   * m_TextWriter;
	RECT              m_MapRect;
	RECT              m_ClientRect;
	HWND              m_WorldWindow;
	CameraPtr         m_Camera;
	float             m_AreaWidth;
	float             m_AreaHeight;
	float             m_OriginX;
	float             m_OriginY;
	float             m_PaddingX;
	float             m_PaddingY;
	int               m_CursorX;
	int               m_CursorY;
	POINT             m_CameraRotateDelta;

	//
	// Active objects being drawn.
	//

	WorldObjectVec    m_WorldObjects;

};

#endif

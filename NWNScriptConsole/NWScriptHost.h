/*++

Copyright (c) Ken Johnson (Skywing). All rights reserved.

Module Name:

	NWScriptHost.h

Abstract:

	This module defines the NWScriptHost class.  The NWScriptHost object is
	responsible for coordinating actions in the scripting environment,
	including both calls to scripts themselves as well as the implementation of
	engine structure wrappers and engine action handlers.

--*/

#ifndef _SOURCE_PROGRAMS_NWNSCRIPTCONSOLE_NWSCRIPTHOST_H
#define _SOURCE_PROGRAMS_NWNSCRIPTCONSOLE_NWSCRIPTHOST_H

#ifdef _MSC_VER
#pragma once
#endif

class AppParameters;

struct IDebugTextOut;
class NWScriptJITLib;

#include "../NWNScriptLib/NWScriptInterfaces.h"
#include "../NWNScriptLib/NWScriptVM.h"
#include "../NWN2DataLib/NWScriptReader.h"
#include "../NWNScriptJIT/NWScriptJITLib.h"

//
// This is an ugly way to pass NWScriptVM::VMState *'s around without having to
// expose NWScriptVM.h to the entire world.
//

struct ScriptSituationPtr
{
	const NWScriptVM::VMState * SavedState;
};

class NWScriptHost : public INWScriptActions
{

public:

	//
	// Define engine structure types.
	//

	typedef enum _ENG_TYPE
	{
		EngTypeEffect        = 0,
		EngTypeEvent         = 1,
		EngTypeLocation      = 2,
		EngTypeTalent        = 3,
		EngTypeItemProperty  = 4,

		EngNumStructures
	} ENG_TYPE, * PENG_TYPE;

	typedef const enum _ENG_TYPE * PCENG_TYPE;

	typedef std::vector< std::string > ScriptParamVec;

	NWScriptHost(
		nwn2dev__in ResourceManager & ResMan,
		nwn2dev__in swutil::TimerManager & TimerManager,
		nwn2dev__in const AppParameters * Params,
		nwn2dev__in IDebugTextOut * TextOut
		);

	~NWScriptHost(
		);

	//
	// Execute a script (note that the script may be reentrant via action
	// handlers).
	//

	inline
	int
	RunScript(
		nwn2dev__in const char * ScriptName,
		nwn2dev__in NWN::OBJECTID ObjectId = NWN::INVALIDOBJID,
		nwn2dev__in int DefaultReturnCode = 0,
		nwn2dev__in ULONG Flags = 0
		)
	{
		return RunScript(
			ScriptName,
			ObjectId,
			ScriptParamVec( ),
			DefaultReturnCode,
			Flags);
	}

	int
	RunScript(
		nwn2dev__in const char * ScriptName,
		nwn2dev__in NWN::OBJECTID ObjectId,
		nwn2dev__in const ScriptParamVec & ScriptParameters,
		nwn2dev__in int DefaultReturnCode = 0,
		nwn2dev__in ULONG Flags = 0
		);

	inline
	int
	RunScript(
		nwn2dev__in const NWN::ResRef32 & ScriptName,
		nwn2dev__in NWN::OBJECTID ObjectId = NWN::INVALIDOBJID,
		nwn2dev__in int DefaultReturnCode = 0,
		nwn2dev__in ULONG Flags = 0
		)
	{
		if (ScriptName.RefStr[ 0 ] == '\0')
			return DefaultReturnCode;

		return RunScript(
			ScriptName,
			ObjectId,
			ScriptParamVec( ),
			DefaultReturnCode,
			Flags);
	}

	int
	RunScript(
		nwn2dev__in const NWN::ResRef32 & ScriptName,
		nwn2dev__in NWN::OBJECTID ObjectId,
		nwn2dev__in const ScriptParamVec & ScriptParameters,
		nwn2dev__in int DefaultReturnCode,
		nwn2dev__in ULONG Flags = 0
		);

	//
	// Execute a script situation.
	//

	void
	RunScriptSituation(
		nwn2dev__in NWScriptVM::VMState * ScriptState,
		nwn2dev__in NWScriptJITLib::SavedState::Ptr & ScriptStateJIT,
		nwn2dev__in NWScriptJITLib::Program::Ptr & ProgramJIT
		);

	//
	// Clear the script cache.
	//

	void
	ClearScriptCache(
		);

	//
	// Called by the main loop to start any pending deferred script situations
	// going.
	//

	bool
	InitiatePendingDeferredScriptSituations(
		);

	//
	// Called by the NWScriptVM when an action must be serviced.  This routine
	// acts as the action service dispatcher for all actions requested by the
	// script VM.
	//

	virtual
	void
	NWSCRIPTACTAPI
	OnExecuteAction(
		nwn2dev__in NWScriptVM & ScriptVM,
		nwn2dev__in NWScriptStack & VMStack,
		nwn2dev__in NWSCRIPT_ACTION ActionId,
		nwn2dev__in size_t NumArguments
		);

	//
	// Called by the NWScriptVM to create an engine structure of the given
	// internal type.
	//

	virtual
	EngineStructurePtr
	NWSCRIPTACTAPI
	CreateEngineStructure(
		nwn2dev__in NWScriptStack::ENGINE_STRUCTURE_NUMBER EngineType
		);

	//
	// Called by the NWScriptJIT when an action must be serviced.  This routine
	// acts as the action service dispatcher for all actions requested by the
	// script JIT environment.
	//

	virtual
	bool
	NWSCRIPTACTAPI
	OnExecuteActionFromJIT(
		nwn2dev__in NWSCRIPT_ACTION ActionId,
		nwn2dev__in size_t NumArguments
		);

	//
	// This callback routine is invoked when an action is executed.  The
	// implementation is responsible for performing whatever actions are
	// required for the given action call number.  Unlike the standard action
	// service handler callback, parameters and return values are not passed on
	// the VM stack.
	//
	// The action routine is permitted to return false on a fatal error
	// error condition, which may terminate the entire script chain.  It is
	// also permissible for the action routine to cause a re-entrant call into
	// the script VM (though the script VM may reject the call if the recursion
	// limit has been reached).
	//
	// N.B.  This routine is invoked by the JIT environment where there is no
	//       VM object in play.
	//
	// N.B.  Presently, engine structures cannot appear in the parameter or
	//       return value list for a fast call.
	//

	virtual
	bool
	NWSCRIPTACTAPI
	OnExecuteActionFromJITFast(
		nwn2dev__in NWSCRIPT_ACTION ActionId,
		nwn2dev__in size_t NumArguments,
		__in_ecount( NumCmds ) PCNWFASTACTION_CMD Cmds,
		nwn2dev__in size_t NumCmds,
		nwn2dev__in uintptr_t * CmdParams
		);

private:

	typedef swutil::SharedPtr< NWScriptReader > NWScriptReaderPtr;
	typedef swutil::SharedPtr< NWScriptVM > NWScriptVMPtr;
	typedef swutil::SharedPtr< NWScriptStack > NWScriptStackPtr;
	typedef swutil::SharedPtr< NWScriptJITLib > NWScriptJITLibPtr;
	typedef swutil::SharedPtr< NWScriptJITManagedSupport > NWScriptJITManagedSupportPtr;

	struct ScriptCacheData
	{
		NWScriptReaderPtr            Reader;
		NWScriptJITLib::Program::Ptr JITProgram;
	};

	//
	// Comparison predicate for ResRefModelMap.
	//

	struct ResRefLess : public std::binary_function< const NWN::ResRef32 &, const NWN::ResRef32 &, bool >
	{

		inline
		bool
		operator()(
			nwn2dev__in const NWN::ResRef32 & left,
			nwn2dev__in const NWN::ResRef32 & right
			) const
		{
			return memcmp( &left, &right, sizeof( left ) ) < 0;
		}

	};

	typedef std::map< NWN::ResRef32, ScriptCacheData, ResRefLess > ScriptCacheMap;

	//
	// Define a deferred script situation, as created by the AssignCommand or
	// DelayCommand action services.
	//

	struct DeferredScriptSituation
	{
		typedef swutil::SharedPtr< DeferredScriptSituation > Ptr;

		NWScriptJITLib::Program::Ptr    ProgramJIT;
		NWScriptJITLib::SavedState::Ptr ScriptSituationJIT;
		NWScriptVM::VMState             ScriptSituation;
		swutil::TimerRegistration::Ptr  Timer;
		ULONG                           DuePeriod;
	};

	typedef std::list< DeferredScriptSituation::Ptr > DeferredScriptSitList;

	//
	// Define the callback registration handler for each action type in the
	// system.
	//

	typedef 
	void
	(NWScriptHost:: * OnScriptActionProc)(
		nwn2dev__in NWScriptVM & ScriptVM,
		nwn2dev__in NWScriptStack & VMStack,
		nwn2dev__in NWSCRIPT_ACTION ActionId,
		nwn2dev__in size_t NumArguments
		);

	struct NWScriptActionEntry
	{
		OnScriptActionProc   ActionHandler;
		NWSCRIPT_ACTION      ActionId;
		const char         * ActionName;
	};

	//
	// Now declare each of the action handlers that we support.  These
	// definitions must match with nwscript.nss.
	//

	enum { MAX_ACTION_ID = 1058 };

#define DECLARE_NSS_HANDLER( Name, Ordinal ) \
	void                                     \
	OnAction_##Name(                          \
	    nwn2dev__in NWScriptVM & ScriptVM,          \
	    nwn2dev__in NWScriptStack & VMStack,        \
	    nwn2dev__in NWSCRIPT_ACTION ActionId,       \
	    nwn2dev__in size_t NumArguments             \
	    );                                   \
	                                         \
	static_assert( Ordinal < MAX_ACTION_ID , "compile time assert failed" );     

#include "NWScriptActionDefs.h"
#undef DECLARE_NSS_HANDLER

	//
	// Register actions with the script system.
	//

	void
	RegisterActions(
		);

	//
	// Locate a script by name (from the cache map or from disk if it has not
	// yet been loaded).
	//

	NWScriptReaderPtr
	LoadScript(
		nwn2dev__in const char * ScriptName,
		nwn2dev__out NWScriptJITLib::Program::Ptr & JITProgram
		);

	//
	// Return an OBJECTID from script.
	//

	inline
	void
	StackPushObjectId(
		nwn2dev__in NWScriptStack & VMStack,
		nwn2dev__in NWN::OBJECTID ObjectId
		)
	{
		VMStack.StackPushObjectId( ObjectId & ~NWN::LISTTYPE_MASK );
	}

	//
	// Retrieve an OBJECTID from script.
	//

	inline
	NWN::OBJECTID
	StackPopObjectId(
		nwn2dev__in NWScriptStack & VMStack
		)
	{
		NWN::OBJECTID ObjectId;

		ObjectId = VMStack.StackPopObjectId( );

		if (ObjectId == NWN::INVALIDOBJID)
			return ObjectId;
		else
			return ObjectId | NWN::LISTTYPE_MASK;
	}

	//
	// Return the object id of the current action object.
	//

	NWN::OBJECTID
	GetCurrentActionObjectId(
		);

	//
	// Insert a deferred script situation into the defer action list.
	//

	void
	CreateDeferredScriptSituation(
		nwn2dev__in NWScriptVM & ScriptVM,
		nwn2dev__in NWN::OBJECTID ObjectId,
		nwn2dev__in ULONG DuePeriod
		);

	//
	// Deferred script situation execution timer.
	//

	bool
	OnDeferredScriptSituationTimer(
		nwn2dev__in DeferredScriptSituation * Situation
		);

	static
	bool
	__stdcall
	OnDeferredScriptSituationTimerThunk(
		nwn2dev__in void * Context1,
		nwn2dev__in void * Context2,
		nwn2dev__in swutil::TimerRegistration * Timer
		);

	//
	// Define subsystem backlinks.
	//

	ResourceManager                & m_ResourceManager;
	swutil::TimerManager           & m_TimerManager;
	IDebugTextOut                  * m_TextOut;
	const AppParameters            * m_AppParams;

	//
	// Define the script VM itself.
	//

	NWScriptVMPtr                    m_VM;

	//
	// Define the JIT system's VM stack, used for action service handler
	// dispatches.
	//

	NWScriptStackPtr                 m_JITStack;

	//
	// Define the JIT engine instance, used to invoke scripts via JIT.
	//

	NWScriptJITLibPtr                m_JITEngine;

	//
	// Define the managed support object, which optionally enables support for
	// scripts authored in managed code.
	//

	NWScriptJITManagedSupportPtr     m_JITManagedSupport;

	//
	// Define whether the last executed action originated from the JIT (or the
	// VM).
	//

	bool                             m_LastActionFromJIT;

	//
	// Define the internal abort flag for scripts.  This is used on fast action
	// service calls, which only check the return value of the dispatcher stub,
	// for performance.
	//

	bool                             m_JITScriptAborted;

	//
	// Define the script cache.  All script entries executed are placed into
	// the cache.
	//

	ScriptCacheMap                   m_ScriptCache;

	//
	// Define the deferred script situation list, where AssignCommand and
	// DelayCommand script situations are stored.  Any script situations here
	// have live timers.
	//

	DeferredScriptSitList            m_DeferredSituations;

	//
	// Define the list of script situations that are ready to be pended.  Any
	// script situations here don't have started timers (yet).
	//

	DeferredScriptSitList            m_PendingDeferredSituations;

	//
	// Define the currently executing script, which may only be referenced from
	// action handlers.
	//

	NWScriptReaderPtr                m_CurrentScript;

	//
	// Define the currently executing script JIT program, which may only be
	// referenced from action handlers.
	//

	NWScriptJITLib::Program::Ptr     m_CurrentJITProgram;

	//
	// Define the current self object, which may only be referenced from
	// action handlers.
	//

	NWN::OBJECTID                    m_CurrentSelfObjectId;

	//
	// Define the action handler table, which is dispatched by the core
	// OnExecuteAction routine.
	//

	NWScriptActionEntry              m_ActionHandlerTable[ MAX_ACTION_ID ];

};

//
// Define each of the engine structures.
//

//
// The EngEffect object represents an effect that can be (or is) applied to an
// object.
//

class EngEffect : public EngineStructure
{

public:

	inline
	EngEffect(
		)
	: EngineStructure( NWScriptHost::EngTypeEffect ),
	  m_EffectType( ULONG_MAX )
	{
	}
	
	inline
	virtual
	~EngEffect(
		)
	{
	}

	inline
	bool
	GetIsEffectValid(
		) const
	{
		return (m_EffectType != ULONG_MAX);
	}

	inline
	bool
	CompareEngineStructure(
		nwn2dev__in const EngineStructure * Other
		) const
	{
		const EngEffect * Eff = (const EngEffect *) Other;

		return (m_EffectType == Eff->m_EffectType);
	}

private:

	ULONG m_EffectType;

};

typedef swutil::SharedPtr< EngEffect > EngEffectPtr;

#ifdef NWSCRIPTHOST_INTERNAL

//
// Turn off the unreferenced parameter warning so that we can make changes to
// the global action prototype for OnAction_* without tripping warnings up all
// over.
//

#ifdef _MSC_VER
#pragma warning(disable:4100) // warning C4100:unreferenced formal argument
#endif

//
// This macro defines an NWScript implementation routine.
//

#define SCRIPT_ACTION( Name )                                  \
	void                                                       \
	NWScriptHost::OnAction_##Name(                             \
	    nwn2dev__in NWScriptVM & ScriptVM,                            \
	    nwn2dev__in NWScriptStack & VMStack,                          \
	    nwn2dev__in NWSCRIPT_ACTION ActionId,                         \
	    nwn2dev__in size_t NumArguments                               \
	    )                                                      
#endif


#endif

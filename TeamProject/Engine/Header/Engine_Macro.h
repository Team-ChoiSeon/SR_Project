#pragma once
#ifndef Engine_Macro_h__
#define Engine_Macro_h__

namespace Engine
{
#define WINCX 800
#define WINCY 600

#define	VTXCNTX		129
#define	VTXCNTZ		129
#define VTXITV		1


inline void ShowMsgBox(const std::string& msg) {
	std::wstring wmsg(msg.begin(), msg.end());
	MessageBoxW(NULL, wmsg.c_str(), L"System Message", MB_OK);
}

#ifndef MSG_BOX
#define MSG_BOX(msg) ShowMsgBox(msg)
#endif

#define			BEGIN(NAMESPACE)		namespace NAMESPACE {
#define			END						}

#define			USING(NAMESPACE)	using namespace NAMESPACE;

#ifdef ENGINE_EXPORTS
#define ENGINE_DLL _declspec(dllexport)
#else
#define ENGINE_DLL _declspec(dllimport)
#endif

#define NO_COPY(CLASSNAME)																	\
	private : 																												\
		CLASSNAME(const CLASSNAME&) = delete;									\
		CLASSNAME& operator =(const CLASSNAME&) = delete;		

#define DECLARE_SINGLETON(CLASSNAME)	\
	NO_COPY(CLASSNAME)									\
private:																					\
	static CLASSNAME* m_pInstance;						\
public:																					\
	static CLASSNAME* Get_Instance(void);			\
	static void Destroy_Instance(void);						

	
#define NO_COPY(CLASSNAME)								\
		private:										\
		CLASSNAME(const CLASSNAME&) = delete;			\
		CLASSNAME& operator = (const CLASSNAME&)= delete;		

#define DECLARE_SINGLETON(CLASSNAME)					\
		NO_COPY(CLASSNAME)								\
		private:										\
		static CLASSNAME*	m_pInstance;				\
		public:											\
		static CLASSNAME*	Get_Instance( void );		\
		static void Destroy_Instance( void );			

#define IMPLEMENT_SINGLETON(CLASSNAME)					\
	CLASSNAME* CLASSNAME::m_pInstance = nullptr;		\
	CLASSNAME* CLASSNAME::Get_Instance(void)				   \
	{																											   \
		if (nullptr == m_pInstance) {													 \
			m_pInstance = new CLASSNAME;								   \
		}																										   \
		return m_pInstance;																	  \
	}																											   \
																													\
	void CLASSNAME::Destroy_Instance(void)							\
	{																											   \
		if (nullptr != m_pInstance) {													  \
			delete m_pInstance;																 \
			m_pInstance = nullptr;															\
		}																											  \
	}		
}

#endif // Engine_Macro_h__
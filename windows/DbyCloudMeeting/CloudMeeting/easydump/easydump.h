#pragma once
 
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
 
namespace cpp4j {
	typedef struct _EXCEPTION_POINTERS EXCEPTION_POINTERS, *PEXCEPTION_POINTERS;
 
	int __cdecl RecordExceptionInfo(PEXCEPTION_POINTERS pExceptPtrs, const TCHAR *szDumpNamePrefix);
	TCHAR *lstrrchr(LPCTSTR string, int ch);
	void DumpMiniDump(HANDLE hFile, PEXCEPTION_POINTERS excpInfo);
}
 
//#define WINMAIN_BEGIN(szDumpNamePrefix) \
//    int __96A9695E_RUN_WINMAIN_FUNC(HINSTANCE hInstance, LPTSTR lpCmdLine);\
//    LONG WINAPI __96A9695E_UnhandledExceptionHandler( _EXCEPTION_POINTERS *pExceptionInfo ) \
//    { \
//        OutputDebugString(TEXT("Create a dump file sine an exception occurred in sub-thread.\n")); \
//        int iRet = cpp4j::RecordExceptionInfo(pExceptionInfo, szDumpNamePrefix); \
//        return iRet; \
//    } \
//    int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) \
//    { \
//        UNREFERENCED_PARAMETER(hPrevInstance); \
//        UNREFERENCED_PARAMETER(nCmdShow); \
//        ::SetUnhandledExceptionFilter( __96A9695E_UnhandledExceptionHandler );\
//        int ret = 0;\
//        __try\
//        {\
//            ret = __96A9695E_RUN_WINMAIN_FUNC(hInstance, lpCmdLine);\
//        }\
//        __except(cpp4j::RecordExceptionInfo(GetExceptionInformation(), szDumpNamePrefix))\
//        {\
//            OutputDebugString(TEXT("Create a dump file sine an exception occurred in main-thread.\n")); \
//        }\
//        return ret;\
//    }\
//    int __96A9695E_RUN_WINMAIN_FUNC(HINSTANCE hInstance, LPTSTR lpCmdLine) \
//    {
// 
//#define WINMAIN_END }
 
 
 
 
//#define MAIN_BEGIN(szDumpName) \
//    int __96A9695E_RUN_MAIN_FUNC(int argc, _TCHAR* argv[]);\
//    LONG WINAPI __96A9695E_UnhandledExceptionHandler( _EXCEPTION_POINTERS *pExceptionInfo ) \
//    { \
//        OutputDebugString(TEXT("Create a dump file since an exception occurred in sub-thread.\n")); \
//        int iRet = cpp4j::RecordExceptionInfo(pExceptionInfo, szDumpName); \
//        return iRet; \
//    } \
//    int _tmain(int argc, _TCHAR* argv[])\
//    { \
//        ::SetUnhandledExceptionFilter( __96A9695E_UnhandledExceptionHandler );\
//        int ret = 0;\
//        __try\
//        {\
//            ret = __96A9695E_RUN_MAIN_FUNC(argc, argv);\
//        }\
//        __except(cpp4j::RecordExceptionInfo(GetExceptionInformation(), szDumpName))\
//        {\
//            OutputDebugString(TEXT("Create a dump file since an exception occurred in main-thread.\n")); \
//        }\
//        return ret;\
//    }\
//    int __96A9695E_RUN_MAIN_FUNC(int argc, _TCHAR* argv[]) \
//    {
// 
//#define MAIN_END }

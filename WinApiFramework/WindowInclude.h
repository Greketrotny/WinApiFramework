#ifndef WINDOW_INCLUDE_H
#define WINDOW_INCLUDE_H

//#define _WIN32_WINNT 0x0601
//#include <sdkddkver.h>

//#define WIN32_LEAN_AND_MEAN
//#define NOGDICAPMASKS
//#define NOSYSMETRICS
//#define NOMENUS
//#define NOSYSCOMMANDS
//#define NORASTEROPS
//#define OEMRESOURCE
//#define NOATOM
//#define NOCLIPBOARD
//#define NOCTLMGR
//#define NODRAWTEXT
//#define NOKERNEL
//#define NONLS
//#define NOMEMMGR
//#define NOMETAFILE
#define NOMINMAX // for testing
//#define NOOPENFILE
//#define NOSERVICE
//#define NOTEXTMETRIC
//#define NOWH
//#define NOCOMM
//#define NOKANJI
//#define NOMCX
//#define NORPC
//#define NOPROXYSTUB
//#define NOTAPE

#define UNICODE

#include <Windows.h>
#include <windowsx.h>

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#include <CommCtrl.h>


#endif // !WINDOW_INCLUDE_H
#ifndef WINDOW_INCLUDE_H
#define WINDOW_INCLUDE_H

//#define _WIN32_WINNT 0x0601
//#include <sdkddkver.h>

//#define WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
//#define NOVIRTUALKEYCODES
//#define NOWINMESSAGES
//#define NOWINSTYLES
//#define NOSYSMETRICS
#define NOMENUS
//#define NOICONS
//#define NOKEYSTATES
//#define NOSYSCOMMANDS
#define NORASTEROPS
//#define NOSHOWWINDOW
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
//#define NOCOLOR
//#define NOCTLMGR
//#define NODRAWTEXT
//#define NOGDI
//#define NOKERNEL
//#define NOUSER
//#define NONLS
//#define NOMB
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
//#define NOMSG
//#define NOOPENFILE
//#define NOSCROLL
#define NOSERVICE
#define NOSOUND
//#define NOTEXTMETRIC
//#define NOWH
//#define NOWINOFFSETS
#define NOCOMM
#define NOKANJI
#define NOHELP
//#define NOPROFILER
//#define NODEFERWINDOWPOS
#define NOMCX

#define UNICODE

#include <Windows.h>
#include <windowsx.h>

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#include <CommCtrl.h>


#endif // !WINDOW_INCLUDE_H
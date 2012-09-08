/*
 * Name  : DeclSpec
 * Path  : 
 * Use   : Export symbols into DLL
 * Author: Sven Schmidt
 * Date  : 08/12/2012
 */
#pragma once

// LINALG_EXPORTS is defined by the compiler in this project only
#ifdef COMMON_EXPORTS
#define COMMON_DECL_SYMBOLS __declspec(dllexport)
//#define EXPIMP_TEMPLATE
#else
#define COMMON_DECL_SYMBOLS __declspec(dllimport)
//#define EXPIMP_TEMPLATE extern
#endif

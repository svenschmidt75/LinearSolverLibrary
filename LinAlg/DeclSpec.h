/*
 * Name  : DeclSpec
 * Path  : 
 * Use   : Export symbols into DLL
 * Author: Sven Schmidt
 * Date  : 07/28/2012
 */
#pragma once

// LINALG_EXPORTS is defined by the compiler in this project only
#ifdef LINALG_EXPORTS
#define DECL_SYMBOLS __declspec(dllexport)
//#define EXPIMP_TEMPLATE
#else
#define DECL_SYMBOLS __declspec(dllimport)
//#define EXPIMP_TEMPLATE extern
#endif

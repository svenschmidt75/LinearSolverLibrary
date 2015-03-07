/*
 * Name  : DeclSpec
 * Path  : 
 * Use   : Export symbols into DLL
 * Author: Sven Schmidt
 * Date  : 08/12/2012
 */
#pragma once

// LINALG_EXPORTS is defined by the compiler in this project only
#ifdef LINEARSOLVERLIBRARY_EXPORTS
#define DECL_SYMBOLS __declspec(dllexport)
#else
#define DECL_SYMBOLS __declspec(dllimport)
#endif

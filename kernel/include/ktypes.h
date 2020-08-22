#pragma once
#include <windef.h>
#include "minwindef.h"
#include <minwinbase.h>

#ifdef _MSC_VER
	typedef __int64	int64;
	typedef unsigned __int64 uint64;
	#define PACKED
	#define NORETURN
#endif

typedef int object_id;
typedef unsigned long mode_t;

#define OS_NAME_LENGTH 20
#define INFINITE_TIMEOUT (((int64) 0x7fffffff << 32) | (int64) 0xffffffff)

#define PAGE_SIZE 0x1000
#define INVALID_PAGE 0xffffffff

// Page protection
typedef uint PageProtection;

#define USER_READ 1
#define USER_WRITE 2
#define USER_EXEC 4
#define SYSTEM_READ 8
#define SYSTEM_WRITE 16
#define SYSTEM_EXEC 32


// Create area flags
#define SEARCH_FROM_TOP 0
#define SEARCH_FROM_BOTTOM 2
#define EXACT_ADDRESS 4

// Map file flags
#define MAP_PRIVATE 8
#define MAP_PHYSMEM 16

typedef enum AreaWiring 
{
	AREA_NOT_WIRED,
	AREA_WIRED
} AreaWiring;


typedef enum StatType 
{
	ST_DIRECTORY,
	ST_FILE
} StatType;

typedef int (*THREAD_START_ENTRY)(void*);

typedef enum ResourceType 
{
	OBJ_ANY = -1,
	OBJ_SEMAPHORE,
	OBJ_TEAM,
	OBJ_THREAD,
	OBJ_AREA,
	OBJ_FD,
	OBJ_IMAGE,
	OBJ_MUTEX,
	OBJ_SPINLOCK,
	OBJ_TIMER,
} ResourceType;

// Wait flags
typedef enum WaitFlags 
{
	WAIT_FOR_ONE = 0,
	WAIT_FOR_ALL = 1
} WaitFlags;

// Create file flags
#define CREATE_FILE 1

/* flags for open() */
#define O_RDONLY		0	/* read only */
#define O_WRONLY		1	/* write only */
#define O_RDWR			2	/* read and write */
#define O_RWMASK		3	/* Mask to get open mode */

#define O_CLOEXEC		0x0040	/* close fd on exec */
#define	O_NONBLOCK		0x0080	/* non blocking io */
#define	O_EXCL			0x0100	/* exclusive creat */
#define O_CREAT			0x0200	/* create and open file */
#define O_TRUNC			0x0400	/* open with truncation */
#define O_APPEND		0x0800	/* to end of file */
#define O_NOCTTY    	0x1000  /* currently unsupported */
#define	O_NOTRAVERSE	0x2000	/* do not traverse leaf link */
#define O_ACCMODE   	0x0003  /* currently unsupported */
#define O_TEXT			0x4000	/* CR-LF translation	*/
#define O_BINARY		0x8000	/* no translation	*/

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2


/* Error codes */
#define E_NO_ERROR 0
#define E_ERROR -1
#define E_TIMED_OUT -2
#define E_BAD_HANDLE -3
#define E_BAD_OBJECT_TYPE -4
#define E_NOT_ALLOWED -5
#define E_BAD_ADDRESS -6
#define E_WAIT_COLLISION -7
#define E_IO -8
#define E_NO_SUCH_FILE -9
#define E_NOT_DIR -10
#define E_INVALID_OPERATION -11
#define E_ENTRY_EXISTS -12
#define E_NO_MEMORY -13
#define E_NOT_IMAGE -14
#define E_INTERRUPTED -15
#define E_WOULD_BLOCK -16




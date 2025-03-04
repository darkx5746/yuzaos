/*
** 2003 April 6
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** This file contains code used to implement the PRAGMA command.
*/




#include "sqliteInt.h"

#if !defined(SQLITE_ENABLE_LOCKING_STYLE)
#  if defined(__APPLE__)
#    define SQLITE_ENABLE_LOCKING_STYLE 1
#  else
#    define SQLITE_ENABLE_LOCKING_STYLE 0
#  endif
#endif

/***************************************************************************
** The next block of code, including the PragTyp_XXXX macro definitions and
** the aPragmaName[] object is composed of generated code. DO NOT EDIT.
**
** To add new pragmas, edit the code in ../tool/mkpragmatab.tcl and rerun
** that script.  Then copy/paste the output in place of the following:
*/
#define PragTyp_HEADER_VALUE                   0
#define PragTyp_AUTO_VACUUM                    1
#define PragTyp_FLAG                           2
#define PragTyp_BUSY_TIMEOUT                   3
#define PragTyp_CACHE_SIZE                     4
#define PragTyp_CASE_SENSITIVE_LIKE            5
#define PragTyp_COLLATION_LIST                 6
#define PragTyp_COMPILE_OPTIONS                7
#define PragTyp_DATA_STORE_DIRECTORY           8
#define PragTyp_DATABASE_LIST                  9
#define PragTyp_DEFAULT_CACHE_SIZE            10
#define PragTyp_ENCODING                      11
#define PragTyp_FOREIGN_KEY_CHECK             12
#define PragTyp_FOREIGN_KEY_LIST              13
#define PragTyp_INCREMENTAL_VACUUM            14
#define PragTyp_INDEX_INFO                    15
#define PragTyp_INDEX_LIST                    16
#define PragTyp_INTEGRITY_CHECK               17
#define PragTyp_JOURNAL_MODE                  18
#define PragTyp_JOURNAL_SIZE_LIMIT            19
#define PragTyp_LOCK_PROXY_FILE               20
#define PragTyp_LOCKING_MODE                  21
#define PragTyp_PAGE_COUNT                    22
#define PragTyp_MMAP_SIZE                     23
#define PragTyp_PAGE_SIZE                     24
#define PragTyp_SECURE_DELETE                 25
#define PragTyp_SHRINK_MEMORY                 26
#define PragTyp_SOFT_HEAP_LIMIT               27
#define PragTyp_STATS                         28
#define PragTyp_SYNCHRONOUS                   29
#define PragTyp_TABLE_INFO                    30
#define PragTyp_TEMP_STORE                    31
#define PragTyp_TEMP_STORE_DIRECTORY          32
#define PragTyp_WAL_AUTOCHECKPOINT            33
#define PragTyp_WAL_CHECKPOINT                34
#define PragTyp_ACTIVATE_EXTENSIONS           35
#define PragTyp_HEXKEY                        36
#define PragTyp_KEY                           37
#define PragTyp_REKEY                         38
#define PragTyp_LOCK_STATUS                   39
#define PragTyp_PARSER_TRACE                  40
#define PragFlag_NeedSchema           0x01
static const struct sPragmaNames {
  const char *const zName;  /* Name of pragma */
  u8 ePragTyp;              /* PragTyp_XXX value */
  u8 mPragFlag;             /* Zero or more PragFlag_XXX values */
  u32 iArg;                 /* Extra argument */
} aPragmaNames[] = {
#if defined(SQLITE_HAS_CODEC) || defined(SQLITE_ENABLE_CEROD)
  { /* zName:     */ "activate_extensions",
    /* ePragTyp:  */ PragTyp_ACTIVATE_EXTENSIONS,
    /* ePragFlag: */ 0,
    /* iArg:      */ 0 },
#endif
#if !defined(SQLITE_OMIT_SCHEMA_VERSION_PRAGMAS)
  { /* zName:     */ "application_id",
    /* ePragTyp:  */ PragTyp_HEADER_VALUE,
    /* ePragFlag: */ 0,
    /* iArg:      */ 0 },
#endif
#if !defined(SQLITE_OMIT_AUTOVACUUM)
  { /* zName:     */ "auto_vacuum",
    /* ePragTyp:  */ PragTyp_AUTO_VACUUM,
    /* ePragFlag: */ PragFlag_NeedSchema,
    /* iArg:      */ 0 },
#endif
#if !defined(SQLITE_OMIT_FLAG_PRAGMAS)
#if !defined(SQLITE_OMIT_AUTOMATIC_INDEX)
  { /* zName:     */ "automatic_index",
    /* ePragTyp:  */ PragTyp_FLAG,
    /* ePragFlag: */ 0,
    /* iArg:      */ SQLITE_AutoIndex },
#endif
#endif
  { /* zName:     */ "busy_timeout",
    /* ePragTyp:  */ PragTyp_BUSY_TIMEOUT,
    /* ePragFlag: */ 0,
    /* iArg:      */ 0 },
#if !defined(SQLITE_OMIT_PAGER_PRAGMAS)
  { /* zName:     */ "cache_size",
    /* ePragTyp:  */ PragTyp_CACHE_SIZE,
    /* ePragFlag: */ PragFlag_NeedSchema,
    /* iArg:      */ 0 },
#endif
#if !defined(SQLITE_OMIT_FLAG_PRAGMAS)
  { /* zName:     */ "cache_spill",
    /* ePragTyp:  */ PragTyp_FLAG,
    /* ePragFlag: */ 0,
    /* iArg:      */ SQLITE_CacheSpill },
#endif
  { /* zName:     */ "case_sensitive_like",
    /* ePragTyp:  */ PragTyp_CASE_SENSITIVE_LIKE,
    /* ePragFlag: */ 0,
    /* iArg:      */ 0 },
#if !defined(SQLITE_OMIT_FLAG_PRAGMAS)
  { /* zName:     */ "checkpoint_fullfsync",
    /* ePragTyp:  */ PragTyp_FLAG,
    /* ePragFlag: */ 0,
    /* iArg:      */ SQLITE_CkptFullFSync },
#endif
#if !defined(SQLITE_OMIT_SCHEMA_PRAGMAS)
  { /* zName:     */ "collation_list",
    /* ePragTyp:  */ PragTyp_COLLATION_LIST,
    /* ePragFlag: */ 0,
    /* iArg:      */ 0 },
#endif
#if !defined(SQLITE_OMIT_COMPILEOPTION_DIAGS)
  { /* zName:     */ "compile_options",
    /* ePragTyp:  */ PragTyp_COMPILE_OPTIONS,
    /* ePragFlag: */ 0,
    /* iArg:      */ 0 },
#endif
#if !defined(SQLITE_OMIT_FLAG_PRAGMAS)
  { /* zName:     */ "count_changes",
    /* ePragTyp:  */ PragTyp_FLAG,
    /* ePragFlag: */ 0,
    /* iArg:      */ SQLITE_CountRows },
#endif
#if !defined(SQLITE_OMIT_PAGER_PRAGMAS) && SQLITE_OS_WIN
  { /* zName:     */ "data_store_directory",
    /* ePragTyp:  */ PragTyp_DATA_STORE_DIRECTORY,
    /* ePragFlag: */ 0,
    /* iArg:      */ 0 },
#endif
#if !defined(SQLITE_OMIT_SCHEMA_PRAGMAS)
  { /* zName:     */ "database_list",
    /* ePragTyp:  */ PragTyp_DATABASE_LIST,
    /* ePragFlag: */ PragFlag_NeedSchema,
    /* iArg:      */ 0 },
#endif
#if !defined(SQLITE_OMIT_PAGER_PRAGMAS) && !defined(SQLITE_OMIT_DEPRECATED)
  { /* zName:     */ "default_cache_size",
    /* ePragTyp:  */ PragTyp_DEFAULT_CACHE_SIZE,
    /* ePragFlag: */ PragFlag_NeedSchema,
    /* iArg:      */ 0 },
#endif
#if !defined(SQLITE_OMIT_FLAG_PRAGMAS)
#if !defined(SQLITE_OMIT_FOREIGN_KEY) && !defined(SQLITE_OMIT_TRIGGER)
  { /* zName:     */ "defer_foreign_keys",
    /* ePragTyp:  */ PragTyp_FLAG,
    /* ePragFlag: */ 0,
    /* iArg:      */ SQLITE_DeferFKs },
#endif
#endif
#if !defined(SQLITE_OMIT_FLAG_PRAGMAS)
  { /* zName:     */ "empty_result_callbacks",
    /* ePragTyp:  */ PragTyp_FLAG,
    /* ePragFlag: */ 0,
    /* iArg:      */ SQLITE_NullCallback },
#endif
#if !defined(SQLITE_OMIT_UTF16)
  { /* zName:     */ "encoding",
    /* ePragTyp:  */ PragTyp_ENCODING,
    /* ePragFlag: */ 0,
    /* iArg:      */ 0 },
#endif
#if !defined(SQLITE_OMIT_FOREIGN_KEY) && !defined(SQLITE_OMIT_TRIGGER)
  { /* zName:     */ "foreign_key_check",
    /* ePragTyp:  */ PragTyp_FOREIGN_KEY_CHECK,
    /* ePragFlag: */ PragFlag_NeedSchema,
    /* iArg:      */ 0 },
#endif
#if !defined(SQLITE_OMIT_FOREIGN_KEY)
  { /* zName:     */ "foreign_key_list",
    /* ePragTyp:  */ PragTyp_FOREIGN_KEY_LIST,
    /* ePragFlag: */ PragFlag_NeedSchema,
    /* iArg:      */ 0 },
#endif
#if !defined(SQLITE_OMIT_FLAG_PRAGMAS)
#if !defined(SQLITE_OMIT_FOREIGN_KEY) && !defined(SQLITE_OMIT_TRIGGER)
  { /* zName:     */ "foreign_keys",
    /* ePragTyp:  */ PragTyp_FLAG,
    /* ePragFlag: */ 0,
    /* iArg:      */ SQLITE_ForeignKeys },
#endif
#endif
#if !defined(SQLITE_OMIT_SCHEMA_VERSION_PRAGMAS)
  { /* zName:     */ "freelist_count",
    /* ePragTyp:  */ PragTyp_HEADER_VALUE,
    /* ePragFlag: */ 0,
    /* iArg:      */ 0 },
#endif
#if !defined(SQLITE_OMIT_FLAG_PRAGMAS)
  { /* zName:     */ "full_column_names",
    /* ePragTyp:  */ PragTyp_FLAG,
    /* ePragFlag: */ 0,
    /* iArg:      */ SQLITE_FullColNames },
  { /* zName:     */ "fullfsync",
    /* ePragTyp:  */ PragTyp_FLAG,
    /* ePragFlag: */ 0,
    /* iArg:      */ SQLITE_FullFSync },
#endif
#if defined(SQLITE_HAS_CODEC)
  { /* zName:     */ "hexkey",
    /* ePragTyp:  */ PragTyp_HEXKEY,
    /* ePragFlag: */ 0,
    /* iArg:      */ 0 },
  { /* zName:     */ "hexrekey",
    /* ePragTyp:  */ PragTyp_HEXKEY,
    /* ePragFlag: */ 0,
    /* iArg:      */ 0 },
#endif
#if !defined(SQLITE_OMIT_FLAG_PRAGMAS)
#if !defined(SQLITE_OMIT_CHECK)
  { /* zName:     */ "ignore_check_constraints",
    /* ePragTyp:  */ PragTyp_FLAG,
    /* ePragFlag: */ 0,
    /* iArg:      */ SQLITE_IgnoreChecks },
#endif
#endif
#if !defined(SQLITE_OMIT_AUTOVACUUM)
  { /* zName:     */ "incremental_vacuum",
    /* ePragTyp:  */ PragTyp_INCREMENTAL_VACUUM,
    /* ePragFlag: */ PragFlag_NeedSchema,
    /* iArg:      */ 0 },
#endif
#if !defined(SQLITE_OMIT_SCHEMA_PRAGMAS)
  { /* zName:     */ "index_info",
    /* ePragTyp:  */ PragTyp_INDEX_INFO,
    /* ePragFlag: */ PragFlag_NeedSchema,
    /* iArg:      */ 0 },
  { /* zName:     */ "index_list",
    /* ePragTyp:  */ PragTyp_INDEX_LIST,
    /* ePragFlag: */ PragFlag_NeedSchema,
    /* iArg:      */ 0 },
#endif
#if !defined(SQLITE_OMIT_INTEGRITY_CHECK)
  { /* zName:     */ "integrity_check",
    /* ePragTyp:  */ PragTyp_INTEGRITY_CHECK,
    /* ePragFlag: */ PragFlag_NeedSchema,
    /* iArg:      */ 0 },
#endif
#if !defined(SQLITE_OMIT_PAGER_PRAGMAS)
  { /* zName:     */ "journal_mode",
    /* ePragTyp:  */ PragTyp_JOURNAL_MODE,
    /* ePragFlag: */ PragFlag_NeedSchema,
    /* iArg:      */ 0 },
  { /* zName:     */ "journal_size_limit",
    /* ePragTyp:  */ PragTyp_JOURNAL_SIZE_LIMIT,
    /* ePragFlag: */ 0,
    /* iArg:      */ 0 },
#endif
#if defined(SQLITE_HAS_CODEC)
  { /* zName:     */ "key",
    /* ePragTyp:  */ PragTyp_KEY,
    /* ePragFlag: */ 0,
    /* iArg:      */ 0 },
#endif
#if !defined(SQLITE_OMIT_FLAG_PRAGMAS)
  { /* zName:     */ "legacy_file_format",
    /* ePragTyp:  */ PragTyp_FLAG,
    /* ePragFlag: */ 0,
    /* iArg:      */ SQLITE_LegacyFileFmt },
#endif
#if !defined(SQLITE_OMIT_PAGER_PRAGMAS) && SQLITE_ENABLE_LOCKING_STYLE
  { /* zName:     */ "lock_proxy_file",
    /* ePragTyp:  */ PragTyp_LOCK_PROXY_FILE,
    /* ePragFlag: */ 0,
    /* iArg:      */ 0 },
#endif
#if defined(SQLITE_DEBUG) || defined(SQLITE_TEST)
  { /* zName:     */ "lock_status",
    /* ePragTyp:  */ PragTyp_LOCK_STATUS,
    /* ePragFlag: */ 0,
    /* iArg:      */ 0 },
#endif
#if !defined(SQLITE_OMIT_PAGER_PRAGMAS)
  { /* zName:     */ "locking_mode",
    /* ePragTyp:  */ PragTyp_LOCKING_MODE,
    /* ePragFlag: */ 0,
    /* iArg:      */ 0 },
  { /* zName:     */ "max_page_count",
    /* ePragTyp:  */ PragTyp_PAGE_COUNT,
    /* ePragFlag: */ PragFlag_NeedSchema,
    /* iArg:      */ 0 },
  { /* zName:     */ "mmap_size",
    /* ePragTyp:  */ PragTyp_MMAP_SIZE,
    /* ePragFlag: */ 0,
    /* iArg:      */ 0 },
  { /* zName:     */ "page_count",
    /* ePragTyp:  */ PragTyp_PAGE_COUNT,
    /* ePragFlag: */ PragFlag_NeedSchema,
    /* iArg:      */ 0 },
  { /* zName:     */ "page_size",
    /* ePragTyp:  */ PragTyp_PAGE_SIZE,
    /* ePragFlag: */ 0,
    /* iArg:      */ 0 },
#endif
#if defined(SQLITE_DEBUG)
  { /* zName:     */ "parser_trace",
    /* ePragTyp:  */ PragTyp_PARSER_TRACE,
    /* ePragFlag: */ 0,
    /* iArg:      */ 0 },
#endif
#if !defined(SQLITE_OMIT_FLAG_PRAGMAS)
  { /* zName:     */ "query_only",
    /* ePragTyp:  */ PragTyp_FLAG,
    /* ePragFlag: */ 0,
    /* iArg:      */ SQLITE_QueryOnly },
#endif
#if !defined(SQLITE_OMIT_INTEGRITY_CHECK)
  { /* zName:     */ "quick_check",
    /* ePragTyp:  */ PragTyp_INTEGRITY_CHECK,
    /* ePragFlag: */ PragFlag_NeedSchema,
    /* iArg:      */ 0 },
#endif
#if !defined(SQLITE_OMIT_FLAG_PRAGMAS)
  { /* zName:     */ "read_uncommitted",
    /* ePragTyp:  */ PragTyp_FLAG,
    /* ePragFlag: */ 0,
    /* iArg:      */ SQLITE_ReadUncommitted },
  { /* zName:     */ "recursive_triggers",
    /* ePragTyp:  */ PragTyp_FLAG,
    /* ePragFlag: */ 0,
    /* iArg:      */ SQLITE_RecTriggers },
#endif
#if defined(SQLITE_HAS_CODEC)
  { /* zName:     */ "rekey",
    /* ePragTyp:  */ PragTyp_REKEY,
    /* ePragFlag: */ 0,
    /* iArg:      */ 0 },
#endif
#if !defined(SQLITE_OMIT_FLAG_PRAGMAS)
  { /* zName:     */ "reverse_unordered_selects",
    /* ePragTyp:  */ PragTyp_FLAG,
    /* ePragFlag: */ 0,
    /* iArg:      */ SQLITE_ReverseOrder },
#endif
#if !defined(SQLITE_OMIT_SCHEMA_VERSION_PRAGMAS)
  { /* zName:     */ "schema_version",
    /* ePragTyp:  */ PragTyp_HEADER_VALUE,
    /* ePragFlag: */ 0,
    /* iArg:      */ 0 },
#endif
#if !defined(SQLITE_OMIT_PAGER_PRAGMAS)
  { /* zName:     */ "secure_delete",
    /* ePragTyp:  */ PragTyp_SECURE_DELETE,
    /* ePragFlag: */ 0,
    /* iArg:      */ 0 },
#endif
#if !defined(SQLITE_OMIT_FLAG_PRAGMAS)
  { /* zName:     */ "short_column_names",
    /* ePragTyp:  */ PragTyp_FLAG,
    /* ePragFlag: */ 0,
    /* iArg:      */ SQLITE_ShortColNames },
#endif
  { /* zName:     */ "shrink_memory",
    /* ePragTyp:  */ PragTyp_SHRINK_MEMORY,
    /* ePragFlag: */ 0,
    /* iArg:      */ 0 },
  { /* zName:     */ "soft_heap_limit",
    /* ePragTyp:  */ PragTyp_SOFT_HEAP_LIMIT,
    /* ePragFlag: */ 0,
    /* iArg:      */ 0 },
#if !defined(SQLITE_OMIT_FLAG_PRAGMAS)
#if defined(SQLITE_DEBUG)
  { /* zName:     */ "sql_trace",
    /* ePragTyp:  */ PragTyp_FLAG,
    /* ePragFlag: */ 0,
    /* iArg:      */ SQLITE_SqlTrace },
#endif
#endif
#if !defined(SQLITE_OMIT_SCHEMA_PRAGMAS)
  { /* zName:     */ "stats",
    /* ePragTyp:  */ PragTyp_STATS,
    /* ePragFlag: */ PragFlag_NeedSchema,
    /* iArg:      */ 0 },
#endif
#if !defined(SQLITE_OMIT_PAGER_PRAGMAS)
  { /* zName:     */ "synchronous",
    /* ePragTyp:  */ PragTyp_SYNCHRONOUS,
    /* ePragFlag: */ PragFlag_NeedSchema,
    /* iArg:      */ 0 },
#endif
#if !defined(SQLITE_OMIT_SCHEMA_PRAGMAS)
  { /* zName:     */ "table_info",
    /* ePragTyp:  */ PragTyp_TABLE_INFO,
    /* ePragFlag: */ PragFlag_NeedSchema,
    /* iArg:      */ 0 },
#endif
#if !defined(SQLITE_OMIT_PAGER_PRAGMAS)
  { /* zName:     */ "temp_store",
    /* ePragTyp:  */ PragTyp_TEMP_STORE,
    /* ePragFlag: */ 0,
    /* iArg:      */ 0 },
  { /* zName:     */ "temp_store_directory",
    /* ePragTyp:  */ PragTyp_TEMP_STORE_DIRECTORY,
    /* ePragFlag: */ 0,
    /* iArg:      */ 0 },
#endif
#if !defined(SQLITE_OMIT_SCHEMA_VERSION_PRAGMAS)
  { /* zName:     */ "user_version",
    /* ePragTyp:  */ PragTyp_HEADER_VALUE,
    /* ePragFlag: */ 0,
    /* iArg:      */ 0 },
#endif
#if !defined(SQLITE_OMIT_FLAG_PRAGMAS)
#if defined(SQLITE_DEBUG)
  { /* zName:     */ "vdbe_addoptrace",
    /* ePragTyp:  */ PragTyp_FLAG,
    /* ePragFlag: */ 0,
    /* iArg:      */ SQLITE_VdbeAddopTrace },
  { /* zName:     */ "vdbe_debug",
    /* ePragTyp:  */ PragTyp_FLAG,
    /* ePragFlag: */ 0,
    /* iArg:      */ SQLITE_SqlTrace|SQLITE_VdbeListing|SQLITE_VdbeTrace },
  { /* zName:     */ "vdbe_eqp",
    /* ePragTyp:  */ PragTyp_FLAG,
    /* ePragFlag: */ 0,
    /* iArg:      */ SQLITE_VdbeEQP },
  { /* zName:     */ "vdbe_listing",
    /* ePragTyp:  */ PragTyp_FLAG,
    /* ePragFlag: */ 0,
    /* iArg:      */ SQLITE_VdbeListing },
  { /* zName:     */ "vdbe_trace",
    /* ePragTyp:  */ PragTyp_FLAG,
    /* ePragFlag: */ 0,
    /* iArg:      */ SQLITE_VdbeTrace },
#endif
#endif
#if !defined(SQLITE_OMIT_WAL)
  { /* zName:     */ "wal_autocheckpoint",
    /* ePragTyp:  */ PragTyp_WAL_AUTOCHECKPOINT,
    /* ePragFlag: */ 0,
    /* iArg:      */ 0 },
  { /* zName:     */ "wal_checkpoint",
    /* ePragTyp:  */ PragTyp_WAL_CHECKPOINT,
    /* ePragFlag: */ PragFlag_NeedSchema,
    /* iArg:      */ 0 },
#endif
#if !defined(SQLITE_OMIT_FLAG_PRAGMAS)
  { /* zName:     */ "writable_schema",
    /* ePragTyp:  */ PragTyp_FLAG,
    /* ePragFlag: */ 0,
    /* iArg:      */ SQLITE_WriteSchema|SQLITE_RecoveryMode },
#endif
};
/* Number of pragmas: 56 on by default, 69 total. */
/* End of the automatically generated pragma table.
***************************************************************************/

/*
** Interpret the given string as a safety level.  Return 0 for OFF,
** 1 for ON or NORMAL and 2 for FULL.  Return 1 for an empty or 
** unrecognized string argument.  The FULL option is disallowed
** if the omitFull parameter it 1.
**
** Note that the values returned are one less that the values that
** should be passed into sqlite3BtreeSetSafetyLevel().  The is done
** to support legacy SQL code.  The safety level used to be boolean
** and older scripts may have used numbers 0 for OFF and 1 for ON.
*/
static u8 getSafetyLevel(const char *z, int omitFull, int dflt){
                             /* 123456789 123456789 */
  static const char zText[] = "onoffalseyestruefull";
  static const u8 iOffset[] = {0, 1, 2, 4, 9, 12, 16};
  static const u8 iLength[] = {2, 2, 3, 5, 3, 4, 4};
  static const u8 iValue[] =  {1, 0, 0, 0, 1, 1, 2};
  int i, n;
  if( sqlite3Isdigit(*z) ){
    return (u8)sqlite3Atoi(z);
  }
  n = sqlite3Strlen30(z);
  for(i=0; i<ArraySize(iLength)-omitFull; i++){
    if( iLength[i]==n && sqlite3StrNICmp(&zText[iOffset[i]],z,n)==0 ){
      return iValue[i];
    }
  }
  return dflt;
}

/*
** Interpret the given string as a boolean value.
*/
u8 sqlite3GetBoolean(const char *z, int dflt){
  return getSafetyLevel(z,1,dflt)!=0;
}

/* The sqlite3GetBoolean() function is used by other modules but the
** remainder of this file is specific to PRAGMA processing.  So omit
** the rest of the file if PRAGMAs are omitted from the build.
*/
#if !defined(SQLITE_OMIT_PRAGMA)

/*
** Interpret the given string as a locking mode value.
*/
static int getLockingMode(const char *z){
  if( z ){
    if( 0==sqlite3StrICmp(z, "exclusive") ) return PAGER_LOCKINGMODE_EXCLUSIVE;
    if( 0==sqlite3StrICmp(z, "normal") ) return PAGER_LOCKINGMODE_NORMAL;
  }
  return PAGER_LOCKINGMODE_QUERY;
}

#ifndef SQLITE_OMIT_AUTOVACUUM
/*
** Interpret the given string as an auto-vacuum mode value.
**
** The following strings, "none", "full" and "incremental" are 
** acceptable, as are their numeric equivalents: 0, 1 and 2 respectively.
*/
static int getAutoVacuum(const char *z){
  int i;
  if( 0==sqlite3StrICmp(z, "none") ) return BTREE_AUTOVACUUM_NONE;
  if( 0==sqlite3StrICmp(z, "full") ) return BTREE_AUTOVACUUM_FULL;
  if( 0==sqlite3StrICmp(z, "incremental") ) return BTREE_AUTOVACUUM_INCR;
  i = sqlite3Atoi(z);
  return (u8)((i>=0&&i<=2)?i:0);
}
#endif /* ifndef SQLITE_OMIT_AUTOVACUUM */

#ifndef SQLITE_OMIT_PAGER_PRAGMAS
/*
** Interpret the given string as a temp db location. Return 1 for file
** backed temporary databases, 2 for the Red-Black tree in memory database
** and 0 to use the compile-time default.
*/
static int getTempStore(const char *z){
  if( z[0]>='0' && z[0]<='2' ){
    return z[0] - '0';
  }else if( sqlite3StrICmp(z, "file")==0 ){
    return 1;
  }else if( sqlite3StrICmp(z, "memory")==0 ){
    return 2;
  }else{
    return 0;
  }
}
#endif /* SQLITE_PAGER_PRAGMAS */

#ifndef SQLITE_OMIT_PAGER_PRAGMAS
/*
** Invalidate temp storage, either when the temp storage is changed
** from default, or when 'file' and the temp_store_directory has changed
*/
static int invalidateTempStorage(Parse *pParse){
  sqlite3 *db = pParse->db;
  if( db->aDb[1].pBt!=0 ){
    if( !db->autoCommit || sqlite3BtreeIsInReadTrans(db->aDb[1].pBt) ){
      sqlite3ErrorMsg(pParse, "temporary storage cannot be changed "
        "from within a transaction");
      return SQLITE_ERROR;
    }
    sqlite3BtreeClose(db->aDb[1].pBt);
    db->aDb[1].pBt = 0;
    sqlite3ResetAllSchemasOfConnection(db);
  }
  return SQLITE_OK;
}
#endif /* SQLITE_PAGER_PRAGMAS */

#ifndef SQLITE_OMIT_PAGER_PRAGMAS
/*
** If the TEMP database is open, close it and mark the database schema
** as needing reloading.  This must be done when using the SQLITE_TEMP_STORE
** or DEFAULT_TEMP_STORE pragmas.
*/
static int changeTempStorage(Parse *pParse, const char *zStorageType){
  int ts = getTempStore(zStorageType);
  sqlite3 *db = pParse->db;
  if( db->temp_store==ts ) return SQLITE_OK;
  if( invalidateTempStorage( pParse ) != SQLITE_OK ){
    return SQLITE_ERROR;
  }
  db->temp_store = (u8)ts;
  return SQLITE_OK;
}
#endif /* SQLITE_PAGER_PRAGMAS */

/*
** Generate code to return a single integer value.
*/
static void returnSingleInt(Parse *pParse, const char *zLabel, i64 value){
  Vdbe *v = sqlite3GetVdbe(pParse);
  int mem = ++pParse->nMem;
  i64 *pI64 = sqlite3DbMallocRaw(pParse->db, sizeof(value));
  if( pI64 ){
    memcpy(pI64, &value, sizeof(value));
  }
  sqlite3VdbeAddOp4(v, OP_Int64, 0, mem, 0, (char*)pI64, P4_INT64);
  sqlite3VdbeSetNumCols(v, 1);
  sqlite3VdbeSetColName(v, 0, COLNAME_NAME, zLabel, SQLITE_STATIC);
  sqlite3VdbeAddOp2(v, OP_ResultRow, mem, 1);
}


/*
** Set the safety_level and pager flags for pager iDb.  Or if iDb<0
** set these values for all pagers.
*/
#ifndef SQLITE_OMIT_PAGER_PRAGMAS
static void setAllPagerFlags(sqlite3 *db){
  if( db->autoCommit ){
    Db *pDb = db->aDb;
    int n = db->nDb;
    assert( SQLITE_FullFSync==PAGER_FULLFSYNC );
    assert( SQLITE_CkptFullFSync==PAGER_CKPT_FULLFSYNC );
    assert( SQLITE_CacheSpill==PAGER_CACHESPILL );
    assert( (PAGER_FULLFSYNC | PAGER_CKPT_FULLFSYNC | PAGER_CACHESPILL)
             ==  PAGER_FLAGS_MASK );
    assert( (pDb->safety_level & PAGER_SYNCHRONOUS_MASK)==pDb->safety_level );
    while( (n--) > 0 ){
      if( pDb->pBt ){
        sqlite3BtreeSetPagerFlags(pDb->pBt,
                 pDb->safety_level | (db->flags & PAGER_FLAGS_MASK) );
      }
      pDb++;
    }
  }
}
#else
# define setAllPagerFlags(X)  /* no-op */
#endif


/*
** Return a human-readable name for a constraint resolution action.
*/
#ifndef SQLITE_OMIT_FOREIGN_KEY
static const char *actionName(u8 action){
  const char *zName;
  switch( action ){
    case OE_SetNull:  zName = "SET NULL";        break;
    case OE_SetDflt:  zName = "SET DEFAULT";     break;
    case OE_Cascade:  zName = "CASCADE";         break;
    case OE_Restrict: zName = "RESTRICT";        break;
    default:          zName = "NO ACTION";  
                      assert( action==OE_None ); break;
  }
  return zName;
}
#endif


/*
** Parameter eMode must be one of the PAGER_JOURNALMODE_XXX constants
** defined in pager.h. This function returns the associated lowercase
** journal-mode name.
*/
const char *sqlite3JournalModename(int eMode){
  static char * const azModeName[] = {
    "delete", "persist", "off", "truncate", "memory"
#ifndef SQLITE_OMIT_WAL
     , "wal"
#endif
  };
  assert( PAGER_JOURNALMODE_DELETE==0 );
  assert( PAGER_JOURNALMODE_PERSIST==1 );
  assert( PAGER_JOURNALMODE_OFF==2 );
  assert( PAGER_JOURNALMODE_TRUNCATE==3 );
  assert( PAGER_JOURNALMODE_MEMORY==4 );
  assert( PAGER_JOURNALMODE_WAL==5 );
  assert( eMode>=0 && eMode<=ArraySize(azModeName) );

  if( eMode==ArraySize(azModeName) ) return 0;
  return azModeName[eMode];
}

/*
** Process a pragma statement.  
**
** Pragmas are of this form:
**
**      PRAGMA [database.]id [= value]
**
** The identifier might also be a string.  The value is a string, and
** identifier, or a number.  If minusFlag is true, then the value is
** a number that was preceded by a minus sign.
**
** If the left side is "database.id" then pId1 is the database name
** and pId2 is the id.  If the left side is just "id" then pId1 is the
** id and pId2 is any empty string.
*/
void sqlite3Pragma(
  Parse *pParse, 
  Token *pId1,        /* First part of [database.]id field */
  Token *pId2,        /* Second part of [database.]id field, or NULL */
  Token *pValue,      /* Token for <value>, or NULL */
  int minusFlag       /* True if a '-' sign preceded <value> */
){
  char *zLeft = 0;       /* Nul-terminated UTF-8 string <id> */
  char *zRight = 0;      /* Nul-terminated UTF-8 string <value>, or NULL */
  const char *zDb = 0;   /* The database name */
  Token *pId;            /* Pointer to <id> token */
  char *aFcntl[4];       /* Argument to SQLITE_FCNTL_PRAGMA */
  int iDb;               /* Database index for <database> */
  int lwr, upr, mid;           /* Binary search bounds */
  int rc;                      /* return value form SQLITE_FCNTL_PRAGMA */
  sqlite3 *db = pParse->db;    /* The database connection */
  Db *pDb;                     /* The specific database being pragmaed */
  Vdbe *v = sqlite3GetVdbe(pParse);  /* Prepared statement */

  if( v==0 ) return;
  sqlite3VdbeRunOnlyOnce(v);
  pParse->nMem = 2;

  /* Interpret the [database.] part of the pragma statement. iDb is the
  ** index of the database this pragma is being applied to in db.aDb[]. */
  iDb = sqlite3TwoPartName(pParse, pId1, pId2, &pId);
  if( iDb<0 ) return;
  pDb = &db->aDb[iDb];

  /* If the temp database has been explicitly named as part of the 
  ** pragma, make sure it is open. 
  */
  if( iDb==1 && sqlite3OpenTempDatabase(pParse) ){
    return;
  }

  zLeft = sqlite3NameFromToken(db, pId);
  if( !zLeft ) return;
  if( minusFlag ){
    zRight = sqlite3MPrintf(db, "-%T", pValue);
  }else{
    zRight = sqlite3NameFromToken(db, pValue);
  }

  assert( pId2 );
  zDb = pId2->n>0 ? pDb->zName : 0;
  if( sqlite3AuthCheck(pParse, SQLITE_PRAGMA, zLeft, zRight, zDb) ){
    goto pragma_out;
  }

  /* Send an SQLITE_FCNTL_PRAGMA file-control to the underlying VFS
  ** connection.  If it returns SQLITE_OK, then assume that the VFS
  ** handled the pragma and generate a no-op prepared statement.
  */
  aFcntl[0] = 0;
  aFcntl[1] = zLeft;
  aFcntl[2] = zRight;
  aFcntl[3] = 0;
  db->busyHandler.nBusy = 0;
  rc = sqlite3_file_control(db, zDb, SQLITE_FCNTL_PRAGMA, (void*)aFcntl);
  if( rc==SQLITE_OK ){
    if( aFcntl[0] ){
      int mem = ++pParse->nMem;
      sqlite3VdbeAddOp4(v, OP_String8, 0, mem, 0, aFcntl[0], 0);
      sqlite3VdbeSetNumCols(v, 1);
      sqlite3VdbeSetColName(v, 0, COLNAME_NAME, "result", SQLITE_STATIC);
      sqlite3VdbeAddOp2(v, OP_ResultRow, mem, 1);
      sqlite3_free(aFcntl[0]);
    }
    goto pragma_out;
  }
  if( rc!=SQLITE_NOTFOUND ){
    if( aFcntl[0] ){
      sqlite3ErrorMsg(pParse, "%s", aFcntl[0]);
      sqlite3_free(aFcntl[0]);
    }
    pParse->nErr++;
    pParse->rc = rc;
    goto pragma_out;
  }

  /* Locate the pragma in the lookup table */
  lwr = 0;
  upr = ArraySize(aPragmaNames)-1;
  while( lwr<=upr ){
    mid = (lwr+upr)/2;
    rc = sqlite3_stricmp(zLeft, aPragmaNames[mid].zName);
    if( rc==0 ) break;
    if( rc<0 ){
      upr = mid - 1;
    }else{
      lwr = mid + 1;
    }
  }
  if( lwr>upr ) goto pragma_out;

  /* Make sure the database schema is loaded if the pragma requires that */
  if( (aPragmaNames[mid].mPragFlag & PragFlag_NeedSchema)!=0 ){
    if( sqlite3ReadSchema(pParse) ) goto pragma_out;
  }

  /* Jump to the appropriate pragma handler */
  switch( aPragmaNames[mid].ePragTyp ){
  
#if !defined(SQLITE_OMIT_PAGER_PRAGMAS) && !defined(SQLITE_OMIT_DEPRECATED)
  /*
  **  PRAGMA [database.]default_cache_size
  **  PRAGMA [database.]default_cache_size=N
  **
  ** The first form reports the current persistent setting for the
  ** page cache size.  The value returned is the maximum number of
  ** pages in the page cache.  The second form sets both the current
  ** page cache size value and the persistent page cache size value
  ** stored in the database file.
  **
  ** Older versions of SQLite would set the default cache size to a
  ** negative number to indicate synchronous=OFF.  These days, synchronous
  ** is always on by default regardless of the sign of the default cache
  ** size.  But continue to take the absolute value of the default cache
  ** size of historical compatibility.
  */
  case PragTyp_DEFAULT_CACHE_SIZE: {
    static const VdbeOpList getCacheSize[] = {
      { OP_Transaction, 0, 0,        0},                         /* 0 */
      { OP_ReadCookie,  0, 1,        BTREE_DEFAULT_CACHE_SIZE},  /* 1 */
      { OP_IfPos,       1, 8,        0},
      { OP_Integer,     0, 2,        0},
      { OP_Subtract,    1, 2,        1},
      { OP_IfPos,       1, 8,        0},
      { OP_Integer,     0, 1,        0},                         /* 6 */
      { OP_Noop,        0, 0,        0},
      { OP_ResultRow,   1, 1,        0},
    };
    int addr;
    sqlite3VdbeUsesBtree(v, iDb);
    if( !zRight ){
      sqlite3VdbeSetNumCols(v, 1);
      sqlite3VdbeSetColName(v, 0, COLNAME_NAME, "cache_size", SQLITE_STATIC);
      pParse->nMem += 2;
      addr = sqlite3VdbeAddOpList(v, ArraySize(getCacheSize), getCacheSize);
      sqlite3VdbeChangeP1(v, addr, iDb);
      sqlite3VdbeChangeP1(v, addr+1, iDb);
      sqlite3VdbeChangeP1(v, addr+6, SQLITE_DEFAULT_CACHE_SIZE);
    }else{
      int size = sqlite3AbsInt32(sqlite3Atoi(zRight));
      sqlite3BeginWriteOperation(pParse, 0, iDb);
      sqlite3VdbeAddOp2(v, OP_Integer, size, 1);
      sqlite3VdbeAddOp3(v, OP_SetCookie, iDb, BTREE_DEFAULT_CACHE_SIZE, 1);
      assert( sqlite3SchemaMutexHeld(db, iDb, 0) );
      pDb->pSchema->cache_size = size;
      sqlite3BtreeSetCacheSize(pDb->pBt, pDb->pSchema->cache_size);
    }
    break;
  }
#endif /* !SQLITE_OMIT_PAGER_PRAGMAS && !SQLITE_OMIT_DEPRECATED */

#if !defined(SQLITE_OMIT_PAGER_PRAGMAS)
  /*
  **  PRAGMA [database.]page_size
  **  PRAGMA [database.]page_size=N
  **
  ** The first form reports the current setting for the
  ** database page size in bytes.  The second form sets the
  ** database page size value.  The value can only be set if
  ** the database has not yet been created.
  */
  case PragTyp_PAGE_SIZE: {
    Btree *pBt = pDb->pBt;
    assert( pBt!=0 );
    if( !zRight ){
      int size = ALWAYS(pBt) ? sqlite3BtreeGetPageSize(pBt) : 0;
      returnSingleInt(pParse, "page_size", size);
    }else{
      /* Malloc may fail when setting the page-size, as there is an internal
      ** buffer that the pager module resizes using sqlite3_realloc().
      */
      db->nextPagesize = sqlite3Atoi(zRight);
      if( SQLITE_NOMEM==sqlite3BtreeSetPageSize(pBt, db->nextPagesize,-1,0) ){
        db->mallocFailed = 1;
      }
    }
    break;
  }

  /*
  **  PRAGMA [database.]secure_delete
  **  PRAGMA [database.]secure_delete=ON/OFF
  **
  ** The first form reports the current setting for the
  ** secure_delete flag.  The second form changes the secure_delete
  ** flag setting and reports thenew value.
  */
  case PragTyp_SECURE_DELETE: {
    Btree *pBt = pDb->pBt;
    int b = -1;
    assert( pBt!=0 );
    if( zRight ){
      b = sqlite3GetBoolean(zRight, 0);
    }
    if( pId2->n==0 && b>=0 ){
      int ii;
      for(ii=0; ii<db->nDb; ii++){
        sqlite3BtreeSecureDelete(db->aDb[ii].pBt, b);
      }
    }
    b = sqlite3BtreeSecureDelete(pBt, b);
    returnSingleInt(pParse, "secure_delete", b);
    break;
  }

  /*
  **  PRAGMA [database.]max_page_count
  **  PRAGMA [database.]max_page_count=N
  **
  ** The first form reports the current setting for the
  ** maximum number of pages in the database file.  The 
  ** second form attempts to change this setting.  Both
  ** forms return the current setting.
  **
  ** The absolute value of N is used.  This is undocumented and might
  ** change.  The only purpose is to provide an easy way to test
  ** the sqlite3AbsInt32() function.
  **
  **  PRAGMA [database.]page_count
  **
  ** Return the number of pages in the specified database.
  */
  case PragTyp_PAGE_COUNT: {
    int iReg;
    sqlite3CodeVerifySchema(pParse, iDb);
    iReg = ++pParse->nMem;
    if( sqlite3Tolower(zLeft[0])=='p' ){
      sqlite3VdbeAddOp2(v, OP_Pagecount, iDb, iReg);
    }else{
      sqlite3VdbeAddOp3(v, OP_MaxPgcnt, iDb, iReg, 
                        sqlite3AbsInt32(sqlite3Atoi(zRight)));
    }
    sqlite3VdbeAddOp2(v, OP_ResultRow, iReg, 1);
    sqlite3VdbeSetNumCols(v, 1);
    sqlite3VdbeSetColName(v, 0, COLNAME_NAME, zLeft, SQLITE_TRANSIENT);
    break;
  }

  /*
  **  PRAGMA [database.]locking_mode
  **  PRAGMA [database.]locking_mode = (normal|exclusive)
  */
  case PragTyp_LOCKING_MODE: {
    const char *zRet = "normal";
    int eMode = getLockingMode(zRight);

    if( pId2->n==0 && eMode==PAGER_LOCKINGMODE_QUERY ){
      /* Simple "PRAGMA locking_mode;" statement. This is a query for
      ** the current default locking mode (which may be different to
      ** the locking-mode of the main database).
      */
      eMode = db->dfltLockMode;
    }else{
      Pager *pPager;
      if( pId2->n==0 ){
        /* This indicates that no database name was specified as part
        ** of the PRAGMA command. In this case the locking-mode must be
        ** set on all attached databases, as well as the main db file.
        **
        ** Also, the sqlite3.dfltLockMode variable is set so that
        ** any subsequently attached databases also use the specified
        ** locking mode.
        */
        int ii;
        assert(pDb==&db->aDb[0]);
        for(ii=2; ii<db->nDb; ii++){
          pPager = sqlite3BtreePager(db->aDb[ii].pBt);
          sqlite3PagerLockingMode(pPager, eMode);
        }
        db->dfltLockMode = (u8)eMode;
      }
      pPager = sqlite3BtreePager(pDb->pBt);
      eMode = sqlite3PagerLockingMode(pPager, eMode);
    }

    assert( eMode==PAGER_LOCKINGMODE_NORMAL
            || eMode==PAGER_LOCKINGMODE_EXCLUSIVE );
    if( eMode==PAGER_LOCKINGMODE_EXCLUSIVE ){
      zRet = "exclusive";
    }
    sqlite3VdbeSetNumCols(v, 1);
    sqlite3VdbeSetColName(v, 0, COLNAME_NAME, "locking_mode", SQLITE_STATIC);
    sqlite3VdbeAddOp4(v, OP_String8, 0, 1, 0, zRet, 0);
    sqlite3VdbeAddOp2(v, OP_ResultRow, 1, 1);
    break;
  }

  /*
  **  PRAGMA [database.]journal_mode
  **  PRAGMA [database.]journal_mode =
  **                      (delete|persist|off|truncate|memory|wal|off)
  */
  case PragTyp_JOURNAL_MODE: {
    int eMode;        /* One of the PAGER_JOURNALMODE_XXX symbols */
    int ii;           /* Loop counter */

    sqlite3VdbeSetNumCols(v, 1);
    sqlite3VdbeSetColName(v, 0, COLNAME_NAME, "journal_mode", SQLITE_STATIC);

    if( zRight==0 ){
      /* If there is no "=MODE" part of the pragma, do a query for the
      ** current mode */
      eMode = PAGER_JOURNALMODE_QUERY;
    }else{
      const char *zMode;
      int n = sqlite3Strlen30(zRight);
      for(eMode=0; (zMode = sqlite3JournalModename(eMode))!=0; eMode++){
        if( sqlite3StrNICmp(zRight, zMode, n)==0 ) break;
      }
      if( !zMode ){
        /* If the "=MODE" part does not match any known journal mode,
        ** then do a query */
        eMode = PAGER_JOURNALMODE_QUERY;
      }
    }
    if( eMode==PAGER_JOURNALMODE_QUERY && pId2->n==0 ){
      /* Convert "PRAGMA journal_mode" into "PRAGMA main.journal_mode" */
      iDb = 0;
      pId2->n = 1;
    }
    for(ii=db->nDb-1; ii>=0; ii--){
      if( db->aDb[ii].pBt && (ii==iDb || pId2->n==0) ){
        sqlite3VdbeUsesBtree(v, ii);
        sqlite3VdbeAddOp3(v, OP_JournalMode, ii, 1, eMode);
      }
    }
    sqlite3VdbeAddOp2(v, OP_ResultRow, 1, 1);
    break;
  }

  /*
  **  PRAGMA [database.]journal_size_limit
  **  PRAGMA [database.]journal_size_limit=N
  **
  ** Get or set the size limit on rollback journal files.
  */
  case PragTyp_JOURNAL_SIZE_LIMIT: {
    Pager *pPager = sqlite3BtreePager(pDb->pBt);
    i64 iLimit = -2;
    if( zRight ){
      sqlite3Atoi64(zRight, &iLimit, sqlite3Strlen30(zRight), SQLITE_UTF8);
      if( iLimit<-1 ) iLimit = -1;
    }
    iLimit = sqlite3PagerJournalSizeLimit(pPager, iLimit);
    returnSingleInt(pParse, "journal_size_limit", iLimit);
    break;
  }

#endif /* SQLITE_OMIT_PAGER_PRAGMAS */

  /*
  **  PRAGMA [database.]auto_vacuum
  **  PRAGMA [database.]auto_vacuum=N
  **
  ** Get or set the value of the database 'auto-vacuum' parameter.
  ** The value is one of:  0 NONE 1 FULL 2 INCREMENTAL
  */
#ifndef SQLITE_OMIT_AUTOVACUUM
  case PragTyp_AUTO_VACUUM: {
    Btree *pBt = pDb->pBt;
    assert( pBt!=0 );
    if( !zRight ){
      returnSingleInt(pParse, "auto_vacuum", sqlite3BtreeGetAutoVacuum(pBt));
    }else{
      int eAuto = getAutoVacuum(zRight);
      assert( eAuto>=0 && eAuto<=2 );
      db->nextAutovac = (u8)eAuto;
      /* Call SetAutoVacuum() to set initialize the internal auto and
      ** incr-vacuum flags. This is required in case this connection
      ** creates the database file. It is important that it is created
      ** as an auto-vacuum capable db.
      */
      rc = sqlite3BtreeSetAutoVacuum(pBt, eAuto);
      if( rc==SQLITE_OK && (eAuto==1 || eAuto==2) ){
        /* When setting the auto_vacuum mode to either "full" or 
        ** "incremental", write the value of meta[6] in the database
        ** file. Before writing to meta[6], check that meta[3] indicates
        ** that this really is an auto-vacuum capable database.
        */
        static const VdbeOpList setMeta6[] = {
          { OP_Transaction,    0,         1,                 0},    /* 0 */
          { OP_ReadCookie,     0,         1,         BTREE_LARGEST_ROOT_PAGE},
          { OP_If,             1,         0,                 0},    /* 2 */
          { OP_Halt,           SQLITE_OK, OE_Abort,          0},    /* 3 */
          { OP_Integer,        0,         1,                 0},    /* 4 */
          { OP_SetCookie,      0,         BTREE_INCR_VACUUM, 1},    /* 5 */
        };
        int iAddr;
        iAddr = sqlite3VdbeAddOpList(v, ArraySize(setMeta6), setMeta6);
        sqlite3VdbeChangeP1(v, iAddr, iDb);
        sqlite3VdbeChangeP1(v, iAddr+1, iDb);
        sqlite3VdbeChangeP2(v, iAddr+2, iAddr+4);
        sqlite3VdbeChangeP1(v, iAddr+4, eAuto-1);
        sqlite3VdbeChangeP1(v, iAddr+5, iDb);
        sqlite3VdbeUsesBtree(v, iDb);
      }
    }
    break;
  }
#endif

  /*
  **  PRAGMA [database.]incremental_vacuum(N)
  **
  ** Do N steps of incremental vacuuming on a database.
  */
#ifndef SQLITE_OMIT_AUTOVACUUM
  case PragTyp_INCREMENTAL_VACUUM: {
    int iLimit, addr;
    if( zRight==0 || !sqlite3GetInt32(zRight, &iLimit) || iLimit<=0 ){
      iLimit = 0x7fffffff;
    }
    sqlite3BeginWriteOperation(pParse, 0, iDb);
    sqlite3VdbeAddOp2(v, OP_Integer, iLimit, 1);
    addr = sqlite3VdbeAddOp1(v, OP_IncrVacuum, iDb);
    sqlite3VdbeAddOp1(v, OP_ResultRow, 1);
    sqlite3VdbeAddOp2(v, OP_AddImm, 1, -1);
    sqlite3VdbeAddOp2(v, OP_IfPos, 1, addr);
    sqlite3VdbeJumpHere(v, addr);
    break;
  }
#endif

#ifndef SQLITE_OMIT_PAGER_PRAGMAS
  /*
  **  PRAGMA [database.]cache_size
  **  PRAGMA [database.]cache_size=N
  **
  ** The first form reports the current local setting for the
  ** page cache size. The second form sets the local
  ** page cache size value.  If N is positive then that is the
  ** number of pages in the cache.  If N is negative, then the
  ** number of pages is adjusted so that the cache uses -N kibibytes
  ** of memory.
  */
  case PragTyp_CACHE_SIZE: {
    assert( sqlite3SchemaMutexHeld(db, iDb, 0) );
    if( !zRight ){
      returnSingleInt(pParse, "cache_size", pDb->pSchema->cache_size);
    }else{
      int size = sqlite3Atoi(zRight);
      pDb->pSchema->cache_size = size;
      sqlite3BtreeSetCacheSize(pDb->pBt, pDb->pSchema->cache_size);
    }
    break;
  }

  /*
  **  PRAGMA [database.]mmap_size(N)
  **
  ** Used to set mapping size limit. The mapping size limit is
  ** used to limit the aggregate size of all memory mapped regions of the
  ** database file. If this parameter is set to zero, then memory mapping
  ** is not used at all.  If N is negative, then the default memory map
  ** limit determined by sqlite3_config(SQLITE_CONFIG_MMAP_SIZE) is set.
  ** The parameter N is measured in bytes.
  **
  ** This value is advisory.  The underlying VFS is free to memory map
  ** as little or as much as it wants.  Except, if N is set to 0 then the
  ** upper layers will never invoke the xFetch interfaces to the VFS.
  */
  case PragTyp_MMAP_SIZE: {
    sqlite3_int64 sz;
#if SQLITE_MAX_MMAP_SIZE>0
    assert( sqlite3SchemaMutexHeld(db, iDb, 0) );
    if( zRight ){
      int ii;
      sqlite3Atoi64(zRight, &sz, sqlite3Strlen30(zRight), SQLITE_UTF8);
      if( sz<0 ) sz = sqlite3GlobalConfig.szMmap;
      if( pId2->n==0 ) db->szMmap = sz;
      for(ii=db->nDb-1; ii>=0; ii--){
        if( db->aDb[ii].pBt && (ii==iDb || pId2->n==0) ){
          sqlite3BtreeSetMmapLimit(db->aDb[ii].pBt, sz);
        }
      }
    }
    sz = -1;
    rc = sqlite3_file_control(db, zDb, SQLITE_FCNTL_MMAP_SIZE, &sz);
#else
    sz = 0;
    rc = SQLITE_OK;
#endif
    if( rc==SQLITE_OK ){
      returnSingleInt(pParse, "mmap_size", sz);
    }else if( rc!=SQLITE_NOTFOUND ){
      pParse->nErr++;
      pParse->rc = rc;
    }
    break;
  }

  /*
  **   PRAGMA temp_store
  **   PRAGMA temp_store = "default"|"memory"|"file"
  **
  ** Return or set the local value of the temp_store flag.  Changing
  ** the local value does not make changes to the disk file and the default
  ** value will be restored the next time the database is opened.
  **
  ** Note that it is possible for the library compile-time options to
  ** override this setting
  */
  case PragTyp_TEMP_STORE: {
    if( !zRight ){
      returnSingleInt(pParse, "temp_store", db->temp_store);
    }else{
      changeTempStorage(pParse, zRight);
    }
    break;
  }

  /*
  **   PRAGMA temp_store_directory
  **   PRAGMA temp_store_directory = ""|"directory_name"
  **
  ** Return or set the local value of the temp_store_directory flag.  Changing
  ** the value sets a specific directory to be used for temporary files.
  ** Setting to a null string reverts to the default temporary directory search.
  ** If temporary directory is changed, then invalidateTempStorage.
  **
  */
  case PragTyp_TEMP_STORE_DIRECTORY: {
    if( !zRight ){
      if( sqlite3_temp_directory ){
        sqlite3VdbeSetNumCols(v, 1);
        sqlite3VdbeSetColName(v, 0, COLNAME_NAME, 
            "temp_store_directory", SQLITE_STATIC);
        sqlite3VdbeAddOp4(v, OP_String8, 0, 1, 0, sqlite3_temp_directory, 0);
        sqlite3VdbeAddOp2(v, OP_ResultRow, 1, 1);
      }
    }else{
#ifndef SQLITE_OMIT_WSD
      if( zRight[0] ){
        int res;
        rc = sqlite3OsAccess(db->pVfs, zRight, SQLITE_ACCESS_READWRITE, &res);
        if( rc!=SQLITE_OK || res==0 ){
          sqlite3ErrorMsg(pParse, "not a writable directory");
          goto pragma_out;
        }
      }
      if( SQLITE_TEMP_STORE==0
       || (SQLITE_TEMP_STORE==1 && db->temp_store<=1)
       || (SQLITE_TEMP_STORE==2 && db->temp_store==1)
      ){
        invalidateTempStorage(pParse);
      }
      sqlite3_free(sqlite3_temp_directory);
      if( zRight[0] ){
        sqlite3_temp_directory = sqlite3_mprintf("%s", zRight);
      }else{
        sqlite3_temp_directory = 0;
      }
#endif /* SQLITE_OMIT_WSD */
    }
    break;
  }

#if SQLITE_OS_WIN
  /*
  **   PRAGMA data_store_directory
  **   PRAGMA data_store_directory = ""|"directory_name"
  **
  ** Return or set the local value of the data_store_directory flag.  Changing
  ** the value sets a specific directory to be used for database files that
  ** were specified with a relative pathname.  Setting to a null string reverts
  ** to the default database directory, which for database files specified with
  ** a relative path will probably be based on the current directory for the
  ** process.  Database file specified with an absolute path are not impacted
  ** by this setting, regardless of its value.
  **
  */
  case PragTyp_DATA_STORE_DIRECTORY: {
    if( !zRight ){
      if( sqlite3_data_directory ){
        sqlite3VdbeSetNumCols(v, 1);
        sqlite3VdbeSetColName(v, 0, COLNAME_NAME, 
            "data_store_directory", SQLITE_STATIC);
        sqlite3VdbeAddOp4(v, OP_String8, 0, 1, 0, sqlite3_data_directory, 0);
        sqlite3VdbeAddOp2(v, OP_ResultRow, 1, 1);
      }
    }else{
#ifndef SQLITE_OMIT_WSD
      if( zRight[0] ){
        int res;
        rc = sqlite3OsAccess(db->pVfs, zRight, SQLITE_ACCESS_READWRITE, &res);
        if( rc!=SQLITE_OK || res==0 ){
          sqlite3ErrorMsg(pParse, "not a writable directory");
          goto pragma_out;
        }
      }
      sqlite3_free(sqlite3_data_directory);
      if( zRight[0] ){
        sqlite3_data_directory = sqlite3_mprintf("%s", zRight);
      }else{
        sqlite3_data_directory = 0;
      }
#endif /* SQLITE_OMIT_WSD */
    }
    break;
  }
#endif

#if SQLITE_ENABLE_LOCKING_STYLE
  /*
  **   PRAGMA [database.]lock_proxy_file
  **   PRAGMA [database.]lock_proxy_file = ":auto:"|"lock_file_path"
  **
  ** Return or set the value of the lock_proxy_file flag.  Changing
  ** the value sets a specific file to be used for database access locks.
  **
  */
  case PragTyp_LOCK_PROXY_FILE: {
    if( !zRight ){
      Pager *pPager = sqlite3BtreePager(pDb->pBt);
      char *proxy_file_path = NULL;
      sqlite3_file *pFile = sqlite3PagerFile(pPager);
      sqlite3OsFileControlHint(pFile, SQLITE_GET_LOCKPROXYFILE, 
                           &proxy_file_path);
      
      if( proxy_file_path ){
        sqlite3VdbeSetNumCols(v, 1);
        sqlite3VdbeSetColName(v, 0, COLNAME_NAME, 
                              "lock_proxy_file", SQLITE_STATIC);
        sqlite3VdbeAddOp4(v, OP_String8, 0, 1, 0, proxy_file_path, 0);
        sqlite3VdbeAddOp2(v, OP_ResultRow, 1, 1);
      }
    }else{
      Pager *pPager = sqlite3BtreePager(pDb->pBt);
      sqlite3_file *pFile = sqlite3PagerFile(pPager);
      int res;
      if( zRight[0] ){
        res=sqlite3OsFileControl(pFile, SQLITE_SET_LOCKPROXYFILE, 
                                     zRight);
      } else {
        res=sqlite3OsFileControl(pFile, SQLITE_SET_LOCKPROXYFILE, 
                                     NULL);
      }
      if( res!=SQLITE_OK ){
        sqlite3ErrorMsg(pParse, "failed to set lock proxy file");
        goto pragma_out;
      }
    }
    break;
  }
#endif /* SQLITE_ENABLE_LOCKING_STYLE */      
    
  /*
  **   PRAGMA [database.]synchronous
  **   PRAGMA [database.]synchronous=OFF|ON|NORMAL|FULL
  **
  ** Return or set the local value of the synchronous flag.  Changing
  ** the local value does not make changes to the disk file and the
  ** default value will be restored the next time the database is
  ** opened.
  */
  case PragTyp_SYNCHRONOUS: {
    if( !zRight ){
      returnSingleInt(pParse, "synchronous", pDb->safety_level-1);
    }else{
      if( !db->autoCommit ){
        sqlite3ErrorMsg(pParse, 
            "Safety level may not be changed inside a transaction");
      }else{
        pDb->safety_level = getSafetyLevel(zRight,0,1)+1;
        setAllPagerFlags(db);
      }
    }
    break;
  }
#endif /* SQLITE_OMIT_PAGER_PRAGMAS */

#ifndef SQLITE_OMIT_FLAG_PRAGMAS
  case PragTyp_FLAG: {
    if( zRight==0 ){
      returnSingleInt(pParse, aPragmaNames[mid].zName,
                     (db->flags & aPragmaNames[mid].iArg)!=0 );
    }else{
      int mask = aPragmaNames[mid].iArg;    /* Mask of bits to set or clear. */
      if( db->autoCommit==0 ){
        /* Foreign key support may not be enabled or disabled while not
        ** in auto-commit mode.  */
        mask &= ~(SQLITE_ForeignKeys);
      }

      if( sqlite3GetBoolean(zRight, 0) ){
        db->flags |= mask;
      }else{
        db->flags &= ~mask;
        if( mask==SQLITE_DeferFKs ) db->nDeferredImmCons = 0;
      }

      /* Many of the flag-pragmas modify the code generated by the SQL 
      ** compiler (eg. count_changes). So add an opcode to expire all
      ** compiled SQL statements after modifying a pragma value.
      */
      sqlite3VdbeAddOp2(v, OP_Expire, 0, 0);
      setAllPagerFlags(db);
    }
    break;
  }
#endif /* SQLITE_OMIT_FLAG_PRAGMAS */

#ifndef SQLITE_OMIT_SCHEMA_PRAGMAS
  /*
  **   PRAGMA table_info(<table>)
  **
  ** Return a single row for each column of the named table. The columns of
  ** the returned data set are:
  **
  ** cid:        Column id (numbered from left to right, starting at 0)
  ** name:       Column name
  ** type:       Column declaration type.
  ** notnull:    True if 'NOT NULL' is part of column declaration
  ** dflt_value: The default value for the column, if any.
  */
  case PragTyp_TABLE_INFO: if( zRight ){
    Table *pTab;
    pTab = sqlite3FindTable(db, zRight, zDb);
    if( pTab ){
      int i, k;
      int nHidden = 0;
      Column *pCol;
      Index *pPk = sqlite3PrimaryKeyIndex(pTab);
      sqlite3VdbeSetNumCols(v, 6);
      pParse->nMem = 6;
      sqlite3CodeVerifySchema(pParse, iDb);
      sqlite3VdbeSetColName(v, 0, COLNAME_NAME, "cid", SQLITE_STATIC);
      sqlite3VdbeSetColName(v, 1, COLNAME_NAME, "name", SQLITE_STATIC);
      sqlite3VdbeSetColName(v, 2, COLNAME_NAME, "type", SQLITE_STATIC);
      sqlite3VdbeSetColName(v, 3, COLNAME_NAME, "notnull", SQLITE_STATIC);
      sqlite3VdbeSetColName(v, 4, COLNAME_NAME, "dflt_value", SQLITE_STATIC);
      sqlite3VdbeSetColName(v, 5, COLNAME_NAME, "pk", SQLITE_STATIC);
      sqlite3ViewGetColumnNames(pParse, pTab);
      for(i=0, pCol=pTab->aCol; i<pTab->nCol; i++, pCol++){
        if( IsHiddenColumn(pCol) ){
          nHidden++;
          continue;
        }
        sqlite3VdbeAddOp2(v, OP_Integer, i-nHidden, 1);
        sqlite3VdbeAddOp4(v, OP_String8, 0, 2, 0, pCol->zName, 0);
        sqlite3VdbeAddOp4(v, OP_String8, 0, 3, 0,
           pCol->zType ? pCol->zType : "", 0);
        sqlite3VdbeAddOp2(v, OP_Integer, (pCol->notNull ? 1 : 0), 4);
        if( pCol->zDflt ){
          sqlite3VdbeAddOp4(v, OP_String8, 0, 5, 0, (char*)pCol->zDflt, 0);
        }else{
          sqlite3VdbeAddOp2(v, OP_Null, 0, 5);
        }
        if( (pCol->colFlags & COLFLAG_PRIMKEY)==0 ){
          k = 0;
        }else if( pPk==0 ){
          k = 1;
        }else{
          for(k=1; ALWAYS(k<=pTab->nCol) && pPk->aiColumn[k-1]!=i; k++){}
        }
        sqlite3VdbeAddOp2(v, OP_Integer, k, 6);
        sqlite3VdbeAddOp2(v, OP_ResultRow, 1, 6);
      }
    }
  }
  break;

  case PragTyp_STATS: {
    Index *pIdx;
    HashElem *i;
    v = sqlite3GetVdbe(pParse);
    sqlite3VdbeSetNumCols(v, 4);
    pParse->nMem = 4;
    sqlite3CodeVerifySchema(pParse, iDb);
    sqlite3VdbeSetColName(v, 0, COLNAME_NAME, "table", SQLITE_STATIC);
    sqlite3VdbeSetColName(v, 1, COLNAME_NAME, "index", SQLITE_STATIC);
    sqlite3VdbeSetColName(v, 2, COLNAME_NAME, "width", SQLITE_STATIC);
    sqlite3VdbeSetColName(v, 3, COLNAME_NAME, "height", SQLITE_STATIC);
    for(i=sqliteHashFirst(&pDb->pSchema->tblHash); i; i=sqliteHashNext(i)){
      Table *pTab = sqliteHashData(i);
      sqlite3VdbeAddOp4(v, OP_String8, 0, 1, 0, pTab->zName, 0);
      sqlite3VdbeAddOp2(v, OP_Null, 0, 2);
      sqlite3VdbeAddOp2(v, OP_Integer,
                           (int)sqlite3LogEstToInt(pTab->szTabRow), 3);
      sqlite3VdbeAddOp2(v, OP_Integer, (int)pTab->nRowEst, 4);
      sqlite3VdbeAddOp2(v, OP_ResultRow, 1, 4);
      for(pIdx=pTab->pIndex; pIdx; pIdx=pIdx->pNext){
        sqlite3VdbeAddOp4(v, OP_String8, 0, 2, 0, pIdx->zName, 0);
        sqlite3VdbeAddOp2(v, OP_Integer,
                             (int)sqlite3LogEstToInt(pIdx->szIdxRow), 3);
        sqlite3VdbeAddOp2(v, OP_Integer, (int)pIdx->aiRowEst[0], 4);
        sqlite3VdbeAddOp2(v, OP_ResultRow, 1, 4);
      }
    }
  }
  break;

  case PragTyp_INDEX_INFO: if( zRight ){
    Index *pIdx;
    Table *pTab;
    pIdx = sqlite3FindIndex(db, zRight, zDb);
    if( pIdx ){
      int i;
      pTab = pIdx->pTable;
      sqlite3VdbeSetNumCols(v, 3);
      pParse->nMem = 3;
      sqlite3CodeVerifySchema(pParse, iDb);
      sqlite3VdbeSetColName(v, 0, COLNAME_NAME, "seqno", SQLITE_STATIC);
      sqlite3VdbeSetColName(v, 1, COLNAME_NAME, "cid", SQLITE_STATIC);
      sqlite3VdbeSetColName(v, 2, COLNAME_NAME, "name", SQLITE_STATIC);
      for(i=0; i<pIdx->nKeyCol; i++){
        i16 cnum = pIdx->aiColumn[i];
        sqlite3VdbeAddOp2(v, OP_Integer, i, 1);
        sqlite3VdbeAddOp2(v, OP_Integer, cnum, 2);
        assert( pTab->nCol>cnum );
        sqlite3VdbeAddOp4(v, OP_String8, 0, 3, 0, pTab->aCol[cnum].zName, 0);
        sqlite3VdbeAddOp2(v, OP_ResultRow, 1, 3);
      }
    }
  }
  break;

  case PragTyp_INDEX_LIST: if( zRight ){
    Index *pIdx;
    Table *pTab;
    int i;
    pTab = sqlite3FindTable(db, zRight, zDb);
    if( pTab ){
      v = sqlite3GetVdbe(pParse);
      sqlite3VdbeSetNumCols(v, 3);
      pParse->nMem = 3;
      sqlite3CodeVerifySchema(pParse, iDb);
      sqlite3VdbeSetColName(v, 0, COLNAME_NAME, "seq", SQLITE_STATIC);
      sqlite3VdbeSetColName(v, 1, COLNAME_NAME, "name", SQLITE_STATIC);
      sqlite3VdbeSetColName(v, 2, COLNAME_NAME, "unique", SQLITE_STATIC);
      for(pIdx=pTab->pIndex, i=0; pIdx; pIdx=pIdx->pNext, i++){
        sqlite3VdbeAddOp2(v, OP_Integer, i, 1);
        sqlite3VdbeAddOp4(v, OP_String8, 0, 2, 0, pIdx->zName, 0);
        sqlite3VdbeAddOp2(v, OP_Integer, pIdx->onError!=OE_None, 3);
        sqlite3VdbeAddOp2(v, OP_ResultRow, 1, 3);
      }
    }
  }
  break;

  case PragTyp_DATABASE_LIST: {
    int i;
    sqlite3VdbeSetNumCols(v, 3);
    pParse->nMem = 3;
    sqlite3VdbeSetColName(v, 0, COLNAME_NAME, "seq", SQLITE_STATIC);
    sqlite3VdbeSetColName(v, 1, COLNAME_NAME, "name", SQLITE_STATIC);
    sqlite3VdbeSetColName(v, 2, COLNAME_NAME, "file", SQLITE_STATIC);
    for(i=0; i<db->nDb; i++){
      if( db->aDb[i].pBt==0 ) continue;
      assert( db->aDb[i].zName!=0 );
      sqlite3VdbeAddOp2(v, OP_Integer, i, 1);
      sqlite3VdbeAddOp4(v, OP_String8, 0, 2, 0, db->aDb[i].zName, 0);
      sqlite3VdbeAddOp4(v, OP_String8, 0, 3, 0,
           sqlite3BtreeGetFilename(db->aDb[i].pBt), 0);
      sqlite3VdbeAddOp2(v, OP_ResultRow, 1, 3);
    }
  }
  break;

  case PragTyp_COLLATION_LIST: {
    int i = 0;
    HashElem *p;
    sqlite3VdbeSetNumCols(v, 2);
    pParse->nMem = 2;
    sqlite3VdbeSetColName(v, 0, COLNAME_NAME, "seq", SQLITE_STATIC);
    sqlite3VdbeSetColName(v, 1, COLNAME_NAME, "name", SQLITE_STATIC);
    for(p=sqliteHashFirst(&db->aCollSeq); p; p=sqliteHashNext(p)){
      CollSeq *pColl = (CollSeq *)sqliteHashData(p);
      sqlite3VdbeAddOp2(v, OP_Integer, i++, 1);
      sqlite3VdbeAddOp4(v, OP_String8, 0, 2, 0, pColl->zName, 0);
      sqlite3VdbeAddOp2(v, OP_ResultRow, 1, 2);
    }
  }
  break;
#endif /* SQLITE_OMIT_SCHEMA_PRAGMAS */

#ifndef SQLITE_OMIT_FOREIGN_KEY
  case PragTyp_FOREIGN_KEY_LIST: if( zRight ){
    FKey *pFK;
    Table *pTab;
    pTab = sqlite3FindTable(db, zRight, zDb);
    if( pTab ){
      v = sqlite3GetVdbe(pParse);
      pFK = pTab->pFKey;
      if( pFK ){
        int i = 0; 
        sqlite3VdbeSetNumCols(v, 8);
        pParse->nMem = 8;
        sqlite3CodeVerifySchema(pParse, iDb);
        sqlite3VdbeSetColName(v, 0, COLNAME_NAME, "id", SQLITE_STATIC);
        sqlite3VdbeSetColName(v, 1, COLNAME_NAME, "seq", SQLITE_STATIC);
        sqlite3VdbeSetColName(v, 2, COLNAME_NAME, "table", SQLITE_STATIC);
        sqlite3VdbeSetColName(v, 3, COLNAME_NAME, "from", SQLITE_STATIC);
        sqlite3VdbeSetColName(v, 4, COLNAME_NAME, "to", SQLITE_STATIC);
        sqlite3VdbeSetColName(v, 5, COLNAME_NAME, "on_update", SQLITE_STATIC);
        sqlite3VdbeSetColName(v, 6, COLNAME_NAME, "on_delete", SQLITE_STATIC);
        sqlite3VdbeSetColName(v, 7, COLNAME_NAME, "match", SQLITE_STATIC);
        while(pFK){
          int j;
          for(j=0; j<pFK->nCol; j++){
            char *zCol = pFK->aCol[j].zCol;
            char *zOnDelete = (char *)actionName(pFK->aAction[0]);
            char *zOnUpdate = (char *)actionName(pFK->aAction[1]);
            sqlite3VdbeAddOp2(v, OP_Integer, i, 1);
            sqlite3VdbeAddOp2(v, OP_Integer, j, 2);
            sqlite3VdbeAddOp4(v, OP_String8, 0, 3, 0, pFK->zTo, 0);
            sqlite3VdbeAddOp4(v, OP_String8, 0, 4, 0,
                              pTab->aCol[pFK->aCol[j].iFrom].zName, 0);
            sqlite3VdbeAddOp4(v, zCol ? OP_String8 : OP_Null, 0, 5, 0, zCol, 0);
            sqlite3VdbeAddOp4(v, OP_String8, 0, 6, 0, zOnUpdate, 0);
            sqlite3VdbeAddOp4(v, OP_String8, 0, 7, 0, zOnDelete, 0);
            sqlite3VdbeAddOp4(v, OP_String8, 0, 8, 0, "NONE", 0);
            sqlite3VdbeAddOp2(v, OP_ResultRow, 1, 8);
          }
          ++i;
          pFK = pFK->pNextFrom;
        }
      }
    }
  }
  break;
#endif /* !defined(SQLITE_OMIT_FOREIGN_KEY) */

#ifndef SQLITE_OMIT_FOREIGN_KEY
#ifndef SQLITE_OMIT_TRIGGER
  case PragTyp_FOREIGN_KEY_CHECK: {
    FKey *pFK;             /* A foreign key constraint */
    Table *pTab;           /* Child table contain "REFERENCES" keyword */
    Table *pParent;        /* Parent table that child points to */
    Index *pIdx;           /* Index in the parent table */
    int i;                 /* Loop counter:  Foreign key number for pTab */
    int j;                 /* Loop counter:  Field of the foreign key */
    HashElem *k;           /* Loop counter:  Next table in schema */
    int x;                 /* result variable */
    int regResult;         /* 3 registers to hold a result row */
    int regKey;            /* Register to hold key for checking the FK */
    int regRow;            /* Registers to hold a row from pTab */
    int addrTop;           /* Top of a loop checking foreign keys */
    int addrOk;            /* Jump here if the key is OK */
    int *aiCols;           /* child to parent column mapping */

    regResult = pParse->nMem+1;
    pParse->nMem += 4;
    regKey = ++pParse->nMem;
    regRow = ++pParse->nMem;
    v = sqlite3GetVdbe(pParse);
    sqlite3VdbeSetNumCols(v, 4);
    sqlite3VdbeSetColName(v, 0, COLNAME_NAME, "table", SQLITE_STATIC);
    sqlite3VdbeSetColName(v, 1, COLNAME_NAME, "rowid", SQLITE_STATIC);
    sqlite3VdbeSetColName(v, 2, COLNAME_NAME, "parent", SQLITE_STATIC);
    sqlite3VdbeSetColName(v, 3, COLNAME_NAME, "fkid", SQLITE_STATIC);
    sqlite3CodeVerifySchema(pParse, iDb);
    k = sqliteHashFirst(&db->aDb[iDb].pSchema->tblHash);
    while( k ){
      if( zRight ){
        pTab = sqlite3LocateTable(pParse, 0, zRight, zDb);
        k = 0;
      }else{
        pTab = (Table*)sqliteHashData(k);
        k = sqliteHashNext(k);
      }
      if( pTab==0 || pTab->pFKey==0 ) continue;
      sqlite3TableLock(pParse, iDb, pTab->tnum, 0, pTab->zName);
      if( pTab->nCol+regRow>pParse->nMem ) pParse->nMem = pTab->nCol + regRow;
      sqlite3OpenTable(pParse, 0, iDb, pTab, OP_OpenRead);
      sqlite3VdbeAddOp4(v, OP_String8, 0, regResult, 0, pTab->zName,
                        P4_TRANSIENT);
      for(i=1, pFK=pTab->pFKey; pFK; i++, pFK=pFK->pNextFrom){
        pParent = sqlite3FindTable(db, pFK->zTo, zDb);
        if( pParent==0 ) continue;
        pIdx = 0;
        sqlite3TableLock(pParse, iDb, pParent->tnum, 0, pParent->zName);
        x = sqlite3FkLocateIndex(pParse, pParent, pFK, &pIdx, 0);
        if( x==0 ){
          if( pIdx==0 ){
            sqlite3OpenTable(pParse, i, iDb, pParent, OP_OpenRead);
          }else{
            sqlite3VdbeAddOp3(v, OP_OpenRead, i, pIdx->tnum, iDb);
            sqlite3VdbeSetP4KeyInfo(pParse, pIdx);
          }
        }else{
          k = 0;
          break;
        }
      }
      assert( pParse->nErr>0 || pFK==0 );
      if( pFK ) break;
      if( pParse->nTab<i ) pParse->nTab = i;
      addrTop = sqlite3VdbeAddOp1(v, OP_Rewind, 0);
      for(i=1, pFK=pTab->pFKey; pFK; i++, pFK=pFK->pNextFrom){
        pParent = sqlite3FindTable(db, pFK->zTo, zDb);
        pIdx = 0;
        aiCols = 0;
        if( pParent ){
          x = sqlite3FkLocateIndex(pParse, pParent, pFK, &pIdx, &aiCols);
          assert( x==0 );
        }
        addrOk = sqlite3VdbeMakeLabel(v);
        if( pParent && pIdx==0 ){
          int iKey = pFK->aCol[0].iFrom;
          assert( iKey>=0 && iKey<pTab->nCol );
          if( iKey!=pTab->iPKey ){
            sqlite3VdbeAddOp3(v, OP_Column, 0, iKey, regRow);
            sqlite3ColumnDefault(v, pTab, iKey, regRow);
            sqlite3VdbeAddOp2(v, OP_IsNull, regRow, addrOk);
            sqlite3VdbeAddOp2(v, OP_MustBeInt, regRow,
               sqlite3VdbeCurrentAddr(v)+3);
          }else{
            sqlite3VdbeAddOp2(v, OP_Rowid, 0, regRow);
          }
          sqlite3VdbeAddOp3(v, OP_NotExists, i, 0, regRow);
          sqlite3VdbeAddOp2(v, OP_Goto, 0, addrOk);
          sqlite3VdbeJumpHere(v, sqlite3VdbeCurrentAddr(v)-2);
        }else{
          for(j=0; j<pFK->nCol; j++){
            sqlite3ExprCodeGetColumnOfTable(v, pTab, 0,
                            aiCols ? aiCols[j] : pFK->aCol[j].iFrom, regRow+j);
            sqlite3VdbeAddOp2(v, OP_IsNull, regRow+j, addrOk);
          }
          if( pParent ){
            sqlite3VdbeAddOp3(v, OP_MakeRecord, regRow, pFK->nCol, regKey);
            sqlite3VdbeChangeP4(v, -1,
                     sqlite3IndexAffinityStr(v,pIdx), P4_TRANSIENT);
            sqlite3VdbeAddOp4Int(v, OP_Found, i, addrOk, regKey, 0);
          }
        }
        sqlite3VdbeAddOp2(v, OP_Rowid, 0, regResult+1);
        sqlite3VdbeAddOp4(v, OP_String8, 0, regResult+2, 0, 
                          pFK->zTo, P4_TRANSIENT);
        sqlite3VdbeAddOp2(v, OP_Integer, i-1, regResult+3);
        sqlite3VdbeAddOp2(v, OP_ResultRow, regResult, 4);
        sqlite3VdbeResolveLabel(v, addrOk);
        sqlite3DbFree(db, aiCols);
      }
      sqlite3VdbeAddOp2(v, OP_Next, 0, addrTop+1);
      sqlite3VdbeJumpHere(v, addrTop);
    }
  }
  break;
#endif /* !defined(SQLITE_OMIT_TRIGGER) */
#endif /* !defined(SQLITE_OMIT_FOREIGN_KEY) */

#ifndef NDEBUG
  case PragTyp_PARSER_TRACE: {
    if( zRight ){
      if( sqlite3GetBoolean(zRight, 0) ){
        sqlite3ParserTrace(0, "parser: ");
      }else{
        sqlite3ParserTrace(0, 0);
      }
    }
  }
  break;
#endif

  /* Reinstall the LIKE and GLOB functions.  The variant of LIKE
  ** used will be case sensitive or not depending on the RHS.
  */
  case PragTyp_CASE_SENSITIVE_LIKE: {
    if( zRight ){
      sqlite3RegisterLikeFunctions(db, sqlite3GetBoolean(zRight, 0));
    }
  }
  break;

#ifndef SQLITE_INTEGRITY_CHECK_ERROR_MAX
# define SQLITE_INTEGRITY_CHECK_ERROR_MAX 100
#endif

#ifndef SQLITE_OMIT_INTEGRITY_CHECK
  /* Pragma "quick_check" is reduced version of 
  ** integrity_check designed to detect most database corruption
  ** without most of the overhead of a full integrity-check.
  */
  case PragTyp_INTEGRITY_CHECK: {
    int i, j, addr, mxErr;

    /* Code that appears at the end of the integrity check.  If no error
    ** messages have been generated, output OK.  Otherwise output the
    ** error message
    */
    static const VdbeOpList endCode[] = {
      { OP_AddImm,      1, 0,        0},    /* 0 */
      { OP_IfNeg,       1, 0,        0},    /* 1 */
      { OP_String8,     0, 3,        0},    /* 2 */
      { OP_ResultRow,   3, 1,        0},
    };

    int isQuick = (sqlite3Tolower(zLeft[0])=='q');

    /* If the PRAGMA command was of the form "PRAGMA <db>.integrity_check",
    ** then iDb is set to the index of the database identified by <db>.
    ** In this case, the integrity of database iDb only is verified by
    ** the VDBE created below.
    **
    ** Otherwise, if the command was simply "PRAGMA integrity_check" (or
    ** "PRAGMA quick_check"), then iDb is set to 0. In this case, set iDb
    ** to -1 here, to indicate that the VDBE should verify the integrity
    ** of all attached databases.  */
    assert( iDb>=0 );
    assert( iDb==0 || pId2->z );
    if( pId2->z==0 ) iDb = -1;

    /* Initialize the VDBE program */
    pParse->nMem = 6;
    sqlite3VdbeSetNumCols(v, 1);
    sqlite3VdbeSetColName(v, 0, COLNAME_NAME, "integrity_check", SQLITE_STATIC);

    /* Set the maximum error count */
    mxErr = SQLITE_INTEGRITY_CHECK_ERROR_MAX;
    if( zRight ){
      sqlite3GetInt32(zRight, &mxErr);
      if( mxErr<=0 ){
        mxErr = SQLITE_INTEGRITY_CHECK_ERROR_MAX;
      }
    }
    sqlite3VdbeAddOp2(v, OP_Integer, mxErr, 1);  /* reg[1] holds errors left */

    /* Do an integrity check on each database file */
    for(i=0; i<db->nDb; i++){
      HashElem *x;
      Hash *pTbls;
      int cnt = 0;

      if( OMIT_TEMPDB && i==1 ) continue;
      if( iDb>=0 && i!=iDb ) continue;

      sqlite3CodeVerifySchema(pParse, i);
      addr = sqlite3VdbeAddOp1(v, OP_IfPos, 1); /* Halt if out of errors */
      sqlite3VdbeAddOp2(v, OP_Halt, 0, 0);
      sqlite3VdbeJumpHere(v, addr);

      /* Do an integrity check of the B-Tree
      **
      ** Begin by filling registers 2, 3, ... with the root pages numbers
      ** for all tables and indices in the database.
      */
      assert( sqlite3SchemaMutexHeld(db, i, 0) );
      pTbls = &db->aDb[i].pSchema->tblHash;
      for(x=sqliteHashFirst(pTbls); x; x=sqliteHashNext(x)){
        Table *pTab = sqliteHashData(x);
        Index *pIdx;
        if( HasRowid(pTab) ){
          sqlite3VdbeAddOp2(v, OP_Integer, pTab->tnum, 2+cnt);
          VdbeComment((v, "%s", pTab->zName));
          cnt++;
        }
        for(pIdx=pTab->pIndex; pIdx; pIdx=pIdx->pNext){
          sqlite3VdbeAddOp2(v, OP_Integer, pIdx->tnum, 2+cnt);
          VdbeComment((v, "%s", pIdx->zName));
          cnt++;
        }
      }

      /* Make sure sufficient number of registers have been allocated */
      pParse->nMem = MAX( pParse->nMem, cnt+8 );

      /* Do the b-tree integrity checks */
      sqlite3VdbeAddOp3(v, OP_IntegrityCk, 2, cnt, 1);
      sqlite3VdbeChangeP5(v, (u8)i);
      addr = sqlite3VdbeAddOp1(v, OP_IsNull, 2);
      sqlite3VdbeAddOp4(v, OP_String8, 0, 3, 0,
         sqlite3MPrintf(db, "*** in database %s ***\n", db->aDb[i].zName),
         P4_DYNAMIC);
      sqlite3VdbeAddOp2(v, OP_Move, 2, 4);
      sqlite3VdbeAddOp3(v, OP_Concat, 4, 3, 2);
      sqlite3VdbeAddOp2(v, OP_ResultRow, 2, 1);
      sqlite3VdbeJumpHere(v, addr);

      /* Make sure all the indices are constructed correctly.
      */
      for(x=sqliteHashFirst(pTbls); x && !isQuick; x=sqliteHashNext(x)){
        Table *pTab = sqliteHashData(x);
        Index *pIdx, *pPk;
        int loopTop;
        int iDataCur, iIdxCur;

        if( pTab->pIndex==0 ) continue;
        pPk = HasRowid(pTab) ? 0 : sqlite3PrimaryKeyIndex(pTab);
        addr = sqlite3VdbeAddOp1(v, OP_IfPos, 1);  /* Stop if out of errors */
        sqlite3VdbeAddOp2(v, OP_Halt, 0, 0);
        sqlite3VdbeJumpHere(v, addr);
        sqlite3ExprCacheClear(pParse);
        sqlite3OpenTableAndIndices(pParse, pTab, OP_OpenRead,
                                   1, 0, &iDataCur, &iIdxCur);
        sqlite3VdbeAddOp2(v, OP_Integer, 0, 7);
        for(j=0, pIdx=pTab->pIndex; pIdx; pIdx=pIdx->pNext, j++){
          sqlite3VdbeAddOp2(v, OP_Integer, 0, 8+j); /* index entries counter */
        }
        pParse->nMem = MAX(pParse->nMem, 8+j);
        sqlite3VdbeAddOp2(v, OP_Rewind, iDataCur, 0);
        loopTop = sqlite3VdbeAddOp2(v, OP_AddImm, 7, 1);
        for(j=0, pIdx=pTab->pIndex; pIdx; pIdx=pIdx->pNext, j++){
          int jmp2, jmp3, jmp4;
          int r1;
          if( pPk==pIdx ) continue;
          r1 = sqlite3GenerateIndexKey(pParse, pIdx, iDataCur, 0, 0, &jmp3);
          sqlite3VdbeAddOp2(v, OP_AddImm, 8+j, 1);  /* increment entry count */
          jmp2 = sqlite3VdbeAddOp4Int(v, OP_Found, iIdxCur+j, 0, r1,
                                      pIdx->nColumn);
          sqlite3VdbeAddOp2(v, OP_AddImm, 1, -1); /* Decrement error limit */
          sqlite3VdbeAddOp4(v, OP_String8, 0, 3, 0, "row ", P4_STATIC);
          sqlite3VdbeAddOp3(v, OP_Concat, 7, 3, 3);
          sqlite3VdbeAddOp4(v, OP_String8, 0, 4, 0, " missing from index ",
                            P4_STATIC);
          sqlite3VdbeAddOp3(v, OP_Concat, 4, 3, 3);
          sqlite3VdbeAddOp4(v, OP_String8, 0, 4, 0, pIdx->zName, P4_TRANSIENT);
          sqlite3VdbeAddOp3(v, OP_Concat, 4, 3, 3);
          sqlite3VdbeAddOp2(v, OP_ResultRow, 3, 1);
          jmp4 = sqlite3VdbeAddOp1(v, OP_IfPos, 1);
          sqlite3VdbeAddOp0(v, OP_Halt);
          sqlite3VdbeJumpHere(v, jmp4);
          sqlite3VdbeJumpHere(v, jmp2);
          sqlite3VdbeResolveLabel(v, jmp3);
        }
        sqlite3VdbeAddOp2(v, OP_Next, iDataCur, loopTop);
        sqlite3VdbeJumpHere(v, loopTop-1);
#ifndef SQLITE_OMIT_BTREECOUNT
        sqlite3VdbeAddOp4(v, OP_String8, 0, 2, 0, 
                     "wrong # of entries in index ", P4_STATIC);
        for(j=0, pIdx=pTab->pIndex; pIdx; pIdx=pIdx->pNext, j++){
          if( pPk==pIdx ) continue;
          addr = sqlite3VdbeCurrentAddr(v);
          sqlite3VdbeAddOp2(v, OP_IfPos, 1, addr+2);
          sqlite3VdbeAddOp2(v, OP_Halt, 0, 0);
          sqlite3VdbeAddOp2(v, OP_Count, iIdxCur+j, 3);
          sqlite3VdbeAddOp3(v, OP_Eq, 8+j, addr+8, 3);
          sqlite3VdbeAddOp2(v, OP_AddImm, 1, -1);
          sqlite3VdbeAddOp4(v, OP_String8, 0, 3, 0, pIdx->zName, P4_TRANSIENT);
          sqlite3VdbeAddOp3(v, OP_Concat, 3, 2, 7);
          sqlite3VdbeAddOp2(v, OP_ResultRow, 7, 1);
        }
#endif /* SQLITE_OMIT_BTREECOUNT */
      } 
    }
    addr = sqlite3VdbeAddOpList(v, ArraySize(endCode), endCode);
    sqlite3VdbeChangeP2(v, addr, -mxErr);
    sqlite3VdbeJumpHere(v, addr+1);
    sqlite3VdbeChangeP4(v, addr+2, "ok", P4_STATIC);
  }
  break;
#endif /* SQLITE_OMIT_INTEGRITY_CHECK */

#ifndef SQLITE_OMIT_UTF16
  /*
  **   PRAGMA encoding
  **   PRAGMA encoding = "utf-8"|"utf-16"|"utf-16le"|"utf-16be"
  **
  ** In its first form, this pragma returns the encoding of the main
  ** database. If the database is not initialized, it is initialized now.
  **
  ** The second form of this pragma is a no-op if the main database file
  ** has not already been initialized. In this case it sets the default
  ** encoding that will be used for the main database file if a new file
  ** is created. If an existing main database file is opened, then the
  ** default text encoding for the existing database is used.
  ** 
  ** In all cases new databases created using the ATTACH command are
  ** created to use the same default text encoding as the main database. If
  ** the main database has not been initialized and/or created when ATTACH
  ** is executed, this is done before the ATTACH operation.
  **
  ** In the second form this pragma sets the text encoding to be used in
  ** new database files created using this database handle. It is only
  ** useful if invoked immediately after the main database i
  */
  case PragTyp_ENCODING: {
    static const struct EncName {
      char *zName;
      u8 enc;
    } encnames[] = {
      { "UTF8",     SQLITE_UTF8        },
      { "UTF-8",    SQLITE_UTF8        },  /* Must be element [1] */
      { "UTF-16le", SQLITE_UTF16LE     },  /* Must be element [2] */
      { "UTF-16be", SQLITE_UTF16BE     },  /* Must be element [3] */
      { "UTF16le",  SQLITE_UTF16LE     },
      { "UTF16be",  SQLITE_UTF16BE     },
      { "UTF-16",   0                  }, /* SQLITE_UTF16NATIVE */
      { "UTF16",    0                  }, /* SQLITE_UTF16NATIVE */
      { 0, 0 }
    };
    const struct EncName *pEnc;
    if( !zRight ){    /* "PRAGMA encoding" */
      if( sqlite3ReadSchema(pParse) ) goto pragma_out;
      sqlite3VdbeSetNumCols(v, 1);
      sqlite3VdbeSetColName(v, 0, COLNAME_NAME, "encoding", SQLITE_STATIC);
      sqlite3VdbeAddOp2(v, OP_String8, 0, 1);
      assert( encnames[SQLITE_UTF8].enc==SQLITE_UTF8 );
      assert( encnames[SQLITE_UTF16LE].enc==SQLITE_UTF16LE );
      assert( encnames[SQLITE_UTF16BE].enc==SQLITE_UTF16BE );
      sqlite3VdbeChangeP4(v, -1, encnames[ENC(pParse->db)].zName, P4_STATIC);
      sqlite3VdbeAddOp2(v, OP_ResultRow, 1, 1);
    }else{                        /* "PRAGMA encoding = XXX" */
      /* Only change the value of sqlite.enc if the database handle is not
      ** initialized. If the main database exists, the new sqlite.enc value
      ** will be overwritten when the schema is next loaded. If it does not
      ** already exists, it will be created to use the new encoding value.
      */
      if( 
        !(DbHasProperty(db, 0, DB_SchemaLoaded)) || 
        DbHasProperty(db, 0, DB_Empty) 
      ){
        for(pEnc=&encnames[0]; pEnc->zName; pEnc++){
          if( 0==sqlite3StrICmp(zRight, pEnc->zName) ){
            ENC(pParse->db) = pEnc->enc ? pEnc->enc : SQLITE_UTF16NATIVE;
            break;
          }
        }
        if( !pEnc->zName ){
          sqlite3ErrorMsg(pParse, "unsupported encoding: %s", zRight);
        }
      }
    }
  }
  break;
#endif /* SQLITE_OMIT_UTF16 */

#ifndef SQLITE_OMIT_SCHEMA_VERSION_PRAGMAS
  /*
  **   PRAGMA [database.]schema_version
  **   PRAGMA [database.]schema_version = <integer>
  **
  **   PRAGMA [database.]user_version
  **   PRAGMA [database.]user_version = <integer>
  **
  **   PRAGMA [database.]freelist_count = <integer>
  **
  **   PRAGMA [database.]application_id
  **   PRAGMA [database.]application_id = <integer>
  **
  ** The pragma's schema_version and user_version are used to set or get
  ** the value of the schema-version and user-version, respectively. Both
  ** the schema-version and the user-version are 32-bit signed integers
  ** stored in the database header.
  **
  ** The schema-cookie is usually only manipulated internally by SQLite. It
  ** is incremented by SQLite whenever the database schema is modified (by
  ** creating or dropping a table or index). The schema version is used by
  ** SQLite each time a query is executed to ensure that the internal cache
  ** of the schema used when compiling the SQL query matches the schema of
  ** the database against which the compiled query is actually executed.
  ** Subverting this mechanism by using "PRAGMA schema_version" to modify
  ** the schema-version is potentially dangerous and may lead to program
  ** crashes or database corruption. Use with caution!
  **
  ** The user-version is not used internally by SQLite. It may be used by
  ** applications for any purpose.
  */
  case PragTyp_HEADER_VALUE: {
    int iCookie;   /* Cookie index. 1 for schema-cookie, 6 for user-cookie. */
    sqlite3VdbeUsesBtree(v, iDb);
    switch( zLeft[0] ){
      case 'a': case 'A':
        iCookie = BTREE_APPLICATION_ID;
        break;
      case 'f': case 'F':
        iCookie = BTREE_FREE_PAGE_COUNT;
        break;
      case 's': case 'S':
        iCookie = BTREE_SCHEMA_VERSION;
        break;
      default:
        iCookie = BTREE_USER_VERSION;
        break;
    }

    if( zRight && iCookie!=BTREE_FREE_PAGE_COUNT ){
      /* Write the specified cookie value */
      static const VdbeOpList setCookie[] = {
        { OP_Transaction,    0,  1,  0},    /* 0 */
        { OP_Integer,        0,  1,  0},    /* 1 */
        { OP_SetCookie,      0,  0,  1},    /* 2 */
      };
      int addr = sqlite3VdbeAddOpList(v, ArraySize(setCookie), setCookie);
      sqlite3VdbeChangeP1(v, addr, iDb);
      sqlite3VdbeChangeP1(v, addr+1, sqlite3Atoi(zRight));
      sqlite3VdbeChangeP1(v, addr+2, iDb);
      sqlite3VdbeChangeP2(v, addr+2, iCookie);
    }else{
      /* Read the specified cookie value */
      static const VdbeOpList readCookie[] = {
        { OP_Transaction,     0,  0,  0},    /* 0 */
        { OP_ReadCookie,      0,  1,  0},    /* 1 */
        { OP_ResultRow,       1,  1,  0}
      };
      int addr = sqlite3VdbeAddOpList(v, ArraySize(readCookie), readCookie);
      sqlite3VdbeChangeP1(v, addr, iDb);
      sqlite3VdbeChangeP1(v, addr+1, iDb);
      sqlite3VdbeChangeP3(v, addr+1, iCookie);
      sqlite3VdbeSetNumCols(v, 1);
      sqlite3VdbeSetColName(v, 0, COLNAME_NAME, zLeft, SQLITE_TRANSIENT);
    }
  }
  break;
#endif /* SQLITE_OMIT_SCHEMA_VERSION_PRAGMAS */

#ifndef SQLITE_OMIT_COMPILEOPTION_DIAGS
  /*
  **   PRAGMA compile_options
  **
  ** Return the names of all compile-time options used in this build,
  ** one option per row.
  */
  case PragTyp_COMPILE_OPTIONS: {
    int i = 0;
    const char *zOpt;
    sqlite3VdbeSetNumCols(v, 1);
    pParse->nMem = 1;
    sqlite3VdbeSetColName(v, 0, COLNAME_NAME, "compile_option", SQLITE_STATIC);
    while( (zOpt = sqlite3_compileoption_get(i++))!=0 ){
      sqlite3VdbeAddOp4(v, OP_String8, 0, 1, 0, zOpt, 0);
      sqlite3VdbeAddOp2(v, OP_ResultRow, 1, 1);
    }
  }
  break;
#endif /* SQLITE_OMIT_COMPILEOPTION_DIAGS */

#ifndef SQLITE_OMIT_WAL
  /*
  **   PRAGMA [database.]wal_checkpoint = passive|full|restart
  **
  ** Checkpoint the database.
  */
  case PragTyp_WAL_CHECKPOINT: {
    int iBt = (pId2->z?iDb:SQLITE_MAX_ATTACHED);
    int eMode = SQLITE_CHECKPOINT_PASSIVE;
    if( zRight ){
      if( sqlite3StrICmp(zRight, "full")==0 ){
        eMode = SQLITE_CHECKPOINT_FULL;
      }else if( sqlite3StrICmp(zRight, "restart")==0 ){
        eMode = SQLITE_CHECKPOINT_RESTART;
      }
    }
    sqlite3VdbeSetNumCols(v, 3);
    pParse->nMem = 3;
    sqlite3VdbeSetColName(v, 0, COLNAME_NAME, "busy", SQLITE_STATIC);
    sqlite3VdbeSetColName(v, 1, COLNAME_NAME, "log", SQLITE_STATIC);
    sqlite3VdbeSetColName(v, 2, COLNAME_NAME, "checkpointed", SQLITE_STATIC);

    sqlite3VdbeAddOp3(v, OP_Checkpoint, iBt, eMode, 1);
    sqlite3VdbeAddOp2(v, OP_ResultRow, 1, 3);
  }
  break;

  /*
  **   PRAGMA wal_autocheckpoint
  **   PRAGMA wal_autocheckpoint = N
  **
  ** Configure a database connection to automatically checkpoint a database
  ** after accumulating N frames in the log. Or query for the current value
  ** of N.
  */
  case PragTyp_WAL_AUTOCHECKPOINT: {
    if( zRight ){
      sqlite3_wal_autocheckpoint(db, sqlite3Atoi(zRight));
    }
    returnSingleInt(pParse, "wal_autocheckpoint", 
       db->xWalCallback==sqlite3WalDefaultHook ? 
           SQLITE_PTR_TO_INT(db->pWalArg) : 0);
  }
  break;
#endif

  /*
  **  PRAGMA shrink_memory
  **
  ** This pragma attempts to free as much memory as possible from the
  ** current database connection.
  */
  case PragTyp_SHRINK_MEMORY: {
    sqlite3_db_release_memory(db);
    break;
  }

  /*
  **   PRAGMA busy_timeout
  **   PRAGMA busy_timeout = N
  **
  ** Call sqlite3_busy_timeout(db, N).  Return the current timeout value
  ** if one is set.  If no busy handler or a different busy handler is set
  ** then 0 is returned.  Setting the busy_timeout to 0 or negative
  ** disables the timeout.
  */
  /*case PragTyp_BUSY_TIMEOUT*/ default: {
    assert( aPragmaNames[mid].ePragTyp==PragTyp_BUSY_TIMEOUT );
    if( zRight ){
      sqlite3_busy_timeout(db, sqlite3Atoi(zRight));
    }
    returnSingleInt(pParse, "timeout",  db->busyTimeout);
    break;
  }

  /*
  **   PRAGMA soft_heap_limit
  **   PRAGMA soft_heap_limit = N
  **
  ** Call sqlite3_soft_heap_limit64(N).  Return the result.  If N is omitted,
  ** use -1.
  */
  case PragTyp_SOFT_HEAP_LIMIT: {
    sqlite3_int64 N;
    if( zRight && sqlite3Atoi64(zRight, &N, 1000000, SQLITE_UTF8)==SQLITE_OK ){
      sqlite3_soft_heap_limit64(N);
    }
    returnSingleInt(pParse, "soft_heap_limit",  sqlite3_soft_heap_limit64(-1));
    break;
  }

#if defined(SQLITE_DEBUG) || defined(SQLITE_TEST)
  /*
  ** Report the current state of file logs for all databases
  */
  case PragTyp_LOCK_STATUS: {
    static const char *const azLockName[] = {
      "unlocked", "shared", "reserved", "pending", "exclusive"
    };
    int i;
    sqlite3VdbeSetNumCols(v, 2);
    pParse->nMem = 2;
    sqlite3VdbeSetColName(v, 0, COLNAME_NAME, "database", SQLITE_STATIC);
    sqlite3VdbeSetColName(v, 1, COLNAME_NAME, "status", SQLITE_STATIC);
    for(i=0; i<db->nDb; i++){
      Btree *pBt;
      const char *zState = "unknown";
      int j;
      if( db->aDb[i].zName==0 ) continue;
      sqlite3VdbeAddOp4(v, OP_String8, 0, 1, 0, db->aDb[i].zName, P4_STATIC);
      pBt = db->aDb[i].pBt;
      if( pBt==0 || sqlite3BtreePager(pBt)==0 ){
        zState = "closed";
      }else if( sqlite3_file_control(db, i ? db->aDb[i].zName : 0, 
                                     SQLITE_FCNTL_LOCKSTATE, &j)==SQLITE_OK ){
         zState = azLockName[j];
      }
      sqlite3VdbeAddOp4(v, OP_String8, 0, 2, 0, zState, P4_STATIC);
      sqlite3VdbeAddOp2(v, OP_ResultRow, 1, 2);
    }
    break;
  }
#endif

#ifdef SQLITE_HAS_CODEC
  case PragTyp_KEY: {
    if( zRight ) sqlite3_key_v2(db, zDb, zRight, sqlite3Strlen30(zRight));
    break;
  }
  case PragTyp_REKEY: {
    if( zRight ) sqlite3_rekey_v2(db, zDb, zRight, sqlite3Strlen30(zRight));
    break;
  }
  case PragTyp_HEXKEY: {
    if( zRight ){
      u8 iByte;
      int i;
      char zKey[40];
      for(i=0, iByte=0; i<sizeof(zKey)*2 && sqlite3Isxdigit(zRight[i]); i++){
        iByte = (iByte<<4) + sqlite3HexToInt(zRight[i]);
        if( (i&1)!=0 ) zKey[i/2] = iByte;
      }
      if( (zLeft[3] & 0xf)==0xb ){
        sqlite3_key_v2(db, zDb, zKey, i/2);
      }else{
        sqlite3_rekey_v2(db, zDb, zKey, i/2);
      }
    }
    break;
  }
#endif
#if defined(SQLITE_HAS_CODEC) || defined(SQLITE_ENABLE_CEROD)
  case PragTyp_ACTIVATE_EXTENSIONS: if( zRight ){
#ifdef SQLITE_HAS_CODEC
    if( sqlite3StrNICmp(zRight, "see-", 4)==0 ){
      sqlite3_activate_see(&zRight[4]);
    }
#endif
#ifdef SQLITE_ENABLE_CEROD
    if( sqlite3StrNICmp(zRight, "cerod-", 6)==0 ){
      sqlite3_activate_cerod(&zRight[6]);
    }
#endif
  }
  break;
#endif

  } /* End of the PRAGMA switch */

pragma_out:
  sqlite3DbFree(db, zLeft);
  sqlite3DbFree(db, zRight);
}

#endif /* SQLITE_OMIT_PRAGMA */

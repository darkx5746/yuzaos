#if !defined(__MHASH_INT)
#define __MHASH_INT

#include <mutils/mincludes.h>
#include <mutils/mutils.h>

typedef struct mhash_hash_entry mhash_hash_entry;

#define MHASH_FAILED ((MHASH) 0x0)

/* information prototypes */

WIN32DLL_DEFINE mutils_word32 mhash_count(void);
WIN32DLL_DEFINE mutils_word32 mhash_get_block_size(hashid type);
WIN32DLL_DEFINE mutils_word8 *mhash_get_hash_name(hashid type);

/* initializing prototypes */

WIN32DLL_DEFINE MHASH mhash_init(hashid type);
WIN32DLL_DEFINE MHASH mhash_init_int(hashid type);

/* update prototype */

WIN32DLL_DEFINE mutils_boolean mhash(MHASH thread, __const void *plaintext, mutils_word32 size);

/* finalizing prototype */

WIN32DLL_DEFINE void *mhash_end(MHASH thread);

WIN32DLL_DEFINE mutils_word32 mhash_get_hash_pblock(hashid type);

WIN32DLL_DEFINE MHASH hmac_mhash_init(__const hashid type, void *key, mutils_word32 keysize,
		      mutils_word32 block);

WIN32DLL_DEFINE void *hmac_mhash_end(MHASH thread);

/* Key generation functions */

#define EMPTY_PROTOTYPES 1

#if defined(EMPTY_PROTOTYPES)

WIN32DLL_DEFINE mutils_error mhash_keygen();
WIN32DLL_DEFINE mutils_error mhash_keygen_ext();
WIN32DLL_DEFINE mutils_word8 *mhash_get_keygen_name();
WIN32DLL_DEFINE mutils_word32 mhash_get_keygen_salt_size();
WIN32DLL_DEFINE mutils_word32 mhash_keygen_count();
WIN32DLL_DEFINE mutils_boolean mhash_keygen_uses_salt();
WIN32DLL_DEFINE mutils_boolean mhash_keygen_uses_count();
WIN32DLL_DEFINE mutils_boolean mhash_keygen_uses_hash_algorithm();

#else

mutils_error mhash_keygen(keygenid algorithm, hashid opt_algorithm,
			  mutils_word64 count,
			  void *keyword, mutils_word32 keysize,
			  void *salt, mutils_word32 saltsize,
			  mutils_word8 *password, mutils_word32 passwordlen);

mutils_error mhash_keygen_ext(keygenid algorithm, KEYGEN data,
			      void *keyword, mutils_word32 keysize,
			      mutils_word8 *password, mutils_word32 passwordlen);

mutils_word8 *mhash_get_keygen_name(hashid type);
mutils_word32 mhash_get_keygen_salt_size(keygenid type);
mutils_word32 mhash_keygen_count(void);
mutils_boolean mhash_keygen_uses_salt(keygenid type);
mutils_boolean mhash_keygen_uses_count(keygenid type);
mutils_boolean mhash_keygen_uses_hash_algorithm(keygenid type);

#endif

#endif


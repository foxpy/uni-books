#include <string.h>
#include <assert.h>
#include <sodium/crypto_pwhash_argon2id.h>
#include "../database-wrapper_impl.h"

#define ITERATIONS 4
#define USED_MEMORY 1024 * 1024 * 2

void hash_password(char const *pwd, char hash[static HASH_STR_SIZE]) {
    int rc = crypto_pwhash_argon2id_str(hash, pwd, strlen(pwd), ITERATIONS, USED_MEMORY);
    assert(rc == 0);
}

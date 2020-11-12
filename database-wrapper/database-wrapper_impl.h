#include <stdbool.h>
#include <sodium/crypto_pwhash_argon2id.h>
#include "database-wrapper.h"

#define HASH_STR_SIZE (crypto_pwhash_argon2id_STRBYTES)

void hash_password(char const *pwd, char hash[static HASH_STR_SIZE]);
bool check_password(char const* pwd, char hash[static HASH_STR_SIZE]);

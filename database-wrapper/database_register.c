#include <stdbool.h>
#include <string.h>
#include <qc.h>
#include "database-wrapper_impl/database-wrapper_impl.h"

#define MIN_PASSWORD_LEN 8
#define MAX_PASSWORD_LEN 40

bool database_register(db* database, char const* username, char const* password, char** err) {
    struct user user;
    char hash[HASH_STR_SIZE];
    qc_err* qcerr = qc_err_new();
    if (!validate_username(username, qcerr)) {
        qc_err_append_front(qcerr, "Invalid username");
        *err = qc_err_to_owned_c_str(qcerr);
        return false;
    } else if (!get_user(username, database->db_file, &user, qcerr)) {
        qc_err_set(qcerr, "Username %s is already taken", username);
        *err = qc_err_to_owned_c_str(qcerr);
        return false;
    } else if (strlen(password) < MIN_PASSWORD_LEN) {
        qc_err_set(qcerr, "Password too short, minimum password length: %d", MIN_PASSWORD_LEN);
        *err = qc_err_to_owned_c_str(qcerr);
        return false;
    } else if (strlen(password) > MAX_PASSWORD_LEN) {
        qc_err_set(qcerr, "Password too long, maximum password length: %d", MAX_PASSWORD_LEN);
        *err = qc_err_to_owned_c_str(qcerr);
        return false;
    }
    hash_password(password, hash);
    // TODO: register user
    qc_err_free(qcerr);
}

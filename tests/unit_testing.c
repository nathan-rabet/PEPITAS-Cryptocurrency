#include "tests_macros.h"
#include "cryptosystem/rsa_test.h"
#include "cryptosystem/signature_test.h"
#include "network/client_test.h"
#include "blockchain/block_test.h"
#include "validation/validations_test.h"

typedef struct infos_st
{
    char is_sychronize;
    char is_validator;
    int validator_id;
    size_t actual_height;
    size_t pdt;
    char serv_type;
}infos_st;

infos_st *ac_infos;

infos_st* get_infos();

int main()
{
    DEBUG(get_keys_test);
    DEBUG(get_keys_equality_test);
    DEBUG(verify_sign_test);
    DEBUG(network_test);
    DEBUG(block_test);
    DEBUG(validations_test);

    return 0;
}
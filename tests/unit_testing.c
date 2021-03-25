#include <unistd.h>
#include <sys/wait.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>

#include "tests_macros.h"
#include "cryptosystem/signature_test.h"
#include "cryptosystem/rsa_test.h"

int main()
{
    OpenSSL_add_all_algorithms();
    DEBUG(generate_key_test);
    DEBUG(get_keys_test);

    system("rm -rf ./.keys");
    
    DEBUG(get_generate_keys_equality_test);
    DEBUG(verify_sign_test);
    return 0;
}
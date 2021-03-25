#include "cryptosystem/rsa_test.h"

int main()
{
    generate_key_test();
    get_keys_test();

    char* msg = "Coucou bande de nouilles!";
    char* signature;
    size_t signature_len;
    sign_message(msg, &signature, &signature_len);
    printf("Sign len: %lu\n", signature_len);
    if (verify_sign(msg, signature, signature_len))
    {
        printf("Signature is true!\n");
    }
    else
    {
        printf("Signature is false. :(\n");
    }
    
    return 0;
}
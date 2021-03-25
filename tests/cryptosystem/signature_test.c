#include "tests_macros.h"
#include "cryptosystem/signature.h"


void verify_sign_test()
{
    char* msg = "Coucou bande de nouilles!";
    char* signature;
    size_t signature_len;
    sign_message(msg, &signature, &signature_len);
    if (verify_sign(msg, signature, signature_len, get_my_wallet()->pub_key))
    {
        TEST_PASSED("Message verification");
    }
    else
    {
        TEST_FAILED("Message verification","sign_message() returned 'false' for a good (msg, signature) tuple");
    }

    char* wrong_msg = "Mauvaise signature, celà doit retourner";
    if (!verify_sign(wrong_msg, signature, signature_len, get_my_wallet()->pub_key))
    {
        TEST_PASSED("Wrong message non-validation");
    }
    else
    {
        TEST_FAILED("Wrong message non-validation","sign_message() returned 'true' for a wrong (wrong_msg, signature) tuple");
    }

}
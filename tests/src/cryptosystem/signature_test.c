#include "tests_macros.h"
#include "cryptosystem/signature.h"

void verify_sign_test()
{
    char msg[] = "Coucou bande de nouilles!";
    char *signature = sign_message(msg, strlen(msg), NULL);
    if (verify_signature(msg, strlen(msg), signature, get_my_wallet()->pub_key))
    {
        TEST_PASSED("Message verification");
    }
    else
    {
        TEST_FAILED("Message verification", "sign_message() returned 'false' for a good (msg, signature) tuple");
    }

    char *wrong_msg = "Mauvaise signature, celà doit retourner";
    if (!verify_signature(wrong_msg, strlen(wrong_msg), signature, get_my_wallet()->pub_key))
    {
        TEST_PASSED("Wrong message non-validation");
    }
    else
    {
        TEST_FAILED("Wrong message non-validation", "sign_message() returned 'true' for a wrong (wrong_msg, signature) tuple");
    }
}
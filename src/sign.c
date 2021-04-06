#include "network/network.h"
#include "network/client.h"
#include "network/server.h"
#include "network/send_data.h"
#include "network/get_data.h"
#include "cryptosystem/signature.h"
#include "cryptosystem/rsa.h"
#include "cryptosystem/hash.h"

int main()
{
    Wallet *w = get_my_wallet();
    get_keys();
    char *msg = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed non risus.";
    char temp[500];
    printf("Message de départ:\n%s\n", msg);
    read(STDIN_FILENO, temp, 1);
    char *output_buffer = sha384_data(msg, strlen(msg));
    printf("Message haché:\n%s\ntaille: 96 characters\n", output_buffer);
    read(STDIN_FILENO, temp, 1);
    size_t signature_len;
    char *signature = sign_message(msg, strlen(msg), &signature_len);
    printf("Signature du message:\n");
    for (size_t i = 0; i < signature_len; i++)
    {
        printf("%c", signature[i]);
    }
    printf("\ntaille: %lu\n", signature_len);

    if (verify_signature(msg, strlen(msg), signature, signature_len, w->pub_key))
        printf("Vérification réussi\n");
    return 0;
}
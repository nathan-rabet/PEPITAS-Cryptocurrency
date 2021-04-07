#include <stdio.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define NB_FAKE_VALIDATORS 10
#define str(x) #x

void gen_validators_file(char path[])
{
    FILE *validators_file = fopen(path, "w+");
    FILE *temp = fopen(".temp_validators", "w+");

    size_t nb_validators[] = {NB_FAKE_VALIDATORS};

    srand(time(NULL));
    fwrite(&nb_validators, sizeof(size_t), 1, validators_file);

    for (size_t i = 0; i < NB_FAKE_VALIDATORS; i++)
    {

#define RSA_BEGIN_SIZE 30
#define RSA_KEY_SIZE 365

        char key_buff[RSA_KEY_SIZE];

        // Validator pkey
        BIGNUM *E = BN_new();
        BN_dec2bn(&E, "3");
        RSA *keypair = RSA_new();
        RSA_generate_key_ex(keypair, 2048, E, NULL);
        PEM_write_RSAPublicKey(temp, keypair);
        fseek(temp, RSA_BEGIN_SIZE + 1, SEEK_SET);
        fread(key_buff, sizeof(char), RSA_KEY_SIZE, temp);
        fwrite(key_buff, sizeof(char), RSA_KEY_SIZE, validators_file);
        size_t power = (size_t)rand() % 1000;

        fwrite(&power, sizeof(size_t), 1, validators_file);
        fseek(temp, 0, SEEK_SET);
    }

    fclose(temp);
    remove(".temp_validators");
    fclose(validators_file);
}
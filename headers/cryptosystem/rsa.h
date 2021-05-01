#ifndef RSA_H
#define RSA_H

#define RSA_KEY_SIZE 366
#define RSA_FILE_TOTAL_SIZE 426
#define RSA_BEGIN_SIZE 31
#define RSA_END_SIZE 29

/**
 * @brief Get the keys object
 * 
 */
void get_keys(char *password);

#endif
#ifndef SEND_DATA_H
#define SEND_DATA_H

/**
 * @brief Sends my client list to a node via 'sockfd'
 * 
 * @param sockfd The sock FD
 * @return 0 if success, -1 otherwise
 */
int send_client_list(int sockfd, char *sockip);

#endif
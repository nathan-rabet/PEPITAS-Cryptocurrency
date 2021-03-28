/**
 * @brief Send my client list to a node via 'sockfd'
 * 
 * @param sockfd The sock FD
 * @return 0 if success, else -1
 */
int send_client_list(int sockfd);
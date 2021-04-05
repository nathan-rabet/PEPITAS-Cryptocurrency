/**
 * @brief Waits a header in 'sockfd', reads it and processes it
 * 
 * @param sockfd The sock FD
 * @return 0 if sucess, -1 if not
 */
int read_header(int sockfd);

/**
 * @brief Merges my neighbours list with the one sent by 'neighbour_id'. 
 * 
 * @return 0 if sucess, -1 if not
 */
int fetch_client_list(int neighbour_id);
/**
 * @brief Waits a header in 'sockfd', reads it and processes it
 * 
 * @param sockfd The sock FD
 * @return 0 if sucess, -1 otherwise
 */
int read_header(int sockfd);

/**
 * @brief Merges my neighbours list with the one sent by 'neighbour_id'. 
 * 
 * @param neighbour_id The id of the neighbour list to merge
 * 
 * @return 0 if sucess, -1 otherwise
 */
int fetch_client_list(int neighbour_id);
#include <stdlib.h>


/**
 * @brief Push an amount on the stake.
 * 
 * This will broadcast a stake push on the network.
 * 
 * @see The stake account public key is '1'
 * 
 * @param amount The amount to push
 * @return 0 if the broadcast succeed, else return -1
 */
int push_stake(size_t amount);

/**
 * @brief Pop an amount on the stake.
 * 
 * This will broadcast a stake pop on the network.
 * 
 * @see The stake account public key is '1'
 * 
 * @param amount The amount to pop
 * @return 0 if the broadcast succeed, else return -1
 */
int pop_stake(size_t amount);
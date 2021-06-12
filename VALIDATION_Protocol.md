# PEPITAS VALIDATION PROTOCOL
## Some general definitions

### EPOCH MAN
The ID of the block creator, related to its comittee.
The ID is selected by priority order in a comittee with this rules : `min(awaken_validator_ID)`
For example if the comittee contains 10 members and the first awaken member is the third, EPOCH MAN is the third member of this comitte.

### COMITAL
If the comittee contains `n` members and the selected EPOCH MAN is the validator with the ID `m`, the comital members ID are from `0` to `m` (excluded) and from `m+1` to n (excluded).

### VALIDATOR
Members of the network who can validate and create block. Each of there **STAKE** must contains at least 50 **PEPITAS**.

### PLÈBE
All non-validators members. Each of there **STAKE** are under 50 **PEPITAS**.

### MEMPOOL
A directory where all pending transactions (transactions that are not in a block) are stocked.

### VOTE
A vote is a validator judgment about a the validity of a certain block. If a validator think that a block is valid, he will send a **postive** vote, otherwise, he will send a **negative** one. Note that the block at height 0 is considered as valid by default.

## How EPOCH MAN create a block
Lets admit that the current validated block is at height `n`.

To create a block, EPOCH MAN do several things :
### Last block validity checking
- First, he creates a new empty block (at height `n+1`).
- Then, he check if the validators votes ratio of the block at height `n`.
	- If the block at height `n` has more positive than negative votes.
		- Writes on the block at height `n+1` that the block at height `n` is valid.
		- Flushes the transactions in the block at height `n` from the mempool.
	- Else 
        - Writes on the block at height `n+1` that the block at height `n` is not valid.

### Rewards and punishments attribution
To motivate the network to do the job correctly (without corruption), EPOCH MAN will create new special transactions called *rewards* and *punishments*. Rewards are transactions that "*send*" money to a validator (actually this transaction creates money) and punishments *take of* money from a validator (this transaction delete money from an account). Before this step, EPOCH MAN checked the validators votes ratio of the block at height `n`, then, he will create rewards transactions for the majority and punishments for the others. If there is equality on votes, the block is considered as non-valid and the rule is applied.

### Sending
After all these steps, EPOCH MAN broadcast his new block.

## How COMITAL send their verdicts
1. A validator waits for a block from a validator that has a lower ID in the next comittee. If it receive one, he will start computing it.
2. Verify the validity of the received block.
3. Send a verdict.
4. Reiterate if the validator receive a block from another EPOCH MAN if his ID is lower than the previous EPOCH MAN for a certain amount of time.
# PEPITAS NETWORK PROTOCOL

## HEADERS

### Sync Headers
 1. CONNECTION TO NETWORK
 2. CONNECTION TO NODE
 3. GET BLOCKS
 4. ACTUAL HEIGHT
 5. SEND BLOCK
 6. GET PENDING TRANSACTION LIST
 7. REJECT DEMAND
### Running Headers
 7. SEND PENDING TRANSACTION
### Validating Headers
 9. SEND BLOCK EPOCHMAN
 10. SEND VOTE 

### CONNECTION TO NETWORK
**Message:**
- char * : "CONNECTION TO NETWORK\r\n\r\n"

**Description**
Send a request to be accepted by a network door.

### CONNECTION TO NODE
**Message:**
- char * : "CONNECTION TO NODE\r\n\r\n"

**Description**
Send a request to be accepted by a network node.
### GET BLOCKS
**Message:**
- char * : "GET BLOCKS\r\n\r\n"
- uint32_t : P_VERSION
- char : Number of demand (max 50)
- size_t * : Block height

**Description**
Send a request to a server for getting blocks. If the genesis block (height 0) is demand then the number of the actual blockchain height is return with "ACTUAL HEIGHT" header. If not, SEND BLOCK or REJECT DEMAND messages are returned.
### ACTUAL HEIGHT
**Message:**
- char * : "ACTUAL HEIGHT\r\n\r\n"
- size_t : Block height

**Description**
Send my actual blockchain height.
### SEND BLOCK
**Message:**
- char * : "SEND BLOCK\r\n\r\n"
- size_t : Block height
- size_t : Block size
- char * : Block struct

**Description**
The block of height demand by "GET BLOCKS".
### GET PENDING TRANSACTION LIST
**Message**
- char * : "GET PENDING TRANSACTION LIST\r\n\r\n"

**Description**
Call "SEND PENDING TRANSACTION LIST".
### SEND PENDING TRANSACTION LIST
**Message**
- char * : "GET PENDING TRANSACTION LIST\r\n\r\n"
- size_t : Number of Transaction id
- size_t : Transaction id

**Description**
Send PDT list.
### REJECT DEMAND
**Message:**
- char * : "REJECT DEMAND\r\n\r\n"

**Description**
Reject a demand if can't reply. For example a "GET BLOCKS" of a not existing block.

### GET PENDING TRANSACTION
**Message:**
- char * : "GET PENDING TRANSACTION\r\n\r\n"
- size_t : Transaction id

**Description**
Demand a PENDING TRANSACTION.
### SEND PENDING TRANSACTION
**Message:**
- char * : "SEND PENDING TRANSACTION\r\n\r\n"
- size_t : Transaction id
- size_t : Transaction struct size octet
- char * : Transaction struct

**Description**
Send the PENDING TRANSACTION demand by SEND PENDING TRANSACTION.
### SEND EPOCH BLOCK
**Message:**
- char * : "SEND EPOCH BLOCK\r\n\r\n"
- size_t : Block height
- size_t : Block struct size octet
- char * : Block struct

**Description**
Send the epoch block of a committee member.

### SEND VOTE
**Message:**

- char * : "SEND VOTE\r\n\r\n"
- char *: Epoch creator pk
- size_t : block height
- int  epoch_id: creator
- char : 0 = False 1 = True 
- char * : signature of vote precedent vars but not "SEND VOTE\r\n\r\n"

**Description**
Send the vote of a committee member.
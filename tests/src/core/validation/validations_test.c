#include "gen/GEN_validators_file.c"
#include "core/validation/validations.h"

void validations_test() {
    gen_validators_file("validators.state");

    get_next_validators();


}
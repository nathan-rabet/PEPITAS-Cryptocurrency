#include "gen/GEN_validators_file.c"
#include "core/validation/validations.h"
#include "tests_macros.h"

void validations_test()
{
    gen_validators_file("validators.state");

    if (get_next_validators() != NULL)
    {
        TEST_PASSED("Getting next validators");
    }
    else
    {
        TEST_FAILED("Getting next validators", "get_next_validators() returned NULL (error)");
    }
}
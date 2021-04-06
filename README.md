# PEPITAS

C cryptocurrency.

[![Unit testing](https://github.com/nathan-rabet/PEPITAS-Cryptocurrency/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/nathan-rabet/PEPITAS-Cryptocurrency/actions/workflows/c-cpp.yml)

## CODING STYLE
### Coding case
- *Functions*, *variables* and *filenames* must be written in `snake_case`.
- *Structures* must be written in `PascalCase`.
- *Constants* or *MACRO* must be written in `UPPER_SNAKE_CASE`.

### Tests
Each function must be tested before **marked as done**. To create a test function, you must write it in the `test/` directory and call the file `filename_test.c` and its functions `functionname_test`. Note that the test file must be at the same relative place than his real function

exemple : if you want to test `init_server()` in the file `network/client.c`, you must write the test in `test/network/client_test.c` and call the test function `init_server_test()`
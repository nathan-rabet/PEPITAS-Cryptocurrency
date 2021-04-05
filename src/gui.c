#include "ui/ui.h"

int main(int argc, char **argv)
{
    gtk_init(&argc, &argv);

    if(setup() == 1)
        return EXIT_FAILURE;

    gtk_main();

    return 0;
}

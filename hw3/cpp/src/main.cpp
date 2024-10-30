#include <cstdlib>

#include "app/App.h"


int main(int argc, char * argv[])
{
    try
    {
        App & app {App::getInstance()};
        app.run();
    }
    catch (...)
    {
        throw;
    }

    return EXIT_SUCCESS;
}

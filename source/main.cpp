#include "Application.hpp"

int main(int argc, char** argv)
{
    Application app;

    app.init(argc, argv);

    app.run();

    return 0;
}
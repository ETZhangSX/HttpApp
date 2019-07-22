#include "TarsInfoServer.h"
#include "TarsInfoImp.h"

using namespace std;

TarsInfoServer g_app;

/////////////////////////////////////////////////////////////////
void
TarsInfoServer::initialize()
{
    //initialize application here:
    //...

    addServant<TarsInfoImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".TarsInfoObj");
}
/////////////////////////////////////////////////////////////////
void
TarsInfoServer::destroyApp()
{
    //destroy application here:
    //...
}
/////////////////////////////////////////////////////////////////
int
main(int argc, char* argv[])
{
    try
    {
        g_app.main(argc, argv);
        g_app.waitForShutdown();
    }
    catch (std::exception& e)
    {
        cerr << "std::exception:" << e.what() << std::endl;
    }
    catch (...)
    {
        cerr << "unknown exception." << std::endl;
    }
    return -1;
}
/////////////////////////////////////////////////////////////////

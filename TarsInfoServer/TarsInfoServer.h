#ifndef _TarsInfoServer_H_
#define _TarsInfoServer_H_

#include <iostream>
#include "servant/Application.h"

using namespace tars;

/**
 *
 **/
class TarsInfoServer : public Application
{
public:
    /**
     *
     **/
    virtual ~TarsInfoServer() {};

    /**
     *
     **/
    virtual void initialize();

    /**
     *
     **/
    virtual void destroyApp();
};

extern TarsInfoServer g_app;

////////////////////////////////////////////
#endif

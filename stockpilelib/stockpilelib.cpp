/*
 *  stockpilelib.cpp
 *  stockpilelib
 *
 *  Created by ADodman on 13/09/2017.
 *  Copyright © 2017 Andrew Dodman. All rights reserved.
 *
 */

#include <iostream>
#include "stockpilelib.hpp"
#include "stockpilelibPriv.hpp"

void stockpilelib::HelloWorld(const char * s)
{
    stockpilelibPriv *theObj = new stockpilelibPriv;
    theObj->HelloWorldPriv(s);
    delete theObj;
};

void stockpilelibPriv::HelloWorldPriv(const char * s) 
{
    std::cout << s << std::endl;
};


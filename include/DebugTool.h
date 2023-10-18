#pragma once

#include <iostream>

#ifdef __TEST__
#define TESTOUT(msg) std::cout << msg << std::endl << std::flush;
#else
#define TESTOUT(msg)
#endif
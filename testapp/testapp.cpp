//__________________________________________________________________________________________________
//  Traintracks.io CONFIDENTIAL: Copyright (C) 2014 Traintracks.io. All Rights Reserved.
//  
//  testapp.cpp
//  
//__________________________________________________________________________________________________

#include "stdafx.h"

#include <string>

#include "util/server_types.h"

#include "util/CommonAssert.h"

#include "util/StringHelpers.h"
#include "util/StringUtils.h"
#include "util/CommonLogging.h"


#include <iostream>
#include <string>
#if !defined(__GNUC__) && defined(_WINDOWS)
#include <direct.h>
#endif
#include <stdlib.h>
#include <boost/format.hpp>


using namespace std;

//__________________________________________________________________________________________________
//  Function: _tmain
//  
//  Description: Entry function for program
//__________________________________________________________________________________________________
#if !defined(__GNUC__) && defined(_WINDOWS)
int _tmain(int argc, const TCHAR* argv[])
#else
int
main(int argc, const TCHAR *argv[])
#endif
{
  int rval = 0;

  return rval;
}


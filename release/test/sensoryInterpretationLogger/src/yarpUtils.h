/** @file yarpUtils.h 

*/

#ifndef YARPUTILS
#define YARPUTILS

#include <iostream>
#include <yarp/sig/all.h>

using namespace std;
/*
 * Custom iostream for VectorOf<int>
 */
ostream& operator<<(ostream& os, const yarp::sig::VectorOf<int>& v);

/*
 * Custom iostream for VectorOf<double>
 */
ostream& operator<<(ostream& os, const yarp::sig::VectorOf<double>& v);

#endif
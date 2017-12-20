
#include "yarpUtils.h"

/*
 * Custom iostream for VectorOf<int>
 */

ostream& operator<<(ostream& os, const yarp::sig::VectorOf<int>& v)
{
   os << "[";
	if (v.size() > 0) os << v[0];
	for (int i=0; i<v.size(); i++) os << ", " << v[i];
	os << "]";
   return os;
}

/*
 * Custom iostream for VectorOf<double>
 */

ostream& operator<<(ostream& os, const yarp::sig::VectorOf<double>& v)
{
   os << "[";
	if (v.size() > 0) os << v[0];
	for (int i=0; i<v.size(); i++) os << ", " << v[i];
	os << "]";
   return os;
}
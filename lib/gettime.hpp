#ifndef MEAVE_GETTIME_HPP_INCLUDED
#define MEAVE_GETTIME_HPP_INCLUDED

#include <cstdlib>
#include <sys/time.h>

namespace meave {

namespace {

double gettime() {
	struct timeval tv;
	if (-1 == gettimeofday(&tv, nullptr))
		::abort(); // This could only happen by some mistake in the program.
	return tv.tv_sec + tv.tv_usec/1000000.0;
}

} /* anonymouse namespace */

} /* namespace meave */

#endif // MEAVE_GETTIME_HPP_INCLUDED

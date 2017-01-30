/**
 * @file utilities.h
 * All typedefs and utility functions go here
 */

#ifndef STATE_UTILITIES_H
#define STATE_UTILITIES_H

#include <vector>

namespace state {

typedef int64_t act_id_t;
typedef std::vector<int64_t> list_act_id_t;

template<typename T>
std::vector<T> flatten(const std::vector<std::vector<T>> &orig)
{
	std::vector<T> ret;
	for(const auto &v: orig)
		ret.insert(ret.end(), v.begin(), v.end());
	return ret;
}

enum PlayerId
{
	PLAYER1 = 0,
	PLAYER2 = 1,
	LAST_PLAYER = PLAYER2,
};

}

#endif

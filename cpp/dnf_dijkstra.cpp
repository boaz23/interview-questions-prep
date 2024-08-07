// See https://www.educative.io/answers/the-dutch-national-flag-problem-in-cpp.
/*
The specification used in this source code is slightly different,
only in the values range:
* 0 -> Negative numbers (< 0)
* 1 -> 0
* 2 -> Positive numbers (> 0)
*/

#include <type_traits>
#include "utils.h"

template<
	typename T,
	typename = typename std::enable_if<
		std::is_arithmetic<T>::value,
		T
	>::type
>
void dnf1(T arr[], size_t n) noexcept {
	T *end = &arr[n];

	for (T *e = arr, *bound = arr; e < end; e++) {
		if (*e < 0) {
			std::swap(*e, *bound);
			bound++;
		}
	}

	for (T *e = end - 1, *bound = end - 1; e >= arr; e--) {
		if (*e > 0) {
			std::swap(*e, *bound);
			bound--;
		}
	}
}

template<
	typename T,
	typename = typename std::enable_if<
		std::is_arithmetic<T>::value,
		T
	>::type
>
void dnf2(T arr[], size_t n) noexcept {
	T* end;
	T *neg, *pos;
	T *zero;

	end = &arr[n];
	neg = zero = arr;
	pos = end - 1;

	// ghost arr_copy = copy(arr);
	while (zero <= pos) {
	/*
	invariant:
		(arr <= neg <= zero) &&
		(neg - 1 <= zero - 1 <= pos < end) &&
		for all (e :: (arr <= e < neg ) => *e <  0) &&
		for all (e :: (neg <= e < zero) => *e == 0) &&
		for all (e :: (pos <  e < end ) => *e >  0) &&
		multiset(arr) == multiset(arr_copy) &&
	*/
		if (*zero < 0) {
			std::swap(*zero, *neg);
			neg++;
			zero++;
		}
		else if (*zero == 0) {
			zero++;
		}
		else {
			std::swap(*zero, *pos);
			pos--;
		}
	}
}

#define _dnf dnf2

template<typename T>
void dnf(T arr[], size_t n) noexcept {
	if (n == 0) {
		return;
	}

	return _dnf(arr, n);
}

int main(int argc, char *argv[]) {
	Tester<int> tester{dnf<int>};

	tester.test(std::array<int, 0>{ }, { });
	tester.test(std::array{ 0, -1, 1, }, { -1, 0, 1, });
	tester.test(std::array{ 0, 1, -1, }, { -1, 0, 1, });
	tester.test(std::array{ 1, 1, -1, }, { -1, 1, 1, });
	tester.test(std::array{ 0, 0, 0, }, { 0, 0, 0, });
	tester.test(std::array{ 1, 1, 1, }, { 1, 1, 1, });
	tester.test(std::array{ 0, 0, 1, -1, 1, 1, -1 }, { -1, -1, 0, 0, 1, 1, 1, });
	tester.test(std::array{ 1, -1, 1, 0, 0, -1, }, { -1, -1, 0, 0, 1, 1, });

	size_t failed_count = tester.get_failed_count();
	if (failed_count == 0) {
		std::cout << "All tests passed" << std::endl;
	}

	return failed_count;
}

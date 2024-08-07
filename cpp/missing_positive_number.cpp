#include "utils.h"

// My solution
template<
	typename T,
	typename = typename std::enable_if<
		std::is_arithmetic<T>::value &&
		std::is_signed<T>::value,
		T
	>::type
>
T find_missing_positive_number1(T arr[], size_t n) noexcept {
	T *end = &arr[n];

	// ghost arr_copy = copy(arr)
	for (T *e = arr; e < end;) {
	/*
	invariant:
		for all ((i: size_t) :: (
			(ih: size_t = e - arr) =>
			(0 <= i < ih) =>
			(set(arr_copy[0..ih]).exists(i) == (arr[i] != 0))
		)
	*/
		bool inc = true;
		if ((0 < *e) && (cmp(*e, n) <= 0)) {
			T *ee = &arr[*e - 1];
			if (ee > e) {
				std::swap(*e, *ee);
				inc = false;
			}
			else {
				*e = 0;
				*ee = 1;
			}
		}
		else {
			*e = 0;
		}

		if (inc) {
			e++;
		}
	}

	size_t i;
	for (i = 0; i < n; i++) {
		if (arr[i] == 0) {
			break;
		}
	}

	return i + 1;
}

// Roee's solution
template<
	typename T,
	typename = typename std::enable_if<
		std::is_arithmetic<T>::value &&
		std::is_signed<T>::value,
		T
	>::type
>
T find_missing_positive_number2(T arr[], size_t n) noexcept {
	T *end = &arr[n];

	for (T *e = arr; e < end; e++) {
	/*
	invariant:
		for all ((i: size_t) ::
			(0 <= i < (e - arr)) =>
				((0 < arr[i] <= n) => (arr[i] == i + 1))
		)
	*/
		// This condition is the negation of the logical implication above.
		while (
			(0 < *e) && (cmp(*e, n) <= 0) &&
			(cmp(*e, (e - arr) + 1) != 0)
		) {
			std::swap(*e, arr[*e - 1]);
		}
	}

	size_t i;
	for (i = 0; i < n; i++) {
		if (cmp(arr[i], i + 1) != 0) {
			break;
		}
	}

	return i + 1;
}

#define find_missing_positive_number find_missing_positive_number2

int main(int argc, char *argv[]) {
	Tester<int> tester{std::function<int(int[], size_t)>{
		find_missing_positive_number<int>
	}};

	tester.test<0>({ }, 1);
	tester.test(std::array{ 0, 2, 3, }, 1);
	tester.test(std::array{ 0, 2, -1, 3, }, 1);
	tester.test(std::array{ 0, 3, 1, 2, }, 4);
	tester.test(std::array{ 0, 3, -1, 1, 2, }, 4);
	tester.test(std::array{ 0, 3, 1, -1, 2, }, 4);
	tester.test(std::array{ -5, -4, -1, 0, 1, 2, }, 3);

	size_t failed_count = tester.get_failed_count();
	if (failed_count == 0) {
		std::cout << "All tests passed" << std::endl;
	}

	return failed_count;
}

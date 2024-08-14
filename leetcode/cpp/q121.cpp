#include <vector>
#include <optional>
#include "arrays.hpp"
#include "numerics.hpp"

template <typename T>
using result_t = std::pair<const T*, const T*>;
template <typename T>
using result_opt_t = std::optional<result_t<T>>;

template <typename T>
struct result_wrapper {
	const T* arr;
	result_opt_t<T> result_opt;
};

template <typename T>
result_wrapper<T> findMaxProfit(const T* arr, size_t n) {
	const T* end = &arr[n];
	pointed_infable_int<T> min{pointed_infable_int<T>::pinf()};
	infable_int<T> max_delta{infable_int<T>::negative_infinity()};
	result_opt_t<T> result = std::nullopt;

	for (const T* e = arr; e < end; e++) {
		pointed_infable_int<T> current{e};
		infable_int<T> delta = current.n() - min.n();
		if (max_delta < delta) {
			max_delta = delta;
			result = result_t<T>{min.ptr(), current.ptr(),};
		}
		min = std::min(min, current);
	}

	return result_wrapper<T>{arr, result};
}

class Solution {
public:
	int maxProfit(const std::vector<int>& prices) {
		result_wrapper<int> result_wrapper = findMaxProfit(
			prices.data(), prices.size()
		);
		const result_opt_t<int> &result_opt = result_wrapper.result_opt;
		if (result_opt.has_value()) {
			const result_t<int>& result = *result_opt;
			int max_delta = *result.second - *result.first;
			return std::max(0, max_delta);
		}
		else {
			return 0;
		}
	}
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const result_wrapper<T>& result_wrapper) {
	const result_opt_t<T> &result_opt = result_wrapper.result_opt;
	if (result_opt.has_value()) {
		result_t<T> result = *result_opt;
		os
			<< result.first - result_wrapper.arr << ", "
				<< result.second - result_wrapper.arr
			<< " (" << *result.first << ", " << *result.second << ")";
	}
	else {
		os << "nullopt";
	}
	return os;
}

template <typename T, size_t N>
void printFindMaxProfit(const std::array<T, N> &arr) {
	std::cout << findMaxProfit(arr.data(), arr.size()) << std::endl;
}

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;

	ArrayTester<int, result_wrapper<int>> tester{findMaxProfit<int>};

	printFindMaxProfit(std::array<int, 0>{ });
	printFindMaxProfit(std::array{ 5, });
	printFindMaxProfit(std::array{ 1000, 11, 15, 65, 10, 20, 92, 9, 10, 22, 90, 20, });
	printFindMaxProfit(std::array{ 11, 15, 65, 10, 20, 92, 9, 10, 22, 90, 20, });
	printFindMaxProfit(std::array{ 7, 1, 5, 3, 6, 4, });
	printFindMaxProfit(std::array{ 7, 6, 4, 3, 1, });

	// size_t failed_count = tester.get_failed_count();
	// if (failed_count == 0) {
	// 	std::cout << "All tests passed" << std::endl;
	// }

	return 0;
}

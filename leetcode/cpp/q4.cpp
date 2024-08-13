#include <iostream>
#include <vector>
#include <memory>
#include "arrays.hpp"
#include "numerics.hpp"

template<typename T>
class pointed_infable_int_of_array : public pointed_infable_int<T> {
protected:
	size_t _idx_of_arr;

public:
	pointed_infable_int_of_array(
		infable_int<T> n, const T *p, size_t idx_of_arr
	) : pointed_infable_int<T>(n, p), _idx_of_arr{idx_of_arr} {}
	pointed_infable_int_of_array(const T* p, size_t idx_of_arr) :
		pointed_infable_int<T>(p), _idx_of_arr{idx_of_arr} {}
	pointed_infable_int_of_array(infable_int<T> n, size_t idx_of_arr) :
		pointed_infable_int<T>(n), _idx_of_arr{idx_of_arr} {}

	static pointed_infable_int_of_array<T> ninf(size_t idx_of_arr) {
		return pointed_infable_int_of_array{
			infable_int<T>::negative_infinity(),
			idx_of_arr
		};
	}
	static pointed_infable_int_of_array<T> pinf(size_t idx_of_arr) {
		return pointed_infable_int_of_array{
			infable_int<T>::positive_infinity(),
			idx_of_arr
		};
	}

	constexpr size_t idx_of_arr() const noexcept { return _idx_of_arr; }
};

template<typename T>
static inline pointed_infable_int_of_array<T> takeLeft(
	const T arr[], size_t n, size_t i, size_t idx_of_arr
) {
	return ((n > 0) && (i > 0)) ?
		pointed_infable_int_of_array<T>{&arr[i - 1], idx_of_arr} :
		pointed_infable_int_of_array<T>::ninf(idx_of_arr);
}

template<typename T>
static inline pointed_infable_int_of_array<T> takeRight(
	const T arr[], size_t n, size_t i, size_t idx_of_arr
) {
	return ((n > 0) && (i < n)) ?
		pointed_infable_int_of_array<T>{&arr[i], idx_of_arr} :
		pointed_infable_int_of_array<T>::pinf(idx_of_arr);
}

template <typename T>
class result_builder {
protected:
	std::vector<const T*> _med_ptrs[2];

public:
	result_builder() :
		_med_ptrs{ std::vector<const T*>{}, std::vector<const T*>{}, }
	{}

	void append(const pointed_infable_int_of_array<T>& value) {
		_med_ptrs[value.idx_of_arr()].push_back(value.ptr());
	}

	std::pair<std::vector<const T*>, std::vector<const T*>> build() {
		return std::pair{ _med_ptrs[0], _med_ptrs[1], };
	}
};

template<
	typename T,
	typename = typename std::enable_if<
		std::is_arithmetic<T>::value,
		T
	>::type
>
void mergeSortedArrays(
	const T arr1[], size_t n1,
	const T arr2[], size_t n2,
	pointed_infable_int_of_array<T> merged_p_arr[]
) {
	size_t n = n1 + n2;
	size_t i1 = 0, i2 = 0;

	for (size_t i = 0; i < n; i++) {
		pointed_infable_int_of_array<T> e1 = takeRight(arr1, n1, i1, 0);
		pointed_infable_int_of_array<T> e2 = takeRight(arr2, n2, i2, 1);
		if (e1 < e2) {
			i1++;
			merged_p_arr[i] = e1;
		}
		else {
			i2++;
			merged_p_arr[i] = e2;
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
std::pair<std::vector<const T*>, std::vector<const T*>> findMedian_simple(
	T arr1[], size_t n1,
	T arr2[], size_t n2
) {
	size_t n = n1 + n2;
	result_builder<T> result_builder{};

	if (n == 0) {
		return result_builder.build();
	}

	std::unique_ptr<pointed_infable_int_of_array<T>[]> merged = std::make_unique<
		typename decltype(merged)::element_type
	>(n);
	mergeSortedArrays(arr1, n1, arr2, n2, merged);
	size_t mid_idx = n / 2;

	result_builder.append(merged[mid_idx - 0]);
	result_builder.append(merged[mid_idx - 1]);
	return result_builder.build();
}

template<
	typename T,
	typename = typename std::enable_if<
		std::is_arithmetic<T>::value,
		T
	>::type
>
std::pair<std::vector<const T*>, std::vector<const T*>> findMedian_optimized(
	const T arr1[], size_t n1,
	const T arr2[], size_t n2
) {
	size_t n = n1 + n2;
	result_builder<T> result_builder{};

	if (n == 0) {
		return result_builder.build();
	}

	if (n1 > n2) {
		std::swap(arr1, arr2);
		std::swap(n1, n2);
	}
	/*
	The above swaps are required for correctness as well.
	Sometimes the counts in the loop can be out of bounds.
	For example: arr1=[2,]; arr2=[].
	*/

	size_t med_items_count = (n + 1) / 2;
	size_t low1 = 0, high1 = n1;

	while (low1 <= high1) {
		size_t p1_count = low1 + ((high1 - low1) / 2);
		size_t p2_count = med_items_count - p1_count;

		pointed_infable_int_of_array<T> l1 = takeLeft(arr1, n1, p1_count, 0);
		pointed_infable_int_of_array<T> l2 = takeLeft(arr2, n2, p2_count, 1);
		pointed_infable_int_of_array<T> r1 = takeRight(arr1, n1, p1_count, 0);
		pointed_infable_int_of_array<T> r2 = takeRight(arr2, n2, p2_count, 1);

		if (l1 <= r2) {
			if (l2 <= r1) {
				result_builder.append(std::max(l1, l2));
				if (n % 2 == 0) {
					result_builder.append(std::min(r1, r2));
				}

				return result_builder.build();
			}
			else {
				low1 = p1_count + 1;
			}
		}
		else {
			high1 = p1_count;
		}
	}

	throw std::runtime_error("???");
}

#define findMedian findMedian_optimized

class Solution {
private:
	double sum;
	double count;

	void append_medians_to_result(std::vector<const int*> medians) {
		for (const auto &ptr : medians) {
			sum += *ptr;
		}
		count += medians.size();
	}

public:
	double findMedianSortedArrays(const std::vector<int>& nums1, const std::vector<int>& nums2) {
		std::pair<std::vector<const int*>, std::vector<const int*>> medians = findMedian(
			nums1.data(), nums1.size(),
			nums2.data(), nums2.size()
		);

		count = 0.0;
		sum = 0.0;
		append_medians_to_result(medians.first);
		append_medians_to_result(medians.second);
		return sum / count;
	}
};

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;

	std::cout << Solution{}.findMedianSortedArrays(
		std::vector{1,3,}, std::vector{2,}
	) << std::endl;

	std::cout << Solution{}.findMedianSortedArrays(
		std::vector{1,2,}, std::vector{3,4,}
	) << std::endl;

	std::cout << Solution{}.findMedianSortedArrays(
		std::vector{2,}, std::vector<int>{}
	) << std::endl;

	return 0;
}

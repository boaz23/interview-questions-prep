#include <iostream>
#include <vector>
#include <memory>
#include "numerics.h"

template<
	typename T,
	typename = typename std::enable_if<
		std::is_arithmetic<T>::value,
		T
	>::type
>
void mergeSortedArrays(
	T arr1[], size_t n1,
	T arr2[], size_t n2,
	T *merged_p_arr[]
) {
	size_t n = n1 + n2;
	size_t i1 = 0, i2 = 0;

	for (size_t i = 0; i < n; i++) {
		T *e1 = (i1 < n1) ? &arr1[i1] : nullptr;
		T *e2 = (i2 < n2) ? &arr2[i2] : nullptr;

		merged_p_arr[i] = ((e1 == nullptr) || ((e2 != nullptr) && (*e2 < *e1))) ? e2 : e1;
	}
}

template<
	typename T,
	typename = typename std::enable_if<
		std::is_arithmetic<T>::value,
		T
	>::type
>
std::pair<T*, T*> findMedian_simple(
	T arr1[], size_t n1,
	T arr2[], size_t n2
) {
	size_t n = n1 + n2;
	if (n == 0) {
		return std::pair{ nullptr, nullptr, };
	}

	std::unique_ptr<T*[]> merged = std::make_unique<T*[]>(n);
	mergeSortedArrays(arr1, n1, arr2, n2, merged);
	size_t mid_idx = n / 2;
	if ((n % 2) == 0) {
		return std::pair{
			merged[mid_idx - 0],
			merged[mid_idx - 1],
		};
	}
	else {
		return std::pair{ merged[mid_idx], nullptr, };
	}
}

template<typename T>
static inline numerical<T> takeLeft(T arr[], size_t count) {
	return (count > 0) ? arr[count - 1] : numerical<T>::negative_infinity();
}

template<typename T>
static inline numerical<T> takeRight(T arr[], size_t n, size_t count) {
	return (count < n) ? arr[count] : numerical<T>::positive_infinity();
}

template<
	typename T,
	typename = typename std::enable_if<
		std::is_arithmetic<T>::value,
		T
	>::type
>
std::pair<T*, T*> findMedian_optimized(
	T arr1[], size_t n1,
	T arr2[], size_t n2
) {
	size_t n = n1 + n2;

	if (n == 0) {
		return std::pair{ nullptr, nullptr, };
	}

	// if (n1 > n2) {
	// 	std::swap(arr1, arr2);
	// 	std::swap(n1, n2);
	// }

	size_t med_items_count = n / 2;
	size_t low1 = 0, high1 = n1;

	while (low1 <= high1) {
		size_t p1_count = low1 + ((high1 - low1) / 2);
		size_t p2_count = med_items_count - p1_count;

		numerical<T> l1, l2, r1, r2;
		l1 = takeLeft(arr1, p1_count);
		l2 = takeLeft(arr2, p2_count);
		r1 = takeRight(arr1, n1, p1_count);
		r2 = takeRight(arr2, n2, p2_count);

		if (l1 <= r2) {
			if (l2 <= r1) {
				T *med = (l1 > l2) ? &arr1[p1_count - 1] : &arr2[p2_count - 1];
				if (n % 2 == 0) {
					T *med2 = (r1 < r2) ? &arr1[p1_count] : &arr2[p2_count];
					return std::pair{ med, med2, };
				}
				else {
					return std::pair{ med, nullptr };
				}
			}
			else {
				low1 = p1_count;
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

	void append_median_to_result(int *median) {
		if (median != NULL) {
			sum += *median;
			count++;
		}
	}

public:
	double findMedianSortedArrays(std::vector<int>& nums1, std::vector<int>& nums2) {
		std::pair<int*, int*> medians = findMedian(
			nums1.data(), nums1.size(),
			nums2.data(), nums2.size()
		);

		count = 0.0;
		sum = 0.0;
		append_median_to_result(medians.first);
		append_median_to_result(medians.second);
		return sum / count;
	}
};

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;

	return 0;
}

#include <iostream>
#include <vector>

template<
	typename T,
	typename = typename std::enable_if<
		std::is_arithmetic<T>::value,
		T
	>::type
>
std::pair<T*, T*> find_median_of_merged(
	T *arr1, size_t arr1_size,
	T *arr2, size_t arr2_size
) {
	
}

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
		std::pair<int*, int*> medians = find_median_of_merged(
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
	return 0;
}

#include <vector>
#include <unordered_map>
#include <cstddef>
#include <optional>
#include <algorithm>
#include <iostream>

template <typename T>
using result_t = std::pair<const T*, const T*>;

template <typename T>
std::optional<result_t<T>> twoSum_unorderedMap(const T arr[], size_t n, T target) {
	std::unordered_map<T, const T*> val_to_ptr_map;

	for (const T* e = arr; e < &arr[n]; e++) {
		if constexpr (!std::is_signed<T>::value) {
			if (target < *e) {
				continue;
			}
		}

		T complement = target - *e;

		auto cit = val_to_ptr_map.find(complement);
		if (cit != val_to_ptr_map.cend()) {
			return result_t<T>{(*cit).second, e};
		}

		val_to_ptr_map[*e] = e;
	}

	return std::nullopt;
}

template <typename T>
std::optional<result_t<T>> twoSum_twoPointers(
	const T arr[], size_t n, T target
) {
	std::vector<std::pair<T, const T*>> sorted{n};
	for (size_t i = 0; i < n; i++) {
		const T* e = &arr[i];
		sorted[i] = std::pair{*e, e};
	}

	std::sort(sorted.begin(), sorted.end());
	size_t left = 0, right = n - 1;
	while (left < right) {
		std::pair<T, const T*> left_p = sorted[left];
		std::pair<T, const T*> right_p = sorted[right];
		T sum = left_p.first + right_p.first;
		if (sum == target) {
			return result_t<T>{left_p.second, right_p.second};
		}
		else if (sum < target) {
			left++;
		}
		else /* sum > target */ {
			right--;
		}
	}

	return std::nullopt;
}

#define _twoSum twoSum_unorderedMap

class Solution {
public:
	std::vector<int> twoSum(std::vector<int>& nums, int target) {
		std::optional<result_t<int>> result_opt = _twoSum(
			nums.data(), nums.size(), target
		);

		std::vector<int> ans;
		if (result_opt.has_value()) {
			result_t<int> result = *result_opt;
			ans.push_back(result.first - nums.data());
			ans.push_back(result.second - nums.data());
		}

		return ans;
	}
};

int main(int argc, char* argv[]) {
	(void)argc; (void)argv;

	std::array a{ 3, 2, 4 };
	std::optional<result_t<int>> result_opt = twoSum_twoPointers(a.data(), a.size(), 6);
	if (result_opt.has_value()) {
		result_t<int> result = *result_opt;
		std::cout <<
			result.first - a.data() << ", " <<
			result.second - a.data() << std::endl;
	}
	else {
		std::cout << "nullopt" << std::endl;
	}

	return 0;
}

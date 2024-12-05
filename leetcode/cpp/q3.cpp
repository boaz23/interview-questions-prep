#include <string>
#include <unordered_map>
#include <iostream>

template <typename T>
using result_t = std::pair<const T*, size_t>;

template <typename T>
result_t<T> findLongestSubstringWithoutRepeatChars(const std::basic_string<T> &s) {
	std::unordered_map<T, const T*> lastOccurrenceMap;

	const T* start = &s[0];
	result_t<T> result{ start, 0, };
	typename std::basic_string<T>::const_iterator it;
	for (it = s.cbegin(); it != s.cend(); it++) {
		auto cit = lastOccurrenceMap.find(*it);

		if (cit != lastOccurrenceMap.cend()) {
			size_t length = it.base() - start;
			if (result.second < length) {
				result = result_t<T>{ start, length };
			}

			const T* end = cit->second;
			for (; start <= end; start++) {
				lastOccurrenceMap.erase(*start);
			}
		}

		lastOccurrenceMap[*it] = it.base();
	}

	size_t length = it.base() - start;
	if (result.second < length) {
		result = result_t<T>{ start, length };
	}

	return result;
}

class Solution {
public:
	int lengthOfLongestSubstring(std::string s) {
		return (int)findLongestSubstringWithoutRepeatChars(s).second;
	}
};

template <typename T>
struct result_printable {
	const result_t<T> &result;
	const std::basic_string<T> &s;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const result_printable<T>& printable_result) {
	const result_t<T> &result = printable_result.result;
	return os <<
		"\"" <<
		std::string_view{result.first, result.second} <<
		"\" [" << result.first - &printable_result.s[0] << ", +" << result.second << ")";
}

template <typename T>
void test(
	const std::basic_string<T> &s,
	const std::pair<size_t, size_t> &expected_result
) {
	result_t<T> result = findLongestSubstringWithoutRepeatChars(s);
	result_t<T> expected_result_r{&s[expected_result.first], expected_result.second};

	if (expected_result_r != result) {
		std::cerr << "Failed test:" << std::endl;
		std::cerr << "  input           = " << s                                         << std::endl;
		std::cerr << "  actual result   = " << result_printable<T>{result, s}            << std::endl;
		std::cerr << "  expected result = " << result_printable<T>{expected_result_r, s} << std::endl;
	}
}

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;

	test<char>("abcabcb", {0, 3});
	test<char>("abcabcbb", {0, 3});
	test<char>("", {0, 0});
	test<char>("b", {0, 1});
	test<char>("bbbbb", {0, 1});
	test<char>("pwwkew", {2, 3});

	return 0;
}

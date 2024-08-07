#include <array>
#include <iostream>
#include <iterator>
#include <functional>
#include <assert.h>

template <typename T, std::size_t N>
std::ostream& operator<<(std::ostream& o, const std::array<T, N>& arr)
{
	o << "{ ";
	std::copy(arr.cbegin(), arr.cend(), std::ostream_iterator<T>(o, ", "));
	o << " }";
	return o;
}

template<typename T>
class Tester {
private:
	size_t _failed_count;
	std::function<void (T array[], size_t n)> _func1;
	std::function<T (T array[], size_t n)> _func2;

	template<size_t N>
	void print_test(const std::array<T, N> &array) {
		std::cerr << "test: " << array << std::endl;
	}

	template<typename TResult, size_t N>
	void test_failed(
		const std::array<T, N> &array,
		TResult actual_result,
		TResult expected_result,
		const std::array<T, N> &array_modified
	) {
		std::cerr << "Failed test:" << std::endl;
		std::cerr << "  input           = " << array           << std::endl;
		std::cerr << "  actual result   = " << actual_result   << std::endl;
		std::cerr << "  expected result = " << expected_result << std::endl;
		std::cerr << "  modified array  = " << array_modified  << std::endl;
		_failed_count++;
	}

public:
	Tester(std::function<void (T *array, size_t n)> func) :
		_failed_count{0},
		_func1{func} {
	}
	Tester(std::function<T (T *array, size_t n)> func) :
		_failed_count{0},
		_func2{func} {
	}

	template<std::size_t N>
	bool test(
		const std::array<T, N> &array,
		const std::array<T, N> &expected_result
	) noexcept {
		assert(_func1);

		std::array<T, N> placeholder = array;
		_func1(placeholder.data(), N);

		if (placeholder != expected_result)
		{
			test_failed(array, placeholder, expected_result, placeholder);
			return false;
		}

		return true;
	}

	template<std::size_t N>
	bool test(const std::array<T, N> &array, T expected_result) {
		assert(_func2);

		std::array<T, N> placeholder = array;
		T result = _func2(placeholder.data(), N);

		if (result != expected_result)
		{
			test_failed(array, result, expected_result, placeholder);
			return false;
		}

		return true;
	}

	size_t get_failed_count() noexcept {
		return _failed_count;
	}
};

template <typename TInt>
[[nodiscard]] constexpr int cmp(TInt a, TInt b) noexcept {
	static_assert(std::is_integral_v<TInt>);

	return
		(a <  b) ? -1 :
		(a == b) ?  0 :
		1;
}

template <typename TIntA, typename TIntB>
[[nodiscard]] constexpr int cmp(TIntA a, TIntB b) noexcept {
	static_assert(std::is_integral_v<TIntA>);
	static_assert(std::is_integral_v<TIntB>);

	if constexpr(sizeof(TIntA) <= sizeof(TIntB)) {
		if constexpr(std::is_signed_v<TIntA> == std::is_signed_v<TIntB>) {
			return cmp(static_cast<TIntB>(a), b);
		}
		else if constexpr(std::is_signed_v<TIntA> && !std::is_signed_v<TIntB>) {
			// A is   signed
			// B is unsigned
			return (a < 0) ? -1 : cmp(static_cast<TIntB>(a), b);
		}
		else /*if constexpr(!std::is_signed_v<TIntA> && std::is_signed_v<TIntB>)*/ {
			// A is unsigned
			// B is   signed
			if constexpr(sizeof(TIntA) == sizeof(TIntB)) {
				return (b < 0) ? 1 : cmp(a, static_cast<TIntA>(b));
			}
			else {
				return cmp(static_cast<TIntB>(a), b);
			}
		}
	}
	else {
		if constexpr(std::is_signed_v<TIntA> || !std::is_signed_v<TIntB>) {
			return cmp(a, static_cast<TIntA>(b));
		}
		else if constexpr(std::is_signed_v<TIntB>) {
			// A is unsigned
			// B is   signed
			return (b < 0) ? 1 : cmp(a, static_cast<TIntA>(b));
		}
	}
}

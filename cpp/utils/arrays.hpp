#ifndef _ARRAYS_HPP
#define _ARRAYS_HPP

#include <array>
#include <vector>
#include <iostream>
#include <iterator>
#include <functional>
#include <assert.h>
#include "numerics.hpp"

template<typename T>
struct _has_const_iterator {
private:
    template<typename C> static char test(typename C::const_iterator*);
    template<typename C> static int  test(...);
public:
    enum { value = sizeof(test<T>(0)) == sizeof(char) };
};

template <
	typename TContainer,
	typename = typename std::enable_if<_has_const_iterator<TContainer>::value, void>::type
>
std::ostream& operator<<(std::ostream& os, const TContainer& container) {
	os << "{ ";
	std::copy(
		container.cbegin(),
		container.cend(),
		std::ostream_iterator<typename TContainer::value_type>(os, ", ")
	);
	os << "}";
	return os;
}

template<typename T>
class pointed_infable_int {
protected:
	infable_int<T> _n;
	const T *_p;

public:
	pointed_infable_int(infable_int<T> n, const T *p) : _n{n}, _p{p} {}
	pointed_infable_int(const T* p) : pointed_infable_int(*p, p) {}
	pointed_infable_int(infable_int<T> n) : pointed_infable_int(n, nullptr) {}

	static pointed_infable_int<T> ninf() {
		return pointed_infable_int{infable_int<T>::negative_infinity()};
	}
	static pointed_infable_int<T> pinf() {
		return pointed_infable_int{infable_int<T>::positive_infinity()};
	}

	constexpr const infable_int<T>& n() const noexcept { return _n; }
	constexpr const T* ptr() const noexcept { return _p; }
};

template<typename T> inline constexpr bool operator< (const pointed_infable_int<T>& v1, const pointed_infable_int<T>& v2) { return v1.n() <  v2.n(); }
template<typename T> inline constexpr bool operator> (const pointed_infable_int<T>& v1, const pointed_infable_int<T>& v2) { return v1.n() >  v2.n(); }
template<typename T> inline constexpr bool operator<=(const pointed_infable_int<T>& v1, const pointed_infable_int<T>& v2) { return v1.n() <= v2.n(); }
template<typename T> inline constexpr bool operator>=(const pointed_infable_int<T>& v1, const pointed_infable_int<T>& v2) { return v1.n() >= v2.n(); }

template<typename T, typename TResult>
class _ArrayTesterImplBase
{
public:
	using func_type = std::function<TResult (T array[], size_t n)>;

protected:
	size_t _failed_count;
	func_type _func;

	template<size_t N>
	void print_test_input(const std::array<T, N> &array) {
		std::cerr << "test: " << array << std::endl;
	}

	template<typename _TResult, size_t N>
	void test_failed(
		const std::array<T, N> &array,
		_TResult actual_result,
		_TResult expected_result
	) {
		std::cerr << "Failed test:" << std::endl;
		std::cerr << "  input           = " << array           << std::endl;
		std::cerr << "  actual result   = " << actual_result   << std::endl;
		std::cerr << "  expected result = " << expected_result << std::endl;
		_failed_count++;
	}

	template<typename _TResult, size_t N>
	void test_failed(
		const std::array<T, N> &array,
		_TResult actual_result,
		_TResult expected_result,
		const std::array<T, N> &array_modified
	) {
		test_failed(array, actual_result, expected_result);
		std::cerr << "  modified array  = " << array_modified  << std::endl;
	}

public:
	_ArrayTesterImplBase(func_type func) : _failed_count{0}, _func{func} {}

	size_t get_failed_count() noexcept {
		return _failed_count;
	}
};

template<typename T, typename TResult>
class ArrayTester : public _ArrayTesterImplBase<T, TResult> {
public:
	ArrayTester(typename _ArrayTesterImplBase<T, TResult>::func_type func) :
		_ArrayTesterImplBase<T, TResult>(func) {}

	template<std::size_t N>
	bool test(
		const std::array<T, N> &array,
		const TResult &expected_result
	) noexcept {
		assert(this->_func);

		std::array<T, N> placeholder = array;
		TResult result = this->_func(placeholder.data(), N);

		if (result != expected_result)
		{
			this->test_failed(array, result, expected_result, placeholder);
			return false;
		}

		return true;
	}
};

template<typename T>
class ArrayTester<T, std::void_t<>> :
	public _ArrayTesterImplBase<T, std::void_t<>> {
public:
	ArrayTester(typename _ArrayTesterImplBase<T, std::void_t<>>::func_type func) :
		_ArrayTesterImplBase<T, std::void_t<>>(func) {}

	template<std::size_t N>
	bool test(
		const std::array<T, N> &array,
		const std::array<T, N> &expected_result
	) noexcept {
		assert(this->_func);

		std::array<T, N> placeholder = array;
		this->_func(placeholder.data(), N);

		if (placeholder != expected_result)
		{
			this->test_failed(array, placeholder, expected_result);
			return false;
		}

		return true;
	}
};

#endif

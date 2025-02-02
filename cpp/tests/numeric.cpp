#include <iostream>
#include <functional>
#include "numerics.hpp"

#define print_test_value(_v)\
	std::cout << #_v << " = " << (_v) << std::endl;

#define print_div_mode_result(_n1, _n2, _div, _mod)\
	std::cout << \
		_n1 << " /% " << _n2 << " = " << \
		_div(_n1, _n2) << ", " << _mod(_n1, _n2) << \
		std::endl;

#define print_div_mode_result_cycle(_n1, _n2, _div, _mod)\
	do {\
		print_div_mode_result(_n1, _n2, _div, _mod);\
		print_div_mode_result(_n1, -(_n2), _div, _mod);\
		print_div_mode_result(-(_n1), _n2, _div, _mod);\
		print_div_mode_result(-(_n1), -(_n2), _div, _mod);\
	} while(0)

static inline void test_print_div_mode(int a, int b) {
	std::function<int(int, int)> trunc_div = [](int n1, int n2) {
		return n1 / n2;
	};
	std::function<int(int, int)> trunc_mod = [](int n1, int n2) {
		return n1 % n2;
	};

	std::function<int(int, int)> floor_div = [](int n1, int n2) {
		int q = n1 / n2;
		if ((n1 % n2 != 0) && (q < 0)) {
			q--;
		}
		return q;
	};
	std::function<int(int, int)> floor_mod = [](int n1, int n2) {
		int r = n1 % n2;
		if ((r != 0) && ((n1 < 0) != (n2 < 0))) {
			r += n2;
		}
		return r;
	};

	std::function<int(int, int)> euclid_div = [&floor_div](int n1, int n2) {
		return signum(n2) * floor_div(n1, std::abs(n2));
	};
	std::function<int(int, int)> euclid_mod = [&floor_div](int n1, int n2) {
			return n1 - (std::abs(n2) * floor_div(n1, std::abs(n2)));
	};

	print_div_mode_result_cycle(a, b, trunc_div, trunc_mod);
	print_div_mode_result_cycle(a, b, floor_div, floor_mod);
	print_div_mode_result_cycle(a, b, euclid_div, euclid_mod);
}

template<
	typename T,
	typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
>
static inline constexpr T div_mod_rev(const T &n1, const T &n2) {
	return ((n1 / n2) * n2) + (n1 % n2);
}

template <typename T> void foo(T);

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;

	infable_int<int> inf = infable_int<int>::positive_infinity();
	infable_int<int> ninf = infable_int<int>::negative_infinity();

	print_test_value(infable_int<long>{INT64_MAX} < infable_int<long>::positive_infinity());
	print_test_value(infable_int<int>{3635} + infable_int<int>::positive_infinity());
	try {
		print_test_value(infable_int<long>::negative_infinity() + infable_int<long>::positive_infinity());
	}
	catch (const undefined_arithmetic_value &e) {
		std::cout << e.what() << std::endl;
	}
	std::cout << std::endl;

	print_test_value(std::is_signed<infable_int<int>>::value);
	print_test_value(std::is_signed<infable_int<unsigned int>>::value);
	print_test_value(std::is_signed<numerical_value_kind_t>::value);
	print_test_value(std::is_arithmetic<infable_int<int>>::value);
	print_test_value(std::is_arithmetic<infable_int<unsigned int>>::value);
	print_test_value(std::is_arithmetic<int>::value);
	print_test_value(std::is_arithmetic<unsigned int>::value);
	print_test_value(signum(-20));
	print_test_value(signum(0));
	print_test_value(signum(6));
	print_test_value(signum(inf));
	print_test_value(signum(ninf));
	print_test_value(signum(NUMBERICAL_VALUE_KIND_NEGATIVE_INFINITY));
	print_test_value(signum(NUMBERICAL_VALUE_KIND_VALUE));
	print_test_value(signum(NUMBERICAL_VALUE_KIND_POSITIVE_INFINITY));
	print_test_value(NUMBERICAL_VALUE_KIND_NEGATIVE_INFINITY);
	print_test_value(infable_int<int>::positive_infinity());
	print_test_value(infable_int<int>::negative_infinity());
	print_test_value(-infable_int<int>::positive_infinity());
	print_test_value(-infable_int<int>::negative_infinity());
	print_test_value(-infable_int<int>{10});
	print_test_value(-infable_int<int>{-12});
	print_test_value(std::abs(infable_int<int>{10}));
	print_test_value(std::abs(infable_int<int>{-12}));
	print_test_value(std::abs(inf));
	print_test_value(std::abs(ninf));
	print_test_value(static_cast<infable_int<int>>((int)-8));
	print_test_value(static_cast<int>(infable_int<int>{-8}));
	print_test_value(infable_int<int>{40} + infable_int<int>{18});
	print_test_value(infable_int<int>{40} % infable_int<int>{18});
	print_test_value(infable_int<int>{40} / inf);
	print_test_value(infable_int<int>{40} % inf);
	print_test_value(inf / infable_int<int>{7});
	print_test_value(inf % infable_int<int>{7});
	print_test_value(inf / infable_int<int>{-7});
	print_test_value(inf % infable_int<int>{-7});
	print_test_value(ninf / infable_int<int>{7});
	print_test_value(ninf % infable_int<int>{7});
	print_test_value(ninf / infable_int<int>{-7});
	print_test_value(ninf % infable_int<int>{-7});
	print_test_value(div_mod_rev(infable_int<int>{24},  infable_int<int>{7}));
	print_test_value(div_mod_rev(inf,  infable_int<int>{7}));
	print_test_value(div_mod_rev(inf,  infable_int<int>{-7}));
	print_test_value(div_mod_rev(ninf, infable_int<int>{7}));
	print_test_value(div_mod_rev(ninf, infable_int<int>{-7}));
	infable_int<int> a = 18;
	a += 2;
	print_test_value(a);

	std::cout << std::endl;
	test_print_div_mode(5, 3);

	// print_test_value(div_mod_rev(ninf, 7));
	// print_test_value(infable_int<int>{40} + 18);
	// print_test_value(typeid(infable_int<int>{40} + 18).name());
	// print_test_value(ninf + 0);
	// print_test_value(cmp(1u, ninf));

	return 0;
}

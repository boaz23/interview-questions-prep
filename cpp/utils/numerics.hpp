#ifndef _NUMERICS_HPP
#define _NUMERICS_HPP

#include <type_traits>
#include <string>
#include <exception>
#include <complex>
#include <iostream>
#include <sstream>

class bad_state_exception : public std::logic_error {
public:
	bad_state_exception() = default;
	explicit bad_state_exception(const std::string &msg) :
		std::logic_error(msg) {};
	virtual ~bad_state_exception() = default;

	const char* what() const noexcept override {
		return "Object was in bad state.";
	}
};

class bad_numeric_access_exception : public std::logic_error {
public:
	bad_numeric_access_exception() = default;
	explicit bad_numeric_access_exception(const std::string &msg) :
		std::logic_error(msg) {};
	virtual ~bad_numeric_access_exception() = default;
};

class undefined_arithmetic_value : public std::exception {
protected:
	const std::string _basic_message =
		"An arithmetic mathematical operation has resulted in an undefined value";

	std::string _message;

public:
	undefined_arithmetic_value() : _message{_basic_message} {
		_message.append(".");
	}

	explicit undefined_arithmetic_value(const std::string &message) :
		_message{_basic_message} {
		_message
			.append(": ")
			.append(message)
			.append(".");
	}

	virtual ~undefined_arithmetic_value() = default;

	const char* what() const noexcept override {
		return _message.c_str();
	}
};

template <
	typename T,
	bool is_signed = std::is_signed<T>::value
> inline constexpr
int signum(T x) {
	if constexpr (is_signed) {
		return (T(0) < x) - (x < T(0));
	}
	else {
		return (T(0) < x);
	}
}

typedef enum numerical_value_kind {
	NUMBERICAL_VALUE_KIND_NEGATIVE_INFINITY = -1,
	NUMBERICAL_VALUE_KIND_VALUE,
	NUMBERICAL_VALUE_KIND_POSITIVE_INFINITY,
} numerical_value_kind_t;

template<
	typename T,
	typename = typename std::enable_if<std::is_integral<T>::value, T>::type
>
class infable_int {
protected:
	int _value_kind;
	T _value;

	static inline std::string build_undefined_value_exception_message(
		const infable_int<T>& v1,
		const infable_int<T>& v2,
		char c
	) {
		std::ostringstream oss{};
		oss << v1 << " " << c << " " << v2;
		return std::move(oss.str());
	}

public:
	infable_int(numerical_value_kind_t value_kind) :
		_value_kind{signum((int)value_kind)},
		_value{}
	{}

	infable_int(int value_kind, T value) :
		_value_kind{signum(value_kind)},
		_value{value}
	{}

	infable_int() : infable_int(NUMBERICAL_VALUE_KIND_VALUE) {}
	infable_int(T value) : infable_int(NUMBERICAL_VALUE_KIND_VALUE, value) {}

	static constexpr infable_int<T> positive_infinity() {
		return infable_int(NUMBERICAL_VALUE_KIND_POSITIVE_INFINITY);
	}

	static constexpr infable_int<T> negative_infinity() {
		static_assert(std::is_signed<T>::value);

		return infable_int(NUMBERICAL_VALUE_KIND_NEGATIVE_INFINITY);
	}

	constexpr inline int value_kind() const noexcept { return _value_kind; }
	constexpr inline bool is_finite() const noexcept {
		return value_kind() == 0;
	}

	constexpr const T*  operator->() const   noexcept { return &_value; }
	constexpr       T*  operator->()         noexcept { return &_value; }
	constexpr const T&  operator* () const&  noexcept { return _value;  }
	constexpr       T&  operator* ()      &  noexcept { return _value;  }
	constexpr const T&& operator* () const&& noexcept { return std::move(_value); }
	constexpr       T&& operator* ()      && noexcept { return std::move(_value); }

	constexpr inline explicit operator T() const {
		if (!is_finite()) {
			throw bad_numeric_access_exception("Cannot convert infinite number to finite number");
		}

		return _value;
	}

	infable_int<T>& operator+=(const infable_int<T>& value) {
		if (is_finite()) {
			if (is_finite()) {
				_value += value._value;
			}

			/*
			finite + any = <kind of any> #(preseves "infinity-ness"):
				* finite + (-inf) = -inf
				* finite + finite = finite
				* finite + inf    = inf
			*/
			_value_kind = value.value_kind();
		}
		else if (value_kind() == -value.value_kind()) {
			// inf + (-inf) = undefined
			throw undefined_arithmetic_value(
				build_undefined_value_exception_message(*this, value, '+')
			);
		}
		else {
			// inf + finite = inf #(changes nothing)
		}

		return *this;
	}

	infable_int<T>& operator*=(const infable_int<T>& value) {
		if (is_finite() && value.is_finite()) {
			_value *= value._value;
		}
		else if (
			(*this == infable_int<T>{0}) ||
			(value == infable_int<T>{0})
		) {
			// inf * 0 = undefined
			throw undefined_arithmetic_value(
				build_undefined_value_exception_message(*this, value, '*')
			);
		}

		if (is_finite()) {
			// finite * inf = inf
			_value_kind = value.value_kind() * signum(*this);
		}
		else {
			// inf * finite = inf #(sign behaves like normal multiplication)
			_value_kind *= signum(value);
		}

		return *this;
	}

	inline constexpr infable_int<T>& operator-=(const infable_int<T>& value) {
		return *this += (-value);
	}

	infable_int<T>& operator/=(const infable_int<T>& value) {
		if (value == infable_int<T>{0}) {
			throw undefined_arithmetic_value("Division by 0");
		}

		if (is_finite()) {
			if (value.is_finite()) {
				_value /= value._value;
			}
			else {
				// finite / inf = 0
				_value = 0;
			}
		}
		else if (!value.is_finite()) {
			// inf / inf = undefined
			throw undefined_arithmetic_value(
				build_undefined_value_exception_message(*this, value, '/')
			);
		}
		else {
			// inf / finite = inf #(sign behaves like normal division)
			_value_kind *= signum(value);
		}

		return *this;
	}

	infable_int<T>& operator%=(const infable_int<T>& value) {
		if (value == infable_int<T>{0}) {
			// any / 0 = undefined
			throw undefined_arithmetic_value("Division by 0");
		}

		if (is_finite()) {
			if (value.is_finite()) {
				_value %= value._value;
			}
			else {
				// finite % inf = 0
				_value = 0;
			}
		}
		else if (!value.is_finite()) {
			// inf % inf = undefined
			throw undefined_arithmetic_value(
				build_undefined_value_exception_message(*this, value, '%')
			);
		}

		return *this;
	}
};

template<typename T> struct std::is_signed<infable_int<T>>     : std::is_signed<T> {};
template<typename T> struct std::is_arithmetic<infable_int<T>> : std::is_arithmetic<T> {};

template<typename T> inline constexpr
bool operator==(const infable_int<T>& n1, const infable_int<T>& n2) {
	if (n1.value_kind() != n2.value_kind()) {
		return false;
	}

	if (n1.is_finite()) {
		return *n1 == *n2;
	}

	return true;
}

template<typename T> inline constexpr
bool operator!=(const infable_int<T>& n1, const infable_int<T>& n2) {
	return !(n1 == n2);
}

template<typename T> inline
bool operator<(const infable_int<T>& n1, const infable_int<T>& n2) {
	if (n1.is_finite() && n2.is_finite()) {
		return *n1 < *n2;
	}

	return n1.value_kind() < n2.value_kind();
}

template<typename T> inline constexpr bool operator> (const infable_int<T>& n1, const infable_int<T>& n2) { return   n2 < n1;  }
template<typename T> inline constexpr bool operator<=(const infable_int<T>& n1, const infable_int<T>& n2) { return !(n1 > n2); }
template<typename T> inline constexpr bool operator>=(const infable_int<T>& n1, const infable_int<T>& n2) { return !(n1 < n2); }

template<typename T> inline constexpr
infable_int<T> operator+(const infable_int<T>& n) { return n; }

template<typename T> inline constexpr
infable_int<T> operator-(const infable_int<T>& n) {
	static_assert(std::is_signed<T>::value);

	return infable_int<T>{
		-n.value_kind(),
		-(*n),
	};
}


namespace std {
	template <typename T>
	inline constexpr infable_int<T> abs(const infable_int<T>& n) {
		return infable_int<T>{
			std::abs(n.value_kind()),
			std::abs(*n),
		};
	}
}

#ifdef _DEFINE_BIN_OP
#error "Macro `_DEFINE_BIN_OP` is already defined"
#else
#define _DEFINE_BIN_OP(_op)\
template<typename T> inline constexpr const \
infable_int<T> operator _op(const infable_int<T>& n1, const infable_int<T>& n2) {\
	infable_int<T> placeholder = n1;\
	placeholder _op##= n2;\
	return placeholder;\
}
#endif

_DEFINE_BIN_OP(+)
_DEFINE_BIN_OP(-)
_DEFINE_BIN_OP(*)
_DEFINE_BIN_OP(/)
_DEFINE_BIN_OP(%)

#undef _DEFINE_BIN_OP

template<typename T, typename TChar, class Traits>
std::basic_ostream<TChar, Traits>& operator<<(
	std::basic_ostream<TChar, Traits>& os,
	const infable_int<T>& n
) {
	if (n.is_finite()) {
		os << *n;
	}
	else {
		if (n < infable_int<T>{0}) {
			os << "-";
		}
		os << "inf";
	}

	return os;
}

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
	static_assert(std::is_integral<TIntA>::value);
	static_assert(std::is_integral<TIntB>::value);

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
		else /* if constexpr(std::is_signed_v<TIntB>) */ {
			// A is unsigned
			// B is   signed
			return (b < 0) ? 1 : cmp(a, static_cast<TIntA>(b));
		}
	}
}

#endif

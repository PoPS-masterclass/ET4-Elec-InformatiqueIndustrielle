#pragma once

#include "TInteger.h"

template<size_t base = 10>
class TFract {
	TInteger<base> num, denom;

	static TInteger<base> Pgcd(TInteger<base> a, TInteger<base> b) {
		TInteger<base> r;

		while (b > 0) {
			r = a%b;
			a = b;
			b = r;
		}

		return a;
	}

	void Normalize() {
		TInteger<base> div = Pgcd(num, denom);
		num /= div;
		denom /= div;
	}

public:
	TFract(long long int val = 0) : num(val), denom(1) {
		Normalize();
	}

	TFract(long long int _num, long long int _denom) : num(_num), denom(_denom) {
		if (denom == 0) {
			throw TNaturalBase::InvalidInitializer("Fraction denominator is null");
		}

		Normalize();
	}

	TFract(const char *_num, const char *_denom) : num(_num), denom(_denom) {
		if (denom == 0) {
			throw TNaturalBase::InvalidInitializer("Fraction denominator is null");
		}

		Normalize();
	}

	bool operator<(const TFract<base>& op) const {
		return op > *this;
	}

	bool operator>(const TFract<base>& op) const {
		if ((denom < 0) ^ (op.denom < 0)) {
			return num*op.denom < op.num*denom;
		} else {
			return num*op.denom > op.num*denom;
		}
	}

	bool operator==(const TFract<base>& op) const {
		return num == op.num && denom == op.denom;
	}

	bool operator!=(const TFract<base>& op) const {
		return !(*this == op);
	}

	bool operator<=(const TFract<base>& op) const {
		return *this < op || *this == op;
	}

	bool operator>=(const TFract<base>& op) const {
		return *this > op || *this == op;
	}

	TFract<base>& operator+=(const TFract<base>& op) {
		num = num*op.denom+op.num*denom;
		denom = denom*op.denom;
		Normalize();
		return *this;
	}

	TFract<base> operator+(const TFract<base>& op) const {
		TFract<base> ret(*this);
		return ret += op;
	}

	TFract<base>& operator*=(const TFract<base>& op) {
		num *= op.num;
		denom *= op.denom;
		Normalize();
		return *this;
	}

	TFract<base> operator-=(const TFract<base>& op) {
		num = num*op.denom-op.num*denom;
		denom = denom*op.denom;
		Normalize();
		return *this;
	}

	TFract<base> operator-(const TFract<base>& op) const {
		TFract<base> ret(*this);
		return ret -= op;
	}

	TFract<base> operator*(const TFract<base>& op) const {
		TFract<base> ret(*this);
		return ret *= op;
	}

	TFract<base>& operator/=(const TFract<base>& op) {
		num *= op.denom;
		denom *= op.num;
		Normalize();
		return *this;
	}

	TFract<base> operator/(const TFract<base>& op) const {
		TFract<base> ret(*this);
		return ret /= op;
	}

	std::ostream& Show(std::ostream& os) const {
		os << num << "/" << denom;
		return os;
	}
};

template<size_t base = 10>
std::ostream& operator<<(std::ostream& os, const TFract<base>& frac) {
	return frac.Show(os);
}
#pragma once

#include "TNatural.h"

template<size_t base = 10>
class TInteger : public TNatural<base> {
	static long long int abs(long long int val) {
		return (val < 0) ? -val : val;
	}

	static const char* skipSignChar(const char* str) {
		if (*str == '+' || *str == '-') {
			return str+1;
		} else {
			return str;
		}
	}

protected:
	bool sign; // true == negative, false == positive
	// zero is ALWAYS encoded as a positive number for consistency reasons

public:
	TInteger(long long int val = 0) : TNatural<base>((unsigned long long int)TInteger<base>::abs(val)) {
		sign = val < 0;
	}

	TInteger(int val) : TInteger((long long int)val) {

	}

	TInteger(const char *str) : TNatural<base>(skipSignChar(str)) {
		sign = *str == '-';
	}

	TInteger(const TNatural<base>& tn) : TNatural<base>(tn) {

	}

	bool operator<(const TInteger<base>& op) const {
		// Handle cases where sign is different
		if (sign && !op.sign) {
			return true;
		} else if (!sign && op.sign) {
			return false;
		}

		// Same sign
		if (sign) {
			return this->TNatural<base>::operator>(op);
		} else {
			return this->TNatural<base>::operator<(op);
		}
	}

	bool operator>(const TInteger<base>& op) const {
		return op < *this;
	}

	bool operator==(const TInteger<base>& op) const {
		if (sign != op.sign) {
			return false;
		} else {
			return op.TNatural<base>::operator==(*this);
		}
	}

	bool operator!=(const TInteger<base>& op) const {
		return !(op == *this);
	}

	TInteger<base> operator+(const TInteger<base>& op) const {
		TInteger<base> ret;

		if (sign == op.sign) {
			ret = op.TNatural<base>::operator+(*this);
			ret.sign = sign;
		} else {
			const TInteger<base>& biggest = (op.TNatural<base>::operator>(*this)) ? op : *this;
			const TInteger<base>& smallest = (op.TNatural<base>::operator>(*this)) ? *this : op;

			ret = biggest.TNatural<base>::operator-(smallest);
			ret.sign = biggest.sign;
		}

		return ret;
	}

	TInteger<base>& operator+=(const TInteger<base>& op) {
		*this = *this+op;
		return *this;
	}

	TInteger<base> operator-(TInteger<base> op) const {
		op.sign = !op.sign;
		return *this+op;
	}

	TInteger<base>& operator-=(const TInteger<base>& op) {
		*this = *this-op;
		return *this;
	}

	TInteger<base> operator*(const TInteger<base>& op) const {
		TInteger<base> ret = op.TNatural<base>::operator*(*this);
		ret.sign = op.sign ^ this->sign;
		return ret;
	}

	TInteger<base> operator*=(const TInteger<base>& op) {
		this->TNatural<base>::operator*=(op);
		this->sign ^= op.sign;
		return *this;
	}

	TInteger<base> operator/(const TInteger<base>& op) const {
		TInteger<base> ret = op.TNatural<base>::operator/(*this);
		ret.sign = op.sign ^ this->sign;
		return ret;
	}

	TInteger<base> operator/=(const TInteger<base>& op) {
		this->TNatural<base>::operator/=(op);
		this->sign ^= op.sign;
		return *this;
	}

	TInteger<base> operator%(const TInteger<base>& op) const {
		TInteger<base> ret(*this);

		while (op >= ret) {
			ret -= op;
		}

		return ret;
	}

	std::ostream& Show(std::ostream& os) const {
		if (sign) {
			os << "-";
		}

		return TNatural<base>::Show(os);
	}
};

template<size_t base = 10>
std::ostream& operator<<(std::ostream& os, const TInteger<base>& ti) {
	return ti.Show(os);
}

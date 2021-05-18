#pragma once

#include <cstring>
#include <vector>
#include "TNaturalBase.h"

inline std::ostream& operator<<(std::ostream& os, TNaturalBase::InvalidCharacterInBaseException& e) {
	return e.Show(os);
}

template<size_t base = 10>
class TNatural {
	static TNaturalBase::TRadix char2digit(char c) {
		if (isdigit(c)) {
			return c - '0';
		} else if (isalpha(c)) {
			return tolower(c) - 'a' + 10;
		} else {
			throw TNaturalBase::InvalidCharacterInBaseException("The character is not alphanumerical");
		}
	}

	static char digit2char(TNaturalBase::TRadix d) {
		if (d < 10) {
			return d+'0';
		} else {
			return d-10+'A';
		}
	}

	void CleanLeadingZeroes() {
		std::vector<TNaturalBase::TRadix>::const_iterator it = number.end();
		while (it != number.begin()) {
			it--;
			if (*it != 0) {
				break;
			} else {
				number.erase(it);
				it = number.end();
			}
		}

		if (number.size() == 0) {
			number.push_back(0);
		}
	}

	TNatural<base>& ShiftLeft() {
		number.insert(number.begin(), 0);
		return *this;
	}

	TNatural<base>& ShiftRight() {
		// TODO
		return *this;
	}

	static size_t max(size_t a, size_t b) {
		return (a > b) ? a : b;
	}

protected:
	std::vector<TNaturalBase::TRadix> number;

public:
	TNatural(int val) : TNatural((unsigned long long int)val) {

	}

	TNatural(unsigned long long int val = 0) {
		// Conversion to the template's base
		do {
			number.push_back(val%base);
			val /= base;
		} while (val > 0);
	}

	TNatural(const char* str) {
		// Remove leading zeros
		while (*str == '0') {
			str++;
		}

		for (size_t i = 0; i < strlen(str); i++) {
			TNaturalBase::TRadix digit = char2digit(str[i]);

			if (digit >= base) {
				throw TNaturalBase::InvalidCharacterInBaseException("The represented digit is above the maximum for the specified base");
			}

			number.insert(number.begin(), digit);
		}

		if (number.size() == 0) {
			number.push_back(0);
		}
	}

	bool operator<(const TNatural<base>& op) const {
		size_t biggestSize = max(op.number.size(), number.size());
		for (size_t i = biggestSize; i > 0; i--) {
			size_t idx = i-1;

			TNaturalBase::TRadix thisDigit = (idx >= number.size()) ? 0 : number[idx];
			TNaturalBase::TRadix opDigit = (idx >= op.number.size()) ? 0 : op.number[idx];

			if (thisDigit > opDigit) {
				return false;
			} else if (opDigit > thisDigit) {
				return true;
			}
		}

		return false;
	}

	bool operator>(const TNatural<base>& op) const {
		return op < *this;
	}

	bool operator==(const TNatural<base>& op) const {
		size_t biggestSize = max(op.number.size(), number.size());
		for (size_t i = biggestSize; i > 0; i--) {
			TNaturalBase::TRadix thisDigit, opDigit;

			size_t idx = i-1;

			if (idx >= number.size()) {
				thisDigit = 0;
			} else {
				thisDigit = number[idx];
			}
			if (idx >= op.number.size()) {
				opDigit = 0;
			} else {
				opDigit = op.number[idx];
			}

			if (thisDigit != opDigit) {
				return false;
			}
		}

		return true;
	}

	bool operator!=(const TNatural<base>& op) const {
		return !(*this == op);
	}

	bool operator<=(const TNatural<base>& op) const {
		return (*this<op) || (*this == op);
	}

	bool operator>=(const TNatural<base>& op) const {
		return (*this>op) || (*this == op);
	}

	TNatural<base>& operator+=(const TNatural<base>& op) {
		std::vector<TNaturalBase::TRadix>::const_iterator opIt = op.number.begin();
		std::vector<TNaturalBase::TRadix>::iterator thisIt = number.begin();
		TNaturalBase::TRadix carry = 0;
		while (opIt != op.number.end() || thisIt != number.end() || carry > 0) {
			TNaturalBase::TRadix res;
			if (opIt != op.number.end() && thisIt != number.end()) {
				res = (*opIt+*thisIt+carry)%base;
				carry = (*opIt+*thisIt+carry)/base;
				*thisIt = res;
			} else if (thisIt != number.end()) {
				res = (*thisIt+carry)%base;
				carry = (*thisIt+carry)/base;
				*thisIt = res;
			} else if (opIt != op.number.end()) {
				res = (*opIt+carry)%base;
				carry = (*opIt+carry)/base;
				number.push_back(res);
				// The above statement will make thisIt not being equal to number.end()
				// so we have to fix that
				thisIt = number.end();
			} else {
				if (carry > 0) {
					number.push_back(carry);
					carry = 0;
					thisIt = number.end();
				}
			}

			if (opIt != op.number.end()) {
				opIt++;
			}

			if (thisIt != number.end()) {
				thisIt++;
			}
		}

		return *this;
	}

	TNatural<base>& operator++() {
		return *this += 1;
	}

	TNatural<base> operator++(int) {
		TNatural<base> thisCpy(*this);
		++(*this);
		return thisCpy;
	}

	
	TNatural<base> operator+(const TNatural<base>& op) const {
		TNatural<base> ret(*this);
		ret += op;
		return ret;
	}

	TNatural<base>& operator-=(const TNatural<base>& op) {
		if (*this < op) {
			throw TNaturalBase::ResultException("Result is negative (natural number)");
		}

		std::vector<TNaturalBase::TRadix>::const_iterator opIt = op.number.begin();
		std::vector<TNaturalBase::TRadix>::iterator thisIt = number.begin();
		TNaturalBase::TRadix borrow = 0;

		while (opIt != op.number.end() || thisIt != number.end() || borrow > 0) {
			TNaturalBase::TRadix res = 0;
			TNaturalBase::TRadix thisDigit = (thisIt != number.end()) ? *thisIt : 0;
			TNaturalBase::TRadix opDigit = (opIt != op.number.end()) ? *opIt : 0;
			if (thisDigit < (opDigit+borrow)) {
				res = base+thisDigit-opDigit-borrow;
				borrow = 1;
			} else {
				res = thisDigit-opDigit-borrow;
				borrow = 0;
			}
			*thisIt = res;

			if (opIt != op.number.end()) {
				opIt++;
			}

			if (thisIt != number.end()) {
				thisIt++;
			}
		}

		CleanLeadingZeroes();

		return *this;
	}

	TNatural<base> operator-(const TNatural<base>& op) const {
		TNatural<base> ret(*this);
		return ret -= op;
	}

	TNatural<base>& operator*=(const TNatural<base>& op) {
		TNatural<base> acc;

		for (TNatural<base> i(0); i < op; i++) {
			acc += *this;
		}
		*this = acc;

		return *this;
	}

	TNatural<base> operator*(const TNatural<base>& op) const {
		TNatural<base> ret(*this);		
		return ret *= op;
	}

	TNatural<base> operator/(const TNatural<base>& op) const {
		if (op == 0) {
			throw TNaturalBase::ResultException("Division by zero");
		}

		TNatural<base> tmp(*this), cnt(0);
		while (op <= tmp) {
			tmp -= op;
			cnt++;
		}

		return cnt;
	}

	TNatural<base>& operator/=(const TNatural<base>& op) {
		*this = *this / op;
		return *this;
	}

	TNatural<base> operator!() const {
		TNatural<base> ret(1);

		for (TNatural<base> i(1); i <= *this; ++i) {
			ret *= i;
		}

		return ret;
	}

	std::ostream& Show(std::ostream& os) const {
		std::vector<TNaturalBase::TRadix>::const_iterator it = number.end();
		while (it != number.begin()) {
			it--;
			os << digit2char(*it);
		}

		if (base != 10) {
			os << "#" << base;
		}

		return os;
	}
};

template<size_t base = 10>
inline std::ostream& operator<<(std::ostream& os, const TNatural<base>& tn) {
	return tn.Show(os);
}

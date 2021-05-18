#pragma once

class TNaturalBase {
public:
	typedef unsigned char TRadix;

	class InvalidInitializer {
		std::string msg;

	public:
		InvalidInitializer(const std::string& _msg) : msg(_msg) {

		}

		std::ostream& Show(std::ostream& os) const {
			return os << "Invalid parameter used for initialization: " << msg << std::endl;
		}
	};

	class InvalidCharacterInBaseException {
		std::string msg;

	public:
		InvalidCharacterInBaseException(const std::string& _msg) : msg(_msg) {

		}

		std::ostream& Show(std::ostream& os) const {
			return os << "Invalid character in the used base: " << msg << std::endl;
		}
	};

	class ResultException {
		std::string msg;

	public:
		ResultException(const std::string& _msg) : msg(_msg) {

		}

		std::ostream& Show(std::ostream& os) const {
			return os << "Unable to calculate: " << msg << std::endl;
		}
	};
};

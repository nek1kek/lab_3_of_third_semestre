#pragma once
#include <string>
#include "exception.h"
#include <vector>
using namespace std;

template<typename _ArgumentType>
struct Argument {
private:
	friend bool operator !=(const Argument<_ArgumentType>&, const Argument<_ArgumentType>&);
	friend bool operator ==(const Argument<_ArgumentType>&, const Argument<_ArgumentType>&);
	_ArgumentType value;
	char flag;
public:
	Argument() {
		this->flag = char();
		this->value = _ArgumentType();
	}
	Argument(char flag, _ArgumentType value) {
		this->flag = flag;
		this->value = value;
	}
	~Argument() {}
	void print() {
		std::cout << std::endl << '-' << this->flag << " " << this->value << std::endl;
	}
	char GetFlag() {
		return this->flag;
	}
	_ArgumentType Getvalue() {
		return this->value;
	}
	bool operator !=(const Argument<_ArgumentType>& other) {
		return !(this->flag == other.flag && this->value == other.value);
	}
	bool operator ==(const Argument<_ArgumentType>& other) {
		return (this->flag == other.flag && this->value == other.value);
	}
};

template<class _ArgumentType>
bool operator !=(const Argument<_ArgumentType>& cur, const Argument<_ArgumentType>& other) {
	return !(cur.flag == other.flag && cur.value == other.value);
}
template<class _ArgumentType>
bool operator ==(const Argument<_ArgumentType>& cur, const Argument<_ArgumentType>& other) {
	return (cur.flag == other.flag && cur.value == other.value);
}

namespace Traits {
	template<typename T>
	struct check_T {
		static char get_type() {
			return false;
		}
	};
	template<>
	struct check_T<int> {
		static bool get_type() {
			return true;
		}
	};
	template<>
	struct check_T<short int> {
		static bool get_type() {
			return true;
		}
	};
	template<>
	struct check_T<long long> {
		static bool get_type() {
			return true;
		}
	};

	template<typename T>
	struct _TypeCast {
		static T cast(std::string x) {
			try {
				return T(x);
			}
			catch (...) {
				throw SetException(IncorrectValue);
			}
		}
	};
	template<>
	struct _TypeCast<int> {
		static int cast(std::string x) {
			try {
				return std::stoi(x);
			}
			catch (...) {
				throw SetException(IncorrectValue);
			}
		}
	};

	template<>
	struct _TypeCast<double> {
		static double cast(std::string x) {
			try {
				return std::stof(x);
			}
			catch (...) {
				throw SetException(IncorrectValue);
			}
		}
	};
	template<>
	struct _TypeCast<string> {
		static string cast(std::string x) {
			return x;
		}
	};

	template<typename T>
	struct _Length {
		static size_t cast(T x) {
			try {
				return to_string(x).size();
			}
			catch (...) {
				throw SetException(IncorrectValue);
			}
		}
	};
	template<>
	struct _Length<string> {
		static size_t cast(string x) {
			return x.size();
		}
	};

	template<typename T>
	struct _ToString {
	public:
		static string cast(T x) {
			return to_string(x);
		}
	};
	string remake_string(string x, size_t n) {
		while (x.size() < n) {
			x += ' ';
		}
		return x;
	}
}

using namespace Traits;

template<typename T>
vector<Argument<T>>* _MakeArgumentList(string s, bool canBeExclusive) {
	vector<Argument<T>>* res = new vector<Argument<T>>;
	string restricted = "!-=+*&^%$#";
	size_t current = s.find('-', 0);
	while (current != -1) {
		if (current == -1) break;
		//if (current + 1 >= s.size() || s[current + 2] != ' ') throw SetException(IncorrectInputFormat);
		char flag = s[current + 1];
		current += 2;
		while (current < s.size() && s[current] == ' ') {
			if (s[current] != ' ') throw SetException(IncorrectInputFormat);
			current++;
		}
		//if (current >= s.size()) throw SetException(IncorrectInputFormat);
		string arg_value;
		if (s[current] != '-') {
			while (current < s.size() && s[current] != ' ') {
				if (restricted.find(s[current]) != -1) throw SetException(IncorrectInputFormat);
				arg_value += s[current];
				current++;
			}
			current = s.find('-', current + 1);
		}
		Argument<T> cur(flag, _TypeCast<T>::cast(arg_value));
		for (vector<Argument<string>>::iterator i = res->begin(); (i != res->end()) && !canBeExclusive; i++) {
			if ((*i).GetFlag() == cur.GetFlag()) {
				throw SetException(MutuallyExclusive);
			}
		}
		if (std::find(res->begin(), res->end(), cur) == res->end()) {
			res->push_back(cur);
		}
	}
	return res;
}

std::string _GetCommand(string s) {
	string res;
	for (int i = 0; i < s.size() && i < s.find(' '); i++) {
		res += s[i];
	}
	return res;
}
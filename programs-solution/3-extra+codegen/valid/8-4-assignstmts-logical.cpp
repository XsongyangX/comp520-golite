#include <iostream>
#include <string>
#include <array>

bool __golite__true = true;
bool __golite__false = false;

namespace GoLite {
template <typename T>
struct __golite_builtin__slice {
	unsigned int __size = 0;
	unsigned int __capacity = 0;
	T *__contents = NULL;
	__golite_builtin__slice<T> append(T element) {
		return __append<__golite_builtin__slice<T>>(element);
	}

	template<class C>
	C __append(T element) {
		C ret;
		ret.__size = __size;
		ret.__capacity = __capacity;
		ret.__contents = __contents;
		if (ret.__contents == NULL || ret.__size + 1 > ret.__capacity) {
			if (ret.__capacity == 0) {
				ret.__capacity = 2;
			} else {
				ret.__capacity = ret.__capacity * 2;
			}
			ret.__contents = new T[ret.__capacity];
			std::copy(__contents, __contents + __size, ret.__contents);
		}
		ret.__contents[__size] = element;
		ret.__size++;
		return ret;
	}

	T& at(unsigned int index) {
		if (index >= __size) {
			std::cerr << "Error: index " << index << " greater than size of slice (" << __size << ")" << std::endl;
			std::exit(EXIT_FAILURE);
		}
		return __contents[index];
	}

	int size() {
		return __size;
	}

	int capacity() {
		return __capacity;
	}
};


int __golite_tmp_g__0 = 0;
int __golite__g = __golite_tmp_g__0;

int __golite__bar(std::string __golite__a)
{
	std::cout << __golite__a << std::endl;
	__golite__g++;
	return __golite__g;
}
void __golite__main()
{
	bool __golite_tmp_a__1 = false;
	bool __golite__a = __golite_tmp_a__1;

	int __golite_tmp_b__2 = 0;
	int __golite_tmp_c__3 = 0;
	int __golite__b = __golite_tmp_b__2;
	int __golite__c = __golite_tmp_c__3;

	int __golite_tmp_call__4 = ([&]() {  return __golite__bar(std::string("lhs1")); })();
	int __golite_tmp_call__5 = ([&]() {  return __golite__bar(std::string("call3")); })();
	bool __golite_tmp__6 = ((__golite_tmp_call__4 == 2) || (([&]() {  return __golite__bar(std::string("rhs1")); })() == 3));
	int __golite_tmp__7 = __golite__g;
	int __golite_tmp__8 = __golite_tmp_call__5;
	__golite__a = __golite_tmp__6;
	__golite__b = __golite_tmp__7;
	__golite__c = __golite_tmp__8;

	std::cout << std::boolalpha << __golite__a << std::noboolalpha << std::endl;
	std::cout << __golite__b << std::endl;
	std::cout << __golite__c << std::endl;
	int __golite_tmp_call__9 = ([&]() {  return __golite__bar(std::string("lhs1")); })();
	int __golite_tmp_call__10 = ([&]() {  return __golite__bar(std::string("call3")); })();
	bool __golite_tmp__11 = ((__golite_tmp_call__9 == 4) && (([&]() {  return __golite__bar(std::string("rhs1")); })() == 6));
	int __golite_tmp__12 = __golite__g;
	int __golite_tmp__13 = __golite_tmp_call__10;
	__golite__a = __golite_tmp__11;
	__golite__b = __golite_tmp__12;
	__golite__c = __golite_tmp__13;

	std::cout << std::boolalpha << __golite__a << std::noboolalpha << std::endl;
	std::cout << __golite__b << std::endl;
	std::cout << __golite__c << std::endl;
}
}
int main() {
	GoLite::__golite__main();
}

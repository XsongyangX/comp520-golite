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


int __golite_tmp_glob__0 = 0;
int __golite__glob = __golite_tmp_glob__0;

std::array<int, 5> __golite_tmp_a__1 = {};
std::array<int, 5> __golite__a = __golite_tmp_a__1;

void __golite__foo(int __golite__a, int __golite__b)
{
	std::cout << __golite__a << std::endl;
	std::cout << __golite__b << std::endl;
}
int __golite__bar()
{
	__golite__glob++;
	return __golite__glob;
}
int __golite__baz(int __golite__b)
{
	int __golite_tmp__2 = 10;
	__golite__a.at(0) = __golite_tmp__2;

	int __golite_tmp__3 = 11;
	__golite__a.at(1) = __golite_tmp__3;

	int __golite_tmp__4 = 12;
	__golite__a.at(2) = __golite_tmp__4;

	int __golite_tmp__5 = 13;
	__golite__a.at(3) = __golite_tmp__5;

	int __golite_tmp__6 = 14;
	__golite__a.at(4) = __golite_tmp__6;

	return __golite__b;
}
void __golite__main()
{
	int __golite_tmp__7 = 0;
	__golite__glob = __golite_tmp__7;

	int __golite_tmp__8 = 0;
	__golite__a.at(0) = __golite_tmp__8;

	int __golite_tmp__9 = 1;
	__golite__a.at(1) = __golite_tmp__9;

	int __golite_tmp__10 = 2;
	__golite__a.at(2) = __golite_tmp__10;

	int __golite_tmp__11 = 3;
	__golite__a.at(3) = __golite_tmp__11;

	int __golite_tmp__12 = 4;
	__golite__a.at(4) = __golite_tmp__12;

	([&]() { int __golite_tmp_call__13 = ([&]() {  return __golite__bar(); })(); int __golite_tmp_call__14 = ([&]() {  return __golite__bar(); })(); int __golite_tmp_call__15 = ([&]() {  return __golite__baz(__golite_tmp_call__14); })();  return __golite__foo(__golite__a.at(__golite_tmp_call__13), __golite_tmp_call__15); })();
}
}
int main() {
	GoLite::__golite__main();
}

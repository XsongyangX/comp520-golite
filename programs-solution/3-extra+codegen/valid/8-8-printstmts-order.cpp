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

std::array<int, 6> __golite_tmp_array__1 = {};
std::array<int, 6> __golite__array = __golite_tmp_array__1;

int __golite__foo()
{
	__golite__glob++;
	return __golite__glob;
}
int __golite__bar(int __golite__a)
{
	std::cout << __golite__a << std::endl;
	return __golite__a;
}
std::array<int, 6> __golite__baz()
{
	__golite__glob++;
	return __golite__array;
}
void __golite__main()
{
	int __golite_tmp__2 = 0;
	__golite__array.at(0) = __golite_tmp__2;

	int __golite_tmp__3 = 1;
	__golite__array.at(1) = __golite_tmp__3;

	int __golite_tmp__4 = 2;
	__golite__array.at(2) = __golite_tmp__4;

	int __golite_tmp__5 = 3;
	__golite__array.at(3) = __golite_tmp__5;

	int __golite_tmp__6 = 4;
	__golite__array.at(4) = __golite_tmp__6;

	int __golite_tmp__7 = 5;
	__golite__array.at(5) = __golite_tmp__7;

	int __golite_tmp_call__8 = ([&]() {  return __golite__foo(); })();
	std::cout << __golite__glob << " " << __golite_tmp_call__8 << " " << __golite__glob << std::endl;
	int __golite_tmp_call__9 = ([&]() {  return __golite__foo(); })();
	int __golite_tmp_call__10 = ([&]() {  return __golite__foo(); })();
	int __golite_tmp_call__11 = ([&]() {  return __golite__foo(); })();
	int __golite_tmp_call__12 = ([&]() {  return __golite__foo(); })();
	std::cout << __golite_tmp_call__9 << " " << __golite__array.at(__golite_tmp_call__10) << " " << __golite__array.at(__golite_tmp_call__11) << " " << __golite_tmp_call__12 << std::endl;
	int __golite_tmp_call__13 = ([&]() {  return __golite__bar(0); })();
	int __golite_tmp_call__14 = ([&]() {  return __golite__bar(__golite_tmp_call__13); })();
	int __golite_tmp_call__15 = ([&]() {  return __golite__bar(1); })();
	std::cout << __golite_tmp_call__14 << " " << __golite_tmp_call__15 << std::endl;
	int __golite_tmp__16 = 0;
	__golite__glob = __golite_tmp__16;

	std::array<int, 6> __golite_tmp_call__17 = ([&]() {  return __golite__baz(); })();
	std::cout << __golite_tmp_call__17.at(__golite__glob) << std::endl;
	std::array<int, 6> __golite_tmp_call__18 = ([&]() {  return __golite__baz(); })();
	int __golite_tmp_call__19 = ([&]() {  return __golite__bar(__golite_tmp_call__18.at(__golite__glob)); })();
	std::cout << __golite_tmp_call__19 << std::endl;
}
}
int main() {
	GoLite::__golite__main();
}

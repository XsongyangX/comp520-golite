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

bool __golite__bar(int __golite__a, int __golite__b)
{
	std::cout << __golite__a << " " << __golite__b << std::endl;
	return (__golite__a < __golite__b);
}
int __golite__foo()
{
	__golite__glob++;
	return __golite__glob;
}
void __golite__main()
{
	int __golite_tmp__1 = 0;
	__golite__glob = __golite_tmp__1;

	int __golite_tmp_a__2 = ((([&]() {  return __golite__foo(); })() + ([&]() {  return __golite__foo(); })()) - ([&]() {  return __golite__foo(); })());
	int __golite__a = __golite_tmp_a__2;

	std::cout << __golite__a << std::endl;
	bool __golite_tmp_b__3 = (([&]() { int __golite_tmp_call__4 = ([&]() {  return __golite__foo(); })(); int __golite_tmp_call__5 = ([&]() {  return __golite__foo(); })();  return __golite__bar(__golite_tmp_call__4, __golite_tmp_call__5); })() || ([&]() { int __golite_tmp_call__6 = ([&]() {  return __golite__foo(); })(); int __golite_tmp_call__7 = ([&]() {  return __golite__foo(); })();  return __golite__bar(__golite_tmp_call__6, __golite_tmp_call__7); })());
	bool __golite__b = __golite_tmp_b__3;

	std::cout << std::boolalpha << __golite__b << std::noboolalpha << std::endl;
	bool __golite_tmp_c__8 = (([&]() { int __golite_tmp_call__9 = ([&]() {  return __golite__foo(); })(); int __golite_tmp_call__10 = ([&]() {  return __golite__foo(); })();  return __golite__bar(__golite_tmp_call__9, __golite_tmp_call__10); })() && ([&]() { int __golite_tmp_call__11 = ([&]() {  return __golite__foo(); })(); int __golite_tmp_call__12 = ([&]() {  return __golite__foo(); })();  return __golite__bar(__golite_tmp_call__11, __golite_tmp_call__12); })());
	bool __golite__c = __golite_tmp_c__8;

	std::cout << std::boolalpha << __golite__c << std::noboolalpha << std::endl;
}
}
int main() {
	GoLite::__golite__main();
}

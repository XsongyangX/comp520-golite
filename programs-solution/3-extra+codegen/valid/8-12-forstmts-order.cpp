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

int __golite__foo()
{
	__golite__glob++;
	return __golite__glob;
}
bool __golite__bar(int __golite__a, int __golite__b)
{
	return (__golite__a > __golite__b);
}
void __golite__main()
{
	bool __golite_tmp_loop__1 = ([&]() { int __golite_tmp_call__2 = ([&]() {  return __golite__foo(); })(); int __golite_tmp_call__3 = ([&]() {  return __golite__foo(); })();  return __golite__bar(__golite_tmp_call__2, __golite_tmp_call__3); })();
	while (__golite_tmp_loop__1)
	{
		{
			std::cout << std::string("Loop") << std::endl;
			goto __golite_tmp_break__4;
		}
__golite_tmp_continue__5: ;
		__golite_tmp_loop__1 = ([&]() { int __golite_tmp_call__6 = ([&]() {  return __golite__foo(); })(); int __golite_tmp_call__7 = ([&]() {  return __golite__foo(); })();  return __golite__bar(__golite_tmp_call__6, __golite_tmp_call__7); })();
	}
__golite_tmp_break__4: ;
	std::cout << std::string("End") << std::endl;
}
}
int main() {
	GoLite::__golite__main();
}

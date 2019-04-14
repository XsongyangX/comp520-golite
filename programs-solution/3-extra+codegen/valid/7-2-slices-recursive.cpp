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

typedef struct __golite_tag__0 __golite_tag__0;
typedef struct __golite_rec__0 __golite_rec__0;
typedef struct __golite_rec__1 __golite_rec__1;
typedef struct __golite_rec__2 __golite_rec__2;
struct __golite_tag__0 {
	__golite_builtin__slice<__golite_tag__0> __golite__a;
	__golite_tag__0() {
		__golite__a = {};
	}
};
struct __golite_rec__0 : std::array<__golite_builtin__slice<__golite_rec__0>, 3> {
};
struct __golite_rec__1 : __golite_builtin__slice<__golite_rec__1> {
	__golite_rec__1 append(__golite_rec__1 element) {
		return __append<__golite_rec__1>(element);
	}
};
struct __golite_rec__2 : __golite_builtin__slice<__golite_builtin__slice<__golite_rec__2>> {
	__golite_rec__2 append(__golite_builtin__slice<__golite_rec__2> element) {
		return __append<__golite_rec__2>(element);
	}
};

void __golite__main()
{

	__golite_tag__0 __golite_tmp_a__0 = {};
	__golite_tag__0 __golite__a = __golite_tmp_a__0;


	__golite_rec__0 __golite_tmp_b__1 = {};
	__golite_rec__0 __golite__b = __golite_tmp_b__1;


	__golite_rec__1 __golite_tmp_c__2 = {};
	__golite_rec__1 __golite_tmp_d__3 = {};
	__golite_rec__1 __golite__c = __golite_tmp_c__2;
	__golite_rec__1 __golite__d = __golite_tmp_d__3;

	__golite_rec__1 __golite_tmp__4 = __golite__c.append(__golite__d);
	__golite__c = __golite_tmp__4;


	__golite_rec__2 __golite_tmp_e__5 = {};
	__golite_rec__2 __golite_tmp_f__6 = {};
	__golite_rec__2 __golite__e = __golite_tmp_e__5;
	__golite_rec__2 __golite__f = __golite_tmp_f__6;

	std::cout << std::string("Success") << std::endl;
}
}
int main() {
	GoLite::__golite__main();
}

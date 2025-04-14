#include "unique_pointer.h"
#include <memory>
#include <iostream>

int main(void) noexcept {

    std::unique_ptr<int[]> arr = std::make_unique<int[]>(5);

    for (int i = 0; i < 5; ++i) {
        arr[i] = i * 10;
    }
    for (int i = 0; i < 5; ++i) {
        std::cout << arr[i] << " ";
    }

	std::unique_ptr<int> std_unique_ptr;
    //std_unique_ptr[10];
	std::unique_ptr<int> std_unique_ptr2(std::move(std_unique_ptr));
	std::unique_ptr<int> std_unique_ptr3 = std::move(std_unique_ptr);


	library::data_structure::unique_pointer<int> a(10);
    
	return 0;
}
// a_and_b_class.h
#pragma once

#include <array>
#include <utility>

// B 클래스 정의
template<int number>
class B {
    // B 클래스의 내용이 여기에 들어갈 수 있습니다.
};

// A 클래스 정의
template<int number>
class A {
public:
    A() {
        static_assert(number >= 0 && number <= 10, "number must be between 0 and 10");
        createBObjects(std::make_index_sequence<number + 1>{});
    }

private:
    template<std::size_t... Is>
    void createBObjects(std::index_sequence<Is...>) {
        ((b_objects[Is] = B<Is>{}), ...);
    }

    std::array<B<0>, 11> b_objects;  // 최대 11개의 B 객체를 저장할 수 있는 배열
};

int main(void) noexcept {
    A<3> a;
}
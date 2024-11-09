// a_and_b_class.h
#pragma once

#include <array>
#include <utility>

// B Ŭ���� ����
template<int number>
class B {
    // B Ŭ������ ������ ���⿡ �� �� �ֽ��ϴ�.
};

// A Ŭ���� ����
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

    std::array<B<0>, 11> b_objects;  // �ִ� 11���� B ��ü�� ������ �� �ִ� �迭
};

int main(void) noexcept {
    A<3> a;
}
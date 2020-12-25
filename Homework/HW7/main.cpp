#include <iostream>
#include "vector.hpp"

#define TEST(target, answer) \
    if (((target) != (answer))) { \
        std::cout << "    - WRONG ANSWER at check point " << total_counter << ". Expected value:" << answer << ", Actual value: " << target << std::endl; \
        wrong_counter++; \
    } \
    total_counter++;

struct Container {
    char ch;
    int integer;
    double number;
};

int main() {
    size_t total_counter = 0;
    size_t wrong_counter = 0;
    Container c1 = {'a', 1, 1.0};
    Container c2 = {'b', 2, 2.0};
    Container c3 = {'c', 3, 3.0};
    Container c4 = {'d', 4, 4.0};
    Container c5 = {'e', 5, 5.0};

    std::cout << "========== BEGIN TEST ==========" << std::endl;

    try {
        // Vector init and operator[] tests: Begin with CP0
        std::cout << "+ Vector init and operator[] tests" << std::endl;
        vector<Container> vec1;
        TEST(vec1.size(), 0)
        vector<double> vec2(5, 1.0);
        TEST(vec2.size(), 5)
        for (auto i: vec2) {
            TEST(i, 1.0)
        }
        vector<Container> vec3 = {c1, c2, c3, c4, c5};
        TEST(vec3.size(), 5)
        for (int i = 0; i < 5; ++i) {
            TEST(vec3[i].ch, 'a' + i)
            TEST(vec3[i].integer, 1 + i)
            TEST(vec3[i].number, 1.0 + i)
        }
        // std::cout << total_counter << std::endl;

        // Vector (const_)iterator tests: Begin with CP23
        std::cout << "+ Vector (const_)iterator tests" << std::endl;
        auto iter1 = vec3.begin();
        auto iter_target = vec3.end();
        int i = 0;
        while (iter1 != iter_target) {
            TEST((*iter1).ch, 'a' + i)
            TEST((*iter1).integer, 1 + i)
            TEST((*iter1).number, 1.0 + i)
            ++i; ++iter1;
        }
        auto citer1 = vec3.cbegin();
        auto citer_target = vec3.cend();
        i = 0;
        while (citer1 != citer_target) {
            TEST((*citer1).ch, 'a' + i)
            TEST((*citer1).integer, 1 + i)
            TEST((*citer1).number, 1.0 + i)
            ++i; ++citer1;
        }
        auto iter2 = --vec3.end();
        iter_target = vec3.begin();
        i = 4;
        while (iter2 != iter_target) {
            TEST((*iter2).ch, 'a' + i)
            TEST((*iter2).integer, 1 + i)
            TEST((*iter2).number, 1.0 + i)
            --i; iter2--;
        }
        auto citer2 = --vec3.cend();
        citer_target = vec3.cbegin();
        i = 4;
        while (citer2 != citer_target) {
            TEST((*citer2).ch, 'a' + i)
            TEST((*citer2).integer, 1 + i)
            TEST((*citer2).number, 1.0 + i)
            --i; citer2--;
        }
        auto iter3 = __detail::__iterator<Container>(&vec3[0]);
        auto iter4 = __detail::__iterator<Container>(iter3);
        TEST((*iter3).ch, 'a')
        TEST((*iter4).number, 1.0)
        auto citer3 = __detail::__const_iterator<Container>(&vec3[0]);
        auto citer4 = __detail::__const_iterator<Container>(iter3);
        TEST((*citer3).ch, 'a')
        TEST((*citer4).number, 1.0)
        iter3 += 1;
        iter4 += 5; iter4 -= 1;
        TEST((iter3 != iter4), true)
        TEST((*iter3).ch, 'b')
        TEST((*iter4).number, 5.0)
        citer3 += 1;
        citer4 += 5; citer4 -= 1;
        TEST((citer3 != citer4), true)
        TEST((*citer3).ch, 'b')
        TEST((*citer4).number, 5.0)
        iter4 = iter3;
        TEST((iter3 == iter4), true)
        TEST((*iter4).integer, 2)
        citer4 = citer3;
        TEST((citer3 == citer4), true)
        TEST((*citer4).integer, 2)
        // std::cout << total_counter << std::endl;

        // Vector insert, push_back, erase, pop_back tests: Begin with CP91
        std::cout << "+ Vector insert, push_back, erase, pop_back tests" << std::endl;
        vector<Container> vec4 = {};
        vec4.push_back(c1);
        TEST(vec4[0].integer, 1)
        vec4.erase(vec4.begin());
        TEST(vec4.size(), 0)
        vec4.push_back(c1);
        TEST(vec4[0].integer, 1)
        vec4.erase(vec4.cbegin());
        TEST(vec4.size(), 0)
        vec4.push_back(c2);
        auto iter5 = vec4.begin();
        iter5 = vec4.insert(iter5, c1);
        TEST((*iter5).integer, 1)
        vec4.push_back(c4);
        iter5 = vec4.begin();
        vec4.insert(iter5 + 2, c3);
        vec4.push_back(c5);                 // vec4 = c1, c2, c3, c4, c5
        TEST(vec4.size(), 5)
        TEST(vec4.end() - vec4.begin(), 5)
        TEST(vec4.cend() - vec4.cbegin(), 5)
        for (int i = 0; i < 5; ++i) {
            TEST(vec4[i].ch, 'a' + i)
            TEST(vec4[i].integer, 1 + i)
            TEST(vec4[i].number, 1.0 + i)
        }
        iter5 = vec4.end();
        --iter5; iter5--; iter5 -= 1;
        iter5 = vec4.erase(iter5);          // vec4 = c1, c2, c4, c5
        TEST((*iter5).ch, 'a' + 3)
        TEST((*iter5).integer, 1 + 3)
        TEST((*iter5).number, 1.0 + 3)
        iter5 = vec4.erase(vec4.end() - 1) - 1;   // vec4 = c1, c2, c4
        TEST((*iter5).ch, 'a' + 3)
        TEST((*iter5).integer, 1 + 3)
        TEST((*iter5).number, 1.0 + 3)
        vec4.pop_back();                    // vec4 = c1, c2
        TEST(vec4.size(), 2)
        for (int i = 0; i < 2; ++i) {
            TEST(vec4[i].ch, 'a' + i)
            TEST(vec4[i].integer, 1 + i)
            TEST(vec4[i].number, 1.0 + i)
        }
        iter5 = vec4.insert(vec4.cend(), c3);
        TEST((*iter5).integer, 3)
        vec4.insert(vec4.cend(), c4);
        vec4.insert(vec4.cend(), c5);
        for (int i = 0; i < 5; ++i) {
            TEST(vec4[i].ch, 'a' + i)
            TEST(vec4[i].integer, 1 + i)
            TEST(vec4[i].number, 1.0 + i)
        }
        auto citer5 = vec4.cend();
        --citer5; citer5--; citer5 -= 1;
        iter5 = vec4.erase(citer5);          // vec4 = c1, c2, c4, c5
        TEST((*iter5).ch, 'a' + 3)
        TEST((*iter5).integer, 1 + 3)
        TEST((*iter5).number, 1.0 + 3)
        iter5 = vec4.erase(vec4.cend() - 1) - 1;   // vec4 = c1, c2, c4
        TEST((*iter5).ch, 'a' + 3)
        TEST((*iter5).integer, 1 + 3)
        TEST((*iter5).number, 1.0 + 3)
        vec4.pop_back();                    // vec4 = c1, c2
        TEST(vec4.size(), 2)
        for (int i = 0; i < 2; ++i) {
            TEST(vec4[i].ch, 'a' + i)
            TEST(vec4[i].integer, 1 + i)
            TEST(vec4[i].number, 1.0 + i)
        }
    } catch (std::exception) {
        std::cout << "RUNTIME ERROR!" << std::endl;
    }

    std::cout << "========== TEST FINISHED ==========" << std::endl;
    std::cout << "Total tests: " << total_counter << ". Wrong tests: " << wrong_counter << std::endl;
}
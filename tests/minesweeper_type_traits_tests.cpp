#include <list>   // std::list
#include <string> // std::string
#include <vector> // std::vector

#include <gtest/gtest.h>

#include <minesweeper/type_traits.h>

class MinesweeperTypeTraitsTest : public ::testing::Test {
  protected:
    // +-----------------------------+
    // | Custom classes for testing: |
    // +-----------------------------+

    class EmptyCustomClass {};

    enum class CustomEnumClass { TESTENUM, SECONDTESTENUM };

    // custom template class with 'value_type' only
    template <typename T>
    class CustomClassWithValueType {
      public:
        using value_type = T;
    };

    // custom template class with 'push_back' only
    template <typename T>
    class CustomClassWithPushBack {
      public:
        void push_back(T x) {}
    };

    // custom class with 'size' only
    class CustomClassWithSize {
      public:
        size_t size() { return 0; }
    };

    // custom template class with 'value_type' and 'push_back'
    template <typename T>
    class CustomClassWithValueTypeAndPushBack {
      public:
        using value_type = T;

        void push_back(value_type x) {}
    };

    // custom template class with 'value_type' and 'size'
    template <typename T>
    class CustomClassWithValueTypeAndSize {
      public:
        using value_type = T;

        size_t size() { return 0; }
    };

    // custom template class with 'push_back' and 'size'
    template <typename T>
    class CustomClassWithPushBackAndSize {
      public:
        void push_back(T x) {}

        size_t size() { return 0; }
    };

    // custom template type (with minimal requirements)
    template <typename T>
    class CustomExpandSeqContainer {
      public:
        using value_type = T;

        void push_back(value_type x) {}

        size_t size() { return 0; }
    };
};

TEST_F(MinesweeperTypeTraitsTest, is_expandable_sequence_containerTest) {

    // simple data types
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container<int>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container<char>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container<int*>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container<bool>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container<short>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container<long long>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container<double>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container<void>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container<EmptyCustomClass>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container<CustomEnumClass>::value);

    // std::vector
    // valid
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container<std::vector<int>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container<std::vector<char>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container<std::vector<int*>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container<std::vector<EmptyCustomClass>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container<std::vector<CustomEnumClass>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container<std::vector<std::vector<int>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container<std::vector<std::vector<EmptyCustomClass>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container<std::vector<std::vector<std::vector<int>>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                std::vector<std::vector<std::vector<CustomEnumClass>>>>::value);

    // std::list
    // valid
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container<std::list<int>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container<std::list<char>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container<std::list<int*>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container<std::list<EmptyCustomClass>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container<std::list<CustomEnumClass>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container<std::list<std::list<int>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container<std::list<std::list<EmptyCustomClass>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container<std::list<std::list<std::list<int>>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container<std::list<std::list<std::list<CustomEnumClass>>>>::value);

    // std::string
    // valid
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container<std::string>::value);

    // CustomExpandSeqContainer
    // valid
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                CustomExpandSeqContainer<int>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                CustomExpandSeqContainer<char>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                CustomExpandSeqContainer<int*>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                CustomExpandSeqContainer<EmptyCustomClass>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                CustomExpandSeqContainer<CustomEnumClass>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                CustomExpandSeqContainer<CustomExpandSeqContainer<int>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                CustomExpandSeqContainer<CustomExpandSeqContainer<EmptyCustomClass>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                CustomExpandSeqContainer<CustomExpandSeqContainer<CustomExpandSeqContainer<int>>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                CustomExpandSeqContainer<CustomExpandSeqContainer<CustomExpandSeqContainer<CustomEnumClass>>>>::value);

    // CustomClassWithValueType
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithValueType<int>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithValueType<char>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithValueType<int*>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithValueType<EmptyCustomClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithValueType<CustomEnumClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithValueType<CustomClassWithValueType<int>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithValueType<CustomClassWithValueType<EmptyCustomClass>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithValueType<CustomClassWithValueType<CustomClassWithValueType<int>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithValueType<CustomClassWithValueType<CustomClassWithValueType<CustomEnumClass>>>>::value);

    // CustomClassWithPushBack
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithPushBack<int>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithPushBack<char>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithPushBack<int*>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithPushBack<EmptyCustomClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithPushBack<CustomEnumClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithPushBack<CustomClassWithPushBack<int>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithPushBack<CustomClassWithPushBack<EmptyCustomClass>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithPushBack<CustomClassWithPushBack<CustomClassWithPushBack<int>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithPushBack<CustomClassWithPushBack<CustomClassWithPushBack<CustomEnumClass>>>>::value);

    // CustomClassWithSize
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container<CustomClassWithSize>::value);

    // CustomClassWithValueTypeAndPushBack
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithValueTypeAndPushBack<int>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithValueTypeAndPushBack<char>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithValueTypeAndPushBack<int*>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithValueTypeAndPushBack<EmptyCustomClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithValueTypeAndPushBack<CustomEnumClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithValueTypeAndPushBack<CustomClassWithValueTypeAndPushBack<int>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithValueTypeAndPushBack<CustomClassWithValueTypeAndPushBack<EmptyCustomClass>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container<                           //
                 CustomClassWithValueTypeAndPushBack<CustomClassWithValueTypeAndPushBack< //
                     CustomClassWithValueTypeAndPushBack<int>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container<                           //
                 CustomClassWithValueTypeAndPushBack<CustomClassWithValueTypeAndPushBack< //
                     CustomClassWithValueTypeAndPushBack<CustomEnumClass>>>>::value);

    // CustomClassWithValueTypeAndSize
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithValueTypeAndSize<int>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithValueTypeAndSize<char>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithValueTypeAndSize<int*>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithValueTypeAndSize<EmptyCustomClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithValueTypeAndSize<CustomEnumClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithValueTypeAndSize<CustomClassWithValueTypeAndSize<int>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithValueTypeAndSize<CustomClassWithValueTypeAndSize<EmptyCustomClass>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container<                   //
                 CustomClassWithValueTypeAndSize<CustomClassWithValueTypeAndSize< //
                     CustomClassWithValueTypeAndSize<int>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container<                   //
                 CustomClassWithValueTypeAndSize<CustomClassWithValueTypeAndSize< //
                     CustomClassWithValueTypeAndSize<CustomEnumClass>>>>::value);

    // CustomClassWithPushBackAndSize
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithPushBackAndSize<int>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithPushBackAndSize<char>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithPushBackAndSize<int*>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithPushBackAndSize<EmptyCustomClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithPushBackAndSize<CustomEnumClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithPushBackAndSize<CustomClassWithPushBackAndSize<int>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithPushBackAndSize<CustomClassWithPushBackAndSize<EmptyCustomClass>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container<                 //
                 CustomClassWithPushBackAndSize<CustomClassWithPushBackAndSize< //
                     CustomClassWithPushBackAndSize<int>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container<                 //
                 CustomClassWithPushBackAndSize<CustomClassWithPushBackAndSize< //
                     CustomClassWithPushBackAndSize<CustomEnumClass>>>>::value);

    // mixed 2d container
    // valid
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                std::vector<std::list<int>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                std::vector<std::list<EmptyCustomClass>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                std::vector<std::string>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                std::vector<CustomClassWithValueType<EmptyCustomClass>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                std::vector<CustomClassWithPushBack<char>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                std::vector<CustomClassWithValueTypeAndPushBack<EmptyCustomClass>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                std::vector<CustomClassWithValueTypeAndSize<int>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                std::vector<CustomClassWithPushBackAndSize<CustomEnumClass>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                std::vector<CustomExpandSeqContainer<char>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                std::vector<CustomExpandSeqContainer<CustomEnumClass>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                std::list<std::vector<char*>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                std::list<CustomClassWithPushBack<EmptyCustomClass>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                std::list<CustomClassWithPushBackAndSize<int>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                std::list<CustomExpandSeqContainer<char>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                CustomExpandSeqContainer<std::list<int>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                CustomExpandSeqContainer<std::string>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                CustomExpandSeqContainer<CustomClassWithValueType<int>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                CustomExpandSeqContainer<CustomClassWithValueTypeAndSize<char*>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                CustomExpandSeqContainer<CustomExpandSeqContainer<EmptyCustomClass>>>::value);
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithValueTypeAndSize<std::list<CustomEnumClass>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithPushBack<CustomClassWithValueType<char>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithPushBack<CustomExpandSeqContainer<int>>>::value);

    // mixed 3d contaier
    // valid
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                std::vector<std::list<std::vector<char*>>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                std::list<std::list<CustomClassWithValueType<int*>>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                std::list<std::list<CustomExpandSeqContainer<int*>>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                std::list<CustomClassWithPushBackAndSize<std::list<bool>>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                std::list<CustomClassWithValueTypeAndPushBack<CustomClassWithPushBackAndSize<char*>>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                std::vector<CustomClassWithValueTypeAndSize<CustomExpandSeqContainer<int>>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                std::vector<CustomExpandSeqContainer<std::vector<EmptyCustomClass>>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                std::vector<CustomExpandSeqContainer<CustomClassWithPushBack<double>>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                std::vector<CustomExpandSeqContainer<CustomExpandSeqContainer<int>>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                CustomExpandSeqContainer<std::list<std::list<double>>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                CustomExpandSeqContainer<std::list<CustomClassWithValueTypeAndSize<char*>>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                CustomExpandSeqContainer<std::list<CustomExpandSeqContainer<EmptyCustomClass>>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                CustomExpandSeqContainer<CustomClassWithPushBackAndSize<std::vector<char>>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container<     //
                CustomExpandSeqContainer<CustomClassWithValueType< //
                    CustomClassWithValueTypeAndPushBack<EmptyCustomClass>>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                CustomExpandSeqContainer<CustomClassWithValueType<CustomExpandSeqContainer<EmptyCustomClass>>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                CustomExpandSeqContainer<CustomExpandSeqContainer<std::list<bool>>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container<     //
                CustomExpandSeqContainer<CustomExpandSeqContainer< //
                    CustomClassWithValueTypeAndPushBack<EmptyCustomClass>>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_sequence_container< //
                CustomExpandSeqContainer<CustomExpandSeqContainer<CustomExpandSeqContainer<CustomEnumClass>>>>::value);
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithValueType<std::vector<std::string>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithPushBack<std::vector<CustomClassWithValueTypeAndSize<double>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithPushBackAndSize<std::vector<CustomExpandSeqContainer<int*>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithValueTypeAndPushBack<CustomClassWithValueType<std::string>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container<            //
                 CustomClassWithValueType<CustomClassWithValueTypeAndSize< //
                     CustomClassWithPushBack<EmptyCustomClass>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container<                       //
                 CustomClassWithValueTypeAndSize<CustomClassWithValueTypeAndPushBack< //
                     CustomExpandSeqContainer<double>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container< //
                 CustomClassWithPushBack<CustomExpandSeqContainer<std::vector<double>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container<     //
                 CustomClassWithValueType<CustomExpandSeqContainer< //
                     CustomClassWithValueTypeAndPushBack<char*>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_sequence_container<           //
                 CustomClassWithPushBackAndSize<CustomExpandSeqContainer< //
                     CustomExpandSeqContainer<EmptyCustomClass>>>>::value);
}

TEST_F(MinesweeperTypeTraitsTest, is_expandable_1D_sequence_containerTest) {

    // simple data types
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container<int>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container<char>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container<int*>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container<bool>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container<short>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container<long long>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container<double>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container<void>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container<EmptyCustomClass>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container<CustomEnumClass>::value);

    // std::vector
    // valid
    EXPECT_TRUE(minesweeper::is_expandable_1D_sequence_container<std::vector<int>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_1D_sequence_container<std::vector<char>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_1D_sequence_container<std::vector<int*>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_1D_sequence_container<std::vector<EmptyCustomClass>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_1D_sequence_container<std::vector<CustomEnumClass>>::value);
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container<std::vector<std::vector<int>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container<std::vector<std::vector<EmptyCustomClass>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container<std::vector<std::vector<std::vector<int>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 std::vector<std::vector<std::vector<CustomEnumClass>>>>::value);

    // std::list
    // valid
    EXPECT_TRUE(minesweeper::is_expandable_1D_sequence_container<std::list<int>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_1D_sequence_container<std::list<char>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_1D_sequence_container<std::list<int*>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_1D_sequence_container<std::list<EmptyCustomClass>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_1D_sequence_container<std::list<CustomEnumClass>>::value);
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container<std::list<std::list<int>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container<std::list<std::list<EmptyCustomClass>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container<std::list<std::list<std::list<int>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 std::list<std::list<std::list<CustomEnumClass>>>>::value);

    // std::string
    // valid
    EXPECT_TRUE(minesweeper::is_expandable_1D_sequence_container<std::string>::value);

    // CustomExpandSeqContainer
    // valid
    EXPECT_TRUE(minesweeper::is_expandable_1D_sequence_container<CustomExpandSeqContainer<int>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_1D_sequence_container<CustomExpandSeqContainer<char>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_1D_sequence_container<CustomExpandSeqContainer<int*>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_1D_sequence_container<CustomExpandSeqContainer<EmptyCustomClass>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_1D_sequence_container<CustomExpandSeqContainer<CustomEnumClass>>::value);
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomExpandSeqContainer<CustomExpandSeqContainer<int>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomExpandSeqContainer<CustomExpandSeqContainer<EmptyCustomClass>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomExpandSeqContainer<CustomExpandSeqContainer<CustomExpandSeqContainer<int>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomExpandSeqContainer<CustomExpandSeqContainer<CustomExpandSeqContainer<CustomEnumClass>>>>::value);

    // CustomClassWithValueType
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithValueType<int>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithValueType<char>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithValueType<int*>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithValueType<EmptyCustomClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithValueType<CustomEnumClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithValueType<CustomClassWithValueType<int>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithValueType<CustomClassWithValueType<EmptyCustomClass>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithValueType<CustomClassWithValueType<CustomClassWithValueType<int>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithValueType<CustomClassWithValueType<CustomClassWithValueType<CustomEnumClass>>>>::value);

    // CustomClassWithPushBack
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithPushBack<int>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithPushBack<char>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithPushBack<int*>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithPushBack<EmptyCustomClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithPushBack<CustomEnumClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithPushBack<CustomClassWithPushBack<int>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithPushBack<CustomClassWithPushBack<EmptyCustomClass>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithPushBack<CustomClassWithPushBack<CustomClassWithPushBack<int>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithPushBack<CustomClassWithPushBack<CustomClassWithPushBack<CustomEnumClass>>>>::value);

    // CustomClassWithSize
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container<CustomClassWithSize>::value);

    // CustomClassWithValueTypeAndPushBack
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithValueTypeAndPushBack<int>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithValueTypeAndPushBack<char>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithValueTypeAndPushBack<int*>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithValueTypeAndPushBack<EmptyCustomClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithValueTypeAndPushBack<CustomEnumClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithValueTypeAndPushBack<CustomClassWithValueTypeAndPushBack<int>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithValueTypeAndPushBack<CustomClassWithValueTypeAndPushBack<EmptyCustomClass>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container<                        //
                 CustomClassWithValueTypeAndPushBack<CustomClassWithValueTypeAndPushBack< //
                     CustomClassWithValueTypeAndPushBack<int>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container<                        //
                 CustomClassWithValueTypeAndPushBack<CustomClassWithValueTypeAndPushBack< //
                     CustomClassWithValueTypeAndPushBack<CustomEnumClass>>>>::value);

    // CustomClassWithValueTypeAndSize
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithValueTypeAndSize<int>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithValueTypeAndSize<char>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithValueTypeAndSize<int*>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithValueTypeAndSize<EmptyCustomClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithValueTypeAndSize<CustomEnumClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithValueTypeAndSize<CustomClassWithValueTypeAndSize<int>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithValueTypeAndSize<CustomClassWithValueTypeAndSize<EmptyCustomClass>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container<                //
                 CustomClassWithValueTypeAndSize<CustomClassWithValueTypeAndSize< //
                     CustomClassWithValueTypeAndSize<int>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container<                //
                 CustomClassWithValueTypeAndSize<CustomClassWithValueTypeAndSize< //
                     CustomClassWithValueTypeAndSize<CustomEnumClass>>>>::value);

    // CustomClassWithPushBackAndSize
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithPushBackAndSize<int>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithPushBackAndSize<char>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithPushBackAndSize<int*>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithPushBackAndSize<EmptyCustomClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithPushBackAndSize<CustomEnumClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithPushBackAndSize<CustomClassWithPushBackAndSize<int>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithPushBackAndSize<CustomClassWithPushBackAndSize<EmptyCustomClass>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container<              //
                 CustomClassWithPushBackAndSize<CustomClassWithPushBackAndSize< //
                     CustomClassWithPushBackAndSize<int>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container<                                             //
                 CustomClassWithPushBackAndSize<CustomClassWithPushBackAndSize<CustomClassWithPushBackAndSize< //
                     CustomEnumClass>>>>::value);

    // mixed 2d container
    // valid
    EXPECT_TRUE(minesweeper::is_expandable_1D_sequence_container< //
                std::vector<CustomClassWithValueType<EmptyCustomClass>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_1D_sequence_container< //
                std::vector<CustomClassWithPushBack<char>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_1D_sequence_container< //
                std::vector<CustomClassWithValueTypeAndPushBack<EmptyCustomClass>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_1D_sequence_container< //
                std::vector<CustomClassWithValueTypeAndSize<int>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_1D_sequence_container< //
                std::vector<CustomClassWithPushBackAndSize<CustomEnumClass>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_1D_sequence_container< //
                std::list<CustomClassWithPushBack<EmptyCustomClass>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_1D_sequence_container< //
                std::list<CustomClassWithPushBackAndSize<int>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_1D_sequence_container< //
                CustomExpandSeqContainer<CustomClassWithValueType<int>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_1D_sequence_container< //
                CustomExpandSeqContainer<CustomClassWithValueTypeAndSize<char*>>>::value);
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithValueTypeAndSize<std::list<CustomEnumClass>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithPushBack<CustomClassWithValueType<char>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithPushBack<CustomExpandSeqContainer<int>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 std::vector<std::list<int>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 std::vector<std::list<EmptyCustomClass>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 std::vector<std::string>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 std::vector<CustomExpandSeqContainer<char>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 std::vector<CustomExpandSeqContainer<CustomEnumClass>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 std::list<std::vector<char*>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 std::list<CustomExpandSeqContainer<char>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomExpandSeqContainer<std::list<int>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomExpandSeqContainer<std::string>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomExpandSeqContainer<CustomExpandSeqContainer<EmptyCustomClass>>>::value);

    // mixed 3d contaier
    // valid

    EXPECT_TRUE(minesweeper::is_expandable_1D_sequence_container< //
                std::list<CustomClassWithPushBackAndSize<std::list<bool>>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_1D_sequence_container< //
                std::list<CustomClassWithValueTypeAndPushBack<CustomClassWithPushBackAndSize<char*>>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_1D_sequence_container< //
                std::vector<CustomClassWithValueTypeAndSize<CustomExpandSeqContainer<int>>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_1D_sequence_container< //
                CustomExpandSeqContainer<CustomClassWithPushBackAndSize<std::vector<char>>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_1D_sequence_container<  //
                CustomExpandSeqContainer<CustomClassWithValueType< //
                    CustomClassWithValueTypeAndPushBack<EmptyCustomClass>>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_1D_sequence_container< //
                CustomExpandSeqContainer<CustomClassWithValueType<CustomExpandSeqContainer<EmptyCustomClass>>>>::value);
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithValueType<std::vector<std::string>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithPushBack<std::vector<CustomClassWithValueTypeAndSize<double>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithPushBackAndSize<std::vector<CustomExpandSeqContainer<int*>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithValueTypeAndPushBack<CustomClassWithValueType<std::string>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container<         //
                 CustomClassWithValueType<CustomClassWithValueTypeAndSize< //
                     CustomClassWithPushBack<EmptyCustomClass>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container<                    //
                 CustomClassWithValueTypeAndSize<CustomClassWithValueTypeAndPushBack< //
                     CustomExpandSeqContainer<double>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomClassWithPushBack<CustomExpandSeqContainer<std::vector<double>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container<  //
                 CustomClassWithValueType<CustomExpandSeqContainer< //
                     CustomClassWithValueTypeAndPushBack<char*>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container<        //
                 CustomClassWithPushBackAndSize<CustomExpandSeqContainer< //
                     CustomExpandSeqContainer<EmptyCustomClass>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 std::vector<std::list<std::vector<char*>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 std::list<std::list<CustomClassWithValueType<int*>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 std::list<std::list<CustomExpandSeqContainer<int*>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 std::vector<CustomExpandSeqContainer<std::vector<EmptyCustomClass>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 std::vector<CustomExpandSeqContainer<CustomClassWithPushBack<double>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 std::vector<CustomExpandSeqContainer<CustomExpandSeqContainer<int>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomExpandSeqContainer<std::list<std::list<double>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomExpandSeqContainer<std::list<CustomClassWithValueTypeAndSize<char*>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomExpandSeqContainer<std::list<CustomExpandSeqContainer<EmptyCustomClass>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomExpandSeqContainer<CustomExpandSeqContainer<std::list<bool>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container<  //
                 CustomExpandSeqContainer<CustomExpandSeqContainer< //
                     CustomClassWithValueTypeAndPushBack<EmptyCustomClass>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_1D_sequence_container< //
                 CustomExpandSeqContainer<CustomExpandSeqContainer<CustomExpandSeqContainer<CustomEnumClass>>>>::value);
}

TEST_F(MinesweeperTypeTraitsTest, is_expandable_2D_sequence_containerTest) {

    // simple data types
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<int>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<char>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<int*>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<bool>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<short>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<long long>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<double>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<void>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<EmptyCustomClass>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<CustomEnumClass>::value);

    // std::vector
    // valid
    EXPECT_TRUE(minesweeper::is_expandable_2D_sequence_container<std::vector<std::vector<int>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_2D_sequence_container<std::vector<std::vector<EmptyCustomClass>>>::value);
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<std::vector<int>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<std::vector<char>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<std::vector<int*>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<std::vector<EmptyCustomClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<std::vector<CustomEnumClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<std::vector<std::vector<std::vector<int>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 std::vector<std::vector<std::vector<CustomEnumClass>>>>::value);

    // std::list
    // valid
    EXPECT_TRUE(minesweeper::is_expandable_2D_sequence_container<std::list<std::list<int>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_2D_sequence_container<std::list<std::list<EmptyCustomClass>>>::value);
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<std::list<int>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<std::list<char>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<std::list<int*>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<std::list<EmptyCustomClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<std::list<CustomEnumClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<std::list<std::list<std::list<int>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 std::list<std::list<std::list<CustomEnumClass>>>>::value);

    // std::string
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<std::string>::value);

    // CustomExpandSeqContainer
    // valid
    EXPECT_TRUE(minesweeper::is_expandable_2D_sequence_container< //
                CustomExpandSeqContainer<CustomExpandSeqContainer<int>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_2D_sequence_container< //
                CustomExpandSeqContainer<CustomExpandSeqContainer<EmptyCustomClass>>>::value);
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomExpandSeqContainer<int>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomExpandSeqContainer<char>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomExpandSeqContainer<int*>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomExpandSeqContainer<EmptyCustomClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomExpandSeqContainer<CustomEnumClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomExpandSeqContainer<CustomExpandSeqContainer<CustomExpandSeqContainer<int>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomExpandSeqContainer<CustomExpandSeqContainer<CustomExpandSeqContainer<CustomEnumClass>>>>::value);

    // CustomClassWithValueType
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithValueType<int>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithValueType<char>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithValueType<int*>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithValueType<EmptyCustomClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithValueType<CustomEnumClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithValueType<CustomClassWithValueType<int>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithValueType<CustomClassWithValueType<EmptyCustomClass>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithValueType<CustomClassWithValueType<CustomClassWithValueType<int>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithValueType<CustomClassWithValueType<CustomClassWithValueType<CustomEnumClass>>>>::value);

    // CustomClassWithPushBack
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithPushBack<int>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithPushBack<char>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithPushBack<int*>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithPushBack<EmptyCustomClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithPushBack<CustomEnumClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithPushBack<CustomClassWithPushBack<int>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithPushBack<CustomClassWithPushBack<EmptyCustomClass>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithPushBack<CustomClassWithPushBack<CustomClassWithPushBack<int>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithPushBack<CustomClassWithPushBack<CustomClassWithPushBack<CustomEnumClass>>>>::value);

    // CustomClassWithSize
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<CustomClassWithSize>::value);

    // CustomClassWithValueTypeAndPushBack
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithValueTypeAndPushBack<int>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithValueTypeAndPushBack<char>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithValueTypeAndPushBack<int*>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithValueTypeAndPushBack<EmptyCustomClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithValueTypeAndPushBack<CustomEnumClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithValueTypeAndPushBack<CustomClassWithValueTypeAndPushBack<int>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithValueTypeAndPushBack<CustomClassWithValueTypeAndPushBack<EmptyCustomClass>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<                        //
                 CustomClassWithValueTypeAndPushBack<CustomClassWithValueTypeAndPushBack< //
                     CustomClassWithValueTypeAndPushBack<int>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<                        //
                 CustomClassWithValueTypeAndPushBack<CustomClassWithValueTypeAndPushBack< //
                     CustomClassWithValueTypeAndPushBack<CustomEnumClass>>>>::value);

    // CustomClassWithValueTypeAndSize
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithValueTypeAndSize<int>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithValueTypeAndSize<char>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithValueTypeAndSize<int*>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithValueTypeAndSize<EmptyCustomClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithValueTypeAndSize<CustomEnumClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithValueTypeAndSize<CustomClassWithValueTypeAndSize<int>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithValueTypeAndSize<CustomClassWithValueTypeAndSize<EmptyCustomClass>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<                //
                 CustomClassWithValueTypeAndSize<CustomClassWithValueTypeAndSize< //
                     CustomClassWithValueTypeAndSize<int>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<                //
                 CustomClassWithValueTypeAndSize<CustomClassWithValueTypeAndSize< //
                     CustomClassWithValueTypeAndSize<CustomEnumClass>>>>::value);

    // CustomClassWithPushBackAndSize
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithPushBackAndSize<int>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithPushBackAndSize<char>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithPushBackAndSize<int*>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithPushBackAndSize<EmptyCustomClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithPushBackAndSize<CustomEnumClass>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithPushBackAndSize<CustomClassWithPushBackAndSize<int>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithPushBackAndSize<CustomClassWithPushBackAndSize<EmptyCustomClass>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<              //
                 CustomClassWithPushBackAndSize<CustomClassWithPushBackAndSize< //
                     CustomClassWithPushBackAndSize<int>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<              //
                 CustomClassWithPushBackAndSize<CustomClassWithPushBackAndSize< //
                     CustomClassWithPushBackAndSize<CustomEnumClass>>>>::value);

    // mixed 2d container
    // valid
    EXPECT_TRUE(minesweeper::is_expandable_2D_sequence_container< //
                std::vector<std::list<int>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_2D_sequence_container< //
                std::vector<std::list<EmptyCustomClass>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_2D_sequence_container< //
                std::vector<std::string>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_2D_sequence_container< //
                std::vector<CustomExpandSeqContainer<char>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_2D_sequence_container< //
                std::vector<CustomExpandSeqContainer<CustomEnumClass>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_2D_sequence_container< //
                std::list<std::vector<char*>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_2D_sequence_container< //
                std::list<CustomExpandSeqContainer<char>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_2D_sequence_container< //
                CustomExpandSeqContainer<std::list<int>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_2D_sequence_container< //
                CustomExpandSeqContainer<std::string>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_2D_sequence_container< //
                CustomExpandSeqContainer<CustomExpandSeqContainer<EmptyCustomClass>>>::value);
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithValueTypeAndSize<std::list<CustomEnumClass>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithPushBack<CustomClassWithValueType<char>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithPushBack<CustomExpandSeqContainer<int>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 std::vector<CustomClassWithValueType<EmptyCustomClass>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 std::vector<CustomClassWithPushBack<char>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 std::vector<CustomClassWithValueTypeAndPushBack<EmptyCustomClass>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 std::vector<CustomClassWithValueTypeAndSize<int>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 std::vector<CustomClassWithPushBackAndSize<CustomEnumClass>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 std::list<CustomClassWithPushBack<EmptyCustomClass>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 std::list<CustomClassWithPushBackAndSize<int>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomExpandSeqContainer<CustomClassWithValueType<int>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomExpandSeqContainer<CustomClassWithValueTypeAndSize<char*>>>::value);

    // mixed 3d contaier CHECKED ABOVE
    // valid
    EXPECT_TRUE(minesweeper::is_expandable_2D_sequence_container< //
                std::list<std::list<CustomClassWithValueType<int*>>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_2D_sequence_container< //
                std::vector<CustomExpandSeqContainer<CustomClassWithPushBack<double>>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_2D_sequence_container< //
                CustomExpandSeqContainer<std::list<CustomClassWithValueTypeAndSize<char*>>>>::value);
    EXPECT_TRUE(minesweeper::is_expandable_2D_sequence_container<  //
                CustomExpandSeqContainer<CustomExpandSeqContainer< //
                    CustomClassWithValueTypeAndPushBack<EmptyCustomClass>>>>::value);
    // invalid
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithValueType<std::vector<std::string>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithPushBack<std::vector<CustomClassWithValueTypeAndSize<double>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithPushBackAndSize<std::vector<CustomExpandSeqContainer<int*>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithValueTypeAndPushBack<CustomClassWithValueType<std::string>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<         //
                 CustomClassWithValueType<CustomClassWithValueTypeAndSize< //
                     CustomClassWithPushBack<EmptyCustomClass>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<                    //
                 CustomClassWithValueTypeAndSize<CustomClassWithValueTypeAndPushBack< //
                     CustomExpandSeqContainer<double>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomClassWithPushBack<CustomExpandSeqContainer<std::vector<double>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<  //
                 CustomClassWithValueType<CustomExpandSeqContainer< //
                     CustomClassWithValueTypeAndPushBack<char*>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<        //
                 CustomClassWithPushBackAndSize<CustomExpandSeqContainer< //
                     CustomExpandSeqContainer<EmptyCustomClass>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 std::vector<std::list<std::vector<char*>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 std::list<std::list<CustomExpandSeqContainer<int*>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 std::list<CustomClassWithPushBackAndSize<std::list<bool>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 std::list<CustomClassWithValueTypeAndPushBack<CustomClassWithPushBackAndSize<char*>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 std::vector<CustomClassWithValueTypeAndSize<CustomExpandSeqContainer<int>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 std::vector<CustomExpandSeqContainer<std::vector<EmptyCustomClass>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 std::vector<CustomExpandSeqContainer<CustomExpandSeqContainer<int>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomExpandSeqContainer<std::list<std::list<double>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomExpandSeqContainer<std::list<CustomExpandSeqContainer<EmptyCustomClass>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomExpandSeqContainer<CustomClassWithPushBackAndSize<std::vector<char>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<  //
                 CustomExpandSeqContainer<CustomClassWithValueType< //
                     CustomClassWithValueTypeAndPushBack<EmptyCustomClass>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container<  //
                 CustomExpandSeqContainer<CustomClassWithValueType< //
                     CustomExpandSeqContainer<EmptyCustomClass>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomExpandSeqContainer<CustomExpandSeqContainer<std::list<bool>>>>::value);
    EXPECT_FALSE(minesweeper::is_expandable_2D_sequence_container< //
                 CustomExpandSeqContainer<CustomExpandSeqContainer<CustomExpandSeqContainer<CustomEnumClass>>>>::value);
}

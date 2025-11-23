#include <gtest/gtest.h>

#include <helpers/BuildTimeList.h>

using namespace BuildTimeList;

class SizeShould : public ::testing::Test
{};

TEST_F(SizeShould, computeTheSizeOfAnIntBuildTimeListWith0Elements) {
    // Given a list type
    using ListType = List<int>;

    // When computing its size
    constexpr size_t result = Size<ListType>::result;

    // Then its size should be its number of elements
    EXPECT_EQ(result, 0);
}

TEST_F(SizeShould, computeTheSizeOfAnIntBuildTimeListWith1Elements) {
    // Given a list type
    using ListType = List<int, 0>;

    // When computing its size
    constexpr size_t result = Size<ListType>::result;

    // Then its size should be its number of elements
    EXPECT_EQ(result, 1);
}

TEST_F(SizeShould, computeTheSizeOfAnIntBuildTimeListWith2Elements) {
    // Given a list type
    using ListType = List<int, 0, 1>;

    // When computing its size
    constexpr size_t result = Size<ListType>::result;

    // Then its size should be its number of elements
    EXPECT_EQ(result, 2);
}

TEST_F(SizeShould, computeTheSizeOfAnUnsignedIntBuildTimeListWith0Elements) {
    // Given a list type
    using ListType = List<size_t>;

    // When computing its size
    constexpr size_t result = Size<ListType>::result;

    // Then its size should be its number of elements
    EXPECT_EQ(result, 0);
}

TEST_F(SizeShould, computeTheSizeOfAnUnsignedIntBuildTimeListWith1Elements) {
    // Given a list type
    using ListType = List<size_t, 0>;

    // When computing its size
    constexpr size_t result = Size<ListType>::result;

    // Then its size should be its number of elements
    EXPECT_EQ(result, 1);
}

TEST_F(SizeShould, computeTheSizeOfAnUnsignedIntBuildTimeListWith2Elements) {
    // Given a list type
    using ListType = List<size_t, 0, 1>;

    // When computing its size
    constexpr size_t result = Size<ListType>::result;

    // Then its size should be its number of elements
    EXPECT_EQ(result, 2);
}

TEST_F(SizeShould, computeTheSizeOfABuildTimeTypeListWith0Elements) {
    // Given a list type
    using ListType = TypeList<>;

    // When computing its size
    constexpr size_t result = Size<ListType>::result;

    // Then its size should be its number of elements
    EXPECT_EQ(result, 0);
}

TEST_F(SizeShould, computeTheSizeOfABuildTimeTypeListWith1Elements) {
    // Given a list type
    using ListType = TypeList<int>;

    // When computing its size
    constexpr size_t result = Size<ListType>::result;

    // Then its size should be its number of elements
    EXPECT_EQ(result, 1);
}

TEST_F(SizeShould, computeTheSizeOfABuildTimeTypeListWith2Elements) {
    // Given a list type
    using ListType = TypeList<int, float>;

    // When computing its size
    constexpr size_t result = Size<ListType>::result;

    // Then its size should be its number of elements
    EXPECT_EQ(result, 2);
}

class GetShould : public ::testing::Test
{};

TEST_F(GetShould, getAnElementFromAnUnsignedIntBuildTimeListAtIndex0) {
    // Given a list type
    using ListType = List<size_t, 0, 1, 2>;

    // When getting its element at index 0
    constexpr size_t result = Get<ListType, 0>::result;

    // Then the result should be its 1st element
    EXPECT_EQ(result, 0);
}

TEST_F(GetShould, getAnElementFromAnUnsignedIntBuildTimeListAtIndex1) {
    // Given a list type
    using ListType = List<size_t, 0, 1, 2>;

    // When getting its element at index 1
    constexpr size_t result = Get<ListType, 1>::result;

    // Then the result should be its 2nd element
    EXPECT_EQ(result, 1);
}

TEST_F(GetShould, getAnElementFromAnUnsignedIntBuildTimeListAtIndex2) {
    // Given a list type
    using ListType = List<size_t, 0, 1, 2>;

    // When getting its element at index 2
    constexpr size_t result = Get<ListType, 2>::result;

    // Then the result should be its 3rd element
    EXPECT_EQ(result, 2);
}

TEST_F(GetShould, getAnElementFromABuildTimeTypeListAtIndex0) {
    // Given a list type
    using ListType = TypeList<int, float, double>;

    // When getting its element at index 0
    using Result = Get<ListType, 0>::result;

    // Then the result should be its 1st element
    constexpr bool expected = std::is_same_v<Result, int>;
    EXPECT_TRUE(expected);
}

TEST_F(GetShould, getAnElementFromABuildTimeTypeListAtIndex1) {
    // Given a list type
    using ListType = TypeList<int, float, double>;

    // When getting its element at index 1
    using Result = Get<ListType, 1>::result;

    // Then the result should be its 2nd element
    constexpr bool expected = std::is_same_v<Result, float>;
    EXPECT_TRUE(expected);
}

TEST_F(GetShould, getAnElementFromABuildTimeTypeListAtIndex3) {
    // Given a list type
    using ListType = TypeList<int, float, double>;

    // When getting its element at index 2
    using Result = Get<ListType, 2>::result;

    // Then the result should be its 3rd element
    constexpr bool expected = std::is_same_v<Result, double>;
    EXPECT_TRUE(expected);
}

class ReverseShould : public ::testing::Test
{};

TEST_F(ReverseShould, reverseAnEmptyIntBuildTimeList) {
    // Given an empty int build time list
    using ListType = List<int>;

    // When we reverse this list
    using Result = Reverse<ListType>;

    // Then the result should be an empty list
    constexpr bool expected = std::is_same_v<Result, List<int>>;
    EXPECT_TRUE(expected);
}

TEST_F(ReverseShould, reverseANonEmptyIntBuildTimeList) {
    // Given a non-empty int build time list
    using ListType = List<int, 0, 1, 2>;

    // When we reverse this list
    using Result = Reverse<ListType>;

    // Then the result should be the reverse of the original list
    constexpr bool expected = std::is_same_v<Result, List<int, 2, 1, 0>>;
    EXPECT_TRUE(expected);
}

TEST_F(ReverseShould, reverseAnEmptyBuildTimeTypeList) {
    // Given an empty build time type list
    using ListType = TypeList<>;

    // When we reverse this list
    using Result = Reverse<ListType>;

    // Then the result should be an empty list
    constexpr bool expected = std::is_same_v<Result, TypeList<>>;
    EXPECT_TRUE(expected);
}

TEST_F(ReverseShould, reverseANonEmptyBuildTimeTypeList) {
    // Given a non-empty build time type list
    using ListType = TypeList<int, float, double>;

    // When we reverse this list
    using Result = Reverse<ListType>;

    // Then the result should be the reverse of the original list
    constexpr bool expected = std::is_same_v<Result, TypeList<double, float, int>>;
    EXPECT_TRUE(expected);
}

class PushRightShould : public ::testing::Test
{};

TEST_F(PushRightShould, pushAnElementToTheRightOfAnIntBuildTimeListOfSize0) {
    // Given a list type and an element to push
    using ListType = List<int>;
    constexpr int element = 0;

    // When pushing the element at the right of the list
    using Result = typename PushRight<ListType, int, element>::list;

    // Then the obtained list should be the original list concatenated with the element to push
    constexpr bool expected = std::is_same_v<Result, List<int, 0>>;
    EXPECT_TRUE(expected);
}

TEST_F(PushRightShould, pushAnElementToTheRightOfAnIntBuildTimeListOfSize1) {
    // Given a list type and an element to push
    using ListType = List<int, 0>;
    constexpr int element = 1;

    // When pushing the element at the right of the list
    using Result = typename PushRight<ListType, int, element>::list;

    // Then the obtained list should be the original list concatenated with the element to push
    constexpr bool expected = std::is_same_v<Result, List<int, 0, 1>>;
    EXPECT_TRUE(expected);
}

TEST_F(PushRightShould, pushAnElementToTheRightOfAnIntBuildTimeListOfSize2) {
    // Given a list type and an element to push
    using ListType = List<int, 0, 1>;
    constexpr int element = 2;

    // When pushing the element at the right of the list
    using Result = typename PushRight<ListType, int, element>::list;

    // Then the obtained list should be the original list concatenated with the element to push
    constexpr bool expected = std::is_same_v<Result, List<int, 0, 1, 2>>;
    EXPECT_TRUE(expected);
}

TEST_F(PushRightShould, pushAnElementToTheRightOfAnUnsignedIntBuildTimeListOfSize0) {
    // Given a list type and an element to push
    using ListType = List<size_t>;
    constexpr size_t element = 0;

    // When pushing the element at the right of the list
    using Result = typename PushRight<ListType, size_t, element>::list;

    // Then the obtained list should be the original list concatenated with the element to push
    constexpr bool expected = std::is_same_v<Result, List<size_t, 0>>;
    EXPECT_TRUE(expected);
}

TEST_F(PushRightShould, pushAnElementToTheRightOfAnUnsignedIntBuildTimeListOfSize1) {
    // Given a list type and an element to push
    using ListType = List<size_t, 0>;
    constexpr size_t element = 1;

    // When pushing the element at the right of the list
    using Result = typename PushRight<ListType, size_t, element>::list;

    // Then the obtained list should be the original list concatenated with the element to push
    constexpr bool expected = std::is_same_v<Result, List<size_t, 0, 1>>;
    EXPECT_TRUE(expected);
}

TEST_F(PushRightShould, pushAnElementToTheRightOfAnUnsignedIntBuildTimeListOfSize2) {
    // Given a list type and an element to push
    using ListType = List<size_t, 0, 1>;
    constexpr size_t element = 2;

    // When pushing the element at the right of the list
    using Result = typename PushRight<ListType, size_t, element>::list;

    // Then the obtained list should be the original list concatenated with the element to push
    constexpr bool expected = std::is_same_v<Result, List<size_t, 0, 1, 2>>;
    EXPECT_TRUE(expected);
}

class PushLeftShould : public ::testing::Test
{};

TEST_F(PushLeftShould, pushAnElementToTheLeftOfAnIntBuildTimeListOfSize0) {
    // Given a list type and an element to push
    using ListType = List<int>;
    constexpr int element = 0;

    // When pushing the element at the left of the list
    using Result = typename PushLeft<ListType, int, element>::list;

    // Then the obtained list should be the original list concatenated with the element to push
    constexpr bool expected = std::is_same_v<Result, List<int, 0>>;
    EXPECT_TRUE(expected);
}

TEST_F(PushLeftShould, pushAnElementToTheLeftOfAnIntBuildTimeListOfSize1) {
    // Given a list type and an element to push
    using ListType = List<int, 2>;
    constexpr int element = 1;

    // When pushing the element at the left of the list
    using Result = typename PushLeft<ListType, int, element>::list;

    // Then the obtained list should be the original list concatenated with the element to push
    constexpr bool expected = std::is_same_v<Result, List<int, 1, 2>>;
    EXPECT_TRUE(expected);
}

TEST_F(PushLeftShould, pushAnElementToTheLeftOfAnIntBuildTimeListOfSize2) {
    // Given a list type and an element to push
    using ListType = List<int, 1, 2>;
    constexpr int element = 0;

    // When pushing the element at the left of the list
    using Result = typename PushLeft<ListType, int, element>::list;

    // Then the obtained list should be the original list concatenated with the element to push
    constexpr bool expected = std::is_same_v<Result, List<int, 0, 1, 2>>;
    EXPECT_TRUE(expected);
}

TEST_F(PushLeftShould, pushAnElementToTheLeftOfAnUnsignedIntBuildTimeListOfSize0) {
    // Given a list type and an element to push
    using ListType = List<size_t>;
    constexpr size_t element = 2;

    // When pushing the element at the left of the list
    using Result = typename PushLeft<ListType, size_t, element>::list;

    // Then the obtained list should be the original list concatenated with the element to push
    constexpr bool expected = std::is_same_v<Result, List<size_t, 2>>;
    EXPECT_TRUE(expected);
}

TEST_F(PushLeftShould, pushAnElementToTheLeftOfAnUnsignedIntBuildTimeListOfSize1) {
    // Given a list type and an element to push
    using ListType = List<size_t, 2>;
    constexpr size_t element = 1;

    // When pushing the element at the left of the list
    using Result = typename PushLeft<ListType, size_t, element>::list;

    // Then the obtained list should be the original list concatenated with the element to push
    constexpr bool expected = std::is_same_v<Result, List<size_t, 1, 2>>;
    EXPECT_TRUE(expected);
}

TEST_F(PushLeftShould, pushAnElementToTheLeftOfAnUnsignedIntBuildTimeListOfSize2) {
    // Given a list type and an element to push
    using ListType = List<size_t, 1, 2>;
    constexpr size_t element = 0;

    // When pushing the element at the left of the list
    using Result = typename PushLeft<ListType, size_t, element>::list;

    // Then the obtained list should be the original list concatenated with the element to push
    constexpr bool expected = std::is_same_v<Result, List<size_t, 0, 1, 2>>;
    EXPECT_TRUE(expected);
}

class PopRightShould : public ::testing::Test
{};

TEST_F(PopRightShould, popAnElementFromTheRightOfAnIntBuildTimeListOfSize1) {
    // Given a list type
    using ListType = List<int, 0>;

    // When an element is poped from the right of this list
    using Result = typename PopRight<ListType>::list;
    constexpr int element = PopRight<ListType>::element;

    // Then the extracted element is the last element and
    // the output list is the same as the original without its last element
    EXPECT_EQ(element, 0);
    constexpr bool expected = std::is_same_v<Result, List<int>>;
    EXPECT_TRUE(expected);
}

TEST_F(PopRightShould, popAnElementFromTheRightOfAnIntBuildTimeListOfSize2) {
    // Given a list type
    using ListType = List<int, 0, 1>;

    // When an element is poped from the right of this list
    using Result = typename PopRight<ListType>::list;
    constexpr int element = PopRight<ListType>::element;

    // Then the extracted element is the last element and
    // the output list is the same as the original without its last element
    EXPECT_EQ(element, 1);
    constexpr bool expected = std::is_same_v<Result, List<int, 0>>;
    EXPECT_TRUE(expected);
}

TEST_F(PopRightShould, popAnElementFromTheRightOfAnIntBuildTimeListOfSize3) {
    // Given a list type
    using ListType = List<int, 0, 1, 2>;

    // When an element is poped from the right of this list
    using Result = typename PopRight<ListType>::list;
    constexpr int element = PopRight<ListType>::element;

    // Then the extracted element is the last element and
    // the output list is the same as the original without its last element
    EXPECT_EQ(element, 2);
    constexpr bool expected = std::is_same_v<Result, List<int, 0, 1>>;
    EXPECT_TRUE(expected);
}

TEST_F(PopRightShould, popAnElementFromTheRightOfAnUnsignedIntBuildTimeListOfSize1) {
    // Given a list type
    using ListType = List<size_t, 0>;

    // When an element is poped from the right of this list
    using Result = typename PopRight<ListType>::list;
    constexpr size_t element = PopRight<ListType>::element;

    // Then the extracted element is the last element and
    // the output list is the same as the original without its last element
    EXPECT_EQ(element, 0);
    constexpr bool expected = std::is_same_v<Result, List<size_t>>;
    EXPECT_TRUE(expected);
}

TEST_F(PopRightShould, popAnElementFromTheRightOfAnUnsignedIntBuildTimeListOfSize2) {
    // Given a list type
    using ListType = List<size_t, 0, 1>;

    // When an element is poped from the right of this list
    using Result = typename PopRight<ListType>::list;
    constexpr size_t element = PopRight<ListType>::element;

    // Then the extracted element is the last element and
    // the output list is the same as the original without its last element
    EXPECT_EQ(element, 1);
    constexpr bool expected = std::is_same_v<Result, List<size_t, 0>>;
    EXPECT_TRUE(expected);
}

TEST_F(PopRightShould, popAnElementFromTheRightOfAnUnsignedIntBuildTimeListOfSize3) {
    // Given a list type
    using ListType = List<size_t, 0, 1, 2>;

    // When an element is poped from the right of this list
    using Result = typename PopRight<ListType>::list;
    constexpr size_t element = PopRight<ListType>::element;

    // Then the extracted element is the last element and
    // the output list is the same as the original without its last element
    EXPECT_EQ(element, 2);
    constexpr bool expected = std::is_same_v<Result, List<size_t, 0, 1>>;
    EXPECT_TRUE(expected);
}

class PopLeftShould : public ::testing::Test
{};

TEST_F(PopLeftShould, popAnElementFromTheLeftOfAnIntBuildTimeListOfSize1) {
    // Given a list type
    using ListType = List<int, 0>;

    // When an element is poped from the left of this list
    using Result = typename PopLeft<ListType>::list;
    constexpr int element = PopLeft<ListType>::element;

    // Then the extracted element is the last element and
    // the output list is the same as the original without its last element
    EXPECT_EQ(element, 0);
    constexpr bool expected = std::is_same_v<Result, List<int>>;
    EXPECT_TRUE(expected);
}

TEST_F(PopLeftShould, popAnElementFromTheLeftOfAnIntBuildTimeListOfSize2) {
    // Given a list type
    using ListType = List<int, 0, 1>;

    // When an element is poped from the left of this list
    using Result = typename PopLeft<ListType>::list;
    constexpr int element = PopLeft<ListType>::element;

    // Then the extracted element is the last element and
    // the output list is the same as the original without its last element
    EXPECT_EQ(element, 0);
    constexpr bool expected = std::is_same_v<Result, List<int, 1>>;
    EXPECT_TRUE(expected);
}

TEST_F(PopLeftShould, popAnElementFromTheLeftOfAnIntBuildTimeListOfSize3) {
    // Given a list type
    using ListType = List<int, 0, 1, 2>;

    // When an element is poped from the left of this list
    using Result = typename PopLeft<ListType>::list;
    constexpr int element = PopLeft<ListType>::element;

    // Then the extracted element is the last element and
    // the output list is the same as the original without its last element
    EXPECT_EQ(element, 0);
    constexpr bool expected = std::is_same_v<Result, List<int, 1, 2>>;
    EXPECT_TRUE(expected);
}

TEST_F(PopLeftShould, popAnElementFromTheLeftOfAnUnsignedIntBuildTimeListOfSize1) {
    // Given a list type
    using ListType = List<size_t, 0>;

    // When an element is poped from the left of this list
    using Result = typename PopLeft<ListType>::list;
    constexpr size_t element = PopLeft<ListType>::element;

    // Then the extracted element is the last element and
    // the output list is the same as the original without its last element
    EXPECT_EQ(element, 0);
    constexpr bool expected = std::is_same_v<Result, List<size_t>>;
    EXPECT_TRUE(expected);
}

TEST_F(PopLeftShould, popAnElementFromTheLeftOfAnUnsignedIntBuildTimeListOfSize2) {
    // Given a list type
    using ListType = List<size_t, 0, 1>;

    // When an element is poped from the left of this list
    using Result = typename PopLeft<ListType>::list;
    constexpr size_t element = PopLeft<ListType>::element;

    // Then the extracted element is the last element and
    // the output list is the same as the original without its last element
    EXPECT_EQ(element, 0);
    constexpr bool expected = std::is_same_v<Result, List<size_t, 1>>;
    EXPECT_TRUE(expected);
}

TEST_F(PopLeftShould, popAnElementFromTheLeftOfAnUnsignedIntBuildTimeListOfSize3) {
    // Given a list type
    using ListType = List<size_t, 0, 1, 2>;

    // When an element is poped from the left of this list
    using Result = typename PopLeft<ListType>::list;
    constexpr size_t element = PopLeft<ListType>::element;

    // Then the extracted element is the last element and
    // the output list is the same as the original without its last element
    EXPECT_EQ(element, 0);
    constexpr bool expected = std::is_same_v<Result, List<size_t, 1, 2>>;
    EXPECT_TRUE(expected);
}

class PushTypeRightShould : public ::testing::Test
{};

TEST_F(PushTypeRightShould, pushAnElementToTheRightOfABuildTimeTypeListOfSize0) {
    // Given a type list and an element to push
    using ListType = TypeList<>;
    using Element = int;

    // When pushing the element at the right of the list
    using Result = typename PushTypeRight<ListType, Element>::list;

    // Then the obtained list should be the original list concatenated with the element to push
    constexpr bool expected = std::is_same_v<Result, TypeList<int>>;
    EXPECT_TRUE(expected);
}

TEST_F(PushTypeRightShould, pushAnElementToTheRightOfABuildTimeTypeListOfSize1) {
    // Given a type list and an element to push
    using ListType = TypeList<int>;
    using Element = float;

    // When pushing the element at the right of the list
    using Result = typename PushTypeRight<ListType, Element>::list;

    // Then the obtained list should be the original list concatenated with the element to push
    constexpr bool expected = std::is_same_v<Result, TypeList<int, float>>;
    EXPECT_TRUE(expected);
}

TEST_F(PushTypeRightShould, pushAnElementToTheRightOfABuildTimeTypeListOfSize2) {
    // Given a type list and an element to push
    using ListType = TypeList<int, float>;
    using Element = double;

    // When pushing the element at the right of the list
    using Result = typename PushTypeRight<ListType, Element>::list;

    // Then the obtained list should be the original list concatenated with the element to push
    constexpr bool expected = std::is_same_v<Result, TypeList<int, float, double>>;
    EXPECT_TRUE(expected);
}

class PushTypeLeftShould : public ::testing::Test
{};

TEST_F(PushTypeLeftShould, pushAnElementToTheLeftOfABuildTimeTypeListOfSize0) {
    // Given a type list and an element to push
    using ListType = TypeList<>;
    using Element = double;

    // When pushing the element at the left of the list
    using Result = typename PushTypeLeft<ListType, Element>::list;

    // Then the obtained list should be the original list concatenated with the element to push
    constexpr bool expected = std::is_same_v<Result, TypeList<double>>;
    EXPECT_TRUE(expected);
}

TEST_F(PushTypeLeftShould, pushAnElementToTheLeftOfABuildTimeTypeListOfSize1) {
    // Given a type list and an element to push
    using ListType = TypeList<double>;
    using Element = float;

    // When pushing the element at the left of the list
    using Result = typename PushTypeLeft<ListType, Element>::list;

    // Then the obtained list should be the original list concatenated with the element to push
    constexpr bool expected = std::is_same_v<Result, TypeList<float, double>>;
    EXPECT_TRUE(expected);
}

TEST_F(PushTypeLeftShould, pushAnElementToTheLeftOfABuildTimeTypeListOfSize2) {
    // Given a type list and an element to push
    using ListType = TypeList<float, double>;
    using Element = int;

    // When pushing the element at the left of the list
    using Result = typename PushTypeLeft<ListType, Element>::list;

    // Then the obtained list should be the original list concatenated with the element to push
    constexpr bool expected = std::is_same_v<Result, TypeList<int, float, double>>;
    EXPECT_TRUE(expected);
}

class PopTypeRightShould : public ::testing::Test
{};

TEST_F(PopTypeRightShould, popAnElementFromTheRightOfABuildTimeTypeListOfSize1) {
    // Given a list type
    using ListType = TypeList<int>;

    // When an element is poped from the right of this list
    using Result = typename PopTypeRight<ListType>::list;
    using Element = PopTypeRight<ListType>::element;

    // Then the extracted element is the last element and
    // the output list is the same as the original without its last element
    constexpr bool expectationOnElement = std::is_same_v<Element, int>;
    EXPECT_TRUE(expectationOnElement);
    constexpr bool expectationOnList = std::is_same_v<Result, TypeList<>>;
    EXPECT_TRUE(expectationOnList);
}

TEST_F(PopTypeRightShould, popAnElementFromTheRightOfABuildTimeTypeListOfSize2) {
    // Given a list type
    using ListType = TypeList<int, float>;

    // When an element is poped from the right of this list
    using Result = typename PopTypeRight<ListType>::list;
    using Element = PopTypeRight<ListType>::element;

    // Then the extracted element is the last element and
    // the output list is the same as the original without its last element
    constexpr bool expectationOnElement = std::is_same_v<Element, float>;
    EXPECT_TRUE(expectationOnElement);
    constexpr bool expectationOnList = std::is_same_v<Result, TypeList<int>>;
    EXPECT_TRUE(expectationOnList);
}

TEST_F(PopTypeRightShould, popAnElementFromTheRightOfABuildTimeTypeListOfSize3) {
    // Given a list type
    using ListType = TypeList<int, float, double>;

    // When an element is poped from the right of this list
    using Result = typename PopTypeRight<ListType>::list;
    using Element = PopTypeRight<ListType>::element;

    // Then the extracted element is the last element and
    // the output list is the same as the original without its last element
    constexpr bool expectationOnElement = std::is_same_v<Element, double>;
    EXPECT_TRUE(expectationOnElement);
    constexpr bool expectationOnList = std::is_same_v<Result, TypeList<int, float>>;
    EXPECT_TRUE(expectationOnList);
}

class PopTypeLeftShould : public ::testing::Test
{};

TEST_F(PopTypeLeftShould, popAnElementFromTheLeftOfABuildTimeTypeListOfSize1) {
    // Given a list type
    using ListType = TypeList<int>;

    // When an element is poped from the left of this list
    using Result = typename PopTypeLeft<ListType>::list;
    using Element = PopTypeLeft<ListType>::element;

    // Then the extracted element is the last element and
    // the output list is the same as the original without its last element
    constexpr bool expectationOnElement = std::is_same_v<Element, int>;
    EXPECT_TRUE(expectationOnElement);
    constexpr bool expectationOnList = std::is_same_v<Result, TypeList<>>;
    EXPECT_TRUE(expectationOnList);
}

TEST_F(PopTypeLeftShould, popAnElementFromTheLeftOfABuildTimeTypeListOfSize2) {
    // Given a list type
    using ListType = TypeList<int, float>;

    // When an element is poped from the left of this list
    using Result = typename PopTypeLeft<ListType>::list;
    using Element = PopTypeLeft<ListType>::element;

    // Then the extracted element is the last element and
    // the output list is the same as the original without its last element
    constexpr bool expectationOnElement = std::is_same_v<Element, int>;
    EXPECT_TRUE(expectationOnElement);
    constexpr bool expectationOnList = std::is_same_v<Result, TypeList<float>>;
    EXPECT_TRUE(expectationOnList);
}

TEST_F(PopTypeLeftShould, popAnElementFromTheLeftOfABuildTimeTypeListOfSize3) {
    // Given a list type
    using ListType = TypeList<int, float, double>;

    // When an element is poped from the left of this list
    using Result = typename PopTypeLeft<ListType>::list;
    using Element = PopTypeLeft<ListType>::element;

    // Then the extracted element is the last element and
    // the output list is the same as the original without its last element
    constexpr bool expectationOnElement = std::is_same_v<Element, int>;
    EXPECT_TRUE(expectationOnElement);
    constexpr bool expectationOnList = std::is_same_v<Result, TypeList<float, double>>;
    EXPECT_TRUE(expectationOnList);
}

class RangeShould : public ::testing::Test
{};

TEST_F(RangeShould, createABuildTimeRangeListOfSize0) {
    // Given the size of the expected range list
    constexpr size_t size = 0;

    // When we create a range of this size
    using Result = Range<size>;

    // Then the result should be a build time list containing the first non-negative integers
    constexpr bool expected = std::is_same_v<Result, IntegerList<>>;
}

TEST_F(RangeShould, createABuildTimeRangeListOfSize1) {
    // Given the size of the expected range list
    constexpr size_t size = 1;

    // When we create a range of this size
    using Result = Range<size>;

    // Then the result should be a build time list containing the first non-negative integers
    constexpr bool expected = std::is_same_v<Result, IntegerList<0>>;
}

TEST_F(RangeShould, createABuildTimeRangeListOfSize2) {
    // Given the size of the expected range list
    constexpr size_t size = 2;

    // When we create a range of this size
    using Result = Range<size>;

    // Then the result should be a build time list containing the first non-negative integers
    constexpr bool expected = std::is_same_v<Result, IntegerList<0, 1>>;
}

TEST_F(RangeShould, createABuildTimeRangeListOfSize3) {
    // Given the size of the expected range list
    constexpr size_t size = 3;

    // When we create a range of this size
    using Result = Range<size>;

    // Then the result should be a build time list containing the first non-negative integers
    constexpr bool expected = std::is_same_v<Result, IntegerList<0, 1, 2>>;
}

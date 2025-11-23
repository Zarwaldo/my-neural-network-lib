#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <helpers/RawTuple.h>

template <typename T>
class Content
{
public:
    Content(const T& value)
        : m_value(value)
    {}

    const T& getValue() const
    {
        return m_value;
    }

    operator T() const
    {
        return m_value;
    }

private:
    T m_value;
};

struct HostMockApplyFunction
{
    MOCK_METHOD(HOST const Content<double>&, constMockMethod, (const int&, const Content<float>&, const Content<double>&), (const));
    MOCK_METHOD(HOST Content<double>&, mockMethod, (int&, Content<float>&, Content<double>&), ());
};

struct HostDeviceMockApplyFunction
{
    MOCK_METHOD(HOST DEVICE const Content<double>&, constMockMethod, (const int&, const Content<float>&, const Content<double>&), (const));
    MOCK_METHOD(HOST DEVICE Content<double>&, mockMethod, (int&, Content<float>&, Content<double>&), ());
};

template <typename InputType, typename Enable = void>
struct SampleMapFunction
{};

template <typename ScalarType>
struct SampleMapFunction<ScalarType, std::enable_if_t<std::is_scalar_v<ScalarType>>>
{
    HOST static ScalarType doIt(const ScalarType& input, double offset)
    {
        return input + static_cast<ScalarType>(offset);
    }
};

template <typename ContentParam>
struct SampleMapFunction<Content<ContentParam>, void>
{
    HOST static ContentParam doIt(const Content<ContentParam>& input, double offset)
    {
        return input.getValue() + static_cast<ContentParam>(offset);
    }
};

class RawTupleShould : public ::testing::Test
{
protected:
    int a = 5;
    Content<float> b = Content<float>(3.14f);
    const Content<double> c = Content<double>(6.28);
    int aToMove = a;
    Content<float> bToMove = b;
    Content<double> cToMove = c;
};

TEST_F(RawTupleShould, getItsFirstElementFromConstLValueRawTuple) {
    // Given a tuple
    const RawTuple<int&, Content<float>&, const Content<double>&> tuple = makeRawTuple(a, b, c);

    // When getting its first element
    const int& result = tuple.get<0>();

    // Then the result is indeed equal to its first element, but is in a different memory space
    EXPECT_EQ(result, a);
    EXPECT_NE(&result, &a);
}

TEST_F(RawTupleShould, getItsFirstElementFromNonConstLValueRawTuple) {
    // Given a tuple
    RawTuple<int&, Content<float>&, const Content<double>&> tuple = makeRawTuple(a, b, c);

    // When getting its first element
    int& result = tuple.get<0>();

    // Then the result is indeed equal to its first element, but is in a different memory space
    EXPECT_EQ(result, a);
    EXPECT_NE(&result, &a);
}

TEST_F(RawTupleShould, getItsFirstElementFromConstRValueRawTuple) {
    // Given a tuple
    const RawTuple<int, Content<float>, Content<double>> tuple = makeRawTuple(std::move(aToMove), std::move(bToMove), std::move(cToMove));

    // When getting its first element
    const int& result = tuple.get<0>();

    // Then the result is indeed equal to its first element, but is in a different memory space
    EXPECT_EQ(result, a);
    EXPECT_NE(&result, &a);
}

TEST_F(RawTupleShould, getItsFirstElementFromNonConstRValueRawTuple) {
    // Given a tuple
    RawTuple<int, Content<float>, Content<double>> tuple = makeRawTuple(std::move(aToMove), std::move(bToMove), std::move(cToMove));

    // When getting its first element
    int& result = tuple.get<0>();

    // Then the result is indeed equal to its first element, but is in a different memory space
    EXPECT_EQ(result, a);
    EXPECT_NE(&result, &a);
}

TEST_F(RawTupleShould, getItsSecondElementFromConstLValueRawTuple) {
    // Given a tuple
    const RawTuple<int&, Content<float>&, const Content<double>&> tuple = makeRawTuple(a, b, c);

    // When getting its second element
    const Content<float>& result = tuple.get<1>();

    // Then the result is indeed equal to its second element, but is in a different memory space
    EXPECT_EQ(result, b);
    EXPECT_NE(&result, &b);
}

TEST_F(RawTupleShould, getItsSecondElementFromNonConstLValueRawTuple) {
    // Given a tuple
    RawTuple<int&, Content<float>&, const Content<double>&> tuple = makeRawTuple(a, b, c);

    // When getting its second element
    Content<float>& result = tuple.get<1>();

    // Then the result is indeed equal to its second element, but is in a different memory space
    EXPECT_EQ(result, b);
    EXPECT_NE(&result, &b);
}

TEST_F(RawTupleShould, getItsSecondElementFromConstRValueRawTuple) {
    // Given a tuple
    const RawTuple<int, Content<float>, Content<double>> tuple = makeRawTuple(std::move(aToMove), std::move(bToMove), std::move(cToMove));

    // When getting its second element
    const Content<float>& result = tuple.get<1>();

    // Then the result is indeed equal to its second element, but is in a different memory space
    EXPECT_EQ(result, b);
    EXPECT_NE(&result, &b);
}

TEST_F(RawTupleShould, getItsSecondElementFromNonConstRValueRawTuple) {
    // Given a tuple
    RawTuple<int, Content<float>, Content<double>> tuple = makeRawTuple(std::move(aToMove), std::move(bToMove), std::move(cToMove));

    // When getting its second element
    Content<float>& result = tuple.get<1>();

    // Then the result is indeed equal to its second element, but is in a different memory space
    EXPECT_EQ(result, b);
    EXPECT_NE(&result, &b);
}

TEST_F(RawTupleShould, getItsThirdElementFromConstLValueRawTuple) {
    // Given a tuple
    const RawTuple<int&, Content<float>&, const Content<double>&> tuple = makeRawTuple(a, b, c);

    // When getting its third element
    const Content<double>& result = tuple.get<2>();

    // Then the result is indeed equal to its third element, but is in a different memory space
    EXPECT_EQ(result, c);
    EXPECT_NE(&result, &c);
}

TEST_F(RawTupleShould, getItsThirdElementFromNonConstLValueRawTuple) {
    // Given a tuple
    RawTuple<int&, Content<float>&, const Content<double>&> tuple = makeRawTuple(a, b, c);

    // When getting its third element
    Content<double>& result = tuple.get<2>();

    // Then the result is indeed equal to its third element, but is in a different memory space
    EXPECT_EQ(result, c);
    EXPECT_NE(&result, &c);
}

TEST_F(RawTupleShould, getItsThirdElementFromConstRValueRawTuple) {
    // Given a tuple
    const RawTuple<int, Content<float>, Content<double>> tuple = makeRawTuple(std::move(aToMove), std::move(bToMove), std::move(cToMove));

    // When getting its third element
    const Content<double>& result = tuple.get<2>();

    // Then the result is indeed equal to its third element, but is in a different memory space
    EXPECT_EQ(result, c);
    EXPECT_NE(&result, &c);
}

TEST_F(RawTupleShould, getItsThirdElementFromNonConstRValueRawTuple) {
    // Given a tuple
    RawTuple<int, Content<float>, Content<double>> tuple = makeRawTuple(std::move(aToMove), std::move(bToMove), std::move(cToMove));

    // When getting its third element
    Content<double>& result = tuple.get<2>();

    // Then the result is indeed equal to its third element, but is in a different memory space
    EXPECT_EQ(result, c);
    EXPECT_NE(&result, &c);
}

TEST_F(RawTupleShould, applyAHostFunctionOnAConstLValueTuple) {
    // The mock method should be called once with the content of the tuple as arguments
    HostMockApplyFunction applyFunctionObj;
    EXPECT_CALL(applyFunctionObj, constMockMethod(::testing::Eq(a), ::testing::Eq(b), ::testing::Eq(c)))
        .Times(1)
        .WillRepeatedly(::testing::Invoke([](const int& a, const Content<float>& b, const Content<double>& c) -> const Content<double>& {
            return c;
        }));

    // Given a tuple
    const RawTuple<const int&, const Content<float>&, const Content<double>&> tuple = makeRawTuple(a, b, c);

    // When applying this tuple on the mock method
    const Content<double>& result = tuple.hostApply(
        [&applyFunctionObj](const int& a, const Content<float>& b, const Content<double>& c) -> const Content<double>& {
            return applyFunctionObj.constMockMethod(a, b, c);
        }
    );

    // Then the result is the same as what the mock method returned
    EXPECT_EQ(result, c);
}

TEST_F(RawTupleShould, applyAHostFunctionOnAConstRValueTuple) {
    // The mock method should be called once with the content of the tuple as arguments
    HostMockApplyFunction applyFunctionObj;
    EXPECT_CALL(applyFunctionObj, constMockMethod(::testing::Eq(a), ::testing::Eq(b), ::testing::Eq(c)))
        .Times(1)
        .WillRepeatedly(::testing::Invoke([](const int& a, const Content<float>& b, const Content<double>& c) -> const Content<double>& {
            return c;
        }));

    // Given a tuple
    const RawTuple<int, Content<float>, Content<double>> tuple = makeRawTuple(std::move(aToMove), std::move(bToMove), std::move(cToMove));

    // When applying this tuple on the mock method
    const Content<double>& result = tuple.hostApply(
        [&applyFunctionObj](const int& a, const Content<float>& b, const Content<double>& c) -> const Content<double>& {
            return applyFunctionObj.constMockMethod(a, b, c);
        }
    );

    // Then the result is the same as what the mock method returned
    EXPECT_EQ(result, c);
}

TEST_F(RawTupleShould, applyAHostFunctionOnANonConstLValueTuple) {
    // The mock method should be called once with the content of the tuple as arguments
    HostMockApplyFunction applyFunctionObj;
    EXPECT_CALL(applyFunctionObj, mockMethod(::testing::Eq(a), ::testing::Eq(b), ::testing::Eq(c)))
        .Times(1)
        .WillRepeatedly(::testing::Invoke([](int& a, Content<float>& b, Content<double>& c) -> Content<double>& {
            a = 0;
            return c;
        }));

    // Given a tuple
    RawTuple<const int&, const Content<float>&, const Content<double>&> tuple = makeRawTuple(a, b, c);

    // When applying this tuple on the mock method
    Content<double>& result = tuple.hostApply(
        [&applyFunctionObj](int& a, Content<float>& b, Content<double>& c) -> Content<double>& {
            return applyFunctionObj.mockMethod(a, b, c);
        }
    );

    // Then the result is the same as what the mock method returned and the tuple should be affected by the changes
    EXPECT_EQ(result, c);
    EXPECT_EQ(tuple.get<0>(), 0);
}

TEST_F(RawTupleShould, applyAHostFunctionOnANonConstRValueTuple) {
    // The mock method should be called once with the content of the tuple as arguments
    HostMockApplyFunction applyFunctionObj;
    EXPECT_CALL(applyFunctionObj, mockMethod(::testing::Eq(a), ::testing::Eq(b), ::testing::Eq(c)))
        .Times(1)
        .WillRepeatedly(::testing::Invoke([](int& a, Content<float>& b, Content<double>& c) -> Content<double>& {
            a = 0;
            return c;
        }));

    // Given a tuple
    RawTuple<int, Content<float>, Content<double>> tuple = makeRawTuple(std::move(aToMove), std::move(bToMove), std::move(cToMove));

    // When applying this tuple on the mock method
    Content<double>& result = tuple.hostApply(
        [&applyFunctionObj](int& a, Content<float>& b, Content<double>& c) -> Content<double>& {
            return applyFunctionObj.mockMethod(a, b, c);
        }
    );

    // Then the result is the same as what the mock method returned and the tuple should be affected by the changes
    EXPECT_EQ(result, c);
    EXPECT_EQ(tuple.get<0>(), 0);
}

TEST_F(RawTupleShould, applyAHostDeviceFunctionOnAConstLValueTuple) {
    // The mock method should be called once with the content of the tuple as arguments
    HostDeviceMockApplyFunction applyFunctionObj;
    EXPECT_CALL(applyFunctionObj, constMockMethod(::testing::Eq(a), ::testing::Eq(b), ::testing::Eq(c)))
        .Times(1)
        .WillRepeatedly(::testing::Invoke([](const int& a, const Content<float>& b, const Content<double>& c) -> const Content<double>& {
            return c;
        }));

    // Given a tuple
    const RawTuple<const int&, const Content<float>&, const Content<double>&> tuple = makeRawTuple(a, b, c);

    // When applying this tuple on the mock method
    const Content<double>& result = tuple.apply(
        [&applyFunctionObj](const int& a, const Content<float>& b, const Content<double>& c) -> const Content<double>& {
            return applyFunctionObj.constMockMethod(a, b, c);
        }
    );

    // Then the result is the same as what the mock method returned
    EXPECT_EQ(result, c);
}

TEST_F(RawTupleShould, applyAHostDeviceFunctionOnAConstRValueTuple) {
    // The mock method should be called once with the content of the tuple as arguments
    HostDeviceMockApplyFunction applyFunctionObj;
    EXPECT_CALL(applyFunctionObj, constMockMethod(::testing::Eq(a), ::testing::Eq(b), ::testing::Eq(c)))
        .Times(1)
        .WillRepeatedly(::testing::Invoke([](const int& a, const Content<float>& b, const Content<double>& c) -> const Content<double>& {
            return c;
        }));

    // Given a tuple
    const RawTuple<int, Content<float>, Content<double>> tuple = makeRawTuple(std::move(aToMove), std::move(bToMove), std::move(cToMove));

    // When applying this tuple on the mock method
    const Content<double>& result = tuple.apply(
        [&applyFunctionObj](const int& a, const Content<float>& b, const Content<double>& c) -> const Content<double>& {
            return applyFunctionObj.constMockMethod(a, b, c);
        }
    );

    // Then the result is the same as what the mock method returned
    EXPECT_EQ(result, c);
}

TEST_F(RawTupleShould, applyAHostDeviceFunctionOnANonConstLValueTuple) {
    // The mock method should be called once with the content of the tuple as arguments
    HostDeviceMockApplyFunction applyFunctionObj;
    EXPECT_CALL(applyFunctionObj, mockMethod(::testing::Eq(a), ::testing::Eq(b), ::testing::Eq(c)))
        .Times(1)
        .WillRepeatedly(::testing::Invoke([](int& a, Content<float>& b, Content<double>& c) -> Content<double>& {
            a = 0;
            return c;
        }));

    // Given a tuple
    RawTuple<const int&, const Content<float>&, const Content<double>&> tuple = makeRawTuple(a, b, c);

    // When applying this tuple on the mock method
    Content<double>& result = tuple.apply(
        [&applyFunctionObj](int& a, Content<float>& b, Content<double>& c) -> Content<double>& {
            return applyFunctionObj.mockMethod(a, b, c);
        }
    );

    // Then the result is the same as what the mock method returned and the tuple should be affected by the changes
    EXPECT_EQ(result, c);
    EXPECT_EQ(tuple.get<0>(), 0);
}

TEST_F(RawTupleShould, applyAHostDeviceFunctionOnANonConstRValueTuple) {
    // The mock method should be called once with the content of the tuple as arguments
    HostDeviceMockApplyFunction applyFunctionObj;
    EXPECT_CALL(applyFunctionObj, mockMethod(::testing::Eq(a), ::testing::Eq(b), ::testing::Eq(c)))
        .Times(1)
        .WillRepeatedly(::testing::Invoke([](int& a, Content<float>& b, Content<double>& c) -> Content<double>& {
            a = 0;
            return c;
        }));

    // Given a tuple
    RawTuple<int, Content<float>, Content<double>> tuple = makeRawTuple(std::move(aToMove), std::move(bToMove), std::move(cToMove));

    // When applying this tuple on the mock method
    Content<double>& result = tuple.apply(
        [&applyFunctionObj](int& a, Content<float>& b, Content<double>& c) -> Content<double>& {
            return applyFunctionObj.mockMethod(a, b, c);
        }
    );

    // Then the result is the same as what the mock method returned and the tuple should be affected by the changes
    EXPECT_EQ(result, c);
    EXPECT_EQ(tuple.get<0>(), 0);
}

TEST_F(RawTupleShould, mapValuesFromAConstLValueReferencesTuple) {
    // Given a tuple
    const RawTuple<const int&, const Content<float>&, const Content<double>&> tuple = makeRawTuple(a, b, c);

    // When mapping SampleMapFunction on the tuple
    const RawTuple<int, float, double> result = tuple.hostMap<SampleMapFunction>(3.14);

    // Then the resulting tuple holds the results of
    // SampleMapFunction applied to each value following the context arguments
    EXPECT_EQ(result.get<0>(), 8);
    EXPECT_EQ(result.get<1>(), 6.28f);
    EXPECT_EQ(result.get<2>(), 9.42);
}

TEST_F(RawTupleShould, mapValuesFromAConstRValueReferencesTuple) {
    // Given a tuple
    const RawTuple<int, Content<float>, Content<double>> tuple = makeRawTuple(std::move(aToMove), std::move(bToMove), std::move(cToMove));

    // When mapping SampleMapFunction on the tuple
    const RawTuple<int, float, double> result = tuple.hostMap<SampleMapFunction>(3.14);

    // Then the resulting tuple holds the results of
    // SampleMapFunction applied to each value following the context arguments
    EXPECT_EQ(result.get<0>(), 8);
    EXPECT_EQ(result.get<1>(), 6.28f);
    EXPECT_EQ(result.get<2>(), 9.42);
}

TEST_F(RawTupleShould, mapValuesFromANonConstLValueReferencesTuple) {
    // Given a tuple
    RawTuple<const int&, const Content<float>&, const Content<double>&> tuple = makeRawTuple(a, b, c);

    // When mapping SampleMapFunction on the tuple
    const RawTuple<int, float, double> result = tuple.hostMap<SampleMapFunction>(3.14);

    // Then the resulting tuple holds the results of
    // SampleMapFunction applied to each value following the context arguments
    EXPECT_EQ(result.get<0>(), 8);
    EXPECT_EQ(result.get<1>(), 6.28f);
    EXPECT_EQ(result.get<2>(), 9.42);
}

TEST_F(RawTupleShould, mapValuesFromANonConstRValueReferencesTuple) {
    // Given a tuple
    RawTuple<int, Content<float>, Content<double>> tuple = makeRawTuple(std::move(aToMove), std::move(bToMove), std::move(cToMove));

    // When mapping SampleMapFunction on the tuple
    const RawTuple<int, float, double> result = tuple.hostMap<SampleMapFunction>(3.14);

    // Then the resulting tuple holds the results of
    // SampleMapFunction applied to each value following the context arguments
    EXPECT_EQ(result.get<0>(), 8);
    EXPECT_EQ(result.get<1>(), 6.28f);
    EXPECT_EQ(result.get<2>(), 9.42);
}

TEST_F(RawTupleShould, castAnLValueTupleIfAllTypeAreConvertible) {
    // Given an l-value tuple of convertible types
    const RawTuple<const Content<float>&, const Content<double>&> tuple = makeRawTuple(b, c);

    // When converting this tuple
    const RawTuple<float, double> result = static_cast<RawTuple<float, double>>(tuple);

    // Then the result should contain the converted values
    EXPECT_EQ(result.get<0>(), b.getValue());
    EXPECT_EQ(result.get<1>(), c.getValue());
}

TEST_F(RawTupleShould, castAnRValueTupleIfAllTypeAreConvertible) {
    // Given an r-value tuple of convertible types
    const RawTuple<Content<float>, Content<double>> tuple = makeRawTuple(std::move(bToMove), std::move(cToMove));

    // When converting this tuple
    const RawTuple<float, double> result = static_cast<RawTuple<float, double>>(tuple);

    // Then the result should contain the converted values
    EXPECT_EQ(result.get<0>(), b.getValue());
    EXPECT_EQ(result.get<1>(), c.getValue());
}

TEST_F(RawTupleShould, getConstPointersFromAConstLValueReferencesTuple) {
    // Given a tuple
    const RawTuple<const int&, const Content<float>&, const Content<double>&> tuple = makeRawTuple(a, b, c);

    // When getting its const pointers
    const RawTuple<const int*, const Content<float>*, const Content<double>*> result = tuple.getPointers();

    // Then the pointers of the resulting tuple point to the data of the original tuple
    EXPECT_EQ(result.get<0>(), &tuple.get<0>());
    EXPECT_EQ(result.get<1>(), &tuple.get<1>());
    EXPECT_EQ(result.get<2>(), &tuple.get<2>());
}

TEST_F(RawTupleShould, getConstPointersFromAConstRValueReferencesTuple) {
    // Given a tuple
    const RawTuple<int, Content<float>, Content<double>> tuple = makeRawTuple(std::move(aToMove), std::move(bToMove), std::move(cToMove));

    // When getting its const pointers
    const RawTuple<const int*, const Content<float>*, const Content<double>*> result = tuple.getPointers();

    // Then the pointers of the resulting tuple point to the data of the original tuple
    EXPECT_EQ(result.get<0>(), &tuple.get<0>());
    EXPECT_EQ(result.get<1>(), &tuple.get<1>());
    EXPECT_EQ(result.get<2>(), &tuple.get<2>());
}

TEST_F(RawTupleShould, getPointersFromAnLValueReferencesTuple) {
    // Given a tuple
    RawTuple<const int&, const Content<float>&, const Content<double>&> tuple = makeRawTuple(a, b, c);

    // When getting its const pointers
    const RawTuple<int*, Content<float>*, Content<double>*> result = tuple.getPointers();

    // Then the pointers of the resulting tuple point to the data of the original tuple
    EXPECT_EQ(result.get<0>(), &tuple.get<0>());
    EXPECT_EQ(result.get<1>(), &tuple.get<1>());
    EXPECT_EQ(result.get<2>(), &tuple.get<2>());
}

TEST_F(RawTupleShould, getPointersFromAnRValueReferencesTuple) {
    // Given a tuple
    RawTuple<int, Content<float>, Content<double>> tuple = makeRawTuple(std::move(aToMove), std::move(bToMove), std::move(cToMove));

    // When getting its const pointers
    const RawTuple<int*, Content<float>*, Content<double>*> result = tuple.getPointers();

    // Then the pointers of the resulting tuple point to the data of the original tuple
    EXPECT_EQ(result.get<0>(), &tuple.get<0>());
    EXPECT_EQ(result.get<1>(), &tuple.get<1>());
    EXPECT_EQ(result.get<2>(), &tuple.get<2>());
}

TEST_F(RawTupleShould, letAnLValueReferencesTupleBeWriteableFromItsPointersTuple) {
    // Given a non-const tuple and its non-const pointers tuple
    RawTuple<const int&, const Content<float>&, const Content<double>&> tuple = makeRawTuple(a, b, c);
    const RawTuple<int*, Content<float>*, Content<double>*> pointers = tuple.getPointers();

    // When writing in the original tuple from its pointers tuple
    *pointers.get<0>() = 8;
    *pointers.get<1>() = 6.28f;
    *pointers.get<2>() = 9.42;

    // Then the pointers of the resulting tuple point to the data of the original tuple
    EXPECT_EQ(tuple.get<0>(), 8);
    EXPECT_EQ(tuple.get<1>(), 6.28f);
    EXPECT_EQ(tuple.get<2>(), 9.42);
}

TEST_F(RawTupleShould, letAnRValueReferencesTupleBeWriteableFromItsPointersTuple) {
    // Given a non-const tuple and its non-const pointers tuple
    RawTuple<int, Content<float>, Content<double>> tuple = makeRawTuple(std::move(aToMove), std::move(bToMove), std::move(cToMove));
    const RawTuple<int*, Content<float>*, Content<double>*> pointers = tuple.getPointers();

    // When writing in the original tuple from its pointers tuple
    *pointers.get<0>() = 8;
    *pointers.get<1>() = 6.28f;
    *pointers.get<2>() = 9.42;

    // Then the pointers of the resulting tuple point to the data of the original tuple
    EXPECT_EQ(tuple.get<0>(), 8);
    EXPECT_EQ(tuple.get<1>(), 6.28f);
    EXPECT_EQ(tuple.get<2>(), 9.42);
}

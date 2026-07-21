#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <helpers/PolymorphicTuple.h>

class AbstractContent
{
public:
    virtual ~AbstractContent()
    {}

    virtual double toDouble() const = 0;

    virtual void fromDouble(double value) = 0;

    virtual bool operator==(const AbstractContent& other) const = 0;
};

template <typename T>
class Content : public AbstractContent
{
public:
    Content(const T& value)
        : m_value(value)
    {}

    virtual ~Content()
    {}

    const T& getValue() const
    {
        return m_value;
    }

    operator T() const
    {
        return m_value;
    }

    virtual double toDouble() const override
    {
        return static_cast<double>(m_value);
    }

    virtual void fromDouble(double value) override
    {
        m_value = static_cast<T>(value);
    }

    virtual bool operator==(const AbstractContent& other) const override
    {
        const Content<T>* otherPtr = dynamic_cast<const Content<T>*>(&other);
        if (otherPtr == nullptr)
        {
            return false;
        }

        return m_value == otherPtr->m_value;
    }

private:
    T m_value;
};

struct HostMockApplyFunction
{
    MOCK_METHOD(HOST const Content<double>&, constMockMethod, (const AbstractContent&, const Content<float>&, const Content<double>&), (const));
    MOCK_METHOD(HOST const Content<double>&, mockMethod, (AbstractContent&, Content<float>&, const Content<double>&), ());
};

struct HostDeviceMockApplyFunction
{
    MOCK_METHOD(HOST DEVICE const Content<double>&, constMockMethod, (const AbstractContent&, const Content<float>&, const Content<double>&), (const));
    MOCK_METHOD(HOST DEVICE const Content<double>&, mockMethod, (AbstractContent&, Content<float>&, const Content<double>&), ());
};

template <typename InputType, typename Enable = void>
struct SampleMapFunction
{};

template <typename AbstractType>
struct SampleMapFunction<AbstractType, std::enable_if_t<std::is_abstract_v<AbstractType>>>
{
    HOST static double doIt(const AbstractType& input, double offset)
    {
        return input.toDouble() + offset;
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

class PolymorphicTupleShould : public ::testing::Test
{
protected:
    void SetUp() override
    {
        a = new Content<int>(5);
    }

    void TearDown() override
    {
        delete a;
    }

    AbstractContent* a;
    Content<float> b = Content<float>(3.14f);
    const Content<double> c = Content<double>(6.28);
    Content<float> bToMove = b;
    Content<double> cToMove = c;
};

TEST_F(PolymorphicTupleShould, getItsFirstElementFromConstLValuePolymorphicTuple) {
    // Given a tuple
    const PolymorphicTuple<AbstractContent&, Content<float>&, const Content<double>&> tuple = makePolymorphicTuple(*a, b, c);

    // When getting its first element
    const AbstractContent& result = tuple.get<0>();

    // Then the result is a reference to the same memory space as its first element
    EXPECT_EQ(&result, a);
}

TEST_F(PolymorphicTupleShould, getItsFirstElementFromNonConstLValuePolymorphicTuple) {
    // Given a tuple
    PolymorphicTuple<AbstractContent&, Content<float>&, const Content<double>&> tuple = makePolymorphicTuple(*a, b, c);

    // When getting its first element
    AbstractContent& result = tuple.get<0>();

    // Then the result is a reference to the same memory space as its first element
    EXPECT_EQ(&result, a);
}

TEST_F(PolymorphicTupleShould, getItsFirstElementFromConstRValuePolymorphicTuple) {
    // Given a tuple
    const PolymorphicTuple<AbstractContent&, Content<float>, Content<double>> tuple = makePolymorphicTuple(*a, std::move(bToMove), std::move(cToMove));

    // When getting its first element
    const AbstractContent& result = tuple.get<0>();

    // Then the result is a reference to the same memory space as its first element
    EXPECT_EQ(&result, a);
}

TEST_F(PolymorphicTupleShould, getItsFirstElementFromNonConstRValuePolymorphicTuple) {
    // Given a tuple
    PolymorphicTuple<AbstractContent&, Content<float>, Content<double>> tuple = makePolymorphicTuple(*a, std::move(bToMove), std::move(cToMove));

    // When getting its first element
    AbstractContent& result = tuple.get<0>();

    // Then the result is a reference to the same memory space as its first element
    EXPECT_EQ(&result, a);
}

TEST_F(PolymorphicTupleShould, getItsSecondElementFromConstLValuePolymorphicTuple) {
    // Given a tuple
    const PolymorphicTuple<AbstractContent&, Content<float>&, const Content<double>&> tuple = makePolymorphicTuple(*a, b, c);

    // When getting its second element
    const Content<float>& result = tuple.get<1>();

    // Then the result is a reference to the same memory space as its second element
    EXPECT_EQ(&result, &b);
}

TEST_F(PolymorphicTupleShould, getItsSecondElementFromNonConstLValuePolymorphicTuple) {
    // Given a tuple
    PolymorphicTuple<AbstractContent&, Content<float>&, const Content<double>&> tuple = makePolymorphicTuple(*a, b, c);

    // When getting its second element
    Content<float>& result = tuple.get<1>();

    // Then the result is a reference to the same memory space as its second element
    EXPECT_EQ(&result, &b);
}

TEST_F(PolymorphicTupleShould, getItsSecondElementFromConstRValuePolymorphicTuple) {
    // Given a tuple
    const PolymorphicTuple<AbstractContent&, Content<float>, Content<double>> tuple = makePolymorphicTuple(*a, std::move(bToMove), std::move(cToMove));

    // When getting its second element (non-reference)
    const Content<float>& result = tuple.get<1>();

    // Then the result is indeed equal to its second element, but located in a different memory space than the original variable
    EXPECT_EQ(result, b);
    EXPECT_NE(&result, &bToMove);
}

TEST_F(PolymorphicTupleShould, getItsSecondElementFromNonConstRValuePolymorphicTuple) {
    // Given a tuple
    PolymorphicTuple<AbstractContent&, Content<float>, Content<double>> tuple = makePolymorphicTuple(*a, std::move(bToMove), std::move(cToMove));

    // When getting its second element (non-reference)
    Content<float>& result = tuple.get<1>();

    // Then the result is indeed equal to its second element, but located in a different memory space than the original variable
    EXPECT_EQ(result, b);
    EXPECT_NE(&result, &bToMove);
}

TEST_F(PolymorphicTupleShould, getItsThirdElementFromConstLValuePolymorphicTuple) {
    // Given a tuple
    const PolymorphicTuple<AbstractContent&, Content<float>&, const Content<double>&> tuple = makePolymorphicTuple(*a, b, c);

    // When getting its third element
    const Content<double>& result = tuple.get<2>();

    // Then the result is a reference to the same memory space as its third element
    EXPECT_EQ(&result, &c);
}

TEST_F(PolymorphicTupleShould, getItsThirdElementFromNonConstLValuePolymorphicTuple) {
    // Given a tuple
    PolymorphicTuple<AbstractContent&, Content<float>&, const Content<double>&> tuple = makePolymorphicTuple(*a, b, c);

    // When getting its third element
    const Content<double>& result = tuple.get<2>();

    // Then the result is a reference to the same memory space as its third element
    EXPECT_EQ(&result, &c);
}

TEST_F(PolymorphicTupleShould, getItsThirdElementFromConstRValuePolymorphicTuple) {
    // Given a tuple
    const PolymorphicTuple<AbstractContent&, Content<float>, Content<double>> tuple = makePolymorphicTuple(*a, std::move(bToMove), std::move(cToMove));

    // When getting its third element (non-reference)
    const Content<double>& result = tuple.get<2>();

    // Then the result is indeed equal to its third element, but located in a different memory space than the original variable
    EXPECT_EQ(result, c);
    EXPECT_NE(&result, &cToMove);
}

TEST_F(PolymorphicTupleShould, getItsThirdElementFromNonConstRValuePolymorphicTuple) {
    // Given a tuple
    PolymorphicTuple<AbstractContent&, Content<float>, Content<double>> tuple = makePolymorphicTuple(*a, std::move(bToMove), std::move(cToMove));

    // When getting its third element (non-reference)
    Content<double>& result = tuple.get<2>();

    // Then the result is indeed equal to its third element, but located in a different memory space than the original variable
    EXPECT_EQ(result, c);
    EXPECT_NE(&result, &cToMove);
}

TEST_F(PolymorphicTupleShould, applyAHostFunctionOnAConstLValueTuple) {
    // The mock method should be called once with the content of the tuple as arguments
    HostMockApplyFunction applyFunctionObj;
    EXPECT_CALL(applyFunctionObj, constMockMethod(::testing::Eq<AbstractContent&>(*a), ::testing::Eq(b), ::testing::Eq(c)))
        .Times(1)
        .WillRepeatedly(::testing::Invoke([](const AbstractContent& a, const Content<float>& b, const Content<double>& c) -> const Content<double>& {
            return c;
        }));

    // Given a tuple
    const PolymorphicTuple<AbstractContent&, Content<float>&, const Content<double>&> tuple = makePolymorphicTuple(*a, b, c);

    // When applying this tuple on the mock method
    const Content<double>& result = tuple.hostApply(
        [&applyFunctionObj](const AbstractContent& a, const Content<float>& b, const Content<double>& c) -> const Content<double>& {
            return applyFunctionObj.constMockMethod(a, b, c);
        }
    );

    // Then the result is the same as what the mock method returned
    EXPECT_EQ(result, c);
}

TEST_F(PolymorphicTupleShould, applyAHostFunctionOnAConstRValueTuple) {
    // The mock method should be called once with the content of the tuple as arguments
    HostMockApplyFunction applyFunctionObj;
    EXPECT_CALL(applyFunctionObj, constMockMethod(::testing::Eq<AbstractContent&>(*a), ::testing::Eq(b), ::testing::Eq(c)))
        .Times(1)
        .WillRepeatedly(::testing::Invoke([](const AbstractContent& a, const Content<float>& b, const Content<double>& c) -> const Content<double>& {
            return c;
        }));

    // Given a tuple
    const PolymorphicTuple<AbstractContent&, Content<float>, Content<double>> tuple = makePolymorphicTuple(*a, std::move(bToMove), std::move(cToMove));

    // When applying this tuple on the mock method
    const Content<double>& result = tuple.hostApply(
        [&applyFunctionObj](const AbstractContent& a, const Content<float>& b, const Content<double>& c) -> const Content<double>& {
            return applyFunctionObj.constMockMethod(a, b, c);
        }
    );

    // Then the result is the same as what the mock method returned
    EXPECT_EQ(result, c);
}

TEST_F(PolymorphicTupleShould, applyAHostFunctionOnANonConstLValueTuple) {
    // The mock method should be called once with the content of the tuple as arguments
    HostMockApplyFunction applyFunctionObj;
    EXPECT_CALL(applyFunctionObj, mockMethod(::testing::Eq<AbstractContent&>(*a), ::testing::Eq(b), ::testing::Eq(c)))
        .Times(1)
        .WillRepeatedly(::testing::Invoke([](AbstractContent& a, Content<float>& b, const Content<double>& c) -> const Content<double>& {
            b = Content<float>(0.f);
            return c;
        }));

    // Given a tuple
    PolymorphicTuple<AbstractContent&, Content<float>&, const Content<double>&> tuple = makePolymorphicTuple(*a, b, c);

    // When applying this tuple on the mock method
    const Content<double>& result = tuple.hostApply(
        [&applyFunctionObj](AbstractContent& a, Content<float>& b, const Content<double>& c) -> const Content<double>& {
            return applyFunctionObj.mockMethod(a, b, c);
        }
    );

    // Then the result is the same as what the mock method returned and the tuple should be affected by the changes
    EXPECT_EQ(result, c);
    EXPECT_EQ(tuple.get<1>().getValue(), 0.f);
}

TEST_F(PolymorphicTupleShould, applyAHostFunctionOnANonConstRValueTuple) {
    // The mock method should be called once with the content of the tuple as arguments
    HostMockApplyFunction applyFunctionObj;
    EXPECT_CALL(applyFunctionObj, mockMethod(::testing::Eq<AbstractContent&>(*a), ::testing::Eq(b), ::testing::Eq(c)))
        .Times(1)
        .WillRepeatedly(::testing::Invoke([](AbstractContent& a, Content<float>& b, const Content<double>& c) -> const Content<double>& {
            b = Content<float>(0.f);
            return c;
        }));

    // Given a tuple
    PolymorphicTuple<AbstractContent&, Content<float>, Content<double>> tuple = makePolymorphicTuple(*a, std::move(bToMove), std::move(cToMove));

    // When applying this tuple on the mock method
    Content<double>& result = tuple.hostApply(
        [&applyFunctionObj](AbstractContent& a, Content<float>& b, const Content<double>& c) -> const Content<double>& {
            return applyFunctionObj.mockMethod(a, b, c);
        }
    );

    // Then the result is the same as what the mock method returned and the tuple should be affected by the changes
    EXPECT_EQ(result, c);
    EXPECT_EQ(tuple.get<1>().getValue(), 0.f);
}

TEST_F(PolymorphicTupleShould, applyAHostDeviceFunctionOnAConstLValueTuple) {
    // The mock method should be called once with the content of the tuple as arguments
    HostDeviceMockApplyFunction applyFunctionObj;
    EXPECT_CALL(applyFunctionObj, constMockMethod(::testing::Eq<AbstractContent&>(*a), ::testing::Eq(b), ::testing::Eq(c)))
        .Times(1)
        .WillRepeatedly(::testing::Invoke([](const AbstractContent& a, const Content<float>& b, const Content<double>& c) -> const Content<double>& {
            return c;
        }));

    // Given a tuple
    const PolymorphicTuple<AbstractContent&, Content<float>&, const Content<double>&> tuple = makePolymorphicTuple(*a, b, c);

    // When applying this tuple on the mock method
    const Content<double>& result = tuple.apply(
        [&applyFunctionObj](const AbstractContent& a, const Content<float>& b, const Content<double>& c) -> const Content<double>& {
            return applyFunctionObj.constMockMethod(a, b, c);
        }
    );

    // Then the result is the same as what the mock method returned
    EXPECT_EQ(result, c);
}

TEST_F(PolymorphicTupleShould, applyAHostDeviceFunctionOnAConstRValueTuple) {
    // The mock method should be called once with the content of the tuple as arguments
    HostDeviceMockApplyFunction applyFunctionObj;
    EXPECT_CALL(applyFunctionObj, constMockMethod(::testing::Eq<AbstractContent&>(*a), ::testing::Eq(b), ::testing::Eq(c)))
        .Times(1)
        .WillRepeatedly(::testing::Invoke([](const AbstractContent& a, const Content<float>& b, const Content<double>& c) -> const Content<double>& {
            return c;
        }));

    // Given a tuple
    const PolymorphicTuple<AbstractContent&, Content<float>, Content<double>> tuple = makePolymorphicTuple(*a, std::move(bToMove), std::move(cToMove));

    // When applying this tuple on the mock method
    const Content<double>& result = tuple.apply(
        [&applyFunctionObj](const AbstractContent& a, const Content<float>& b, const Content<double>& c) -> const Content<double>& {
            return applyFunctionObj.constMockMethod(a, b, c);
        }
    );

    // Then the result is the same as what the mock method returned
    EXPECT_EQ(result, c);
}

TEST_F(PolymorphicTupleShould, applyAHostDeviceFunctionOnANonConstLValueTuple) {
    // The mock method should be called once with the content of the tuple as arguments
    HostDeviceMockApplyFunction applyFunctionObj;
    EXPECT_CALL(applyFunctionObj, mockMethod(::testing::Eq<AbstractContent&>(*a), ::testing::Eq(b), ::testing::Eq(c)))
        .Times(1)
        .WillRepeatedly(::testing::Invoke([](AbstractContent& a, Content<float>& b, const Content<double>& c) -> const Content<double>& {
            b = Content<float>(0.f);
            return c;
        }));

    // Given a tuple
    PolymorphicTuple<AbstractContent&, Content<float>&, const Content<double>&> tuple = makePolymorphicTuple(*a, b, c);

    // When applying this tuple on the mock method
    Content<double>& result = tuple.apply(
        [&applyFunctionObj](AbstractContent& a, Content<float>& b, const Content<double>& c) -> const Content<double>& {
            return applyFunctionObj.mockMethod(a, b, c);
        }
    );

    // Then the result is the same as what the mock method returned and the tuple should be affected by the changes
    EXPECT_EQ(result, c);
    EXPECT_EQ(tuple.get<1>().getValue(), 0.f);
}

TEST_F(PolymorphicTupleShould, applyAHostDeviceFunctionOnANonConstRValueTuple) {
    // The mock method should be called once with the content of the tuple as arguments
    HostDeviceMockApplyFunction applyFunctionObj;
    EXPECT_CALL(applyFunctionObj, mockMethod(::testing::Eq<AbstractContent&>(*a), ::testing::Eq(b), ::testing::Eq(c)))
        .Times(1)
        .WillRepeatedly(::testing::Invoke([](AbstractContent& a, Content<float>& b, const Content<double>& c) -> const Content<double>& {
            b = Content<float>(0.f);
            return c;
        }));

    // Given a tuple
    PolymorphicTuple<AbstractContent&, Content<float>, Content<double>> tuple = makePolymorphicTuple(*a, std::move(bToMove), std::move(cToMove));

    // When applying this tuple on the mock method
    Content<double>& result = tuple.apply(
        [&applyFunctionObj](AbstractContent& a, Content<float>& b, const Content<double>& c) -> const Content<double>& {
            return applyFunctionObj.mockMethod(a, b, c);
        }
    );

    // Then the result is the same as what the mock method returned and the tuple should be affected by the changes
    EXPECT_EQ(result, c);
    EXPECT_EQ(tuple.get<1>().getValue(), 0.f);
}

TEST_F(PolymorphicTupleShould, mapValuesFromAConstLValueReferencesTuple) {
    // Given a tuple
    const PolymorphicTuple<AbstractContent&, Content<float>&, const Content<double>&> tuple = makePolymorphicTuple(*a, b, c);

    // When mapping SampleMapFunction on the tuple
    const PolymorphicTuple<double, float, double> result = tuple.hostMap<SampleMapFunction>(3.14);

    // Then the resulting tuple holds the results of
    // SampleMapFunction applied to each value following the context arguments
    EXPECT_EQ(result.get<0>(), 8.14);
    EXPECT_EQ(result.get<1>(), 6.28f);
    EXPECT_EQ(result.get<2>(), 9.42);
}

TEST_F(PolymorphicTupleShould, mapValuesFromAConstRValueReferencesTuple) {
    // Given a tuple
    const PolymorphicTuple<AbstractContent&, Content<float>, Content<double>> tuple = makePolymorphicTuple(*a, std::move(bToMove), std::move(cToMove));

    // When mapping SampleMapFunction on the tuple
    const PolymorphicTuple<double, float, double> result = tuple.hostMap<SampleMapFunction>(3.14);

    // Then the resulting tuple holds the results of
    // SampleMapFunction applied to each value following the context arguments
    EXPECT_EQ(result.get<0>(), 8.14);
    EXPECT_EQ(result.get<1>(), 6.28f);
    EXPECT_EQ(result.get<2>(), 9.42);
}

TEST_F(PolymorphicTupleShould, mapValuesFromANonConstLValueReferencesTuple) {
    // Given a tuple
    PolymorphicTuple<AbstractContent&, Content<float>&, const Content<double>&> tuple = makePolymorphicTuple(*a, b, c);

    // When mapping SampleMapFunction on the tuple
    const PolymorphicTuple<double, float, double> result = tuple.hostMap<SampleMapFunction>(3.14);

    // Then the resulting tuple holds the results of
    // SampleMapFunction applied to each value following the context arguments
    EXPECT_EQ(result.get<0>(), 8.14);
    EXPECT_EQ(result.get<1>(), 6.28f);
    EXPECT_EQ(result.get<2>(), 9.42);
}

TEST_F(PolymorphicTupleShould, mapValuesFromANonConstRValueReferencesTuple) {
    // Given a tuple
    PolymorphicTuple<AbstractContent&, Content<float>, Content<double>> tuple = makePolymorphicTuple(*a, std::move(bToMove), std::move(cToMove));

    // When mapping SampleMapFunction on the tuple
    const PolymorphicTuple<double, float, double> result = tuple.hostMap<SampleMapFunction>(3.14);

    // Then the resulting tuple holds the results of
    // SampleMapFunction applied to each value following the context arguments
    EXPECT_EQ(result.get<0>(), 8.14);
    EXPECT_EQ(result.get<1>(), 6.28f);
    EXPECT_EQ(result.get<2>(), 9.42);
}

TEST_F(PolymorphicTupleShould, castAnLValueTupleIfAllTypeAreConvertible) {
    // Given an l-value tuple of convertible types
    const PolymorphicTuple<const Content<float>&, const Content<double>&> tuple = makePolymorphicTuple(b, c);

    // When converting this tuple
    const PolymorphicTuple<float, double> result = static_cast<PolymorphicTuple<float, double>>(tuple);

    // Then the result should contain the converted values
    EXPECT_EQ(result.get<0>(), b.getValue());
    EXPECT_EQ(result.get<1>(), c.getValue());
}

TEST_F(PolymorphicTupleShould, castAnRValueTupleIfAllTypeAreConvertible) {
    // Given an r-value tuple of convertible types
    const PolymorphicTuple<Content<float>, Content<double>> tuple = makePolymorphicTuple(std::move(bToMove), std::move(cToMove));

    // When converting this tuple
    const PolymorphicTuple<float, double> result = static_cast<PolymorphicTuple<float, double>>(tuple);

    // Then the result should contain the converted values
    EXPECT_EQ(result.get<0>(), b.getValue());
    EXPECT_EQ(result.get<1>(), c.getValue());
}

TEST_F(PolymorphicTupleShould, getConstPointersFromAConstLValueReferencesTuple) {
    // Given a const tuple
    const PolymorphicTuple<AbstractContent&, Content<float>&, const Content<double>&> tuple = makePolymorphicTuple(*a, b, c);

    // When getting its const pointers
    const PolymorphicTuple<const AbstractContent*, const Content<float>*, const Content<double>*> result = tuple.getPointers();

    // Then the pointers of the resulting tuple point to the data of the original tuple
    EXPECT_EQ(result.get<0>(), &tuple.get<0>());
    EXPECT_EQ(result.get<1>(), &tuple.get<1>());
    EXPECT_EQ(result.get<2>(), &tuple.get<2>());
}

TEST_F(PolymorphicTupleShould, getConstPointersFromAConstRValueReferencesTuple) {
    // Given a const tuple
    const PolymorphicTuple<AbstractContent&, Content<float>, Content<double>> tuple = makePolymorphicTuple(*a, std::move(bToMove), std::move(cToMove));

    // When getting its const pointers
    const PolymorphicTuple<const AbstractContent*, const Content<float>*, const Content<double>*> result = tuple.getPointers();

    // Then the pointers of the resulting tuple point to the data of the original tuple
    EXPECT_EQ(result.get<0>(), &tuple.get<0>());
    EXPECT_EQ(result.get<1>(), &tuple.get<1>());
    EXPECT_EQ(result.get<2>(), &tuple.get<2>());
}

TEST_F(PolymorphicTupleShould, getNonConstPointersFromANonConstLValueReferencesTuple) {
    // Given a non-const tuple
    PolymorphicTuple<AbstractContent&, Content<float>&, const Content<double>&> tuple = makePolymorphicTuple(*a, b, c);

    // When getting its non-const pointers
    const PolymorphicTuple<AbstractContent*, Content<float>*, const Content<double>*> result = tuple.getPointers();

    // Then the pointers of the resulting tuple point to the data of the original tuple
    EXPECT_EQ(result.get<0>(), &tuple.get<0>());
    EXPECT_EQ(result.get<1>(), &tuple.get<1>());
    EXPECT_EQ(result.get<2>(), &tuple.get<2>());
}

TEST_F(PolymorphicTupleShould, getNonConstPointersFromANonConstRValueReferencesTuple) {
    // Given a non-const tuple
    PolymorphicTuple<AbstractContent&, Content<float>, Content<double>> tuple = makePolymorphicTuple(*a, std::move(bToMove), std::move(cToMove));

    // When getting its non-const pointers
    const PolymorphicTuple<AbstractContent*, Content<float>*, Content<double>*> result = tuple.getPointers();

    // Then the pointers of the resulting tuple point to the data of the original tuple
    EXPECT_EQ(result.get<0>(), &tuple.get<0>());
    EXPECT_EQ(result.get<1>(), &tuple.get<1>());
    EXPECT_EQ(result.get<2>(), &tuple.get<2>());
}

TEST_F(PolymorphicTupleShould, getPointersFromAnLValueReferencesTuple) {
    // Given a tuple
    PolymorphicTuple<AbstractContent&, Content<float>&, const Content<double>&> tuple = makePolymorphicTuple(*a, b, c);

    // When getting its const pointers
    const PolymorphicTuple<AbstractContent*, Content<float>*, const Content<double>*> result = tuple.getPointers();

    // Then the pointers of the resulting tuple point to the data of the original tuple
    EXPECT_EQ(result.get<0>(), &tuple.get<0>());
    EXPECT_EQ(result.get<1>(), &tuple.get<1>());
    EXPECT_EQ(result.get<2>(), &tuple.get<2>());
}

TEST_F(PolymorphicTupleShould, getPointersFromAnRValueReferencesTuple) {
    // Given a tuple
    PolymorphicTuple<AbstractContent&, Content<float>, Content<double>> tuple = makePolymorphicTuple(*a, std::move(bToMove), std::move(cToMove));

    // When getting its const pointers
    const PolymorphicTuple<AbstractContent*, Content<float>*, Content<double>*> result = tuple.getPointers();

    // Then the pointers of the resulting tuple point to the data of the original tuple
    EXPECT_EQ(result.get<0>(), &tuple.get<0>());
    EXPECT_EQ(result.get<1>(), &tuple.get<1>());
    EXPECT_EQ(result.get<2>(), &tuple.get<2>());
}

TEST_F(PolymorphicTupleShould, letAnLValueReferencesTupleBeWriteableFromItsPointersTuple) {
    // Given a non-const tuple and its non-const pointers tuple
    PolymorphicTuple<AbstractContent&, Content<float>&, const Content<double>&> tuple = makePolymorphicTuple(*a, b, c);
    const PolymorphicTuple<AbstractContent*, Content<float>*, const Content<double>*> pointers = tuple.getPointers();

    // When writing in the original tuple from its pointers tuple
    pointers.get<0>()->fromDouble(8.14);
    *pointers.get<1>() = 6.28f;

    // Then the pointers of the resulting tuple point to the data of the original tuple
    EXPECT_EQ(tuple.get<0>().toDouble(), 8);
    EXPECT_EQ(tuple.get<1>(), 6.28f);
}

TEST_F(PolymorphicTupleShould, letAnRValueReferencesTupleBeWriteableFromItsPointersTuple) {
    // Given a non-const tuple and its non-const pointers tuple
    PolymorphicTuple<AbstractContent&, Content<float>, Content<double>> tuple = makePolymorphicTuple(*a, std::move(bToMove), std::move(cToMove));
    const PolymorphicTuple<AbstractContent*, Content<float>*, Content<double>*> pointers = tuple.getPointers();

    // When writing in the original tuple from its pointers tuple
    pointers.get<0>()->fromDouble(8.14);
    *pointers.get<1>() = 6.28f;
    *pointers.get<2>() = 9.42;

    // Then the pointers of the resulting tuple point to the data of the original tuple
    EXPECT_EQ(tuple.get<0>().toDouble(), 8);
    EXPECT_EQ(tuple.get<1>(), 6.28f);
    EXPECT_EQ(tuple.get<2>(), 9.42);
}

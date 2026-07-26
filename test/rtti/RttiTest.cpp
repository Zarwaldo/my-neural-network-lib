#include <gtest/gtest.h>

#include <rtti/AbstractRtti.h>
#include <rtti/Initializer.h>
#include <rtti/Rtti.h>
#include <rtti/RttiHolder.impl.h>
#include <rtti/StaticRttiHolder.h>
#include <rtti/TemplateRtti.h>

using namespace BuildTimeList;

class Base
{
public:
    Base()
    {}

    virtual ~Base()
    {}

    virtual std::string toString() const = 0;

    DECLARE_AS_RTTI_BASE_TYPE(Base)
};

class ConcreteClass : public Base
{
public:
    ConcreteClass()
        : ConcreteClass(2)
    {}

    ConcreteClass(int x)
        : ConcreteClass(x, 1)
    {}

    ConcreteClass(int x, int y)
        : m_x(x)
        , m_y(y)
    {}

    virtual ~ConcreteClass()
    {}

    virtual std::string toString() const override
    {
        return getRtti()->getTypeName() + "(" + std::to_string(m_x) + "," + std::to_string(m_y) + ")";
    }

private:
    int m_x;
    int m_y;

    DECLARE_RTTI(Base)
};

IMPLEMENT_RTTI(ConcreteClass, Base, PACK(), PACK())

namespace
{
    ConcreteClass* createConcreteClass(char x)
    {
        return new ConcreteClass(static_cast<int>(x), static_cast<int>(2*x));
    }
}

template<typename T>
class TemplateClass : public Base
{
public:
    TemplateClass(T x)
        : TemplateClass(x, 1.0)
    {}

    TemplateClass(T x, T y)
        : m_x(x)
        , m_y(y)
    {}

    virtual ~TemplateClass()
    {}

    virtual std::string toString() const override
    {
        return getRtti()->getTypeName() + "(" + std::to_string(m_x) + "," + std::to_string(m_y) + ")";
    }

private:
    T m_x;
    T m_y;

    DECLARE_RTTI(Base)
};

IMPLEMENT_RTTI(TemplateClass, Base, PACK(typename), PACK(T))

template <int X, int Y>
class TemplateClass2 : public Base
{
public:
    TemplateClass2()
        : TemplateClass2(X, Y)
    {}

    TemplateClass2(int x)
        : TemplateClass2(x, Y)
    {}

    TemplateClass2(int x, int y)
        : m_x(x)
        , m_y(y)
    {}

    virtual ~TemplateClass2()
    {}

    virtual std::string toString() const override
    {
        return getRtti()->getTypeName() + "(" + std::to_string(m_x) + "," + std::to_string(m_y) + ")";
    }

private:
    int m_x;
    int m_y;

    DECLARE_RTTI(Base)
};

IMPLEMENT_RTTI(TemplateClass2, Base, PACK(int, int), PACK(X, Y))

class RttiShould : public ::testing::Test
{
protected:
    void SetUp() override
    {
        staticRttiHolder = new StaticRttiHolder<Base>([](RttiHolderToken<Base>& token) {
            token.subscribe(new Rtti<Base, ConcreteClass, TypeList<>, BuildTimeFunctionPointer<createConcreteClass>, TypeList<int>, TypeList<int, int>>("ConcreteClass"));
            token.subscribe(new Rtti<Base, TemplateClass<float>, TypeList<float>, TypeList<float, float>>("TemplateClass<float>"));
            token.subscribe(new Rtti<Base, TemplateClass<double>, TypeList<double>, TypeList<double, double>>("TemplateClass<double>"));

            TemplateRtti<Base, TemplateClass2>& templateClass2Rtti = token.getOrSubscribeTemplateRtti<TemplateClass2>("TemplateClass2");
            templateClass2Rtti.subscribe<int{0}, int{0}>(new Rtti<Base, TemplateClass2<0, 0>, TypeList<>, TypeList<int>, TypeList<int, int>>("TemplateClass2<0,0>"));
            templateClass2Rtti.subscribe<int{1}, int{2}>(new Rtti<Base, TemplateClass2<1, 2>, TypeList<>, TypeList<int>, TypeList<int, int>>("TemplateClass2<1,2>"));
            templateClass2Rtti.subscribe<int{2}, int{4}>(new Rtti<Base, TemplateClass2<2, 4>, TypeList<>, TypeList<int>, TypeList<int, int>>("TemplateClass2<2,4>"));
        });
    }

    void TearDown() override
    {
        delete staticRttiHolder;
    }

    const RttiHolder<Base>& getRttiHolder() const
    {
        return staticRttiHolder->getRttiHolder();
    }

    StaticRttiHolder<Base>* staticRttiHolder;
};

TEST_F(RttiShould, returnConcreteClassRTTIOnCallToGetTypeOnConcreteClass) {
    // Given an RTTI holder and ConcreteClass's type name
    const std::string classATypeName("ConcreteClass");

    // When we get the RTTI of ConcreteClass
    const AbstractRtti<Base>* rtti = ConcreteClass::getRtti();

    // Then the returned RTTI should have the name ConcreteClass
    EXPECT_NE(rtti, nullptr);
    EXPECT_EQ(rtti->getTypeName(), classATypeName);
}

TEST_F(RttiShould, returnTemplateClassFloatRTTIOnCallToGetTypeOnTemplateClassFloat) {
    // Given an RTTI holder and TemplateClass<float>'s type name
    const std::string classBTypeName("TemplateClass<float>");

    // When we get the RTTI of TemplateClass<float>
    const AbstractRtti<Base>* rtti = TemplateClass<float>::getRtti();

    // Then the returned RTTI should have the name TemplateClass<float>
    EXPECT_NE(rtti, nullptr);
    EXPECT_EQ(rtti->getTypeName(), classBTypeName);
}

TEST_F(RttiShould, returnTemplateClassDoubleRTTIOnCallToGetTypeOnTemplateClassDouble) {
    // Given an RTTI holder and TemplateClass<double>'s type name
    const std::string classBTypeName("TemplateClass<double>");

    // When we get the RTTI of TemplateClass<double>
    const AbstractRtti<Base>* rtti = TemplateClass<double>::getRtti();

    // Then the returned RTTI should have the name TemplateClass<double>
    EXPECT_NE(rtti, nullptr);
    EXPECT_EQ(rtti->getTypeName(), classBTypeName);
}

TEST_F(RttiShould, returnConcreteClassRTTIOnCallToGetClassTypeOnConcreteClassObject) {
    // Given an RTTI holder and an object of type ConcreteClass
    ConcreteClass obj;

    // When we get the RTTI of the object
    const AbstractRtti<Base>* rtti = obj.getClassRtti();

    // Then the returned RTTI should be the RTTI of ConcreteClass
    EXPECT_EQ(rtti, ConcreteClass::getRtti());
}

TEST_F(RttiShould, returnTemplateClassFloatRTTIOnCallToGetClassTypeOnTemplateClassFloatObject) {
    // Given an RTTI holder and an object of type TemplateClass<float>
    TemplateClass<float> obj(5.0f);

    // When we get the RTTI of the object
    const AbstractRtti<Base>* rtti = obj.getClassRtti();

    // Then the returned RTTI should be the RTTI of TemplateClass<float>
    EXPECT_EQ(rtti, TemplateClass<float>::getRtti());
}

TEST_F(RttiShould, returnTemplateClassDoubleRTTIOnCallToGetClassTypeOnTemplateClassDoubleObject) {
    // Given an RTTI holder and an object of type TemplateClass<double>
    TemplateClass<double> obj(5.0);

    // When we get the RTTI of the object
    const AbstractRtti<Base>* rtti = obj.getClassRtti();

    // Then the returned RTTI should be the RTTI of TemplateClass<double>
    EXPECT_EQ(rtti, TemplateClass<double>::getRtti());
}

TEST_F(RttiShould, returnConcreteClassRTTIOnConcreteClassTypeName) {
    // Given an RTTI holder and ConcreteClass's type name
    const std::string classATypeName("ConcreteClass");

    // When an RTTI with this name is fetched in the RTTI holder
    const AbstractRtti<Base>* rtti = getRttiHolder().getRttiByName(classATypeName);

    // Then the returned RTTI should be the RTTI of ConcreteClass
    EXPECT_EQ(rtti, ConcreteClass::getRtti());
}

TEST_F(RttiShould, returnTemplateClassFloatRTTIOnTemplateClassFloatTypeName) {
    // Given an RTTI holder and TemplateClass<float>'s type name
    const std::string classBTypeName("TemplateClass<float>");

    // When an RTTI with this name is fetched in the RTTI holder
    const AbstractRtti<Base>* rtti = getRttiHolder().getRttiByName(classBTypeName);

    // Then the returned RTTI should be the RTTI of TemplateClass<float>
    EXPECT_EQ(rtti, TemplateClass<float>::getRtti());
}

TEST_F(RttiShould, returnTemplateClassDoubleRTTIOnTemplateClassDoubleTypeName) {
    // Given an RTTI holder and TemplateClass<double>'s type name
    const std::string classBTypeName("TemplateClass<double>");

    // When an RTTI with this name is fetched in the RTTI holder
    const AbstractRtti<Base>* rtti = getRttiHolder().getRttiByName(classBTypeName);

    // Then the returned RTTI should be the RTTI of TemplateClass<double>
    EXPECT_EQ(rtti, TemplateClass<double>::getRtti());
}

TEST_F(RttiShould, returnNullptrOnInvalidTypeName) {
    // Given an RTTI holder and an invalid type name
    const std::string invalidTypeName("InvalidClassName");

    // When an RTTI with this name is fetched in the RTTI holder
    const AbstractRtti<Base>* rtti = getRttiHolder().getRttiByName(invalidTypeName);

    // Then the returned RTTI should be null
    EXPECT_EQ(rtti, nullptr);
}

TEST_F(RttiShould, returnNullptrWhenSearchingConcreteRttiOnATemplateRttiName) {
    // Given an RTTI holder and a template class name
    const std::string templateClass("TemplateClass2");

    // When a template RTTI with this name is fetched in the RTTI holder
    const AbstractRtti<Base>* rtti = getRttiHolder().getRttiByName(templateClass);

    // Then the returned RTTI should be null
    EXPECT_EQ(rtti, nullptr);
}

TEST_F(RttiShould, returnACorrectTemplateRttiOnATemplateRttiName) {
    // Given an RTTI holder and a template class name
    const std::string templateClass("TemplateClass2");

    // When a template RTTI with this name is fetched in the RTTI holder
    const AbstractTemplateRtti<Base>* templateRtti = getRttiHolder().getTemplateRttiByName(templateClass);

    // Then the returned RTTI should not be null
    EXPECT_NE(templateRtti, nullptr);
    // And have the correct type name
    EXPECT_EQ(templateRtti->getTypeName(), "TemplateClass2");
}

TEST_F(RttiShould, returnNullptrWhenSearchingTemplateRttiOnAConcreteClassName) {
    // Given an RTTI holder and an invalid template type name
    const std::string concreteTypeName("ConcreteClass");

    // When a template RTTI with this name is fetched in the RTTI holder
    const AbstractTemplateRtti<Base>* templateRtti = getRttiHolder().getTemplateRttiByName(concreteTypeName);

    // Then the returned RTTI should be null
    EXPECT_EQ(templateRtti, nullptr);
}

TEST_F(RttiShould, returnNullptrWhenSearchingTemplateRttiOnAnInvalidName) {
    // Given an RTTI holder and an invalid template type name
    const std::string invalidTemplateTypeName("InvalidTemplateClassName");

    // When a template RTTI with this name is fetched in the RTTI holder
    const AbstractTemplateRtti<Base>* templateRtti = getRttiHolder().getTemplateRttiByName(invalidTemplateTypeName);

    // Then the returned RTTI should be null
    EXPECT_EQ(templateRtti, nullptr);
}

TEST_F(RttiShould, createConcreteClassInstanceFromDefaultConstructor) {
    // Given the RTTI of ConcreteClass
    const AbstractRtti<Base>* rtti = ConcreteClass::getRtti();

    // When an object is created with default constructor
    const Base* obj = rtti->createInstance(Initializer<>());

    // Then the object returns the expected object-representing string
    EXPECT_EQ(obj->toString(), "ConcreteClass(2,1)");

    // Free the created object
    delete obj;
}

TEST_F(RttiShould, createConcreteClassInstanceFromOneArgConstructor) {
    // Given the RTTI of ConcreteClass
    const AbstractRtti<Base>* rtti = ConcreteClass::getRtti();

    // When an object is created with one int argument constructor
    const Base* obj = rtti->createInstance(Initializer<int>(5));

    // Then the object returns the expected object-representing string
    EXPECT_EQ(obj->toString(), "ConcreteClass(5,1)");

    // Free the created object
    delete obj;
}

TEST_F(RttiShould, createConcreteClassInstanceFromTwoArgsConstructor) {
    // Given the RTTI of ConcreteClass
    const AbstractRtti<Base>* rtti = ConcreteClass::getRtti();

    // When an object is created with two int arguments constructor
    const Base* obj = rtti->createInstance(Initializer<int, int>(5, 6));

    // Then the object returns the expected object-representing string
    EXPECT_EQ(obj->toString(), "ConcreteClass(5,6)");

    // Free the created object
    delete obj;
}

TEST_F(RttiShould, createConcreteClassInstanceFromOneCharArgCreationFunction) {
    // Given the RTTI of ConcreteClass
    const AbstractRtti<Base>* rtti = ConcreteClass::getRtti();

    // When an object is created with one char argument
    const Base* obj = rtti->createInstance(Initializer<char>(5));

    // Then the object should have been created with the createConcreteClass() method
    // and return the expected object-representing string
    EXPECT_EQ(obj->toString(), "ConcreteClass(5,10)");

    // Free the created object
    delete obj;
}

TEST_F(RttiShould, createTemplateClassFloatInstanceFromOneArgConstructor) {
    // Given the RTTI of TemplateClass<float>
    const AbstractRtti<Base>* rtti = TemplateClass<float>::getRtti();

    // When an object is created with one argument
    const Base* obj = rtti->createInstance(Initializer<float>(5.0f));

    // Then the object returns the expected object-representing string
    EXPECT_EQ(obj->toString(), "TemplateClass<float>(5.000000,1.000000)");

    // Free the created object
    delete obj;
}

TEST_F(RttiShould, createTemplateClassFloatInstanceFromTwoArgsConstructor) {
    // Given the RTTI of TemplateClass<float>
    const AbstractRtti<Base>* rtti = TemplateClass<float>::getRtti();

    // When an object is created with two arguments
    const Base* obj = rtti->createInstance(Initializer<float, float>(5.0f, 6.0f));

    // Then the object returns the expected object-representing string
    EXPECT_EQ(obj->toString(), "TemplateClass<float>(5.000000,6.000000)");

    // Free the created object
    delete obj;
}

TEST_F(RttiShould, createTemplateClassDoubleInstanceFromOneArgConstructor) {
    // Given the RTTI of TemplateClass<double>
    const AbstractRtti<Base>* rtti = TemplateClass<double>::getRtti();

    // When an object is created with one argument
    const Base* obj = rtti->createInstance(Initializer<double>(5.0));

    // Then the object returns the expected object-representing string
    EXPECT_EQ(obj->toString(), "TemplateClass<double>(5.000000,1.000000)");

    // Free the created object
    delete obj;
}

TEST_F(RttiShould, createTemplateClassDoubleInstanceFromTwoArgsConstructor) {
    // Given the RTTI of TemplateClass<double>
    const AbstractRtti<Base>* rtti = TemplateClass<double>::getRtti();

    // When an object is created with two arguments
    const Base* obj = rtti->createInstance(Initializer<double, double>(5.0, 6.0));

    // Then the object returns the expected object-representing string
    EXPECT_EQ(obj->toString(), "TemplateClass<double>(5.000000,6.000000)");

    // Free the created object
    delete obj;
}

TEST_F(RttiShould, instantiateATemplateRttiWithFirstTemplateArgsSet) {
    // Given a template RTTI
    const AbstractTemplateRtti<Base>* templateRtti = getRttiHolder().getTemplateRttiByName("TemplateClass2");

    // When instantiating a template RTTI on its first template arguments set
    const AbstractRtti<Base>* rtti = templateRtti->instantiate(Initializer<int, int>(0, 0));

    // Then the resulting RTTI is non-null
    EXPECT_NE(rtti, nullptr);
    // And instantiating an object on the resulting RTTI should return the right object
    const Base* obj = rtti->createInstance(Initializer<int, int>(5, 7));
    EXPECT_NE(obj, nullptr);
    EXPECT_EQ(obj->toString(), "TemplateClass2<0,0>(5,7)");

    // Free the created object
    delete obj;
}

TEST_F(RttiShould, instantiateATemplateRttiWithSecondTemplateArgsSet) {
    // Given a template RTTI
    const AbstractTemplateRtti<Base>* templateRtti = getRttiHolder().getTemplateRttiByName("TemplateClass2");

    // When instantiating a template RTTI on its second template arguments set
    const AbstractRtti<Base>* rtti = templateRtti->instantiate(Initializer<int, int>(1, 2));

    // Then the resulting RTTI is non-null
    EXPECT_NE(rtti, nullptr);
    // And instantiating an object on the resulting RTTI should return the right object
    const Base* obj = rtti->createInstance(Initializer<int, int>(5, 7));
    EXPECT_NE(obj, nullptr);
    EXPECT_EQ(obj->toString(), "TemplateClass2<1,2>(5,7)");

    // Free the created object
    delete obj;
}

TEST_F(RttiShould, instantiateATemplateRttiWithThirdTemplateArgsSet) {
    // Given a template RTTI
    const AbstractTemplateRtti<Base>* templateRtti = getRttiHolder().getTemplateRttiByName("TemplateClass2");

    // When instantiating a template RTTI on its third template arguments set
    const AbstractRtti<Base>* rtti = templateRtti->instantiate(Initializer<int, int>(2, 4));

    // Then the resulting RTTI is non-null
    EXPECT_NE(rtti, nullptr);
    // And instantiating an object on the resulting RTTI should return the right object
    const Base* obj = rtti->createInstance(Initializer<int, int>(5, 7));
    EXPECT_NE(obj, nullptr);
    EXPECT_EQ(obj->toString(), "TemplateClass2<2,4>(5,7)");

    // Free the created object
    delete obj;
}

TEST_F(RttiShould, notInstantiateATemplateRttiWithAnInvalidTemplateArgsSet) {
    // Given a template RTTI
    const AbstractTemplateRtti<Base>* templateRtti = getRttiHolder().getTemplateRttiByName("TemplateClass2");

    // When instantiating a template RTTI on an invalid template args set
    const AbstractRtti<Base>* rtti = templateRtti->instantiate(Initializer<int, int>(0, 1));

    // Then the resulting RTTI is null
    EXPECT_EQ(rtti, nullptr);
}

class RttiHolderTokenShould : public ::testing::Test
{
protected:
    void SetUp() override
    {
        rttiHolder = new RttiHolder<Base>;
    }

    void TearDown() override
    {
        delete rttiHolder;
    }

    RttiHolder<Base>* rttiHolder;
};

TEST_F(RttiHolderTokenShould, deleteOnlyTheRttisCreatedByThisToken) {
    // Given two rtti tokens that both subscribed some rttis
    RttiHolderToken<Base>* rttiToken1 = new RttiHolderToken<Base>(std::move(rttiHolder->edit()));
    rttiToken1->subscribe(new Rtti<Base, ConcreteClass, TypeList<>, TypeList<int>, TypeList<int, int>>("ConcreteClass"));
    RttiHolderToken<Base>* rttiToken2 = new RttiHolderToken<Base>(std::move(rttiHolder->edit()));
    rttiToken2->subscribe(new Rtti<Base, TemplateClass<float>, TypeList<float>, TypeList<float, float>>("TemplateClass<float>"));
    rttiToken2->subscribe(new Rtti<Base, TemplateClass<double>, TypeList<double>, TypeList<double, double>>("TemplateClass<double>"));

    // When deleting the first token
    delete rttiToken1;

    // Then the rttis registered by the first rtti are no longer available
    EXPECT_EQ(rttiHolder->getRttiByName("ConcreteClass"), nullptr);
    // But the rttis registered by the second rtti are still available
    EXPECT_NE(rttiHolder->getRttiByName("TemplateClass<float>"), nullptr);
    EXPECT_NE(rttiHolder->getRttiByName("TemplateClass<double>"), nullptr);

    // Free the remaining token
    delete rttiToken2;
}

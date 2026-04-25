#include <gtest/gtest.h>

#include <tensor/ManagedMemorySharedPtr.h>

template <typename TypeParam>
class ManagedMemorySharedPtrShould : public ::testing::Test
{
protected:
    void expectPtrIsAllocated(TypeParam* const ptr) const
    {
        const TypeParam formerValue = *ptr;
        const TypeParam otherValue = formerValue + 1;

        *ptr = otherValue;
        EXPECT_EQ(*ptr, otherValue);
        *ptr = formerValue;
        EXPECT_EQ(*ptr, formerValue);
    }

    void expectSharedPtrIsAllocated(ManagedMemorySharedPtr<TypeParam>& ptr, const size_t size) const
    {
        for (size_t i = 0; i < size; ++i)
            expectPtrIsAllocated(&ptr[i]);
    }
};

using TestedTypes = ::testing::Types<bool, float, double>;
TYPED_TEST_SUITE(ManagedMemorySharedPtrShould, TestedTypes);

TYPED_TEST(ManagedMemorySharedPtrShould, allocateAValidDataArray) {
    // Given an array size
    const size_t size = 5;

    // When a shared pointer is allocated
    ManagedMemorySharedPtr<TypeParam>* const ptr = new ManagedMemorySharedPtr<TypeParam>(size);

    // Then the shared pointer's internal space is correctly allocated
    expectSharedPtrIsAllocated(*ptr, size);

    // Free the shared ptr
    delete ptr;
}

TYPED_TEST(ManagedMemorySharedPtrShould, beAbleToGetCopiedInAnotherCreatedSharedPtr) {
    // Given an allocated shared pointer and a pointer to its intern memory
    const size_t size = 5;
    ManagedMemorySharedPtr<TypeParam>* const ptr = new ManagedMemorySharedPtr<TypeParam>(size);
    TypeParam* const memory = ptr->getPtr();

    // When we copy this shared pointer in another and deallocate it
    ManagedMemorySharedPtr<TypeParam> copy(*ptr);
    delete ptr;

    // Then the copy should still be allocated and lead to the same space as the original shared pointer
    expectSharedPtrIsAllocated(copy, size);
    EXPECT_EQ(copy.getPtr(), memory);
}

TYPED_TEST(ManagedMemorySharedPtrShould, beAbleToGetMovedInAnotherCreatedSharedPtr) {
    // Given an allocated shared pointer and a pointer to its intern memory
    const size_t size = 5;
    ManagedMemorySharedPtr<TypeParam>* const ptr = new ManagedMemorySharedPtr<TypeParam>(size);
    TypeParam* const memory = ptr->getPtr();

    // When we move this shared pointer in another and deallocate it
    ManagedMemorySharedPtr<TypeParam> moved(std::move(*ptr));
    delete ptr;

    // Then the moved pointer should still be allocated and lead to the same space as the original shared pointer
    expectSharedPtrIsAllocated(moved, size);
    EXPECT_EQ(moved.getPtr(), memory);
}

TYPED_TEST(ManagedMemorySharedPtrShould, beAbleToGetCopiedInAnotherExistingSharedPtr) {
    // Given two allocated shared pointers and a pointer to the intern memory of the former one
    const size_t size = 5;
    ManagedMemorySharedPtr<TypeParam>* const ptr = new ManagedMemorySharedPtr<TypeParam>(size);
    TypeParam* const memory = ptr->getPtr();
    ManagedMemorySharedPtr<TypeParam> copy(size);

    // When we copy the first shared pointer in the second and deallocate it
    copy = *ptr;
    delete ptr;

    // Then the copy should still be allocated and lead to the same space as the original shared pointer
    expectSharedPtrIsAllocated(copy, size);
    EXPECT_EQ(copy.getPtr(), memory);
}

TYPED_TEST(ManagedMemorySharedPtrShould, beAbleToGetMovedInAnotherExistingSharedPtr) {
    // Given an allocated shared pointer and a pointer to its intern memory
    const size_t size = 5;
    ManagedMemorySharedPtr<TypeParam>* const ptr = new ManagedMemorySharedPtr<TypeParam>(size);
    TypeParam* const memory = ptr->getPtr();
    ManagedMemorySharedPtr<TypeParam> moved(size);

    // When we move this shared pointer in another and deallocate it
    moved = std::move(*ptr);
    delete ptr;

    // Then the moved pointer should still be allocated and lead to the same space as the original shared pointer
    expectSharedPtrIsAllocated(moved, size);
    EXPECT_EQ(moved.getPtr(), memory);
}

TYPED_TEST(ManagedMemorySharedPtrShould, beDereferencedAsThePointerToItsInternalData) {
    // Given an allocated shared pointer
    const ManagedMemorySharedPtr<TypeParam> ptr(5);

    // When dereferencing it
    const TypeParam& data = *ptr;

    // Then the reference should correspond to the shared pointer's internal memory
    EXPECT_EQ(&data, ptr.getPtr());
}

TYPED_TEST(ManagedMemorySharedPtrShould, tellTwoEqualPointersAreEqual) {
    // Given two identical shared pointers
    const ManagedMemorySharedPtr<TypeParam> ptr(5);
    const ManagedMemorySharedPtr<TypeParam> ptr2(ptr);

    // When asking whether they are equal
    bool result = (ptr == ptr2);

    // Then the result should be true
    EXPECT_EQ(result, true);
}

TYPED_TEST(ManagedMemorySharedPtrShould, tellTwoDifferentPointersAreNotEqual) {
    // Given two different shared pointers
    const ManagedMemorySharedPtr<TypeParam> ptr(5);
    const ManagedMemorySharedPtr<TypeParam> ptr2(5);

    // When asking whether they are equal
    bool result = (ptr == ptr2);

    // Then the result should be false
    EXPECT_EQ(result, false);
}

TYPED_TEST(ManagedMemorySharedPtrShould, tellTwoEqualPointersAreNotDifferent) {
    // Given two identical shared pointers
    const ManagedMemorySharedPtr<TypeParam> ptr(5);
    const ManagedMemorySharedPtr<TypeParam> ptr2(ptr);

    // When asking whether they are equal
    bool result = (ptr != ptr2);

    // Then the result should be false
    EXPECT_EQ(result, false);
}

TYPED_TEST(ManagedMemorySharedPtrShould, tellTwoDifferentPointersAreDifferent) {
    // Given two different shared pointers
    const ManagedMemorySharedPtr<TypeParam> ptr(5);
    const ManagedMemorySharedPtr<TypeParam> ptr2(5);

    // When asking whether they are equal
    bool result = (ptr != ptr2);

    // Then the result should be true
    EXPECT_EQ(result, true);
}

TYPED_TEST(ManagedMemorySharedPtrShould, haveItsFirstPieceOfDataAtThePointerToItsInternalMemory) {
    // Given a shared pointer
    const ManagedMemorySharedPtr<TypeParam> ptr(5);

    // When getting the reference to the first piece of data of the shared pointer
    const TypeParam& result = ptr[0];

    // Then the result should be pointed by the internal memory pointer of the shared pointer
    EXPECT_EQ(&result, ptr.getPtr());
}

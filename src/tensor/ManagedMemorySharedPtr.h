#pragma once

#include <CommonMacros.h>

template <typename Type>
class ManagedMemorySharedPtr
{
public:
    HOST explicit ManagedMemorySharedPtr(size_t size);
    HOST ManagedMemorySharedPtr(const ManagedMemorySharedPtr& ptr);
    HOST ManagedMemorySharedPtr(ManagedMemorySharedPtr&& ptr);

    HOST ~ManagedMemorySharedPtr();

    HOST ManagedMemorySharedPtr& operator=(const ManagedMemorySharedPtr& other);
    HOST ManagedMemorySharedPtr& operator=(ManagedMemorySharedPtr&& other);

    HOST DEVICE const Type* getPtr() const;
    HOST DEVICE Type* getPtr();

    HOST DEVICE const Type& operator*() const;
    HOST DEVICE Type& operator*();

    HOST DEVICE bool operator==(const ManagedMemorySharedPtr& other) const;
    HOST DEVICE bool operator!=(const ManagedMemorySharedPtr& other) const;

    HOST DEVICE const Type& operator[](size_t index) const;
    HOST DEVICE Type& operator[](size_t index);

private:
    HOST void ref();
    HOST void unref();

    Type* m_ptr;
    size_t* m_counter;
};

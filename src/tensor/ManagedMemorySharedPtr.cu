#include <tensor/ManagedMemorySharedPtr.h>

#include <tensor/api.h>

template <typename Type>
HOST
ManagedMemorySharedPtr<Type>::ManagedMemorySharedPtr(size_t size)
    : m_ptr(nullptr)
    , m_counter(new size_t)
{
    cudaMallocManaged(&m_ptr, sizeof(Type) * size);
    *m_counter = 0;
    ref();
}

template <typename Type>
HOST
ManagedMemorySharedPtr<Type>::ManagedMemorySharedPtr(const ManagedMemorySharedPtr<Type>& ptr)
    : m_ptr(ptr.m_ptr)
    , m_counter(ptr.m_counter)
{
    ref();
}

template <typename Type>
HOST
ManagedMemorySharedPtr<Type>::ManagedMemorySharedPtr(ManagedMemorySharedPtr<Type>&& ptr)
    : m_ptr(ptr.m_ptr)
    , m_counter(ptr.m_counter)
{
    ptr.m_ptr = nullptr;
}

template <typename Type>
HOST
ManagedMemorySharedPtr<Type>::~ManagedMemorySharedPtr()
{
    if (m_ptr == nullptr)
        return;

    unref();
}

template <typename Type>
HOST DEVICE
const Type*
ManagedMemorySharedPtr<Type>::getPtr() const
{
    return m_ptr;
}

template <typename Type>
HOST DEVICE
Type*
ManagedMemorySharedPtr<Type>::getPtr()
{
    return const_cast<Type*>(static_cast<const ManagedMemorySharedPtr<Type>*>(this)->getPtr());
}

template <typename Type>
HOST DEVICE
const Type&
ManagedMemorySharedPtr<Type>::operator*() const
{
    return *m_ptr;
}

template <typename Type>
HOST DEVICE
Type&
ManagedMemorySharedPtr<Type>::operator*()
{
    return const_cast<Type&>(*static_cast<const ManagedMemorySharedPtr<Type>&>(*this));
}

template <typename Type>
HOST
ManagedMemorySharedPtr<Type>&
ManagedMemorySharedPtr<Type>::operator=(const ManagedMemorySharedPtr<Type>& other)
{
    unref();

    m_ptr = other.m_ptr;
    m_counter = other.m_counter;

    ref();

    return *this;
}

template <typename Type>
HOST
ManagedMemorySharedPtr<Type>&
ManagedMemorySharedPtr<Type>::operator=(ManagedMemorySharedPtr<Type>&& other)
{
    unref();

    m_ptr = other.m_ptr;
    m_counter = other.m_counter;

    other.m_ptr = nullptr;

    return *this;
}

template <typename Type>
HOST DEVICE
bool
ManagedMemorySharedPtr<Type>::operator==(const ManagedMemorySharedPtr<Type>& other) const
{
    return m_ptr == other.m_ptr;
}

template <typename Type>
HOST DEVICE
bool
ManagedMemorySharedPtr<Type>::operator!=(const ManagedMemorySharedPtr<Type>& other) const
{
    return !(*this == other);
}

template <typename Type>
HOST DEVICE
const Type&
ManagedMemorySharedPtr<Type>::operator[](size_t index) const
{
    return m_ptr[index];
}

template <typename Type>
HOST DEVICE
Type&
ManagedMemorySharedPtr<Type>::operator[](size_t index)
{
    return const_cast<Type&>(static_cast<const ManagedMemorySharedPtr<Type>&>(*this)[index]);
}

template <typename Type>
HOST
void
ManagedMemorySharedPtr<Type>::ref()
{
    if (m_ptr == nullptr)
        return;

    (*m_counter)++;
}

template <typename Type>
HOST
void
ManagedMemorySharedPtr<Type>::unref()
{
    if (m_ptr == nullptr)
        return;

    (*m_counter)--;
    if (*m_counter == 0)
        cudaFree(m_ptr);
}

template class MY_NEURAL_NETWORK_LIB__TENSOR__CUDA_API ManagedMemorySharedPtr<bool>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__CUDA_API ManagedMemorySharedPtr<float>;
template class MY_NEURAL_NETWORK_LIB__TENSOR__CUDA_API ManagedMemorySharedPtr<double>;

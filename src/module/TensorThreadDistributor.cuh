#pragma once

#include <CommonMacros.h>

#include <type_traits>

template <typename ValueType, size_t Dimension>
class RawTensor;

template <size_t Dimension>
class RawTensorIndex;

template <typename ValueType, size_t Dimension>
class TensorThreadDistributor
{
public:
    DEVICE inline TensorThreadDistributor(RawTensor<ValueType, Dimension>& rawTensor);
    DEVICE inline ~TensorThreadDistributor();

    HOST static dim3 getNbThreads();
    HOST static dim3 getNbBlocks(const RawTensor<ValueType, Dimension>& rawTensor);

    DEVICE inline RawTensorIndex<Dimension> askNewIndex(const RawTensorIndex<Dimension>& previous) const;

    template <typename FunctionType>
    DEVICE
    inline
    std::enable_if_t<
        std::is_same_v<
            void,
            decltype( // This method returns void and takes a function as a parameter
                std::declval<FunctionType>()(
                    std::declval<const RawTensorIndex<Dimension>&>() // The parameter function takes a tensor index as parameter
                )
            )
        >,
        void
    >
    iterate(const FunctionType&& function);

private:
    RawTensor<ValueType, Dimension>& m_rawTensor;
};

#include <module/TensorThreadDistributor.impl.cuh>

#pragma once

#include <network/AbstractNetworkBuilder.h>

#include <map>
#include <string>

template <typename ValueType>
class AbstractTensor;
template <typename ValueType>
class NeuralNetwork;

template <typename ValueType>
struct DuplicateTensorBuilderPimpl;

template <typename ValueType>
class DuplicateTensorBuilder : public AbstractNetworkBuilder<ValueType>
{
public:
    DuplicateTensorBuilder(NeuralNetwork<ValueType>& network, const AbstractTensor<ValueType>& tensor);
    DuplicateTensorBuilder(const DuplicateTensorBuilder& other) = delete;
    DuplicateTensorBuilder(DuplicateTensorBuilder&& other);

    virtual ~DuplicateTensorBuilder();

    DuplicateTensorBuilder& operator=(const DuplicateTensorBuilder& other) = delete;
    DuplicateTensorBuilder& operator=(DuplicateTensorBuilder&& other);

    virtual std::map<std::string, void*> build() override;

    static inline const std::string addedTensorKey = "added_tensor";

private:
    DuplicateTensorBuilderPimpl<ValueType>* m_pimpl;

    DECLARE_RTTI(AbstractNetworkBuilder<ValueType>)
};

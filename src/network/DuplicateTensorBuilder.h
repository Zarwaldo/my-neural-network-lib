#pragma once

#include <network/AbstractNetworkBuilder.h>

#include <map>
#include <string>

template <typename ScalarType>
class AbstractTensor;
template <typename ScalarType>
class NeuralNetwork;

template <typename ScalarType>
struct DuplicateTensorBuilderPimpl;

template <typename ScalarType>
class DuplicateTensorBuilder : public AbstractNetworkBuilder<ScalarType>
{
public:
    DuplicateTensorBuilder(NeuralNetwork<ScalarType>& network, const AbstractTensor<ScalarType>& tensor);
    DuplicateTensorBuilder(const DuplicateTensorBuilder& other) = delete;
    DuplicateTensorBuilder(DuplicateTensorBuilder&& other);

    virtual ~DuplicateTensorBuilder();

    DuplicateTensorBuilder& operator=(const DuplicateTensorBuilder& other) = delete;
    DuplicateTensorBuilder& operator=(DuplicateTensorBuilder&& other);

    virtual std::map<std::string, void*> build() override;

    static inline const std::string addedTensorKey = "added_tensor";

private:
    DuplicateTensorBuilderPimpl<ScalarType>* m_pimpl;

    DECLARE_RTTI(AbstractNetworkBuilder<ScalarType>)
};

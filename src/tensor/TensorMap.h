#pragma once

#include <tensor/AbstractTensorMap.h>

#include <tensor/TensorMapKeyEnum.h>

#include <initializer_list>

template <typename ScalarType, typename KeyEnum>
class TensorMap : public AbstractTensorMap<ScalarType>
{
public:
    HOST TensorMap(AbstractTensor<ScalarType>* const tensors[KeyEnum::NbValues]);
    HOST TensorMap(const std::initializer_list<AbstractTensor<ScalarType>*>& list);
    HOST TensorMap(const TensorMap& other);
    HOST virtual ~TensorMap();

    HOST virtual size_t nbTensors() const override;

    HOST virtual const AbstractTensor<ScalarType>& get(size_t index) const override;
    HOST virtual AbstractTensor<ScalarType>& get(size_t index) override;

    HOST const AbstractTensor<ScalarType>& get(KeyEnum key) const;
    HOST AbstractTensor<ScalarType>& get(KeyEnum key);

private:
    AbstractTensor<ScalarType>* m_tensors[KeyEnum::NbValues];

    DECLARE_RTTI(AbstractTensorMap<ScalarType>)
};

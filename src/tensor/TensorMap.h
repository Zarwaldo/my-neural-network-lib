#pragma once

#include <tensor/AbstractTensorMap.h>

#include <tensor/TensorMapKeyEnum.h>

#include <initializer_list>

template <typename ValueType, typename KeyEnum>
class TensorMap : public AbstractTensorMap<ValueType>
{
public:
    HOST TensorMap(AbstractTensor<ValueType>* const tensors[KeyEnum::NbValues]);
    HOST TensorMap(const std::initializer_list<AbstractTensor<ValueType>*>& list);
    HOST TensorMap(const TensorMap& other);
    HOST virtual ~TensorMap();

    HOST virtual size_t nbTensors() const override;

    HOST virtual const AbstractTensor<ValueType>& get(size_t index) const override;
    HOST virtual AbstractTensor<ValueType>& get(size_t index) override;

    HOST const AbstractTensor<ValueType>& get(KeyEnum key) const;
    HOST AbstractTensor<ValueType>& get(KeyEnum key);

private:
    AbstractTensor<ValueType>* m_tensors[KeyEnum::NbValues];

    DECLARE_RTTI(AbstractTensorMap<ValueType>)
};

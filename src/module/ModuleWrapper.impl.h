#pragma once

#include <module/ModuleWrapper.h>

template <typename ValueType, class ModuleType, typename CtorParamType>
ModuleWrapper<ValueType, ModuleType, CtorParamType>::ModuleWrapper(const CtorParamType& param)
    : m_module(new ModuleType(param))
{}

template <typename ValueType, class ModuleType, typename CtorParamType>
ModuleWrapper<ValueType, ModuleType, CtorParamType>::ModuleWrapper(ModuleWrapper&& other)
    : m_module(other.m_module)
{
    other.m_module = nullptr;
}

template <typename ValueType, class ModuleType, typename CtorParamType>
ModuleWrapper<ValueType, ModuleType, CtorParamType>::~ModuleWrapper()
{
    delete m_module;
}

template <typename ValueType, class ModuleType, typename CtorParamType>
ModuleWrapper<ValueType, ModuleType, CtorParamType>&
ModuleWrapper<ValueType, ModuleType, CtorParamType>::operator=(ModuleWrapper&& other)
{
    std::swap(m_module, other.m_module);
    return *this;
}

template <typename ValueType, class ModuleType, typename CtorParamType>
AbstractTensorMap<ValueType>*
ModuleWrapper<ValueType, ModuleType, CtorParamType>::getInputAbstractTensorMap() const
{
    return m_module->getInputAbstractTensorMap();
}

template <typename ValueType, class ModuleType, typename CtorParamType>
const AbstractTensorMap<ValueType>&
ModuleWrapper<ValueType, ModuleType, CtorParamType>::getParameterAbstractTensorMap() const
{
    return m_module->getParameterAbstractTensorMap();
}

template <typename ValueType, class ModuleType, typename CtorParamType>
AbstractTensorMap<ValueType>&
ModuleWrapper<ValueType, ModuleType, CtorParamType>::getParameterAbstractTensorMap()
{
    return m_module->getParameterAbstractTensorMap();
}

template <typename ValueType, class ModuleType, typename CtorParamType>
AbstractTensorMap<ValueType>*
ModuleWrapper<ValueType, ModuleType, CtorParamType>::getOutputAbstractTensorMap() const
{
    return m_module->getOutputAbstractTensorMap();
}

template <typename ValueType, class ModuleType, typename CtorParamType>
void
ModuleWrapper<ValueType, ModuleType, CtorParamType>::setInputTensorMap(AbstractTensorMap<ValueType>* map)
{
    m_module->setInputTensorMap(map);
}

template <typename ValueType, class ModuleType, typename CtorParamType>
void
ModuleWrapper<ValueType, ModuleType, CtorParamType>::setOutputTensorMap(AbstractTensorMap<ValueType>* map)
{
    m_module->setOutputTensorMap(map);
}

template <typename ValueType, class ModuleType, typename CtorParamType>
void
ModuleWrapper<ValueType, ModuleType, CtorParamType>::compute() const
{
    m_module->compute();
}

template <typename ValueType, class ModuleType, typename CtorParamType>
void
ModuleWrapper<ValueType, ModuleType, CtorParamType>::backpropagate(AbstractTensorMap<ValueType>& costPartDerivWRTInput, AbstractTensorMap<ValueType>& costPartDerivWRTParameter, const AbstractTensorMap<ValueType>& costPartDerivWRTOutput) const
{
    m_module->backpropagate(costPartDerivWRTInput, costPartDerivWRTParameter, costPartDerivWRTOutput);
}

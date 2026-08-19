#pragma once

#include <module/ModuleWrapper.h>

template <typename ScalarType, class ModuleType, typename CtorParamType>
ModuleWrapper<ScalarType, ModuleType, CtorParamType>::ModuleWrapper(const CtorParamType& param)
    : m_module(new ModuleType(param))
{}

template <typename ScalarType, class ModuleType, typename CtorParamType>
ModuleWrapper<ScalarType, ModuleType, CtorParamType>::ModuleWrapper(ModuleWrapper&& other)
    : m_module(other.m_module)
{
    other.m_module = nullptr;
}

template <typename ScalarType, class ModuleType, typename CtorParamType>
ModuleWrapper<ScalarType, ModuleType, CtorParamType>::~ModuleWrapper()
{
    delete m_module;
}

template <typename ScalarType, class ModuleType, typename CtorParamType>
ModuleWrapper<ScalarType, ModuleType, CtorParamType>&
ModuleWrapper<ScalarType, ModuleType, CtorParamType>::operator=(ModuleWrapper&& other)
{
    std::swap(m_module, other.m_module);
    return *this;
}

template <typename ScalarType, class ModuleType, typename CtorParamType>
AbstractTensorMap<ScalarType>*
ModuleWrapper<ScalarType, ModuleType, CtorParamType>::getInputAbstractTensorMap() const
{
    return m_module->getInputAbstractTensorMap();
}

template <typename ScalarType, class ModuleType, typename CtorParamType>
const AbstractTensorMap<ScalarType>&
ModuleWrapper<ScalarType, ModuleType, CtorParamType>::getParameterAbstractTensorMap() const
{
    return m_module->getParameterAbstractTensorMap();
}

template <typename ScalarType, class ModuleType, typename CtorParamType>
AbstractTensorMap<ScalarType>&
ModuleWrapper<ScalarType, ModuleType, CtorParamType>::getParameterAbstractTensorMap()
{
    return m_module->getParameterAbstractTensorMap();
}

template <typename ScalarType, class ModuleType, typename CtorParamType>
AbstractTensorMap<ScalarType>*
ModuleWrapper<ScalarType, ModuleType, CtorParamType>::getOutputAbstractTensorMap() const
{
    return m_module->getOutputAbstractTensorMap();
}

template <typename ScalarType, class ModuleType, typename CtorParamType>
void
ModuleWrapper<ScalarType, ModuleType, CtorParamType>::setInputTensorMap(AbstractTensorMap<ScalarType>* map)
{
    m_module->setInputTensorMap(map);
}

template <typename ScalarType, class ModuleType, typename CtorParamType>
void
ModuleWrapper<ScalarType, ModuleType, CtorParamType>::setOutputTensorMap(AbstractTensorMap<ScalarType>* map)
{
    m_module->setOutputTensorMap(map);
}

template <typename ScalarType, class ModuleType, typename CtorParamType>
void
ModuleWrapper<ScalarType, ModuleType, CtorParamType>::compute() const
{
    m_module->compute();
}

template <typename ScalarType, class ModuleType, typename CtorParamType>
void
ModuleWrapper<ScalarType, ModuleType, CtorParamType>::backpropagate(AbstractTensorMap<ScalarType>& costPartDerivWRTInput, AbstractTensorMap<ScalarType>& costPartDerivWRTParameter, const AbstractTensorMap<ScalarType>& costPartDerivWRTOutput) const
{
    m_module->backpropagate(costPartDerivWRTInput, costPartDerivWRTParameter, costPartDerivWRTOutput);
}

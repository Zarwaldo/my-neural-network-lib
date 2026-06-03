#pragma once

#include <module/Module.h>

template <typename ValueType, class ModuleType, typename CtorParamType>
class ModuleWrapper : public Module<ValueType>
{
public:
    HOST explicit ModuleWrapper(const CtorParamType& param);
    HOST ModuleWrapper(const ModuleWrapper& other) = delete;
    HOST ModuleWrapper(ModuleWrapper&& other);

    HOST ~ModuleWrapper();
    
    HOST ModuleWrapper& operator=(const ModuleWrapper& other) = delete;
    HOST ModuleWrapper& operator=(ModuleWrapper&& other);

    HOST virtual AbstractTensorMap<ValueType>* getInputAbstractTensorMap() const override;

    HOST virtual const AbstractTensorMap<ValueType>& getParameterAbstractTensorMap() const override;
    HOST virtual AbstractTensorMap<ValueType>& getParameterAbstractTensorMap() override;

    HOST virtual AbstractTensorMap<ValueType>* getOutputAbstractTensorMap() const override;

    HOST virtual void setInputTensorMap(AbstractTensorMap<ValueType>* map) override;
    HOST virtual void setOutputTensorMap(AbstractTensorMap<ValueType>* map) override;

    HOST virtual void compute() const override;

    HOST virtual void backpropagate(AbstractTensorMap<ValueType>& costPartDerivWRTInput, AbstractTensorMap<ValueType>& costPartDerivWRTParameter, const AbstractTensorMap<ValueType>& costPartDerivWRTOutput) const override;

private:
    Module<ValueType>* m_module;
};

#define __MODULE_WRAPPER__UNPACK__IMPL__PACK(...) __VA_ARGS__
#define __MODULE_WRAPPER__UNPACK_NOEXPAND(x) __MODULE_WRAPPER__UNPACK__IMPL__##x
#define __MODULE_WRAPPER__UNPACK(x, ...) __MODULE_WRAPPER__UNPACK_NOEXPAND(x)

#define DECLARE_MODULE_WRAPPER(ConcreteModuleName, ValueType, WrappedModuleName, CtorParamTypePack, TemplateParamTypesPack, TemplateParamNamesPack)                                                                     \
    TEMPLATE_MACROS__TEMPLATE_PREFIX(TemplateParamTypesPack, TemplateParamNamesPack)                                                                                                                                    \
    class ConcreteModuleName : public ModuleWrapper<ValueType, TEMPLATE_MACROS__TEMPLATE_INSTANTIATION(WrappedModuleName, TemplateParamTypesPack, TemplateParamNamesPack), __MODULE_WRAPPER__UNPACK(CtorParamTypePack)> \
    {                                                                                                                                                                                                                   \
    public:                                                                                                                                                                                                             \
        ConcreteModuleName(const __MODULE_WRAPPER__UNPACK(CtorParamTypePack)& param);                                                                                                                                   \
                                                                                                                                                                                                                        \
        DECLARE_RTTI(Module<ValueType>)                                                                                                                                                                                 \
    };

#define IMPLEMENT_MODULE_WRAPPER(ConcreteModuleName, ValueType, WrappedModuleName, CtorParamTypePack, TemplateParamTypesPack, TemplateParamNamesPack)                                              \
    TEMPLATE_MACROS__TEMPLATE_PREFIX(TemplateParamTypesPack, TemplateParamNamesPack)                                                                                                               \
    TEMPLATE_MACROS__TEMPLATE_INSTANTIATION(ConcreteModuleName, TemplateParamTypesPack, TemplateParamNamesPack)::ConcreteModuleName(const __MODULE_WRAPPER__UNPACK(CtorParamTypePack)& param)      \
        : ModuleWrapper<ValueType, TEMPLATE_MACROS__TEMPLATE_INSTANTIATION(WrappedModuleName, TemplateParamTypesPack, TemplateParamNamesPack), __MODULE_WRAPPER__UNPACK(CtorParamTypePack)>(param) \
    {}                                                                                                                                                                                             \
                                                                                                                                                                                                   \
    IMPLEMENT_RTTI(ConcreteModuleName, Module<ValueType>, TemplateParamTypesPack, TemplateParamNamesPack)

#include <module/ModuleWrapper.impl.h>

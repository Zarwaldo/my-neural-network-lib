#pragma once

#include <module/Module.h>

template <typename ScalarType, class ModuleType, typename CtorParamType>
class ModuleWrapper : public Module<ScalarType>
{
public:
    HOST explicit ModuleWrapper(const CtorParamType& param);
    HOST ModuleWrapper(const ModuleWrapper& other) = delete;
    HOST ModuleWrapper(ModuleWrapper&& other);

    HOST ~ModuleWrapper();
    
    HOST ModuleWrapper& operator=(const ModuleWrapper& other) = delete;
    HOST ModuleWrapper& operator=(ModuleWrapper&& other);

    HOST virtual AbstractTensorMap<ScalarType>* getInputAbstractTensorMap() const override;

    HOST virtual const AbstractTensorMap<ScalarType>& getParameterAbstractTensorMap() const override;
    HOST virtual AbstractTensorMap<ScalarType>& getParameterAbstractTensorMap() override;

    HOST virtual AbstractTensorMap<ScalarType>* getOutputAbstractTensorMap() const override;

    HOST virtual void setInputTensorMap(AbstractTensorMap<ScalarType>* map) override;
    HOST virtual void setOutputTensorMap(AbstractTensorMap<ScalarType>* map) override;

    HOST virtual void compute() const override;

    HOST virtual void backpropagate(AbstractTensorMap<ScalarType>& costPartDerivWRTInput, AbstractTensorMap<ScalarType>& costPartDerivWRTParameter, const AbstractTensorMap<ScalarType>& costPartDerivWRTOutput) const override;

private:
    Module<ScalarType>* m_module;
};

#define __MODULE_WRAPPER__UNPACK__IMPL__PACK(...) __VA_ARGS__
#define __MODULE_WRAPPER__UNPACK_NOEXPAND(x) __MODULE_WRAPPER__UNPACK__IMPL__##x
#define __MODULE_WRAPPER__UNPACK(x, ...) __MODULE_WRAPPER__UNPACK_NOEXPAND(x)

#define DECLARE_MODULE_WRAPPER(ConcreteModuleName, ScalarType, WrappedModuleName, CtorParamTypePack, TemplateParamTypesPack, TemplateParamNamesPack)                                                                     \
    TEMPLATE_MACROS__TEMPLATE_PREFIX(TemplateParamTypesPack, TemplateParamNamesPack)                                                                                                                                     \
    class ConcreteModuleName : public ModuleWrapper<ScalarType, TEMPLATE_MACROS__TEMPLATE_INSTANTIATION(WrappedModuleName, TemplateParamTypesPack, TemplateParamNamesPack), __MODULE_WRAPPER__UNPACK(CtorParamTypePack)> \
    {                                                                                                                                                                                                                    \
    public:                                                                                                                                                                                                              \
        ConcreteModuleName(const __MODULE_WRAPPER__UNPACK(CtorParamTypePack)& param);                                                                                                                                    \
                                                                                                                                                                                                                         \
        DECLARE_RTTI(Module<ScalarType>)                                                                                                                                                                                 \
    };

#define IMPLEMENT_MODULE_WRAPPER(ConcreteModuleName, ScalarType, WrappedModuleName, CtorParamTypePack, TemplateParamTypesPack, TemplateParamNamesPack)                                              \
    TEMPLATE_MACROS__TEMPLATE_PREFIX(TemplateParamTypesPack, TemplateParamNamesPack)                                                                                                                \
    TEMPLATE_MACROS__TEMPLATE_INSTANTIATION(ConcreteModuleName, TemplateParamTypesPack, TemplateParamNamesPack)::ConcreteModuleName(const __MODULE_WRAPPER__UNPACK(CtorParamTypePack)& param)       \
        : ModuleWrapper<ScalarType, TEMPLATE_MACROS__TEMPLATE_INSTANTIATION(WrappedModuleName, TemplateParamTypesPack, TemplateParamNamesPack), __MODULE_WRAPPER__UNPACK(CtorParamTypePack)>(param) \
    {}                                                                                                                                                                                              \
                                                                                                                                                                                                    \
    IMPLEMENT_RTTI(ConcreteModuleName, Module<ScalarType>, TemplateParamTypesPack, TemplateParamNamesPack)

#include <module/ModuleWrapper.impl.h>

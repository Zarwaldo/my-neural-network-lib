#pragma once

#include <module/AbstractModule.h>

#include <rtti/Rtti.h>
#include <helpers/Macros.h>

template <typename ScalarType, size_t Dimension>
class RawTensor;

template <size_t Dimension>
class TensorIndex;

// UNPACK macros

#define __MODULE__UNPACK__IMPL__PACK(...) __VA_ARGS__
#define __MODULE__UNPACK_NOEXPAND(x) __MODULE__UNPACK__IMPL__##x
#define __MODULE__UNPACK(x, ...) __MODULE__UNPACK_NOEXPAND(x)

#define __MODULE__UNPACK2__IMPL__PACK(...) __VA_ARGS__
#define __MODULE__UNPACK2_NOEXPAND(x) __MODULE__UNPACK2__IMPL__##x
#define __MODULE__UNPACK2(x, ...) __MODULE__UNPACK2_NOEXPAND(x)

#define __MODULE__UNPACK3__IMPL__PACK(...) __VA_ARGS__
#define __MODULE__UNPACK3_NOEXPAND(x) __MODULE__UNPACK3__IMPL__##x
#define __MODULE__UNPACK3(x, ...) __MODULE__UNPACK3_NOEXPAND(x)

// Parameter macros

#define __MODULE__RAW_TENSOR_PTR_PARAM(Dimension, ScalarType, constQualifier) constQualifier RawTensor<ScalarType, Dimension>*
#define __MODULE__TENSOR_INDEX_REF_PARAM(Dimension, constQualifier) constQualifier TensorIndex<Dimension>&
#define __MODULE__PACKED_RAW_TENSOR_PTR_PARAM(Dimension, ScalarType, constQualifier) PACK(__MODULE__RAW_TENSOR_PTR_PARAM(Dimension, ScalarType, constQualifier))

// Computation kernel method

/// Name

#define __MODULE__COMPUTATION_KERNEL_NAME_FROM_ENUM_VALUE_NAME_NOEXPAND(ENUM_VALUE) computationKernel__##ENUM_VALUE
#define __MODULE__COMPUTATION_KERNEL_NAME_FROM_ENUM_VALUE_NAME(ENUM_VALUE) __MODULE__COMPUTATION_KERNEL_NAME_FROM_ENUM_VALUE_NAME_NOEXPAND(ENUM_VALUE)
#define __MODULE__COMPUTATION_KERNEL_NAME_FROM_INDEX(index, OutputKeyEnum) __MODULE__COMPUTATION_KERNEL_NAME_FROM_ENUM_VALUE_NAME(TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_GET_VALUE_NAME(OutputKeyEnum, index))

/// Tensor dimensions

#define __IMPLEMENT_MODULE__COMPUTATION_KERNEL__OUTPUT_TENSOR_DIMENSION(index, OutputTensorDimensionsPack) GET_FROM_INDEX(index, __MODULE__UNPACK(OutputTensorDimensionsPack))
#define __IMPLEMENT_MODULE__COMPUTATION_KERNEL__INPUT_TENSOR_DIMENSIONS(InputTensorDimensionsPack, ParameterTensorDimensionsPack) \
    __MODULE__UNPACK(ParameterTensorDimensionsPack), __MODULE__UNPACK(InputTensorDimensionsPack)

/// Parameter types

#define __IMPLEMENT_MODULE__COMPUTATION_KERNEL__PACKED_PARAMS_TYPES(index, ScalarType, InputTensorDimensionsPack, ParameterTensorDimensionsPack, OutputTensorDimensionsPack)                               \
    __MODULE__PACKED_RAW_TENSOR_PTR_PARAM(__IMPLEMENT_MODULE__COMPUTATION_KERNEL__OUTPUT_TENSOR_DIMENSION(index, OutputTensorDimensionsPack), ScalarType, ),                                               \
    FOR_EACH(__MODULE__PACKED_RAW_TENSOR_PTR_PARAM, FORWARD(ScalarType, const), __IMPLEMENT_MODULE__COMPUTATION_KERNEL__INPUT_TENSOR_DIMENSIONS(InputTensorDimensionsPack, ParameterTensorDimensionsPack))

#define __IMPLEMENT_MODULE__COMPUTATION_KERNEL__PARAMS_TYPES(index, ScalarType, InputTensorDimensionsPack, ParameterTensorDimensionsPack, OutputTensorDimensionsPack)                                           \
    FOR_EACH(__MODULE__UNPACK, FORWARD(), __IMPLEMENT_MODULE__COMPUTATION_KERNEL__PACKED_PARAMS_TYPES(index, ScalarType, InputTensorDimensionsPack, ParameterTensorDimensionsPack, OutputTensorDimensionsPack))

/// Params names

#define __IMPLEMENT_MODULE__COMPUTATION_KERNEL__PARAM_NAME_NOEXPAND(ENUM_VALUE, prefix) prefix##__##ENUM_VALUE
#define __IMPLEMENT_MODULE__COMPUTATION_KERNEL__PARAM_NAME(ENUM_VALUE, type) __IMPLEMENT_MODULE__COMPUTATION_KERNEL__PARAM_NAME_NOEXPAND(ENUM_VALUE, type)

#define __IMPLEMENT_MODULE__COMPUTATION_KERNEL__PARAMS_NAMES(index, InputKeyEnum, ParameterKeyEnum, OutputKeyEnum)                               \
    __IMPLEMENT_MODULE__COMPUTATION_KERNEL__PARAM_NAME(GET_FROM_INDEX(index, TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_VALUES(OutputKeyEnum)), output), \
    FOR_EACH(__IMPLEMENT_MODULE__COMPUTATION_KERNEL__PARAM_NAME, FORWARD(param), TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_VALUES(ParameterKeyEnum)),   \
    FOR_EACH(__IMPLEMENT_MODULE__COMPUTATION_KERNEL__PARAM_NAME, FORWARD(input), TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_VALUES(InputKeyEnum))

/// Named parameters

#define __IMPLEMENT_MODULE__COMPUTATION_KERNEL__NAMED_PARAMETER(type, paramName) __MODULE__UNPACK(type) paramName
#define __IMPLEMENT_MODULE__COMPUTATION_KERNEL__NAMED_PARAMETER__PACKED(arg, ...) __MODULE__UNPACK3(PACK(__IMPLEMENT_MODULE__COMPUTATION_KERNEL__NAMED_PARAMETER)(__MODULE__UNPACK2(arg)))
#define __IMPLEMENT_MODULE__COMPUTATION_KERNEL__NAMED_PARAMS(index, ScalarType, InputKeyEnum, ParameterKeyEnum, OutputKeyEnum, InputTensorDimensionsPack, ParameterTensorDimensionsPack, OutputTensorDimensionsPack) \
    FOR_EACH4(__IMPLEMENT_MODULE__COMPUTATION_KERNEL__NAMED_PARAMETER__PACKED, FORWARD(), INTERTWINE_PACKS23(                                                                                                        \
        PACK(__IMPLEMENT_MODULE__COMPUTATION_KERNEL__PACKED_PARAMS_TYPES(index, ScalarType, InputTensorDimensionsPack, ParameterTensorDimensionsPack, OutputTensorDimensionsPack)),                                  \
        PACK(__IMPLEMENT_MODULE__COMPUTATION_KERNEL__PARAMS_NAMES(index, InputKeyEnum, ParameterKeyEnum, OutputKeyEnum))                                                                                             \
    ))

/// Declaration

#define __MODULE__DECLARE_COMPUTATION_KERNEL(index, ScalarType, InputKeyEnum, ParameterKeyEnum, OutputKeyEnum, InputTensorDimensionsPack, ParameterTensorDimensionsPack, OutputTensorDimensionsPack, TemplateParamNamesPack) \
    DEVICE static TEMPLATE_MACROS__INLINE(TemplateParamNamesPack) void __MODULE__COMPUTATION_KERNEL_NAME_FROM_INDEX(index, OutputKeyEnum)(                                                                                   \
        __IMPLEMENT_MODULE__COMPUTATION_KERNEL__NAMED_PARAMS(index, ScalarType, InputKeyEnum, ParameterKeyEnum, OutputKeyEnum, InputTensorDimensionsPack, ParameterTensorDimensionsPack, OutputTensorDimensionsPack)         \
    );

// Input backpropagation kernel method

/// Name

#define __MODULE__INPUT_BACKPROPAGATION_KERNEL_NAME_FROM_ENUM_VALUE_NAME_NOEXPAND(ENUM_VALUE) inputBackpropagationKernel__##ENUM_VALUE
#define __MODULE__INPUT_BACKPROPAGATION_KERNEL_NAME_FROM_ENUM_VALUE_NAME(ENUM_VALUE) __MODULE__INPUT_BACKPROPAGATION_KERNEL_NAME_FROM_ENUM_VALUE_NAME_NOEXPAND(ENUM_VALUE)
#define __MODULE__INPUT_BACKPROPAGATION_KERNEL_NAME_FROM_INDEX(index, InputKeyEnum) __MODULE__INPUT_BACKPROPAGATION_KERNEL_NAME_FROM_ENUM_VALUE_NAME(TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_GET_VALUE_NAME(InputKeyEnum, index))

/// Tensor dimensions

#define __IMPLEMENT_MODULE__INPUT_BACKPROPAGATION_KERNEL__OUTPUT_TENSOR_DIMENSION(index, InputTensorDimensionsPack) GET_FROM_INDEX(index, __MODULE__UNPACK(InputTensorDimensionsPack))
#define __IMPLEMENT_MODULE__INPUT_BACKPROPAGATION_KERNEL__INPUT_TENSOR_DIMENSIONS(InputTensorDimensionsPack, ParameterTensorDimensionsPack, OutputTensorDimensionsPack)                      \
    __MODULE__UNPACK(OutputTensorDimensionsPack), __MODULE__UNPACK(OutputTensorDimensionsPack), __MODULE__UNPACK(ParameterTensorDimensionsPack), __MODULE__UNPACK(InputTensorDimensionsPack)

/// Parameter types

#define __IMPLEMENT_MODULE__INPUT_BACKPROPAGATION_KERNEL__PACKED_PARAMS_TYPES(index, ScalarType, InputTensorDimensionsPack, ParameterTensorDimensionsPack, OutputTensorDimensionsPack)                                                           \
    __MODULE__PACKED_RAW_TENSOR_PTR_PARAM(__IMPLEMENT_MODULE__INPUT_BACKPROPAGATION_KERNEL__OUTPUT_TENSOR_DIMENSION(index, InputTensorDimensionsPack), ScalarType, ),                                                                            \
    FOR_EACH(__MODULE__PACKED_RAW_TENSOR_PTR_PARAM, FORWARD(ScalarType, const), __IMPLEMENT_MODULE__INPUT_BACKPROPAGATION_KERNEL__INPUT_TENSOR_DIMENSIONS(InputTensorDimensionsPack, ParameterTensorDimensionsPack, OutputTensorDimensionsPack))

#define __IMPLEMENT_MODULE__INPUT_BACKPROPAGATION_KERNEL__PARAMS_TYPES(index, ScalarType, InputTensorDimensionsPack, ParameterTensorDimensionsPack, OutputTensorDimensionsPack)                                            \
    FOR_EACH2(__MODULE__UNPACK, FORWARD(), __IMPLEMENT_MODULE__INPUT_BACKPROPAGATION_KERNEL__PACKED_PARAMS_TYPES(index, ScalarType, InputTensorDimensionsPack, ParameterTensorDimensionsPack, OutputTensorDimensionsPack))

/// Params names

#define __IMPLEMENT_MODULE__BACKPROPAGATION_KERNEL__PARAM_NAME_NOEXPAND(ENUM_VALUE, prefix) prefix##__##ENUM_VALUE
#define __IMPLEMENT_MODULE__BACKPROPAGATION_KERNEL__PARAM_NAME(ENUM_VALUE, type) __IMPLEMENT_MODULE__BACKPROPAGATION_KERNEL__PARAM_NAME_NOEXPAND(ENUM_VALUE, type)

#define __IMPLEMENT_MODULE__INPUT_BACKPROPAGATION_KERNEL__PARAMS_NAMES(index, InputKeyEnum, ParameterKeyEnum, OutputKeyEnum)                                       \
    __IMPLEMENT_MODULE__BACKPROPAGATION_KERNEL__PARAM_NAME(GET_FROM_INDEX(index, TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_VALUES(InputKeyEnum)), costPartDerivWRTInput), \
    FOR_EACH(__IMPLEMENT_MODULE__BACKPROPAGATION_KERNEL__PARAM_NAME, FORWARD(costPartDerivWRTOutput), TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_VALUES(OutputKeyEnum)),   \
    FOR_EACH(__IMPLEMENT_MODULE__BACKPROPAGATION_KERNEL__PARAM_NAME, FORWARD(output), TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_VALUES(OutputKeyEnum)),                   \
    FOR_EACH(__IMPLEMENT_MODULE__BACKPROPAGATION_KERNEL__PARAM_NAME, FORWARD(param), TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_VALUES(ParameterKeyEnum)),                 \
    FOR_EACH(__IMPLEMENT_MODULE__BACKPROPAGATION_KERNEL__PARAM_NAME, FORWARD(input), TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_VALUES(InputKeyEnum))

/// Named parameters

#define __IMPLEMENT_MODULE__BACKPROPAGATION_KERNEL__NAMED_PARAMETER(type, paramName) __MODULE__UNPACK(type) paramName
#define __IMPLEMENT_MODULE__BACKPROPAGATION_KERNEL__NAMED_PARAMETER__PACKED(arg, ...) __MODULE__UNPACK3(PACK(__IMPLEMENT_MODULE__BACKPROPAGATION_KERNEL__NAMED_PARAMETER)(__MODULE__UNPACK2(arg)))

#define __IMPLEMENT_MODULE__INPUT_BACKPROPAGATION_KERNEL__NAMED_PARAMS(index, ScalarType, InputKeyEnum, ParameterKeyEnum, OutputKeyEnum, InputTensorDimensionsPack, ParameterTensorDimensionsPack, OutputTensorDimensionsPack) \
    FOR_EACH4(__IMPLEMENT_MODULE__BACKPROPAGATION_KERNEL__NAMED_PARAMETER__PACKED, FORWARD(), INTERTWINE_PACKS23(                                                                                                              \
        PACK(__IMPLEMENT_MODULE__INPUT_BACKPROPAGATION_KERNEL__PACKED_PARAMS_TYPES(index, ScalarType, InputTensorDimensionsPack, ParameterTensorDimensionsPack, OutputTensorDimensionsPack)),                                  \
        PACK(__IMPLEMENT_MODULE__INPUT_BACKPROPAGATION_KERNEL__PARAMS_NAMES(index, InputKeyEnum, ParameterKeyEnum, OutputKeyEnum))                                                                                             \
    ))

/// Declaration

#define __MODULE__DECLARE_INPUT_BACKPROPAGATION_KERNEL(index, ScalarType, InputKeyEnum, ParameterKeyEnum, OutputKeyEnum, InputTensorDimensionsPack, ParameterTensorDimensionsPack, OutputTensorDimensionsPack, TemplateParamNamesPack) \
    DEVICE static TEMPLATE_MACROS__INLINE(TemplateParamNamesPack) void __MODULE__INPUT_BACKPROPAGATION_KERNEL_NAME_FROM_INDEX(index, InputKeyEnum)(                                                                                    \
        __IMPLEMENT_MODULE__INPUT_BACKPROPAGATION_KERNEL__NAMED_PARAMS(index, ScalarType, InputKeyEnum, ParameterKeyEnum, OutputKeyEnum, InputTensorDimensionsPack, ParameterTensorDimensionsPack, OutputTensorDimensionsPack)         \
    );

// Parameter backpropagation kernel method

/// Name

#define __MODULE__PARAM_BACKPROPAGATION_KERNEL_NAME_FROM_ENUM_VALUE_NAME_NOEXPAND(ENUM_VALUE) parameterBackpropagationKernel__##ENUM_VALUE
#define __MODULE__PARAM_BACKPROPAGATION_KERNEL_NAME_FROM_ENUM_VALUE_NAME(ENUM_VALUE) __MODULE__PARAM_BACKPROPAGATION_KERNEL_NAME_FROM_ENUM_VALUE_NAME_NOEXPAND(ENUM_VALUE)
#define __MODULE__PARAM_BACKPROPAGATION_KERNEL_NAME_FROM_INDEX(index, ParameterKeyEnum) __MODULE__PARAM_BACKPROPAGATION_KERNEL_NAME_FROM_ENUM_VALUE_NAME(TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_GET_VALUE_NAME(ParameterKeyEnum, index))

/// Tensor dimensions

#define __IMPLEMENT_MODULE__PARAM_BACKPROPAGATION_KERNEL__OUTPUT_TENSOR_DIMENSION(index, ParameterTensorDimensionsPack) GET_FROM_INDEX(index, __MODULE__UNPACK(ParameterTensorDimensionsPack))
#define __IMPLEMENT_MODULE__PARAM_BACKPROPAGATION_KERNEL__INPUT_TENSOR_DIMENSIONS(InputTensorDimensionsPack, ParameterTensorDimensionsPack, OutputTensorDimensionsPack)                      \
    __MODULE__UNPACK(OutputTensorDimensionsPack), __MODULE__UNPACK(OutputTensorDimensionsPack), __MODULE__UNPACK(ParameterTensorDimensionsPack), __MODULE__UNPACK(InputTensorDimensionsPack)

/// Parameter types

#define __IMPLEMENT_MODULE__PARAM_BACKPROPAGATION_KERNEL__PACKED_PARAMS_TYPES(index, ScalarType, InputTensorDimensionsPack, ParameterTensorDimensionsPack, OutputTensorDimensionsPack)                                                           \
    __MODULE__PACKED_RAW_TENSOR_PTR_PARAM(__IMPLEMENT_MODULE__PARAM_BACKPROPAGATION_KERNEL__OUTPUT_TENSOR_DIMENSION(index, ParameterTensorDimensionsPack), ScalarType, ),                                                                        \
    FOR_EACH(__MODULE__PACKED_RAW_TENSOR_PTR_PARAM, FORWARD(ScalarType, const), __IMPLEMENT_MODULE__PARAM_BACKPROPAGATION_KERNEL__INPUT_TENSOR_DIMENSIONS(InputTensorDimensionsPack, ParameterTensorDimensionsPack, OutputTensorDimensionsPack))

#define __IMPLEMENT_MODULE__PARAM_BACKPROPAGATION_KERNEL__PARAMS_TYPES(index, ScalarType, InputTensorDimensionsPack, ParameterTensorDimensionsPack, OutputTensorDimensionsPack)                                            \
    FOR_EACH2(__MODULE__UNPACK, FORWARD(), __IMPLEMENT_MODULE__PARAM_BACKPROPAGATION_KERNEL__PACKED_PARAMS_TYPES(index, ScalarType, InputTensorDimensionsPack, ParameterTensorDimensionsPack, OutputTensorDimensionsPack))

/// Params names

#define __IMPLEMENT_MODULE__PARAM_BACKPROPAGATION_KERNEL__PARAMS_NAMES(index, InputKeyEnum, ParameterKeyEnum, OutputKeyEnum)                                           \
    __IMPLEMENT_MODULE__BACKPROPAGATION_KERNEL__PARAM_NAME(GET_FROM_INDEX(index, TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_VALUES(ParameterKeyEnum)), costPartDerivWRTParam), \
    FOR_EACH(__IMPLEMENT_MODULE__BACKPROPAGATION_KERNEL__PARAM_NAME, FORWARD(costPartDerivWRTOutput), TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_VALUES(OutputKeyEnum)),       \
    FOR_EACH(__IMPLEMENT_MODULE__BACKPROPAGATION_KERNEL__PARAM_NAME, FORWARD(output), TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_VALUES(OutputKeyEnum)),                       \
    FOR_EACH(__IMPLEMENT_MODULE__BACKPROPAGATION_KERNEL__PARAM_NAME, FORWARD(param), TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_VALUES(ParameterKeyEnum)),                     \
    FOR_EACH(__IMPLEMENT_MODULE__BACKPROPAGATION_KERNEL__PARAM_NAME, FORWARD(input), TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_VALUES(InputKeyEnum))

/// Named parameters

#define __IMPLEMENT_MODULE__PARAM_BACKPROPAGATION_KERNEL__NAMED_PARAMS(index, ScalarType, InputKeyEnum, ParameterKeyEnum, OutputKeyEnum, InputTensorDimensionsPack, ParameterTensorDimensionsPack, OutputTensorDimensionsPack) \
    FOR_EACH4(__IMPLEMENT_MODULE__BACKPROPAGATION_KERNEL__NAMED_PARAMETER__PACKED, FORWARD(), INTERTWINE_PACKS23(                                                                                                              \
        PACK(__IMPLEMENT_MODULE__PARAM_BACKPROPAGATION_KERNEL__PACKED_PARAMS_TYPES(index, ScalarType, InputTensorDimensionsPack, ParameterTensorDimensionsPack, OutputTensorDimensionsPack)),                                  \
        PACK(__IMPLEMENT_MODULE__PARAM_BACKPROPAGATION_KERNEL__PARAMS_NAMES(index, InputKeyEnum, ParameterKeyEnum, OutputKeyEnum))                                                                                             \
    ))

/// Declaration

#define __MODULE__DECLARE_PARAM_BACKPROPAGATION_KERNEL(index, ScalarType, InputKeyEnum, ParameterKeyEnum, OutputKeyEnum, InputTensorDimensionsPack, ParameterTensorDimensionsPack, OutputTensorDimensionsPack, TemplateParamNamesPack) \
    DEVICE static TEMPLATE_MACROS__INLINE(TemplateParamNamesPack) void __MODULE__PARAM_BACKPROPAGATION_KERNEL_NAME_FROM_INDEX(index, ParameterKeyEnum)(                                                                                \
        __IMPLEMENT_MODULE__PARAM_BACKPROPAGATION_KERNEL__NAMED_PARAMS(index, ScalarType, InputKeyEnum, ParameterKeyEnum, OutputKeyEnum, InputTensorDimensionsPack, ParameterTensorDimensionsPack, OutputTensorDimensionsPack)         \
    );

// Main declatation

#define DECLARE_MODULE(ConcreteModuleName, ScalarType, InputKeyEnum, ParameterKeyEnum, OutputKeyEnum, InputTensorDimensionsPack, ParameterTensorDimensionsPack, OutputTensorDimensionsPack, TemplateParamTypesPack, TemplateParamNamesPack)                                          \
    TEMPLATE_MACROS__TEMPLATE_PREFIX(TemplateParamTypesPack, TemplateParamNamesPack)                                                                                                                                                                                                 \
    class ConcreteModuleName : public AbstractModule<                                                                                                                                                                                                                                \
        ScalarType,                                                                                                                                                                                                                                                                  \
        InputKeyEnum,                                                                                                                                                                                                                                                                \
        ParameterKeyEnum,                                                                                                                                                                                                                                                            \
        OutputKeyEnum,                                                                                                                                                                                                                                                               \
        BuildTimeList::IntegerList<__MODULE__UNPACK(InputTensorDimensionsPack)>,                                                                                                                                                                                                     \
        BuildTimeList::IntegerList<__MODULE__UNPACK(ParameterTensorDimensionsPack)>,                                                                                                                                                                                                 \
        BuildTimeList::IntegerList<__MODULE__UNPACK(OutputTensorDimensionsPack)>                                                                                                                                                                                                     \
    >                                                                                                                                                                                                                                                                                \
    {                                                                                                                                                                                                                                                                                \
    public:                                                                                                                                                                                                                                                                          \
        HOST explicit TEMPLATE_MACROS__INLINE(TemplateParamNamesPack) ConcreteModuleName(                                                                                                                                                                                            \
            const RawTuple<FOR_EACH(__MODULE__TENSOR_INDEX_REF_PARAM, FORWARD(const), __MODULE__UNPACK(ParameterTensorDimensionsPack))>& parameterTensorsSizes                                                                                                                       \
        );                                                                                                                                                                                                                                                                           \
                                                                                                                                                                                                                                                                                     \
        HOST virtual TEMPLATE_MACROS__INLINE(TemplateParamNamesPack) void compute() const override;                                                                                                                                                                                  \
        HOST virtual TEMPLATE_MACROS__INLINE(TemplateParamNamesPack) void backpropagate(AbstractTensorMap<ScalarType>& costPartDerivWRTInput, AbstractTensorMap<ScalarType>& costPartDerivWRTParameter, const AbstractTensorMap<ScalarType>& costPartDerivWRTOutput) const override; \
                                                                                                                                                                                                                                                                                     \
        HOST static TEMPLATE_MACROS__INLINE(TemplateParamNamesPack) bool areSizesCorrect(                                                                                                                                                                                            \
            const RawTuple<FOR_EACH(__MODULE__TENSOR_INDEX_REF_PARAM, FORWARD(const), __MODULE__UNPACK(InputTensorDimensionsPack))>& inputTensorsSizes,                                                                                                                              \
            const RawTuple<FOR_EACH(__MODULE__TENSOR_INDEX_REF_PARAM, FORWARD(const), __MODULE__UNPACK(ParameterTensorDimensionsPack))>& parameterTensorsSizes,                                                                                                                      \
            const RawTuple<FOR_EACH(__MODULE__TENSOR_INDEX_REF_PARAM, FORWARD(const), __MODULE__UNPACK(OutputTensorDimensionsPack))>& outputTensorsSizes                                                                                                                             \
        );                                                                                                                                                                                                                                                                           \
                                                                                                                                                                                                                                                                                     \
        FOR_EACH_SEP5(                                                                                                                                                                                                                                                               \
            __MODULE__DECLARE_COMPUTATION_KERNEL,                                                                                                                                                                                                                                    \
            FORWARD(),                                                                                                                                                                                                                                                               \
            FORWARD(ScalarType, InputKeyEnum, ParameterKeyEnum, OutputKeyEnum, InputTensorDimensionsPack, ParameterTensorDimensionsPack, OutputTensorDimensionsPack, TemplateParamNamesPack),                                                                                        \
            RANGE(TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_NB_VALUES(OutputKeyEnum))                                                                                                                                                                                                       \
        )                                                                                                                                                                                                                                                                            \
                                                                                                                                                                                                                                                                                     \
        FOR_EACH_SEP5(                                                                                                                                                                                                                                                               \
            __MODULE__DECLARE_INPUT_BACKPROPAGATION_KERNEL,                                                                                                                                                                                                                          \
            FORWARD(),                                                                                                                                                                                                                                                               \
            FORWARD(ScalarType, InputKeyEnum, ParameterKeyEnum, OutputKeyEnum, InputTensorDimensionsPack, ParameterTensorDimensionsPack, OutputTensorDimensionsPack, TemplateParamNamesPack),                                                                                        \
            RANGE(TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_NB_VALUES(InputKeyEnum))                                                                                                                                                                                                        \
        )                                                                                                                                                                                                                                                                            \
                                                                                                                                                                                                                                                                                     \
        FOR_EACH_SEP5(                                                                                                                                                                                                                                                               \
            __MODULE__DECLARE_PARAM_BACKPROPAGATION_KERNEL,                                                                                                                                                                                                                          \
            FORWARD(),                                                                                                                                                                                                                                                               \
            FORWARD(ScalarType, InputKeyEnum, ParameterKeyEnum, OutputKeyEnum, InputTensorDimensionsPack, ParameterTensorDimensionsPack, OutputTensorDimensionsPack, TemplateParamNamesPack),                                                                                        \
            RANGE(TENSOR_MAP_KEY_ENUM__INTERNAL_ENUM_NB_VALUES(ParameterKeyEnum))                                                                                                                                                                                                    \
        )                                                                                                                                                                                                                                                                            \
                                                                                                                                                                                                                                                                                     \
        DECLARE_RTTI(Module<ScalarType>)                                                                                                                                                                                                                                             \
    };

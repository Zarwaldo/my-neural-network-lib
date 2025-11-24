#pragma once

#include <helpers/IntertwinePacksMacro.h>
#include <helpers/TernaryMacro.h>

#define __TEMPLATE_MACROS__UNPACK__IMPL__PACK(...) __VA_ARGS__
#define __TEMPLATE_MACROS__UNPACK_NOEXPAND(x) __TEMPLATE_MACROS__UNPACK__IMPL__##x
#define __TEMPLATE_MACROS__UNPACK(x, ...) __TEMPLATE_MACROS__UNPACK_NOEXPAND(x)

#define __TEMPLATE_MACROS__UNPACK2__IMPL__PACK(...) __VA_ARGS__
#define __TEMPLATE_MACROS__UNPACK2_NOEXPAND(x) __TEMPLATE_MACROS__UNPACK2__IMPL__##x
#define __TEMPLATE_MACROS__UNPACK2(x, ...) __TEMPLATE_MACROS__UNPACK2_NOEXPAND(x)

#define __TEMPLATE_MACROS__TYPED_TEMPLATE_PARAMS__TYPED_PARAMETER(templateParamType, templateParamName) templateParamType templateParamName
#define __TEMPLATE_MACROS__TYPED_TEMPLATE_PARAMS__TYPED_PARAMETER_PACKED(pack, ...) __TEMPLATE_MACROS__UNPACK2(PACK(__TEMPLATE_MACROS__TYPED_TEMPLATE_PARAMS__TYPED_PARAMETER)(__TEMPLATE_MACROS__UNPACK(pack)))
#define __TEMPLATE_MACROS__TYPED_TEMPLATE_PARAMS(TemplateParamTypesPack, TemplateParamNamesPack) \
    TERNARY(                                                                                     \
        __TEMPLATE_MACROS__UNPACK(TemplateParamNamesPack),                                       \
        FOR_EACH3(                                                                               \
            __TEMPLATE_MACROS__TYPED_TEMPLATE_PARAMS__TYPED_PARAMETER_PACKED,                    \
            FORWARD(),                                                                           \
            INTERTWINE_PACKS12(TemplateParamTypesPack, TemplateParamNamesPack)                   \
        ),                                                                                       \
                                                                                                 \
    )

#define TEMPLATE_MACROS__TEMPLATE_PREFIX(TemplateParamTypesPack, TemplateParamNamesPack)                     \
    TERNARY2(                                                                                                \
        __TEMPLATE_MACROS__UNPACK(TemplateParamNamesPack),                                                   \
        template <__TEMPLATE_MACROS__TYPED_TEMPLATE_PARAMS(TemplateParamTypesPack, TemplateParamNamesPack)>, \
                                                                                                             \
    )

#define TEMPLATE_MACROS__INLINE(TemplateParamNamesPack) TERNARY(__TEMPLATE_MACROS__UNPACK(TemplateParamNamesPack), inline, )

#define TEMPLATE_MACROS__TEMPLATE_INSTANTIATION(TemplateName, TemplateParamTypesPack, TemplateParamNamesPack) \
    TERNARY(                                                                                                  \
        __TEMPLATE_MACROS__UNPACK(TemplateParamNamesPack),                                                    \
        TemplateName<__TEMPLATE_MACROS__UNPACK(TemplateParamNamesPack)>,                                      \
        TemplateName                                                                                          \
    )

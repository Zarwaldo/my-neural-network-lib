#pragma once

#include <helpers/ForwardMacro.h>
#include <helpers/NArgMacro.h>
#include <helpers/RangeMacro.h>


#define __FOR_EACH__ITERATION_1(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context)
#define __FOR_EACH__ITERATION_2(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH__ITERATION_1(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH__ITERATION_3(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH__ITERATION_2(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH__ITERATION_4(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH__ITERATION_3(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH__ITERATION_5(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH__ITERATION_4(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH__ITERATION_6(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH__ITERATION_5(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH__ITERATION_7(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH__ITERATION_6(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH__ITERATION_8(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH__ITERATION_7(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH__ITERATION_9(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH__ITERATION_8(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH__ITERATION_10(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH__ITERATION_9(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH__ITERATION_11(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH__ITERATION_10(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH__ITERATION_12(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH__ITERATION_11(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH__ITERATION_13(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH__ITERATION_12(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH__ITERATION_14(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH__ITERATION_13(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH__ITERATION_15(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH__ITERATION_14(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH__ITERATION_16(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH__ITERATION_15(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)

#define __FOR_EACH__ITERATION_N_NOEXPAND(NbElements, FUNCTION, separator, context, ...) __FOR_EACH__ITERATION_##NbElements(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH__ITERATION_N(NbElements, FUNCTION, separator, context, ...) __FOR_EACH__ITERATION_N_NOEXPAND(NbElements, FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH(FUNCTION, separator, context, ...) __FOR_EACH__ITERATION_N(NARG(__VA_ARGS__), FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)

#define FOR_EACH(FUNCTION, context, ...) __FOR_EACH(FUNCTION, FORWARD(,), FORWARD(context), __VA_ARGS__)
#define FOR_EACH_SEP(FUNCTION, separator, context, ...) __FOR_EACH(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __JOIN__IDENTITY(x, ...) x
#define JOIN(separator, ...) __FOR_EACH(__JOIN__IDENTITY, FORWARD(separator), FORWARD(), __VA_ARGS__)
#define __REPEAT__CONTEXT(it, context) context
#define REPEAT(text, number) FOR_EACH(__REPEAT__CONTEXT, FORWARD(text), RANGE(number))


#define __FOR_EACH2__ITERATION_1(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context)
#define __FOR_EACH2__ITERATION_2(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH2__ITERATION_1(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH2__ITERATION_3(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH2__ITERATION_2(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH2__ITERATION_4(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH2__ITERATION_3(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH2__ITERATION_5(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH2__ITERATION_4(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH2__ITERATION_6(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH2__ITERATION_5(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH2__ITERATION_7(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH2__ITERATION_6(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH2__ITERATION_8(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH2__ITERATION_7(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH2__ITERATION_9(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH2__ITERATION_8(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH2__ITERATION_10(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH2__ITERATION_9(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH2__ITERATION_11(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH2__ITERATION_10(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH2__ITERATION_12(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH2__ITERATION_11(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH2__ITERATION_13(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH2__ITERATION_12(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH2__ITERATION_14(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH2__ITERATION_13(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH2__ITERATION_15(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH2__ITERATION_14(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH2__ITERATION_16(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH2__ITERATION_15(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)

#define __FOR_EACH2__ITERATION_N_NOEXPAND(NbElements, FUNCTION, separator, context, ...) __FOR_EACH2__ITERATION_##NbElements(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH2__ITERATION_N(NbElements, FUNCTION, separator, context, ...) __FOR_EACH2__ITERATION_N_NOEXPAND(NbElements, FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH2(FUNCTION, separator, context, ...) __FOR_EACH2__ITERATION_N(NARG(__VA_ARGS__), FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)

#define FOR_EACH2(FUNCTION, context, ...) __FOR_EACH2(FUNCTION, FORWARD(,), FORWARD(context), __VA_ARGS__)
#define FOR_EACH_SEP2(FUNCTION, separator, context, ...) __FOR_EACH2(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __JOIN2__IDENTITY(x, ...) x
#define JOIN2(separator, ...) __FOR_EACH2(__JOIN2__IDENTITY, FORWARD(separator), FORWARD(), __VA_ARGS__)
#define __REPEAT2__CONTEXT(it, context) context
#define REPEAT2(text, number) FOR_EACH2(__REPEAT2__CONTEXT, FORWARD(text), RANGE(number))


#define __FOR_EACH3__ITERATION_1(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context)
#define __FOR_EACH3__ITERATION_2(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH3__ITERATION_1(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH3__ITERATION_3(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH3__ITERATION_2(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH3__ITERATION_4(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH3__ITERATION_3(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH3__ITERATION_5(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH3__ITERATION_4(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH3__ITERATION_6(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH3__ITERATION_5(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH3__ITERATION_7(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH3__ITERATION_6(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH3__ITERATION_8(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH3__ITERATION_7(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH3__ITERATION_9(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH3__ITERATION_8(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH3__ITERATION_10(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH3__ITERATION_9(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH3__ITERATION_11(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH3__ITERATION_10(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH3__ITERATION_12(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH3__ITERATION_11(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH3__ITERATION_13(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH3__ITERATION_12(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH3__ITERATION_14(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH3__ITERATION_13(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH3__ITERATION_15(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH3__ITERATION_14(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH3__ITERATION_16(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH3__ITERATION_15(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)

#define __FOR_EACH3__ITERATION_N_NOEXPAND(NbElements, FUNCTION, separator, context, ...) __FOR_EACH3__ITERATION_##NbElements(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH3__ITERATION_N(NbElements, FUNCTION, separator, context, ...) __FOR_EACH3__ITERATION_N_NOEXPAND(NbElements, FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH3(FUNCTION, separator, context, ...) __FOR_EACH3__ITERATION_N(NARG(__VA_ARGS__), FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)

#define FOR_EACH3(FUNCTION, context, ...) __FOR_EACH3(FUNCTION, FORWARD(,), FORWARD(context), __VA_ARGS__)
#define FOR_EACH_SEP3(FUNCTION, separator, context, ...) __FOR_EACH3(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __JOIN3__IDENTITY(x, ...) x
#define JOIN3(separator, ...) __FOR_EACH3(__JOIN3__IDENTITY, FORWARD(separator), FORWARD(), __VA_ARGS__)
#define __REPEAT3__CONTEXT(it, context) context
#define REPEAT3(text, number) FOR_EACH3(__REPEAT3__CONTEXT, FORWARD(text), RANGE(number))


#define __FOR_EACH4__ITERATION_1(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context)
#define __FOR_EACH4__ITERATION_2(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH4__ITERATION_1(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH4__ITERATION_3(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH4__ITERATION_2(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH4__ITERATION_4(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH4__ITERATION_3(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH4__ITERATION_5(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH4__ITERATION_4(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH4__ITERATION_6(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH4__ITERATION_5(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH4__ITERATION_7(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH4__ITERATION_6(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH4__ITERATION_8(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH4__ITERATION_7(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH4__ITERATION_9(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH4__ITERATION_8(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH4__ITERATION_10(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH4__ITERATION_9(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH4__ITERATION_11(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH4__ITERATION_10(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH4__ITERATION_12(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH4__ITERATION_11(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH4__ITERATION_13(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH4__ITERATION_12(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH4__ITERATION_14(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH4__ITERATION_13(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH4__ITERATION_15(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH4__ITERATION_14(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH4__ITERATION_16(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH4__ITERATION_15(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)

#define __FOR_EACH4__ITERATION_N_NOEXPAND(NbElements, FUNCTION, separator, context, ...) __FOR_EACH4__ITERATION_##NbElements(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH4__ITERATION_N(NbElements, FUNCTION, separator, context, ...) __FOR_EACH4__ITERATION_N_NOEXPAND(NbElements, FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH4(FUNCTION, separator, context, ...) __FOR_EACH4__ITERATION_N(NARG(__VA_ARGS__), FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)

#define FOR_EACH4(FUNCTION, context, ...) __FOR_EACH4(FUNCTION, FORWARD(,), FORWARD(context), __VA_ARGS__)
#define FOR_EACH_SEP4(FUNCTION, separator, context, ...) __FOR_EACH4(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __JOIN4__IDENTITY(x, ...) x
#define JOIN4(separator, ...) __FOR_EACH4(__JOIN4__IDENTITY, FORWARD(separator), FORWARD(), __VA_ARGS__)
#define __REPEAT4__CONTEXT(it, context) context
#define REPEAT4(text, number) FOR_EACH4(__REPEAT4__CONTEXT, FORWARD(text), RANGE(number))


#define __FOR_EACH5__ITERATION_1(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context)
#define __FOR_EACH5__ITERATION_2(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH5__ITERATION_1(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH5__ITERATION_3(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH5__ITERATION_2(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH5__ITERATION_4(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH5__ITERATION_3(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH5__ITERATION_5(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH5__ITERATION_4(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH5__ITERATION_6(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH5__ITERATION_5(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH5__ITERATION_7(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH5__ITERATION_6(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH5__ITERATION_8(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH5__ITERATION_7(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH5__ITERATION_9(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH5__ITERATION_8(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH5__ITERATION_10(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH5__ITERATION_9(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH5__ITERATION_11(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH5__ITERATION_10(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH5__ITERATION_12(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH5__ITERATION_11(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH5__ITERATION_13(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH5__ITERATION_12(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH5__ITERATION_14(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH5__ITERATION_13(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH5__ITERATION_15(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH5__ITERATION_14(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH5__ITERATION_16(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH5__ITERATION_15(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)

#define __FOR_EACH5__ITERATION_N_NOEXPAND(NbElements, FUNCTION, separator, context, ...) __FOR_EACH5__ITERATION_##NbElements(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH5__ITERATION_N(NbElements, FUNCTION, separator, context, ...) __FOR_EACH5__ITERATION_N_NOEXPAND(NbElements, FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH5(FUNCTION, separator, context, ...) __FOR_EACH5__ITERATION_N(NARG(__VA_ARGS__), FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)

#define FOR_EACH5(FUNCTION, context, ...) __FOR_EACH5(FUNCTION, FORWARD(,), FORWARD(context), __VA_ARGS__)
#define FOR_EACH_SEP5(FUNCTION, separator, context, ...) __FOR_EACH5(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __JOIN5__IDENTITY(x, ...) x
#define JOIN5(separator, ...) __FOR_EACH5(__JOIN5__IDENTITY, FORWARD(separator), FORWARD(), __VA_ARGS__)
#define __REPEAT5__CONTEXT(it, context) context
#define REPEAT5(text, number) FOR_EACH5(__REPEAT5__CONTEXT, FORWARD(text), RANGE(number))


#define __FOR_EACH6__ITERATION_1(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context)
#define __FOR_EACH6__ITERATION_2(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH6__ITERATION_1(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH6__ITERATION_3(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH6__ITERATION_2(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH6__ITERATION_4(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH6__ITERATION_3(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH6__ITERATION_5(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH6__ITERATION_4(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH6__ITERATION_6(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH6__ITERATION_5(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH6__ITERATION_7(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH6__ITERATION_6(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH6__ITERATION_8(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH6__ITERATION_7(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH6__ITERATION_9(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH6__ITERATION_8(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH6__ITERATION_10(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH6__ITERATION_9(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH6__ITERATION_11(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH6__ITERATION_10(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH6__ITERATION_12(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH6__ITERATION_11(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH6__ITERATION_13(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH6__ITERATION_12(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH6__ITERATION_14(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH6__ITERATION_13(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH6__ITERATION_15(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH6__ITERATION_14(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH6__ITERATION_16(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH6__ITERATION_15(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)

#define __FOR_EACH6__ITERATION_N_NOEXPAND(NbElements, FUNCTION, separator, context, ...) __FOR_EACH6__ITERATION_##NbElements(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH6__ITERATION_N(NbElements, FUNCTION, separator, context, ...) __FOR_EACH6__ITERATION_N_NOEXPAND(NbElements, FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH6(FUNCTION, separator, context, ...) __FOR_EACH6__ITERATION_N(NARG(__VA_ARGS__), FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)

#define FOR_EACH6(FUNCTION, context, ...) __FOR_EACH6(FUNCTION, FORWARD(,), FORWARD(context), __VA_ARGS__)
#define FOR_EACH_SEP6(FUNCTION, separator, context, ...) __FOR_EACH6(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __JOIN6__IDENTITY(x, ...) x
#define JOIN6(separator, ...) __FOR_EACH6(__JOIN6__IDENTITY, FORWARD(separator), FORWARD(), __VA_ARGS__)
#define __REPEAT6__CONTEXT(it, context) context
#define REPEAT6(text, number) FOR_EACH6(__REPEAT6__CONTEXT, FORWARD(text), RANGE(number))


#define __FOR_EACH7__ITERATION_1(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context)
#define __FOR_EACH7__ITERATION_2(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH7__ITERATION_1(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH7__ITERATION_3(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH7__ITERATION_2(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH7__ITERATION_4(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH7__ITERATION_3(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH7__ITERATION_5(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH7__ITERATION_4(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH7__ITERATION_6(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH7__ITERATION_5(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH7__ITERATION_7(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH7__ITERATION_6(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH7__ITERATION_8(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH7__ITERATION_7(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH7__ITERATION_9(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH7__ITERATION_8(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH7__ITERATION_10(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH7__ITERATION_9(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH7__ITERATION_11(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH7__ITERATION_10(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH7__ITERATION_12(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH7__ITERATION_11(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH7__ITERATION_13(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH7__ITERATION_12(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH7__ITERATION_14(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH7__ITERATION_13(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH7__ITERATION_15(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH7__ITERATION_14(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH7__ITERATION_16(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH7__ITERATION_15(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)

#define __FOR_EACH7__ITERATION_N_NOEXPAND(NbElements, FUNCTION, separator, context, ...) __FOR_EACH7__ITERATION_##NbElements(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH7__ITERATION_N(NbElements, FUNCTION, separator, context, ...) __FOR_EACH7__ITERATION_N_NOEXPAND(NbElements, FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH7(FUNCTION, separator, context, ...) __FOR_EACH7__ITERATION_N(NARG(__VA_ARGS__), FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)

#define FOR_EACH7(FUNCTION, context, ...) __FOR_EACH7(FUNCTION, FORWARD(,), FORWARD(context), __VA_ARGS__)
#define FOR_EACH_SEP7(FUNCTION, separator, context, ...) __FOR_EACH7(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __JOIN7__IDENTITY(x, ...) x
#define JOIN7(separator, ...) __FOR_EACH7(__JOIN7__IDENTITY, FORWARD(separator), FORWARD(), __VA_ARGS__)
#define __REPEAT7__CONTEXT(it, context) context
#define REPEAT7(text, number) FOR_EACH7(__REPEAT7__CONTEXT, FORWARD(text), RANGE(number))


#define __FOR_EACH8__ITERATION_1(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context)
#define __FOR_EACH8__ITERATION_2(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH8__ITERATION_1(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH8__ITERATION_3(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH8__ITERATION_2(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH8__ITERATION_4(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH8__ITERATION_3(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH8__ITERATION_5(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH8__ITERATION_4(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH8__ITERATION_6(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH8__ITERATION_5(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH8__ITERATION_7(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH8__ITERATION_6(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH8__ITERATION_8(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH8__ITERATION_7(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH8__ITERATION_9(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH8__ITERATION_8(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH8__ITERATION_10(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH8__ITERATION_9(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH8__ITERATION_11(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH8__ITERATION_10(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH8__ITERATION_12(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH8__ITERATION_11(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH8__ITERATION_13(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH8__ITERATION_12(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH8__ITERATION_14(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH8__ITERATION_13(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH8__ITERATION_15(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH8__ITERATION_14(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH8__ITERATION_16(FUNCTION, separator, context, firstElement, ...) FUNCTION(firstElement, context) separator __FOR_EACH8__ITERATION_15(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)

#define __FOR_EACH8__ITERATION_N_NOEXPAND(NbElements, FUNCTION, separator, context, ...) __FOR_EACH8__ITERATION_##NbElements(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH8__ITERATION_N(NbElements, FUNCTION, separator, context, ...) __FOR_EACH8__ITERATION_N_NOEXPAND(NbElements, FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __FOR_EACH8(FUNCTION, separator, context, ...) __FOR_EACH8__ITERATION_N(NARG(__VA_ARGS__), FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)

#define FOR_EACH8(FUNCTION, context, ...) __FOR_EACH8(FUNCTION, FORWARD(,), FORWARD(context), __VA_ARGS__)
#define FOR_EACH_SEP8(FUNCTION, separator, context, ...) __FOR_EACH8(FUNCTION, FORWARD(separator), FORWARD(context), __VA_ARGS__)
#define __JOIN8__IDENTITY(x, ...) x
#define JOIN8(separator, ...) __FOR_EACH8(__JOIN8__IDENTITY, FORWARD(separator), FORWARD(), __VA_ARGS__)
#define __REPEAT8__CONTEXT(it, context) context
#define REPEAT8(text, number) FOR_EACH8(__REPEAT8__CONTEXT, FORWARD(text), RANGE(number))

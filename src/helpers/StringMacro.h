#pragma once


#define __STRING_NOEXPAND(...) #__VA_ARGS__
#define STRING(...) __STRING_NOEXPAND(__VA_ARGS__)

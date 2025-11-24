#include <gtest/gtest.h>

#include <helpers/IntertwinePacksMacro.h>
#include <helpers/StringMacro.h>


class INTERTWINE_PACKS12Should : public ::testing::Test
{};

#define INTERTWINE_PACKS12Should__SAMPLE_PACK_1 PACK(_1_1, _1_2, _1_3, _1_4)
#define INTERTWINE_PACKS12Should__SAMPLE_PACK_2 PACK(_2_1, _2_2, _2_3, _2_4)
#define INTERTWINE_PACKS12Should__SAMPLE_PACK_3 PACK(_3_1, _3_2, _3_3, _3_4)
#define INTERTWINE_PACKS12Should__SAMPLE_PACK_4 PACK(_4_1, _4_2, _4_3, _4_4)

TEST_F(INTERTWINE_PACKS12Should, intertwine2Packs) {
    // Given 2 packs

    // When we intertwine these packs
    const std::string result = STRING(INTERTWINE_PACKS12(
        INTERTWINE_PACKS12Should__SAMPLE_PACK_1,
        INTERTWINE_PACKS12Should__SAMPLE_PACK_2
    ));

    // Then the result is this pack intertwined
    EXPECT_EQ(result, "PACK(_1_1 , _2_1) , PACK(_1_2 , _2_2) , PACK(_1_3 , _2_3) , PACK(_1_4 , _2_4)");
}

TEST_F(INTERTWINE_PACKS12Should, intertwine3Packs) {
    // Given 3 packs

    // When we intertwine these packs
    const std::string result = STRING(INTERTWINE_PACKS12(
        INTERTWINE_PACKS12Should__SAMPLE_PACK_1,
        INTERTWINE_PACKS12Should__SAMPLE_PACK_2,
        INTERTWINE_PACKS12Should__SAMPLE_PACK_3
    ));

    // Then the result is this pack intertwined
    EXPECT_EQ(result, "PACK(_1_1 , _2_1 , _3_1) , PACK(_1_2 , _2_2 , _3_2) , PACK(_1_3 , _2_3 , _3_3) , PACK(_1_4 , _2_4 , _3_4)");
}

TEST_F(INTERTWINE_PACKS12Should, intertwine4Packs) {
    // Given 4 packs

    // When we intertwine these packs
    const std::string result = STRING(INTERTWINE_PACKS12(
        INTERTWINE_PACKS12Should__SAMPLE_PACK_1,
        INTERTWINE_PACKS12Should__SAMPLE_PACK_2,
        INTERTWINE_PACKS12Should__SAMPLE_PACK_3,
        INTERTWINE_PACKS12Should__SAMPLE_PACK_4
    ));

    // Then the result is this pack intertwined
    EXPECT_EQ(result, "PACK(_1_1 , _2_1 , _3_1 , _4_1) , PACK(_1_2 , _2_2 , _3_2 , _4_2) , PACK(_1_3 , _2_3 , _3_3 , _4_3) , PACK(_1_4 , _2_4 , _3_4 , _4_4)");
}

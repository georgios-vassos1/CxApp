/*
 * bits_test.c — Unit tests for Module 5: Bit Manipulation.
 * TDD RED PHASE — implement bits.c until all tests pass.
 */

#include "bits.h"
#include "../test_framework.h"

/* ================================================================== */
/*  bit_set                                                            */
/* ================================================================== */

static void test_BitSet_SetsLSB(void)
{
    EXPECT_EQ(bit_set(0u, 0), 1u);
}

static void test_BitSet_SetsMSB(void)
{
    EXPECT_EQ(bit_set(0u, 31), 0x80000000u);
}

static void test_BitSet_AlreadySetIsIdempotent(void)
{
    EXPECT_EQ(bit_set(0xFFFFFFFFu, 7), 0xFFFFFFFFu);
}

static void test_BitSet_DoesNotAffectOtherBits(void)
{
    /* Set bit 4 in 0b00000001 → 0b00010001 */
    EXPECT_EQ(bit_set(0x01u, 4), 0x11u);
}

/* ================================================================== */
/*  bit_clear                                                          */
/* ================================================================== */

static void test_BitClear_ClearsLSB(void)
{
    EXPECT_EQ(bit_clear(0xFFFFFFFFu, 0), 0xFFFFFFFEu);
}

static void test_BitClear_ClearsMSB(void)
{
    EXPECT_EQ(bit_clear(0xFFFFFFFFu, 31), 0x7FFFFFFFu);
}

static void test_BitClear_AlreadyClearIsIdempotent(void)
{
    EXPECT_EQ(bit_clear(0u, 5), 0u);
}

/* ================================================================== */
/*  bit_toggle                                                         */
/* ================================================================== */

static void test_BitToggle_ZeroToOne(void)
{
    EXPECT_EQ(bit_toggle(0u, 3), 8u);
}

static void test_BitToggle_OneToZero(void)
{
    EXPECT_EQ(bit_toggle(8u, 3), 0u);
}

static void test_BitToggle_TwiceIsIdentity(void)
{
    uint32_t x = 0xABCDEF01u;
    EXPECT_EQ(bit_toggle(bit_toggle(x, 15), 15), x);
}

/* ================================================================== */
/*  bit_get                                                            */
/* ================================================================== */

static void test_BitGet_ReturnsOne(void)
{
    EXPECT_EQ(bit_get(0xFFFFFFFFu, 0),  1);
    EXPECT_EQ(bit_get(0x80000000u, 31), 1);
}

static void test_BitGet_ReturnsZero(void)
{
    EXPECT_EQ(bit_get(0u, 0),  0);
    EXPECT_EQ(bit_get(0u, 31), 0);
}

static void test_BitGet_SpecificPosition(void)
{
    /* 0b1010 = 10: bit 1 = 1, bit 0 = 0 */
    EXPECT_EQ(bit_get(0b1010u, 1), 1);
    EXPECT_EQ(bit_get(0b1010u, 0), 0);
}

/* ================================================================== */
/*  bit_count (popcount)                                               */
/* ================================================================== */

static void test_BitCount_Zero(void)       { EXPECT_EQ(bit_count(0u), 0); }
static void test_BitCount_One(void)        { EXPECT_EQ(bit_count(1u), 1); }
static void test_BitCount_AllOnes(void)    { EXPECT_EQ(bit_count(0xFFFFFFFFu), 32); }
static void test_BitCount_PowerOfTwo(void) { EXPECT_EQ(bit_count(256u), 1); }
static void test_BitCount_Alternating(void)
{
    /* 0xAAAAAAAA = 10101010...  → 16 set bits */
    EXPECT_EQ(bit_count(0xAAAAAAAAu), 16);
}

/* ================================================================== */
/*  bit_is_pow2                                                        */
/* ================================================================== */

static void test_BitIsPow2_Zero(void)        { EXPECT_FALSE(bit_is_pow2(0u)); }
static void test_BitIsPow2_One(void)         { EXPECT_TRUE(bit_is_pow2(1u)); }
static void test_BitIsPow2_PowersOfTwo(void)
{
    EXPECT_TRUE(bit_is_pow2(2u));
    EXPECT_TRUE(bit_is_pow2(64u));
    EXPECT_TRUE(bit_is_pow2(0x80000000u));
}
static void test_BitIsPow2_NonPowers(void)
{
    EXPECT_FALSE(bit_is_pow2(3u));
    EXPECT_FALSE(bit_is_pow2(6u));
    EXPECT_FALSE(bit_is_pow2(0xFFFFFFFFu));
}

/* ================================================================== */
/*  bit_next_pow2                                                      */
/* ================================================================== */

static void test_BitNextPow2_Zero(void)     { EXPECT_EQ(bit_next_pow2(0u), 1u); }
static void test_BitNextPow2_One(void)      { EXPECT_EQ(bit_next_pow2(1u), 1u); }
static void test_BitNextPow2_ExactPow(void) { EXPECT_EQ(bit_next_pow2(8u), 8u); }
static void test_BitNextPow2_NonPow(void)
{
    EXPECT_EQ(bit_next_pow2(5u),  8u);
    EXPECT_EQ(bit_next_pow2(9u),  16u);
    EXPECT_EQ(bit_next_pow2(33u), 64u);
}

/* ================================================================== */
/*  bit_extract                                                        */
/* ================================================================== */

static void test_BitExtract_LowNibble(void)
{
    /* Extract bits [0,4): 0b11010110 → low 4 bits = 0b0110 = 6 */
    EXPECT_EQ(bit_extract(0b11010110u, 0, 4), 0b0110u);
}

static void test_BitExtract_HighNibble(void)
{
    /* Extract bits [4,8): 0b11010110 → bits 4-7 = 0b1101 = 13 */
    EXPECT_EQ(bit_extract(0b11010110u, 4, 4), 0b1101u);
}

static void test_BitExtract_SingleBit(void)
{
    EXPECT_EQ(bit_extract(0b1010u, 1, 1), 1u);
    EXPECT_EQ(bit_extract(0b1010u, 0, 1), 0u);
}

static void test_BitExtract_FullWord(void)
{
    EXPECT_EQ(bit_extract(0xDEADBEEFu, 0, 32), 0xDEADBEEFu);
}

/* ================================================================== */
/*  Flags                                                              */
/* ================================================================== */

static void test_FlagsAllSet_AllPresent(void)
{
    uint8_t field = FLAG_READ | FLAG_WRITE;
    EXPECT_TRUE(flags_all_set(field, FLAG_READ | FLAG_WRITE));
}

static void test_FlagsAllSet_OneMissing(void)
{
    uint8_t field = FLAG_READ;
    EXPECT_FALSE(flags_all_set(field, FLAG_READ | FLAG_WRITE));
}

static void test_FlagsAnySet_OnePresent(void)
{
    uint8_t field = FLAG_EXEC;
    EXPECT_TRUE(flags_any_set(field, FLAG_READ | FLAG_EXEC));
}

static void test_FlagsAnySet_NonePresent(void)
{
    uint8_t field = FLAG_READ;
    EXPECT_FALSE(flags_any_set(field, FLAG_WRITE | FLAG_EXEC));
}

/* ================================================================== */
/*  Main                                                               */
/* ================================================================== */

int main(void)
{
    printf("C Fundamentals  M05 — Bit Manipulation\n");
    printf("TDD Red Phase: non-trivial tests are expected to FAIL\n");
    printf("========================================\n\n");

    printf("bit_set\n");
    printf("-------\n");
    RUN_TEST(test_BitSet_SetsLSB);
    RUN_TEST(test_BitSet_SetsMSB);
    RUN_TEST(test_BitSet_AlreadySetIsIdempotent);
    RUN_TEST(test_BitSet_DoesNotAffectOtherBits);

    printf("\nbit_clear\n");
    printf("---------\n");
    RUN_TEST(test_BitClear_ClearsLSB);
    RUN_TEST(test_BitClear_ClearsMSB);
    RUN_TEST(test_BitClear_AlreadyClearIsIdempotent);

    printf("\nbit_toggle\n");
    printf("----------\n");
    RUN_TEST(test_BitToggle_ZeroToOne);
    RUN_TEST(test_BitToggle_OneToZero);
    RUN_TEST(test_BitToggle_TwiceIsIdentity);

    printf("\nbit_get\n");
    printf("-------\n");
    RUN_TEST(test_BitGet_ReturnsOne);
    RUN_TEST(test_BitGet_ReturnsZero);
    RUN_TEST(test_BitGet_SpecificPosition);

    printf("\nbit_count\n");
    printf("---------\n");
    RUN_TEST(test_BitCount_Zero);
    RUN_TEST(test_BitCount_One);
    RUN_TEST(test_BitCount_AllOnes);
    RUN_TEST(test_BitCount_PowerOfTwo);
    RUN_TEST(test_BitCount_Alternating);

    printf("\nbit_is_pow2\n");
    printf("-----------\n");
    RUN_TEST(test_BitIsPow2_Zero);
    RUN_TEST(test_BitIsPow2_One);
    RUN_TEST(test_BitIsPow2_PowersOfTwo);
    RUN_TEST(test_BitIsPow2_NonPowers);

    printf("\nbit_next_pow2\n");
    printf("-------------\n");
    RUN_TEST(test_BitNextPow2_Zero);
    RUN_TEST(test_BitNextPow2_One);
    RUN_TEST(test_BitNextPow2_ExactPow);
    RUN_TEST(test_BitNextPow2_NonPow);

    printf("\nbit_extract\n");
    printf("-----------\n");
    RUN_TEST(test_BitExtract_LowNibble);
    RUN_TEST(test_BitExtract_HighNibble);
    RUN_TEST(test_BitExtract_SingleBit);
    RUN_TEST(test_BitExtract_FullWord);

    printf("\nflags\n");
    printf("-----\n");
    RUN_TEST(test_FlagsAllSet_AllPresent);
    RUN_TEST(test_FlagsAllSet_OneMissing);
    RUN_TEST(test_FlagsAnySet_OnePresent);
    RUN_TEST(test_FlagsAnySet_NonePresent);

    return TEST_SUMMARY();
}

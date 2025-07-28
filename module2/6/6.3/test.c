#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "math.h"



void test_sum_pos(void){ CU_ASSERT(sum(2,3) == 5);}
void test_sum_neg(void){ CU_ASSERT(sum(-2,-3) == -5);}

void test_sub_pos(void){CU_ASSERT(sub(3,2) == 1);}
void test_sub_neg(void){CU_ASSERT(sub(2,3) == -1);}

void test_mul_pos(void){CU_ASSERT(mul(2,3) == 6);}
void test_mul_neg(void){CU_ASSERT(mul(-2,3) == -6);}

void test_del_pos(void){CU_ASSERT(del(12,2) == 6);}
void test_del_neg(void){CU_ASSERT(del(-12,2) == -6);}
void test_del_zero_a(void){CU_ASSERT(del(0,2) == 0);}
void test_del_zero_b(void){CU_ASSERT(del(2,0) == 0);}

void test_mod_a(void){CU_ASSERT(mod(12,2) == 0);}
void test_mod_b(void){CU_ASSERT(mod(2,12) == 2);}

int main(){
    if(CUE_SUCCESS != CU_initialize_registry())return CU_get_error();
    CU_pSuite pSuite = NULL;

    pSuite = CU_add_suite("Suite_1",0,0);

    CU_add_test(pSuite,"test : add : a + b",test_sum_pos);
    CU_add_test(pSuite,"test : add : -a + (-b)",test_sum_neg);
    CU_add_test(pSuite,"test : sub : a - b",test_sub_pos);
    CU_add_test(pSuite,"test : sub : -a - b",test_sub_neg);
    CU_add_test(pSuite,"test : mul : a * b",test_mul_pos);
    CU_add_test(pSuite,"test : mul : -a * b",test_mul_neg);
    CU_add_test(pSuite,"test : del : a / b",test_del_pos);
    CU_add_test(pSuite,"test : del : - a / b",test_del_neg);
    CU_add_test(pSuite,"test : del : a / b (a = 0)",test_del_zero_a);
    CU_add_test(pSuite,"test : del : a / b (b = 0)",test_del_zero_b);
    CU_add_test(pSuite,"test : del : a % b (a > b)",test_mod_a);
    CU_add_test(pSuite,"test : del : a % b (b > a)",test_mod_b);


    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}

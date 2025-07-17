#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "contact.h"
#include "book.h"
#include <string.h>

// Tests : contact_t

// for some reason b is used a global variable so you have to delete elements to pass tests

void test_empty_c(void){
    contact_t c;
    char *firstname = "Peter";
    char *lastname = "Griffin";
    char *workplace = "Griffins";
    char *role = "Janitor";
    char *tg = "peter.tg";
    char *ig = "peter.ig";
    char *vk = "peter.vk";

    c = init_c(1,7,firstname,lastname,workplace,role,tg,ig,vk);
    c = empty_c(c);

    CU_ASSERT_EQUAL(c.id,0);
    CU_ASSERT_STRING_EQUAL(c.name.firstname,"");
    CU_ASSERT_STRING_EQUAL(c.name.lastname,"");
    CU_ASSERT_STRING_EQUAL(c.job.workplace,"");
    CU_ASSERT_STRING_EQUAL(c.job.role,"");
    CU_ASSERT_STRING_EQUAL(c.socials.tg,"");
    CU_ASSERT_STRING_EQUAL(c.socials.ig,"");
    CU_ASSERT_STRING_EQUAL(c.socials.vk,"");
}

void test_init_c_mandatory(void){
    contact_t c;
    char *firstname = "Peter";
    char *lastname = "Griffin";

    c = empty_c(c);
    c = init_c(0,2,firstname,lastname);

    CU_ASSERT_EQUAL(c.id,0);
    CU_ASSERT_STRING_EQUAL(c.name.firstname,"Peter");
    CU_ASSERT_STRING_EQUAL(c.name.lastname,"Griffin");
}

void test_init_c_full(void){
    contact_t c;
    char *firstname = "Peter";
    char *lastname = "Griffin";
    char *workplace = "Griffins";
    char *role = "Janitor";
    char *tg = "peter.tg";
    char *ig = "peter.ig";
    char *vk = "peter.vk";

    c = empty_c(c);
    c = init_c(1,7,firstname,lastname,workplace,role,tg,ig,vk);
    
    CU_ASSERT_EQUAL(c.id,1);
    CU_ASSERT_STRING_EQUAL(c.name.firstname,"Peter");
    CU_ASSERT_STRING_EQUAL(c.name.lastname,"Griffin");
    CU_ASSERT_STRING_EQUAL(c.job.workplace,"Griffins");
    CU_ASSERT_STRING_EQUAL(c.job.role,"Janitor");
    CU_ASSERT_STRING_EQUAL(c.socials.tg,"peter.tg");
    CU_ASSERT_STRING_EQUAL(c.socials.ig,"peter.ig");
    CU_ASSERT_STRING_EQUAL(c.socials.vk,"peter.vk");
}


// Tests : book_t

void test_add_c(void){
    book_t b;
    contact_t c1 = init_c(1,2,"Peter","Griffin");
    contact_t c2 = init_c(2,2,"Lois","Griffin");
    b = add_c(b,c1);
    b = add_c(b,c2);
    CU_ASSERT_EQUAL(b.size,2);
    b = delete_c(b,1);
    b = delete_c(b,2);
}

void test_delete_c(void){
    book_t b;
    contact_t c1 = init_c(1,2,"Peter","Griffin");
    contact_t c2 = init_c(2,2,"Lois","Griffin");
    b = add_c(b,c1);
    b = add_c(b,c2);
    b = delete_c(b,2);
    CU_ASSERT_EQUAL(b.size,1);
}

void test_edit_c(void){
    book_t b;
    b.size = 0;
    contact_t c1 = init_c(1,2,"Peter","Griffin");
    contact_t c2 = init_c(2,2,"Lois","Griffin");
    b = add_c(b,c1);
    b = edit_c(b,c2,1);
    
    CU_ASSERT_EQUAL(b.contacts[0].id,2);
    CU_ASSERT_STRING_EQUAL(b.contacts[0].name.firstname,"Lois");
    CU_ASSERT_STRING_EQUAL(b.contacts[0].name.lastname,"Griffin");
}

int main(){
    if(CUE_SUCCESS != CU_initialize_registry())return CU_get_error();
    CU_pSuite pSuite = NULL;

    pSuite = CU_add_suite("Suite_1",0,0);

    CU_add_test(pSuite,"test : contact_t : empty_c",test_empty_c);
    CU_add_test(pSuite,"test : contact_t : init_c - mandatory params",test_init_c_mandatory);
    CU_add_test(pSuite,"test : contact_t : init_c - full params",test_init_c_full);
    CU_add_test(pSuite,"test : book_t: add_c",test_add_c);
    CU_add_test(pSuite,"test : book_t: delete_c",test_delete_c);
    CU_add_test(pSuite,"test : book_t: edit_c",test_edit_c);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
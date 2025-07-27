#include <stdio.h>
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include "display.h"
#include "prompt.h"
#include "contact.h"

void menu_operation(char);
void menu_insert();
void menu_edit();
void menu_rm();
void menu_view();


tree_t tree;

int main(){
    char action;
    printf("Contact Book App : Binary Tree\n");
    printf("Controls : i(nser) e(dit) r(emove) v(iew) b(alance) q(uit)\n");
    while (1){
        printf("?:");
        action = getchar();
        if(action == 'q')break;
        menu_operation(action);
    }
    
    return 0;
}

void menu_operation(char action){
    if(action == 'i' || action == 'I') menu_insert();
    if(action == 'e' || action == 'E') menu_edit();
    if(action == 'r' || action == 'R') menu_rm();
    if(action == 'b' || action == 'B') balance_tree(&tree);
    if(action == 'v' || action == 'V') menu_view();
}

void menu_insert(){
    printf("Insert operation:\n");
    getchar();
    insert(&tree,prompt_create_c());
}

void menu_edit(){
    int id;
    printf("Edit at id:\n");
    getchar();
    printf("id=");
    scanf("%d",&id);
    prompt_edit_c(search(&tree,id));
}

void menu_rm(){
    int id;
    printf("Delete at id:\n");
    getchar();
    printf("id=");
    scanf("%d",&id);
    if(prompt_remove_c(search(&tree,id)))delete(&tree,id);
}

void menu_view(){
    char action;
    printf("View options:\n");
    printf("1.View as a list 2.Ordered view : ");
    action = getchar();
    scanf("%c",&action);
    if(action == '1')print_in_order(&tree);
    if(action == '2')display_tree(&tree);
}
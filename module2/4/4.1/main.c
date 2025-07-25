#include <stdio.h>
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include "display.h"
#include "prompt.h"

list_t list;

int main(){
    char action;
    printf("Contact Book App : Double Linked List\n");
    printf("Controls : a(dd) e(dit) r(emove) v(iew)\n");
    while (1){
        action = getchar();
        menu_operation(action);
    }
    
    return 0;
}

void menu_operation(char action){
    if(action == "a" || action == "A") menu_add();
    if(action == "e" || action == "E") menu_edit();
    if(action == "r" || action == "R") menu_rm();
    if(action == "v" || action == "V") display_list(&list); 
}


void menu_add(){
    char action;
    int pos;
    
    printf("Add operation options:\n");
    printf("1.Add first\n");
    printf("2.Add last\n");
    printf("3.Add pos\n");
    
    action = getchar();

    if(action == "1") add_first(&list,prompt_create_c());
    
    if(action == "2") add_last(&list,prompt_create_c());

    if(action == "3"){
        printf("pos=");
        scanf("%d",&pos);
        add_at(&list,prompt_create_c(),pos);
    }
}

void menu_edit(){
    char action;
    int pos;
    
    printf("Edit operation options:\n");
    printf("1.Edit first\n");
    printf("2.Edit last\n");
    printf("3.Edit pos\n");
    
    action = getchar();

    if(action == "1") prompt_edit_c(get_first(&list));
    
    if(action == "2") prompt_edit_c(get_last(&list));

    if(action == "3"){
        printf("pos=");
        scanf("%d",&pos);
        prompt_edit_c(get_at(&list,pos));
    }
}

void menu_rm(){
    char action;
    int pos;
    
    printf("Remove operation options:\n");
    printf("1.Remove first\n");
    printf("2.Remove last\n");
    printf("3.Remove pos\n");
    
    action = getchar();

    if(action == "1"){
        if(prompt_remove_c(get_first(&list))){
            delete_first(&list);
        }
    }
    
    if(action == "2"){
        if(prompt_remove_c(get_last(&list))){
            delete_last(&list);
        }
    }

    if(action == "3"){
        printf("pos=");
        scanf("%d",&pos);
        if(prompt_remove_c(get_at(&list,pos))){
            delete_at(&list,pos);
        }
    }
}

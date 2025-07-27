#include <stdio.h>
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include "display.h"
#include "prompt.h"
#include "contact.h"

void menu_add(void);
void menu_edit(void);
void menu_rm(void);
void menu_sort(void);
void menu_operation(char);
    
list_t list;

int main(){
    char action;
    printf("Contact Book App : Double Linked List\n");
    printf("Controls : a(dd) e(dit) r(emove) v(iew) s(ort) q(uit)\n");
    while (1){
        printf("?:");
        action = getchar();
        if(action == 'q')break;
        menu_operation(action);
    }
    
    return 0;
}

void menu_operation(char action){
    if(action == 'a' || action == 'A') menu_add();
    if(action == 'e' || action == 'E') menu_edit();
    if(action == 'r' || action == 'R') menu_rm();
    if(action == 's' || action == 's') menu_sort();
    if(action == 'v' || action == 'V') display_list(&list);
}

void menu_add(){
    char action;
    int pos;
    
    printf("Add operation options:\n");
    printf("1.Add first 2.Add last 3.Add pos : ");
    
    action = getchar();
    scanf("%c",&action);

    if(action == '1') add_first(&list,prompt_create_c());
    
    if(action == '2') add_last(&list,prompt_create_c());

    if(action == '3'){
        printf("pos=");
        scanf("%d",&pos);
        add_at(&list,prompt_create_c(),pos);
    }
}

void menu_edit(){
    char action;
    int pos;
    
    printf("Edit operation options:\n");
    printf("1.Edit first 2.Edit last 3.Edit pos : ");
    
    action = getchar();
    scanf("%c",&action);

    if(action == '1') prompt_edit_c(get_first(&list));
    
    if(action == '2') prompt_edit_c(get_last(&list));

    if(action == '3'){
        printf("pos=");
        scanf("%d",&pos);
        prompt_edit_c(get_at(&list,pos));
    }
}

void menu_rm(){
    char action;
    int pos;
    
    printf("Remove operation options:\n");
    printf("1.Remove first 2.Remove last 3.Remove pos : ");

    action = getchar();
    scanf("%c",&action);

    if(action == '1'){
        if(prompt_remove_c(get_first(&list))){
            delete_first(&list);
        }
    }
    
    if(action == '2'){
        if(prompt_remove_c(get_last(&list))){
            delete_last(&list);
        }
    }

    if(action == '3'){
        printf("pos=");
        scanf("%d",&pos);
        if(prompt_remove_c(get_at(&list,pos))){
            delete_at(&list,pos);
        }
    }
}

void menu_sort(){
    char action;
    printf("Sort operation options:\n");
    printf("1.Sort by name 2.Sort by surname 3.Sort by workplace 4.Sort by role 5.Sort by e-mail : ");
    action = getchar();
    scanf("%c",&action);
    if(action == '1')sort_list(&list,compare_by_name);
    if(action == '2')sort_list(&list,compare_by_surname);
    if(action == '3')sort_list(&list,compare_by_workplace);
    if(action == '4')sort_list(&list,compare_by_role);
    if(action == '5')sort_list(&list,compare_by_email);
}
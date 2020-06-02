//
//  main.c
//  22
//
//  Created by 阿騰 on 2020/5/22.
//  Copyright © 2020 阿騰. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define OP_SIZE 11 //op_code陣列大小空間 Opcode space size
struct op_code //存op_code_空間-star
{
    char op_name[10];//op 名稱
    char op_m[5];//記憶體
    char op_format[5];//格式
    char op_cod[3];//實際的op碼
    struct op_code *next;//假如赫旭超過 預設為NULL
}op_code[OP_SIZE];//存op_code_空間-end 大小為OP_SIZE
struct srcpro{
 char symname[11]; //symbom name
 char exformat; //extent format-4
 char opcode[10]; //Opcode
 char optag; // (#, @, =
 char optr_1[10]; //op-1
 char optr; //+ - * /
 char optr_2[10]; //op-2
 int address;
// Use use_block;
 char str[50];
 char destination[20];
 struct srcpro *next;
};
unsigned int Hash(char* str)//赫序加起來-star
{
    long int hash=0;
    int i;
    for(i = 0;str[i]; i++)
    {
        
        if (str[i]!=' '&&str[i]!='+') {//為了讓讀入不會因為空格跟+號出錯
            hash*=10;
            hash += str[i];
        }
        
    }
    return hash%OP_SIZE;
}//赫序加起來-end
void init_op_cod_arr()//初始化全域的op_code陣列-star
{
    int i;
    for (i=0;i<OP_SIZE;i++) {
        op_code[i].next=NULL;
        strcpy(op_code[i].op_name, "NULL");
    }
}//初始化全域的op_code陣列-end
struct op_code *create_note()
{
    struct op_code * ptr;
    ptr = malloc(sizeof(struct op_code));
    ptr->next =NULL;
    return ptr;
}
void red_op_code()//讀op_code-star
{
    FILE *fp_r = fopen("OP_code.txt", "r");
    char reg1[100];
    char *substr = NULL;//暫存分割字串
    struct op_code *ptr; //存目前赫旭陣列存放位置
    if (fp_r == NULL)
    {
        printf("------------讀取op_code錯誤-----------");
        return ;
    }
    while(!feof(fp_r)) {//讀取資料
        fscanf(fp_r, "%s", &reg1);
        substr=strtok(reg1,",");//這裡取出了第一個也就是OP_Name
        //        printf("%d\n",Hash(substr));//現在處理赫旭
        int opNumber = Hash(substr);//放赫旭
        ptr = &op_code[opNumber];//將赫旭出來的陣列位置存下
        if (strcmp(ptr->op_name, "NULL")==0) {
            strcpy(ptr->op_name, substr);
        } else {
            while (ptr->next != NULL) {
                ptr = ptr->next;
            }
            ptr->next=create_note();
            ptr=ptr->next;
            strcpy(ptr->op_name, substr);
        }
        substr = strtok(NULL, ",");//切割m
        strcpy(ptr->op_m, substr);
        substr = strtok(NULL, ",");//切割格式
        strcpy(ptr->op_format, substr);
        substr = strtok(NULL, ",");//切割op_code
        strcpy(ptr->op_cod, substr);
    }
    fclose(fp_r);
}//讀op_code-end
void test_print_op_code()//測試用看op_code對不對-start
{
    int i;
    struct op_code *ptr;
    for (i=0; i<OP_SIZE; i++) {
        ptr = &op_code[i];
        if (strcmp(ptr->op_name, "NULL")!=0) {
            do {
                printf("%s %s %s %s\n",ptr->op_name,ptr->op_m,ptr->op_format,ptr->op_cod);
                if (ptr->next!=NULL) {
                    ptr = ptr->next;
                }
                else
                {
                    break;
                }
            } while (&free);
            
        }
    }
}//測試用看op_code對不對-end
void red_srcpro()//讀題目-start
{
    char reg1[100];
    FILE *fp_r = fopen("srcpro.txt", "r");
    
    while(!feof(fp_r)){//讀取資料
    fgets(&reg1,100, fp_r);
        reg1[6]=';';//我打算用;來分隔
    printf("%s",reg1);
        
    }
    fclose(fp_r);
}//讀題目-end
int main(){
    char reg1[100], reg2[100], reg3[100];
    FILE *fp_w = fopen("data_out.txt", "w");
    init_op_cod_arr();
    red_op_code();
//    red_srcpro();
    printf("%d  %d",Hash("STL  "),Hash("STL"));
//    test_print_op_code();//測試print_opOCD
    if (fp_w == NULL)
        return -1;
    
    fclose(fp_w);
}

//
//  main.c
//  22
//
//  Created by 阿騰 on 2020/5/22.
//  Copyright © 2020 阿騰. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#define OP_SIZE 11 //op_code陣列大小空間 Opcode space size
struct op_code //存op_code_空間-star
{
    char op_name[10];//op 名稱
    char op_m[5];//記憶體
    int op_format;//格式
    int op_cod;//實際的op碼
    struct op_code *next;//假如赫旭超過 預設為NULL
}op_code[OP_SIZE];//存op_code_空間-end 大小為OP_SIZE

unsigned int Hash(char* str)//赫序加起來-star
{
    long int hash=0;
    int i;
   for(i = 0;str[i]; i++)
   {
       hash*=10;
       hash += str[i];
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
        if (strcmp(op_code[opNumber].op_name, "NULL")==0) {
            ptr = &op_code[opNumber];//將赫旭出來的陣列位置存下
            strcpy(ptr->op_name, "123");
        } else {
            
        }
        do {//字串分割
            substr = strtok(NULL, ",");
           } while (substr);
         printf("----%s\n", reg1);
    //     fprintf(fp_w, "%s\n",reg1);
       }
    fclose(fp_r);
}//讀op_code-end
int main(){
char reg1[100], reg2[100], reg3[100];
   FILE *fp_w = fopen("data_out.txt", "w");
    init_op_cod_arr();
    red_op_code();

   if (fp_w == NULL)
     return -1;

   fclose(fp_w);
 }

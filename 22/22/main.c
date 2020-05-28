//
//  main.c
//  22
//
//  Created by 阿騰 on 2020/5/22.
//  Copyright © 2020 阿騰. All rights reserved.
//

#include <stdio.h>
#include <string.h>
struct op_code
{
    char op_name[10];
    char op_m[5];
    int op_format;
    int op_cod;
    struct op_code *next;
}op_code[11];
void red_op_code()
{
    FILE *fp_r = fopen("OP_code.txt", "r");
    char reg1[100];
    if (fp_r == NULL)
    {
        printf("------------讀取op_code錯誤-----------");
        return ;
    }
    while(!feof(fp_r)) {
         fscanf(fp_r, "%s", &reg1);
        
         printf("%s\n", reg1);
    //     fprintf(fp_w, "%s\n",reg1);
       }
    fclose(fp_r);
}
int main(){
char reg1[100], reg2[100], reg3[100];
   FILE *fp_w = fopen("data_out.txt", "w");
    red_op_code();

   if (fp_w == NULL)
     return -1;

   fclose(fp_w);
 }

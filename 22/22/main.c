//
//  main.c
//  22
//
//  Created by 阿騰 on 2020/5/22.
//  Copyright © 2020 阿騰. All rights reserved.
//
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define OP_SIZE 11 //op_code陣列大小空間 Opcode space size
#define MAX_Srcpro_SIZE 100 //指令行數的上限
int Srcpro_size=0;
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
    bool exformat; //extent format-4
    char opcode[10]; //Opcode
    char optag; // (#, @, =
    char optr_1[10]; //op-1
    char optr; //+ - * /
    char optr_2[10]; //op-2
    int address;
}save_srcpro[MAX_Srcpro_SIZE];

unsigned int Hash(char* str)//赫序加起來-star
{
    long int hash=0;
    int i;
    for(i = 0;str[i]; i++)
    {
        if (str[i]!=' '&&str[i]!='+'&&str[i]!='#'&&str[i]!='@'&&str[i]!='='&&str[i]!='\n') {//為了讓讀入不會因為空格跟+號和#@=\n
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
    ptr = (struct op_code *)malloc(sizeof(struct op_code));
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
    char reg1[100],*substr=NULL;
    FILE *fp_r = fopen("srcpro.txt", "r");
    int i = 0 ;
    while(!feof(fp_r)){//讀取資料.開始
        fgets(&reg1,100, fp_r);
        
        if(reg1[strlen(reg1) - 1] == '\n')
        reg1[strlen(reg1) - 1] = '\0';
        
        //處理分割.開始
        if (reg1[6]==' ') {
            reg1[6]=';';//我打算用;來分隔 測資輸入為第七個是分隔空白
        }
        if (reg1[14]==' ') {
            reg1[14]=';';//以及第14個為分隔空白
        }
        if (reg1[24]!='\0') {//利用sprint來插入字串
            char temap[20];
            strcpy(temap, &reg1[24]);//先暫存
            sprintf(&reg1[24], "%s%s",";",temap);
        }
        substr = strtok(reg1, ";");//切割符號
        //處理分割.結束
        
        if (substr!=NULL) {//第一段
            strcpy(save_srcpro[i].symname, substr);
            substr = strtok(NULL, ";");//切割符號
        }//第一段
       
        if (substr!=NULL) {//第二段
            if (substr[0]=='+') {
                save_srcpro[i].exformat=true;
            }
            strcpy(save_srcpro[i].opcode, substr);
            substr = strtok(NULL, ";");//切割符號
        }//第二段
        if (substr!=NULL) {//第三段
            save_srcpro[i].optag=substr[0];
            
            strcpy(save_srcpro[i].optr_1, &substr[1]);
            substr = strtok(NULL, ";");//切割符號
        }//第三段
        if (substr!=NULL) {//第四段
            save_srcpro[i].optr=substr[0];
            strcpy(save_srcpro[i].optr_2, &substr[1]);
        }//第四段
        i++;
    }//讀取資料.結束
    fclose(fp_r);
    Srcpro_size = i;
}//讀題目-end
void init_red_srcpro()//初始化以防意外
{
    int i;
    for (i=0; i<MAX_Srcpro_SIZE; i++) {
        save_srcpro[i].exformat=false;
    }
}
void test_print_srcpro()//測試輸出用-不重要
{
    int i;
    for (i=0; i<Srcpro_size-1; i++) {
        printf("%2d ",i);
        char *temp;
        
        temp = save_srcpro[i].symname;
        if (temp!=NULL) {
            printf("%s",temp);
        }
        temp = save_srcpro[i].opcode;
        if (temp!=NULL) {
            printf("%s",temp);
        }
//        printf("%d",save_srcpro[i].exformat);//測試加號有沒有進
        temp = save_srcpro[i].optr_1;
        if (temp!=NULL) {
            printf("%s",temp);
        }
        if (save_srcpro[i].optr!=NULL) {
            printf("%c",save_srcpro[i].optr);
        }
        temp = save_srcpro[i].optr_2;
        if (temp!=NULL) {
//            printf("%c",save_srcpro[i].optr);
            printf("%s",temp);
        }
        printf("\n");
    }
}
int main(){
    char reg1[100], reg2[100], reg3[100];
    FILE *fp_w = fopen("data_out.txt", "w");
    init_op_cod_arr();
    red_op_code();
//    test_print_op_code();//測試print_opOCD
    init_red_srcpro();
    red_srcpro();
    //    printf("%d  %d",Hash("STL  "),Hash("STL"));赫緒測試
    test_print_srcpro();
    if (fp_w == NULL)
        return -1;
    
    fclose(fp_w);
}

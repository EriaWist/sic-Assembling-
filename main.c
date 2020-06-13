
//
//  main.c
//  22
//
//  Created by 阿騰 on 2020/5/22.
//  Copyright c 2020 阿騰. All rights reserved.
//
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HASH_SIZE 11 //op_code陣列大小空間 Opcode space size
#define MAX_Srcpro_SIZE 100 //指令行數的上限
#define MAX_block_locctr_SIZE 100//use空間表
int Srcpro_size=0;
int BASE=0;
char regs_name[][4] = {"A","X","L","B","S","T","F","","PC","SW"};
int all=0;
struct obj_arr_address
{
    int address_int;
    char address_char[10];


}address_arr[MAX_Srcpro_SIZE];
struct block_locctr//存use空間
{
    char block_name[10];
    int address;
} block_locctr_arrary[MAX_block_locctr_SIZE];
struct LTORG//常數表
{
    int address;
    struct LTORG *next;
    char name[10];
    int isltorg;
    int use;
    char content[100];
} LTORG_Arr[HASH_SIZE];
struct symname
{
    int address;
    char name[10];
    int use;
    struct symname *next;
    int content;//內容假如有
} symname_arr[HASH_SIZE];
struct op_code //存op_code_空間-star
{
    char op_name[10];//op 名稱
    char op_m[10];//記憶體
    char op_format[5];//格式
    char op_cod[3];//實際的op碼
    int op_cod_int;
    struct op_code *next;//假如赫旭超過 預設為NULL
} op_code[HASH_SIZE]; //存op_code_空間-end 大小為HASH_SIZE
struct srcpro
{
    char symname[11]; //symbom name
    bool exformat; //extent format-4
    char opcode[10]; //Opcode
    char optag; // (#, @, =
    char optr_1[10]; //op-1
    char optr; //+ - * /
    char optr_2[10]; //op-2
    int address;
    int address_size;
    int use;
    char obj_code_str[20];
    struct srcpro *next;
} save_srcpro[MAX_Srcpro_SIZE];

unsigned int Hash(char* str)//赫序加起來-star
{
    long int hash=0;
    int i;
    for(i = 0; str[i]; i++)
    {
        if (str[i]!=' '&&str[i]!='+'&&str[i]!='#'&&str[i]!='@'&&str[i]!='='&&str[i]!='\n'&&str[i]!='\r')  //為了讓讀入不會因為空格跟+號和#@=\n
        {
            hash*=10;
            hash += str[i];
        }

    }
    return hash%HASH_SIZE;
}//赫序加起來-end
void init_op_cod_arr()//初始化全域的op_code陣列-star
{
    int i;
    for (i=0; i<HASH_SIZE; i++)
    {
        op_code[i].next=NULL;
        strcpy(op_code[i].op_name, "NULL");
        op_code[i].op_cod_int=0;
    }
}//初始化全域的op_code陣列-end
struct op_code *create_note()
{
    struct op_code * ptr;
    ptr = (struct op_code *)malloc(sizeof(struct op_code));
    ptr->next =NULL;
    ptr->op_cod_int=0;
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
    while(!feof(fp_r))  //讀取資料
    {
        fscanf(fp_r, "%s", &reg1);
        substr=strtok(reg1,",");//這裡取出了第一個也就是OP_Name
        //        printf("%d\n",Hash(substr));//現在處理赫旭
        int opNumber = Hash(substr);//放赫旭
        ptr = &op_code[opNumber];//將赫旭出來的陣列位置存下
        if (strcmp(ptr->op_name, "NULL")==0)
        {
            strcpy(ptr->op_name, substr);
        }
        else
        {
            while (ptr->next != NULL)
            {
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
        if (ptr->op_cod[0]-'0'>=0&&ptr->op_cod[0]-'0'<=9)
        {
            char temp = ptr->op_cod[0];
            ptr->op_cod_int+=(temp-'0')*16;
        }
        else
        {
            char temp = ptr->op_cod[0];
            ptr->op_cod_int+=(temp-'A'+10)*16;

        }
        if (ptr->op_cod[1]-'0'>=0&&ptr->op_cod[1]-'0'<=9)
        {
            char temp = ptr->op_cod[1];
            ptr->op_cod_int+=temp-'0';
        }
        else
        {
            char temp = ptr->op_cod[1];
            ptr->op_cod_int+=(temp-'A'+10);

        }


    }
    fclose(fp_r);

}//讀op_code-end

void test_print_op_code()//測試用看op_code對不對-start
{
    int i;
    struct op_code *ptr;
    for (i=0; i<HASH_SIZE; i++)
    {
        ptr = &op_code[i];
        if (strcmp(ptr->op_name, "NULL")!=0)
        {
            do
            {
                printf("%5s %5s %5s %5s\n",ptr->op_name,ptr->op_m,ptr->op_format,ptr->op_cod);
                if (ptr->next!=NULL)
                {
                    ptr = ptr->next;
                }
                else
                {
                    break;
                }
            }
            while (&free);

        }
    }
}//測試用看op_code對不對-end
void red_srcpro()//讀題目-start
{
    char reg1[100],*substr=NULL;
    FILE *fp_r = fopen("srcpro.txt", "r");
    int i = 0 ;
    while(!feof(fp_r)) //讀取資料.開始
    {
        fgets(&reg1,100, fp_r);

        if(reg1[strlen(reg1) - 1] == '\n')
            reg1[strlen(reg1) - 1] = '\0';
        if (reg1[strlen(reg1) - 1] == '\r')
            reg1[strlen(reg1) - 1] = '\0';
        //坑-要把\r拿掉
        //處理分割.開始
        if (reg1[6]==' ')
        {
            reg1[6]=';';//我打算用;來分隔 測資輸入為第七個是分隔空白
        }
        if (reg1[14]==' ')
        {
            reg1[14]=';';//以及第14個為分隔空白
        }
        if (reg1[24]!='\0')  //利用sprint來插入字串
        {
            char temap[20];
            strcpy(temap, &reg1[24]);//先暫存
            sprintf(&reg1[24], "%s%s",";",temap);
        }
        substr = strtok(reg1, ";");//切割符號
        //處理分割.結束

        if (substr!=NULL)  //第一段
        {
            strcpy(save_srcpro[i].symname, substr);
            substr = strtok(NULL, ";");//切割符號
        }//第一段

        if (substr!=NULL)  //第二段
        {
            if (substr[0]=='+')
            {
                save_srcpro[i].exformat=true;
            }
            strcpy(save_srcpro[i].opcode, &substr[1]);
            substr = strtok(NULL, ";");//切割符號
        }//第二段
        if (substr!=NULL)  //第三段
        {
            save_srcpro[i].optag=substr[0];

            strcpy(save_srcpro[i].optr_1, &substr[1]);
            substr = strtok(NULL, ";");//切割符號
        }//第三段
        if (substr!=NULL)  //第四段
        {
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
    for (i=0; i<MAX_Srcpro_SIZE; i++)
    {
        save_srcpro[i].exformat=false;
    }
}
void test_print_srcpro()//測試輸出用-star
{
    int i;
    for (i=0; i<Srcpro_size; i++)
    {
        printf("%2d ",i);

        char *temp;
        printf("%04x ",save_srcpro[i].address);
        temp = save_srcpro[i].symname;
        if (temp!=NULL)
        {
            printf("%s",temp);
        }
        temp = save_srcpro[i].opcode;
        if (save_srcpro[i].exformat)  // +
        {
            printf("+");
        }
        else
        {
            printf(" ");
        }
        if (temp!=NULL)
        {
            printf("%s",temp);
        }
        if (save_srcpro[i].optag!=NULL)  // (#, @, =
        {
            printf("%c",save_srcpro[i].optag);
        }
        else
        {
            printf(" ");
        }
        temp = save_srcpro[i].optr_1;
        if (temp!=NULL)  //第三店
        {
            printf("%-10s",temp);
        }
        if (save_srcpro[i].optr!=NULL)  //符號 + - * / ,
        {
            printf("%c",save_srcpro[i].optr);
        }
        else
        {
            printf(" ");
        }
        temp = save_srcpro[i].optr_2;
        if (temp!=NULL)  //第四段
        {
            //            printf("%c",save_srcpro[i].optr);
            printf("%-5s",temp);
        }

        printf(" %15s ",save_srcpro[i].obj_code_str);
        printf("\n");
        int j;

        if (strcmp(save_srcpro[i].opcode,"LTORG ")==0)
        {
            for(j=0; j<HASH_SIZE; j++)
            {
                struct LTORG *ptr_lto=&LTORG_Arr[j];
                if(strcmp(LTORG_Arr[j].name,"NULL")!=0&&LTORG_Arr[j].isltorg==1)
                {

                    printf("   %04x      * %s",LTORG_Arr[j].address,LTORG_Arr[j].name);
                    printf("%33s",ptr_lto->content);
                    printf("\n");
                }
                while(ptr_lto->next!=NULL)
                {
                    ptr_lto=ptr_lto->next;
                    if(strcmp(ptr_lto->name,"NULL")!=0&&ptr_lto->isltorg==1)
                    {
                        printf("   %04x      * %s",ptr_lto->address,ptr_lto->name);
                        printf("%33s",ptr_lto->content);
                        printf("\n");
                    }
                }
            }
        }
        else if(strcmp(save_srcpro[i].opcode,"END   ")==0)
        {
            for(j=0; j<HASH_SIZE; j++)
            {
                struct LTORG *ptr_lto=&LTORG_Arr[j];
                if(strcmp(LTORG_Arr[j].name,"NULL")!=0&&LTORG_Arr[j].isltorg==0)
                {

                    printf("   %04x      * %s",LTORG_Arr[j].address,LTORG_Arr[j].name);
                    printf("%33s",ptr_lto->content);
                    printf("\n");
                }
                while(ptr_lto->next!=NULL)
                {
                    ptr_lto=ptr_lto->next;
                    if(strcmp(ptr_lto->name,"NULL")!=0&&ptr_lto->isltorg==0)
                    {

                        printf("   %04x      * %s",ptr_lto->address,ptr_lto->name);
                        printf("%33s",ptr_lto->content);
                        printf("\n");
                    }

                }
            }
        }

    }
}//測試輸出用-end
int check_op_code(char *in_ptr)
{
    int hash=Hash(in_ptr);
    struct op_code *ptr = &op_code[hash];
    if (strcmp(in_ptr, ptr->op_name)==0)
    {
        if (ptr->op_format[0]=='3')
        {
            return 3;
        }
        else
        {
            return 2;
        }
    }
    while (ptr->next!=NULL)
    {
        ptr=ptr->next;
        if (strcmp(in_ptr, ptr->op_name)==0)
        {
            if (ptr->op_format[0]=='3')
            {
                return 3;
            }
            else
            {
                return 2;
            }
        }
    }
    return -1;
}

void get_address_size ()//算每一條指令站多少byte-開始
{
    int i,use=0,sw=0,add_sw=0;

    for (i=0; i<Srcpro_size; i++)
    {
        sw=0;//這個是用在LTORG當到LTORG打開開關讓後續不多於處理地址
        add_sw=0;//在添加時作為赫序表內有時打開
        if (save_srcpro[i].optag=='=')
        {
            int hash =Hash(save_srcpro[i].optr_1);
            struct LTORG *ptr=&LTORG_Arr[hash];
            if (strcmp(ptr->name, "NULL")==0)
            {
                strcpy(ptr->name, save_srcpro[i].optr_1);
                //                printf("%s\n",ptr->name);
            }
            else
            {
                while (ptr->next!=NULL)
                {
                    ptr=ptr->next;
                    if (strcmp(ptr->name, save_srcpro[i].optr_1)==0)  //赫序重複add_sw打開
                    {
                        add_sw=1;
                    }
                }
                if (add_sw==0)
                {
                    ptr->next = malloc(sizeof(struct LTORG));
                    ptr=ptr->next;
                    ptr->next=NULL;
                    strcpy(ptr->name, save_srcpro[i].optr_1);
                    //                    printf("%s\n",ptr->name);
                }
            }



        }
        char temp[100];//空白清除用
        strcpy(temp, save_srcpro[i].opcode);//空白清除用
        strtok(temp, " ");//空白清除用
        if (strcmp(temp, "START")==0)
        {
            save_srcpro[i].address_size=0;
        }
        else if (save_srcpro[i].exformat==true)
        {
            save_srcpro[i].address_size=4;
            //                printf("%d ",save_srcpro[i].address_size=4);
        }
        else if (strcmp(temp, "WORD")==0)
        {
            save_srcpro[i].address_size=3;
        }
        else if (strcmp(temp, "RESW")==0)
        {
            save_srcpro[i].address_size=atoi(save_srcpro[i].optr_1)*3;
            //                printf("%d ",save_srcpro[i].address_size);
        }
        else if (strcmp(temp, "RESB")==0)
        {
            save_srcpro[i].address_size=atoi(save_srcpro[i].optr_1);
            //                                printf("%d ",save_srcpro[i].address_size);
        }
        else if (strcmp(temp, "BYTE")==0)
        {
            strcpy(temp, save_srcpro[i].optr_1);
            strtok(temp, " ");
            save_srcpro[i].address_size=strlen(temp)-3;
            if (save_srcpro[i].optr_1[0]=='X')
            {
                save_srcpro[i].address_size/=2;
            }
            //                printf("%d %s",save_srcpro[i].address_size,temp);//當位置有問題可以看看
        }
        else if (strcmp(temp, "BASE")==0)
        {
            save_srcpro[i].address_size=0;
        }
        else if (strcmp(temp, "LTORG")==0)
        {
            save_srcpro[i].address = block_locctr_arrary[use].address;
            block_locctr_arrary[use].address += save_srcpro[i].address_size;
            sw=1;
            //            Srcpro_size++;
            int j;
            for (j=0; j<HASH_SIZE; j++)
            {
                struct LTORG *ptr=&LTORG_Arr[j];
                if (strcmp(ptr->name, "NULL")!=0)  //當不等於NULL 這裡是看陣列第一個是不是
                {
                    ptr->address = block_locctr_arrary[use].address;
                    block_locctr_arrary[use].address += strlen(ptr->name)-3;
                    ptr->isltorg=1;
                    ptr->use=use;
                    if(ptr->name[0]=='C')
                    {
                        // ptr->name[0]='\'';
                        char lt_temp[10],*lt_temp2;
                        strcpy(lt_temp,ptr->name);
                        strtok(lt_temp, "'");
                        lt_temp2=strtok(NULL, "'");

                        int k;
                        strcpy(ptr->content,"");//歸零

                        for(k=0; k<strlen(lt_temp2); k++)
                        {
                            sprintf(ptr->content,"%s%X",ptr->content,lt_temp2[k]);//拼接16進ascii
                        }

                    }
                    else if(ptr->name[0]=='X')
                    {

                        char lt_temp[10],*lt_temp2;
                        strcpy(lt_temp,ptr->name);
                        strtok(lt_temp, "'");
                        lt_temp2=strtok(NULL, "'");
                        strcpy(ptr->content,lt_temp2);//因為16進位不需要再轉換
                    }

                    while (ptr->next!=NULL)  //未測試可能有安全隱患 這裡看後面接上的對不對
                    {
                        ptr=ptr->next;
                        ptr->address = block_locctr_arrary[use].address;
                        block_locctr_arrary[use].address += strlen(ptr->name)-3;
                        ptr->isltorg=1;
                        ptr->use=use;
                        if(ptr->name[0]=='C')
                        {
                            // ptr->name[0]='\'';
                            int j;
                            char lt_temp[10],*lt_temp2;
                            strcpy(lt_temp,ptr->name);
                            strtok(lt_temp, "'");
                            lt_temp2=strtok(NULL, "'");
                            int k;
                            strcpy(ptr->content,"");//歸零

                            for(k=0; k<strlen(lt_temp2); k++)
                            {
                                sprintf(ptr->content,"%s%X",ptr->content,lt_temp2[k]);//拼接16進ascii
                            }


                        }
                        else if(ptr->name[0]=='X')
                        {

                            char lt_temp[10],*lt_temp2;
                            strcpy(lt_temp,ptr->name);
                            strtok(lt_temp, "'");
                            lt_temp2=strtok(NULL, "'");
                            strcpy(ptr->content,lt_temp2);//因為16進位不需要再轉換
                        }


                    }
                }
            }
            save_srcpro[i].address_size=0;
        }
        else if (strcmp(temp, "EQU")==0)
        {

        }
        else if (strcmp(temp, "END")==0)
        {
            sw=1;
            save_srcpro[i].address_size=0;
            save_srcpro[i].address = block_locctr_arrary[use].address;
            block_locctr_arrary[use].address += save_srcpro[i].address_size;
            int j;
            for (j=0; j<HASH_SIZE; j++)
            {
                struct LTORG *ptr=&LTORG_Arr[j];
                if (strcmp(ptr->name, "NULL")!=0)  //當不等於NULL
                {
                    if (ptr->isltorg!=1)
                    {
                        ptr->address = block_locctr_arrary[use].address;
                        ptr->isltorg=0;
                        ptr->use=use;
                        if(ptr->name[0]=='C')
                        {
                            block_locctr_arrary[use].address += strlen(ptr->name)-3;
                            // ptr->name[0]='\'';
                            int j;
                            char lt_temp[10],*lt_temp2;
                            strcpy(lt_temp,ptr->name);
                            strtok(lt_temp, "'");
                            lt_temp2=strtok(NULL, "'");
                            int k;
                            strcpy(ptr->content,"");//歸零

                            for(k=0; k<strlen(lt_temp2); k++)
                            {
                                sprintf(ptr->content,"%s%X",ptr->content,lt_temp2[k]);//拼接16進ascii
                            }


                        }
                        else if(ptr->name[0]=='X')
                        {
                            block_locctr_arrary[use].address += (strlen(ptr->name)-3)/2;
                            char lt_temp[10],*lt_temp2;
                            strcpy(lt_temp,ptr->name);
                            strtok(lt_temp, "'");
                            lt_temp2=strtok(NULL, "'");
                            strcpy(ptr->content,lt_temp2);//因為16進位不需要再轉換
                        }

                    }
                    while (ptr->next!=NULL)  //未測試可能有安全隱患
                    {
                        ptr=ptr->next;
                        if (ptr->isltorg!=1)
                        {
                            ptr->address = block_locctr_arrary[use].address;

                            ptr->isltorg=0;
                            ptr->use=use;
                            if(ptr->name[0]=='C')
                            {
                                block_locctr_arrary[use].address += strlen(ptr->name)-3;
                                // ptr->name[0]='\'';
                                int j;
                                char lt_temp[10],*lt_temp2;
                                strcpy(lt_temp,ptr->name);
                                strtok(lt_temp, "'");
                                lt_temp2=strtok(NULL, "'");
                                int k;
                                strcpy(ptr->content,"");//歸零

                                for(k=0; k<strlen(lt_temp2); k++)
                                {
                                    sprintf(ptr->content,"%s%X",ptr->content,lt_temp2[k]);//拼接16進ascii
                                }


                            }
                            else if(ptr->name[0]=='X')
                            {
                                block_locctr_arrary[use].address += (strlen(ptr->name)-3)/2;
                                char lt_temp[10],*lt_temp2;
                                strcpy(lt_temp,ptr->name);
                                strtok(lt_temp, "'");
                                lt_temp2=strtok(NULL, "'");
                                strcpy(ptr->content,lt_temp2);//因為16進位不需要再轉換
                            }
                        }
                    }
                }
            }


        }
        else if (strcmp(temp, "USE")==0)
        {
            int j=0,bool_block=0;
            while (strcmp(block_locctr_arrary[j++].block_name, "NULL"))
            {
                if (strcmp(save_srcpro[i].optr_1, block_locctr_arrary[j].block_name))
                {
                    use = j;
                    save_srcpro[i].use=j;
                    bool_block=1;
                }
            }
            if (bool_block==1)
            {
                strcpy(block_locctr_arrary[j-1].block_name, save_srcpro[i].optr_1);
                use = j-1;
                save_srcpro[i].use=j-1;
            }
        }
        else
        {
            int form=check_op_code(temp);
            if(form!=-1)
            {
                save_srcpro[i].address_size=form;
            }
            else
            {
                save_srcpro[i].address_size=0;
                //                    printf("- %s -\n",temp);
            }
        }
        if (sw==0)
        {
            save_srcpro[i].address = block_locctr_arrary[use].address;
            block_locctr_arrary[use].address += save_srcpro[i].address_size;
        }

        if (strcmp(save_srcpro[i].symname, "      ")!=0&&strcmp(save_srcpro[i].symname, "")!=0)
        {
            int stname_hash =Hash(save_srcpro[i].symname);
            struct symname *ptr= &symname_arr[stname_hash];
            if(strcmp(ptr->name,"NULL")==0)
            {
                strcpy(ptr->name,save_srcpro[i].symname);
                ptr->content=atoi(save_srcpro[i].optr_1);
                ptr->address=save_srcpro[i].address;
                ptr->use=use;
            }
            else
            {
                while(ptr->next!=NULL)
                {
                    ptr=ptr->next;
                }
                ptr->next=(struct symname*)malloc(sizeof(struct symname));
                ptr=ptr->next;
                strcpy(ptr->name,save_srcpro[i].symname);
                ptr->content=atoi(save_srcpro[i].optr_1);
                ptr->use=use;
                ptr->address=save_srcpro[i].address;
                ptr->next=NULL;


            }
        }

    }

}//算每一條指令站多少byte-結束

void equ()
{

    int i;
    for (i=0; i<Srcpro_size; i++)
    {
        char temp[100];//空白清除用
        strcpy(temp, save_srcpro[i].opcode);//空白清除用
        strtok(temp, " ");//空白清除用
        if(strcmp(temp, "EQU")==0)
        {
            if (strcmp(save_srcpro[i].optr_1, "*")==0)
            {

                int hash=Hash(save_srcpro[i].symname);
                struct symname *ptr=&symname_arr[hash];
                if (strcmp(save_srcpro[i].symname, ptr->name)==0)
                {

                    ptr->content=save_srcpro[i].address;
                }
                while (ptr->next!=NULL)
                {
                    ptr=ptr->next;
                    if (strcmp(save_srcpro[i].symname, ptr->name)==0)
                    {

                        ptr->content=save_srcpro[i].address;
                    }
                }

            }
            else
            {

                int hash;
                int op_1,op_2;
                if(atoi(save_srcpro[i].optr_1)!=0)
                {

                    hash=Hash(save_srcpro[i].symname);//直接用當前符號查
                    struct symname *ptr=&symname_arr[hash];

                    if (strcmp(temp, ptr->name)==0)
                    {


                        op_1=atoi(save_srcpro[i].optr_1);
                        if (save_srcpro[i].optr=="")
                        {
                            ptr->content=op_1;
                        }
                    }
                    while (ptr->next!=NULL)
                    {
                        ptr=ptr->next;
                        if (strcmp(temp, ptr->name)==0)
                        {
                            op_1=atoi(save_srcpro[i].optr_1);
                            if (save_srcpro[i].optr=="")
                            {
                                ptr->content=op_1;
                            }
                        }
                    }
                }
                else
                {
                    char EQU_op1_temp[100];//空白清除用
                    strcpy(EQU_op1_temp,save_srcpro[i].optr_1);//空白清除用
                    strtok(EQU_op1_temp, " ");//空白清除用

                    hash=Hash(save_srcpro[i].optr_1);//因為放者其他符號所以查詢其他符號
                    struct symname *ptr=&symname_arr[hash];
                    if (strcmp(EQU_op1_temp, ptr->name)==0)
                    {
                        op_1=ptr->address;
                        if (save_srcpro[i].optr=="")
                        {
                            ptr->content=op_1;
                        }
                    }
                    while (ptr->next!=NULL)
                    {
                        ptr=ptr->next;
                        if (strcmp(EQU_op1_temp, ptr->name)==0)
                        {
                            op_1=ptr->address;
                            if (save_srcpro[i].optr=="")
                            {
                                ptr->content=op_1;
                            }
                        }
                    }
                }

                ////////////////////////////^^^^^^^^^^^不確定大蓋好
                if (save_srcpro[i].optr!= "")//判斷有沒有後面
                {


                    struct symname *ptr;

                    if(atoi(save_srcpro[i].optr_2)!=0)
                    {
                        hash=Hash(save_srcpro[i].symname);//直接用當前符號查
                        ptr=&symname_arr[hash];
                        if (strcmp(temp, ptr->name)==0)
                        {

                            op_2=atoi(save_srcpro[i].optr_2);
                        }
                        while (ptr->next!=NULL)
                        {
                            ptr=ptr->next;
                            if (strcmp(temp, ptr->name)==0)
                            {
                                op_2=atoi(save_srcpro[i].optr_2);

                            }
                        }

                    }
                    else
                    {
                        char EQU_temp[100];//空白清除用
                        strcpy(EQU_temp,save_srcpro[i].optr_2);//空白清除用
                        strtok(EQU_temp, " ");//空白清除用
                        hash=Hash(save_srcpro[i].optr_2);//因為放者其他符號所以查詢其他符號
                        ptr=&symname_arr[hash];

                        if (strcmp(EQU_temp, ptr->name)==0)
                        {
                            op_2=ptr->address;
                        }
                        while (ptr->next!=NULL)
                        {

                            ptr=ptr->next;

                            if (strcmp(EQU_temp, ptr->name)==0)
                            {
                                op_2=ptr->address;

                            }
                        }
                    }

                    char Now_EQU_temp[100];//空白清除用
                    strcpy(Now_EQU_temp,save_srcpro[i].optr_2);//空白清除用
                    strtok(Now_EQU_temp, " ");//空白清除用
                    int hash=Hash(save_srcpro[i].symname);
                    struct symname *now_ptr=&symname_arr[hash];
                    if (strcmp(Now_EQU_temp, now_ptr->name)==0)
                    {

                    }
                    else
                    {

                        while (now_ptr->next!=NULL)
                        {
                            now_ptr=now_ptr->next;
                            if (strcmp(Now_EQU_temp, now_ptr->name)==0)
                            {
                                break;
                            }
                        }
                    }
                    if(save_srcpro[i].optr=='-')
                    {


                        now_ptr->content=op_1-op_2;
                        now_ptr->address=op_1-op_2;
                        save_srcpro[i].address=op_1-op_2;

                    }


                }



            }
        }
    }
}
void init_block()
{
    int i;
    for (i=0; i<MAX_block_locctr_SIZE; i++)
    {
        strcpy(block_locctr_arrary[i].block_name, "NULL");
    }
}
void init_LTORG_Arr()
{
    int i;
    for (i=0; i<HASH_SIZE; i++)
    {
        strcpy(LTORG_Arr[i].name, "NULL");
        LTORG_Arr[i].next==NULL;
        LTORG_Arr[i].isltorg=0;
    }
}
void init_symname ()
{
    int i;
    for(i=0; i<HASH_SIZE; i++)
    {
        strcpy(symname_arr[i].name, "NULL");
        symname_arr[i].next==NULL;
        symname_arr[i].use=0;
    }
}
void priint_symname()
{
    int i;

    printf("name  use address Hash\n");
    for(i=0; i<HASH_SIZE; i++)
    {
        struct symname *ptr = &symname_arr[i];
        if(strcmp(ptr->name,"NULL"))
        {
            printf("%s %d  %04x %d \n",ptr->name,ptr->use,ptr->address,i);
        }
        while(ptr->next!=NULL)
        {
            ptr=ptr->next;
            printf("%s %d  %04x %d \n",ptr->name,ptr->use,ptr->address,i);
        }
    }

}
void obj_code()
{
    int i;
    for (i=0; i<Srcpro_size; i++)
    {
        char temp[100];//空白清除用
        strcpy(temp, save_srcpro[i].opcode);//空白清除用
        strtok(temp, " ");//空白清除用
        int hash = Hash(temp);
        struct op_code *ptr=&op_code[hash];
        if (strcmp(temp, "START")==0)
        {

        }
        else if (save_srcpro[i].exformat==true)//exformat 4 格式四
        {
            do
            {
                if (strcmp(temp, ptr->op_name)==0)
                {
                    int t=((int)pow(16, 1))*ptr->op_cod_int,t2;
                    if (save_srcpro[i].optag=='#')
                    {
                        t+=1*(int)pow(16, 1);
                    }
                    else if(save_srcpro[i].optag=='@')
                    {
                        t+=2*(int)pow(16, 1);
                    }
                    else
                    {
                        t+=3*(int)pow(16, 1);
                    }
                    t+=1;//沒意外都是1
                    struct symname *sy_ptr=&symname_arr[Hash(save_srcpro[i].optr_1)];
                    do
                    {
                        char sy_temp[100];//空白清除用
                        strcpy(sy_temp, sy_ptr->name);//空白清除用
                        strtok(sy_temp, " ");//空白清除用
                        char srcpro_temp[100];//空白清除用
                        strcpy(srcpro_temp, save_srcpro[i].optr_1);//空白清除用
                        strtok(srcpro_temp, " ");//空白清除用

                        // printf("%s\n",sy_temp);
                        if (strcmp(sy_temp,srcpro_temp)==0)
                        {
                            t2=sy_ptr->address;
                            break;
                        }
                        if(sy_ptr->next==NULL)
                            break;
                        sy_ptr=sy_ptr->next;
                    }
                    while (1);

                    sprintf(save_srcpro[i].obj_code_str,"%03X%05X", t,t2);
                    break;
                }
                if (ptr->next==NULL)
                    break;
                ptr=ptr->next;
            }
            while (1);
        }
        else if (strcmp(temp, "WORD")==0)
        {
            save_srcpro[i].address_size=3;
        }
        else if (strcmp(temp, "RESW")==0)
        {
            save_srcpro[i].address_size=atoi(save_srcpro[i].optr_1)*3;
            //                printf("%d ",save_srcpro[i].address_size);
        }
        else if (strcmp(temp, "RESB")==0)
        {
            save_srcpro[i].address_size=atoi(save_srcpro[i].optr_1);
            //                                printf("%d ",save_srcpro[i].address_size);
        }
        else if (strcmp(temp, "BYTE")==0)
        {

            if(save_srcpro[i].optr_1[0]=='C')
            {
                    //好像有問題

            }
            else if(save_srcpro[i].optr_1[0]=='X')
            {
                printf("%s\n",save_srcpro[i].optr_1);
                char lt_temp[10],*lt_temp2;
                strcpy(lt_temp,save_srcpro[i].optr_1);
                strtok(lt_temp, "'");
                lt_temp2=strtok(NULL, "'");
                strcpy(save_srcpro[i].obj_code_str,lt_temp2);//因為16進位不需要再轉換
            }
        }
        else if (strcmp(temp, "BASE")==0)
        {

//            BASE=save_srcpro[i].address;
        }
        else if (strcmp(temp, "LTORG")==0)
        {

        }
        else if (strcmp(temp, "EQU")==0)
        {

        }
        else if (strcmp(temp, "END")==0)
        {
            int j;



        }
        else if (strcmp(temp, "USE")==0)
        {
            int j=0,bool_block=0;
            while (strcmp(block_locctr_arrary[j++].block_name, "NULL"))
            {
                if (strcmp(save_srcpro[i].optr_1, block_locctr_arrary[j].block_name))
                {

                }
            }
            if (bool_block==1)
            {

            }
        }
        else
        {
            //這裡繼續
            do
            {
                if (strcmp(temp, ptr->op_name)==0)//比較 op_code temp為 save_srcpro[i].opcode
                {
                    int t=((int)pow(16, 1))*ptr->op_cod_int,t2;//把op_code從字串轉成0XF 16進位 t為op ni xbpe

                    if (save_srcpro[i].optag=='#')
                    {
                        t+=1*(int)pow(16, 1);
                    }
                    else if(save_srcpro[i].optag=='@')
                    {
                        t+=2*(int)pow(16, 1);
                    }
                    else
                    {
                        t+=3*(int)pow(16, 1);
                    }
                    struct symname *sy_ptr=&symname_arr[Hash(save_srcpro[i].optr_1)];//取得符號在赫序表裡面   sy_ptr =在赫序表裡的 optr_1
                    struct LTORG *LT_ptr=&LTORG_Arr[Hash(save_srcpro[i].optr_1)];//常數表
                    if(save_srcpro[i].optag=='#')
                    {
                        sprintf(save_srcpro[i].obj_code_str,"%03X%03X",t,atoi(save_srcpro[i].optr_1));
                    }


                    do  //比對符號跟ptr
                    {
                        char sy_temp[100];//空白清除用
                        strcpy(sy_temp, sy_ptr->name);//空白清除用
                        strtok(sy_temp, " ");//空白清除用
                        char srcpro_temp[100];//空白清除用
                        strcpy(srcpro_temp, save_srcpro[i].optr_1);//空白清除用
                        strtok(srcpro_temp, " ");//空白清除用

                        // printf("%s\n",sy_temp);
                        if (strcmp(sy_temp,srcpro_temp)==0)//sy_temp是符號名稱  srcpro_temp是save_srcpro的符號
                        {
                            if (save_srcpro[i].optr==',')
                            {
                                t+=8;
                            }
                            if (strcmp(temp,"LDB")==0)
                            {
                                BASE=sy_ptr->address;
                            }
                            int address = save_srcpro[i].address+save_srcpro[i].address_size;
                            int lessAddress=sy_ptr->address-address;//相減判斷有沒有在範圍內
                            if (lessAddress<=2047&&lessAddress>=-2048)
                            {
                                t+=2;
                                char t_x[100],t_x2[4];
                                int t_x_len;
                                sprintf(t_x, "%03X",lessAddress);
                                t_x_len=strlen(t_x);
                                t_x2[0]=t_x[t_x_len-3];
                                t_x2[1]=t_x[t_x_len-2];
                                t_x2[2]=t_x[t_x_len-1];
                                sprintf(save_srcpro[i].obj_code_str,"%03X%s",t,t_x2);
                            }
                            else
                            {
                                t+=4;
                                lessAddress = sy_ptr->address-BASE;
                                sprintf(save_srcpro[i].obj_code_str,"%03X%03x",t,lessAddress);
                            }
                            t2=sy_ptr->address;
                            break;
                        }
                        if(sy_ptr->next==NULL)
                            break;
                        sy_ptr=sy_ptr->next;
                    }
                    while (1);

                    //處理特殊記憶體A X S-star
                    char srcpro_temp[100];//空白清除用
                    strcpy(srcpro_temp, save_srcpro[i].optr_1);//空白清除用
                    strtok(srcpro_temp, " ");//空白清除用
                    if(strcmp("",srcpro_temp)==0)
                    {
                        sprintf(save_srcpro[i].obj_code_str,"%03X000",t);
                    }
                    else//處理特殊記憶體A X S-star
                    {

                        int j;
                        char reg_temp1[2],reg_temp2[2];
                        sprintf(reg_temp1,"%c",srcpro_temp[0]);
                        sprintf(reg_temp2,"%c",srcpro_temp[2]);
                        for(j=0; j<=9; j++)
                        {
                            if(strcmp(srcpro_temp,regs_name[j])==0)
                            {
                                t=((int)pow(16, 1))*ptr->op_cod_int;
                                sprintf(save_srcpro[i].obj_code_str,"%03X0",t+j);
                            }
                            else if(strcmp(reg_temp1,regs_name[j])==0&&strcmp(save_srcpro[i].opcode,"COMPR ")==0)
                            {
                                int k;
                                for(k=0; k<=9; k++)
                                {
                                    if(strcmp(reg_temp2,regs_name[k])==0)
                                    {
                                        t=((int)pow(16, 1))*ptr->op_cod_int;
                                        sprintf(save_srcpro[i].obj_code_str,"%03X%X",t+j,k);
                                    }
                                }
                            }
                        }


                    }

                    //處理特殊記憶體A X S-end

                    if(save_srcpro[i].optag=='=')//等於走長數表
                    {

                        char srcpro_temp[100],srcpro_temp2[100];//空白清除用
                        strcpy(srcpro_temp, save_srcpro[i].optr_1);//空白清除用
                        strtok(srcpro_temp, " ");//空白清除用
                        struct LTORG *LT_ptr=&LTORG_Arr[Hash(save_srcpro[i].optr_1)];
                        char LT_temp[100];//空白清除用
                        strcpy(LT_temp, LT_ptr->name);//空白清除用
                        strtok(LT_temp, " ");//空白清除用
                        while(1)
                        {
                            strcpy(LT_temp, LT_ptr->name);//空白清除用
                            strtok(LT_temp, " ");//空白清除用

                            if(strcmp(LT_temp,srcpro_temp)==0)
                            {

                                int address = save_srcpro[i].address+save_srcpro[i].address_size;
                                int lessAddress=LT_ptr->address-address;//相減判斷有沒有在範圍內
                                if (lessAddress<=2047&&lessAddress>=-2048)//用pc
                                {
                                    t+=2;
                                    sprintf(save_srcpro[i].obj_code_str,"%03X%03X",t,lessAddress);

                                }
                                else//用BASE
                                {
                                    t+=4;
                                    lessAddress = sy_ptr->address-BASE;
                                    sprintf(save_srcpro[i].obj_code_str,"%03X%03x",t,lessAddress);
                                }
                            }
                            if(LT_ptr->next!=NULL)
                            {
                                LT_ptr=LT_ptr->next;
                            }
                            else
                            {
                                break;
                            }

                        }

                    }

                    break;
                }
                if (ptr->next==NULL)
                    break;
                ptr=ptr->next;
            }
            while (1);
        }

    }
}

void test_print_LITTAB()//常數表輸出
{
    int i;
    for(i=0; i<HASH_SIZE; i++)
    {
        if(strcmp(LTORG_Arr[i].name,"NULL")!=0)
        {
            printf("%6s %5x\n",LTORG_Arr[i].name,LTORG_Arr[i].address);

        }
        struct LTORG *ptr=&LTORG_Arr[i];
        while(ptr->next!=NULL)
        {
            ptr=ptr->next;
            printf("%6s %5x\n",ptr->name,ptr->address);
        }
    }
}
void print_obj_cod()
{
     int i,f=0;
    for (i=0; i<Srcpro_size; i++)
    {
        //printf("%2d ",i);

        char *temp;
        //printf("%04x ",save_srcpro[i].address); //地址
        //temp = save_srcpro[i].symname;
      //  if (temp!=NULL)
       // {
      //      printf("%s",temp);
      //  }
        temp = save_srcpro[i].opcode;
        if (save_srcpro[i].exformat)  // +
        {
            f=4;
        }
        else
        {
           f=save_srcpro[i].address_size;
        }

        temp = save_srcpro[i].optr_1;
        int j;
        if (strcmp(save_srcpro[i].opcode,"LTORG ")==0)
        {
            for(j=0; j<HASH_SIZE; j++)
            {
                struct LTORG *ptr_lto=&LTORG_Arr[j];
                if(strcmp(LTORG_Arr[j].name,"NULL")!=0&&LTORG_Arr[j].isltorg==1)
                {
                     printf("%s\n",LTORG_Arr[j].content);

                }
                while(ptr_lto->next!=NULL)
                {
                    ptr_lto=ptr_lto->next;
                    if(strcmp(ptr_lto->name,"NULL")!=0&&ptr_lto->isltorg==1)
                    {
                         printf("%s\n",ptr_lto->content);
                    }
                }
            }
        }
        else if(strcmp(save_srcpro[i].opcode,"END   ")==0)
        {
            for(j=0; j<HASH_SIZE; j++)
            {
                struct LTORG *ptr_lto=&LTORG_Arr[j];
                if(strcmp(LTORG_Arr[j].name,"NULL")!=0&&LTORG_Arr[j].isltorg==0)
                {

                     printf("%s\n",ptr_lto->content);
                }
                while(ptr_lto->next!=NULL)
                {
                    ptr_lto=ptr_lto->next;
                    if(strcmp(ptr_lto->name,"NULL")!=0&&ptr_lto->isltorg==0)
                    {

                         printf("%s\n",ptr_lto->content);
                    }

                }
            }
        }else if(strcmp(save_srcpro[i].obj_code_str,"")!=0)
        {
             printf("%s\n",save_srcpro[i].obj_code_str);
        }





    }
}
int main()
{
    char reg1[100], reg2[100], reg3[100];
    FILE *fp_w = fopen("data_out.txt", "w");
    init_symname ();
    init_op_cod_arr();
    red_op_code();
    init_LTORG_Arr();

    init_red_srcpro();
    red_srcpro();
    //        printf("%d  %d",Hash("RSUB  "),Hash("RSUB"));//赫緒測試
    get_address_size();
    equ();
    obj_code();
    test_print_srcpro();
    priint_symname();
    test_print_op_code();//測試print_opOCD
    test_print_LITTAB();

    print_obj_cod();
    printf("\n%x",block_locctr_arrary[0].address);
    if (fp_w == NULL)
        return -1;
    fclose(fp_w);
}



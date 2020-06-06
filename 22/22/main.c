
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
#define HASH_SIZE 11 //op_code陣列大小空間 Opcode space size
#define MAX_Srcpro_SIZE 100 //指令行數的上限
#define MAX_block_locctr_SIZE 100//use空間表
int Srcpro_size=0;
struct block_locctr//存use空間
{
    char block_name[10];
    int address;
} block_locctr_arrary[MAX_block_locctr_SIZE];
struct LTORG
{
    int address;
    struct LTORG *next;
    char name[10];
    int isltorg;
    int use;
} LTORG_Arr[HASH_SIZE];
struct symname
{
    int address;
    char name[10];
    int use;
    struct symname *next;
} symname_arr[HASH_SIZE];
struct op_code //存op_code_空間-star
{
    char op_name[10];//op 名稱
    char op_m[5];//記憶體
    char op_format[5];//格式
    char op_cod[3];//實際的op碼
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
                printf("%s %s %s %s\n",ptr->op_name,ptr->op_m,ptr->op_format,ptr->op_cod);
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
            printf("%s",temp);
        }
        if (save_srcpro[i].optr!=NULL)  //符號 + - * / ,
        {
            printf("%c",save_srcpro[i].optr);
        }
        temp = save_srcpro[i].optr_2;
        if (temp!=NULL)  //第四段
        {
            //            printf("%c",save_srcpro[i].optr);
            printf("%s",temp);
        }
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
                    printf("\n");
                }
                while(ptr_lto->next!=NULL)
                {
                    ptr_lto=ptr_lto->next;
                    if(strcmp(ptr_lto->name,"NULL")!=0&&ptr_lto->isltorg==1)
                    {
                        printf("   %04x      * %s",ptr_lto->address,ptr_lto->name);
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
                    printf("\n");
                }
                while(ptr_lto->next!=NULL)
                {
                    ptr_lto=ptr_lto->next;
                    if(strcmp(ptr_lto->name,"NULL")!=0&&ptr_lto->isltorg==0)
                    {
                        printf("   %04x      * %s",ptr_lto->address,ptr_lto->name);
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
                if (strcmp(ptr->name, "NULL")!=0)  //當不等於NULL
                {
                    ptr->address = block_locctr_arrary[use].address;
                    block_locctr_arrary[use].address += strlen(ptr->name)-3;
                    ptr->isltorg=1;
                    ptr->use=use;
                    while (ptr->next!=NULL)  //未測試可能有安全隱患
                    {
                        ptr=ptr->next;
                        ptr->address = block_locctr_arrary[use].address;
                        block_locctr_arrary[use].address += strlen(ptr->name)-3;
                        ptr->isltorg=1;
                        ptr->use=use;
                    }
                }
            }
            save_srcpro[i].address_size=0;
        }
        else if (strcmp(temp, "EQU")==0)
        {
            sw=1;
            save_srcpro[i].address_size=0;
            if (strcmp(save_srcpro[i].optr_1, "*")==0) {
                sw=0;
            } else {
                
                int hash;
                int op_1,op_2;
                if (atoi(save_srcpro[i].optr_1)!=0) {
                    op_1=atoi(save_srcpro[i].optr_1)!=0;
                }
                else{
                hash=Hash(save_srcpro[i].optr_1);
                struct symname *ptr=&symname_arr[hash];
                if (strcmp(save_srcpro[i].optr_1, ptr->name)==0) {
                    op_1=ptr->address;//沒處理use
                }
                while (ptr->next!=NULL) {
                    ptr=ptr->next;
                    if (strcmp(save_srcpro[i].optr_1, ptr->name)==0) {
                        op_1=ptr->address;//沒處理use
                    }
                }
                }
                if (save_srcpro[i].optr!= "") {
                    hash=Hash(save_srcpro[i].optr_2);
                    struct symname *ptr=&symname_arr[hash];
                    if (strcmp(save_srcpro[i].optr_2, ptr->name)==0) {
                        op_2=ptr->address;//沒處理use
                    }
                    while (ptr->next!=NULL) {
                        ptr=ptr->next;
                        if (strcmp(save_srcpro[i].optr_2, ptr->name)==0) {
                            op_2=ptr->address;//沒處理use
                        }
                    }
                    if (save_srcpro[i].optr!= "-") {
                        
                        save_srcpro[i].address=op_1=op_2;
                        printf("%d",save_srcpro[i].address);
                    }
                }
                else
                {
                    
                }
                
            }
            
            
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
                        block_locctr_arrary[use].address += strlen(ptr->name)-3;
                        ptr->isltorg=0;
                        ptr->use=use;
                    }
                    while (ptr->next!=NULL)  //未測試可能有安全隱患
                    {
                        ptr=ptr->next;
                        if (ptr->isltorg!=1)
                        {
                            ptr->address = block_locctr_arrary[use].address;
                            block_locctr_arrary[use].address += strlen(ptr->name)-3;
                            ptr->isltorg=0;
                            ptr->use=use;
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
                ptr->use=use;
                ptr->address=save_srcpro[i].address;
                ptr->next=NULL;
            }
        }
        
    }
    
}//算每一條指令站多少byte-結束

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
    
    printf("name  use address\n");
    for(i=0; i<HASH_SIZE; i++)
    {
        struct symname *ptr = &symname_arr[i];
        if(strcmp(ptr->name,"NULL"))
        {
            printf("%s %d  %04x\n",ptr->name,ptr->use,ptr->address);
        }
        while(ptr->next!=NULL)
        {
            ptr=ptr->next;
            printf("%s %d  %04x\n",ptr->name,ptr->use,ptr->address);
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
    //        test_print_op_code();//測試print_opOCD
    init_red_srcpro();
    red_srcpro();
    //        printf("%d  %d",Hash("RSUB  "),Hash("RSUB"));//赫緒測試
    
    get_address_size();
    
    test_print_srcpro();
    priint_symname();
    //測試區
    //    char *substr,temp[100];
    //    strcpy(temp, save_srcpro[1].opcode);
    //    substr = strtok(temp, " ");
    //    printf("%s\n%s\n%s\n %d",temp,save_srcpro[1].opcode,op_code[Hash(save_srcpro[1].opcode)].op_name,strcmp(temp, op_code[Hash(save_srcpro[1].opcode)].op_name));
    //測試區
    
    if (fp_w == NULL)
        return -1;
    
    fclose(fp_w);
}

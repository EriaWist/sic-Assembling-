//
//  main.c
//  22
//
//  Created by 阿騰 on 2020/5/22.
//  Copyright © 2020 阿騰. All rights reserved.
//

//#include <stdio.h>
//
//
//
//int main(){
//
//
//
//    FILE *pFile;
//
//    char buffer[]={ 'H','e','y' };
//
//
//
//    pFile = fopen( "write.txt","w" );
//
//    if( NULL == pFile ){
//
//        printf( "open failure" );
//
//        return 1;
//
//    }else{
//
//        fwrite(buffer,1,sizeof(buffer),pFile);
//
//    }
//
//
//
//    fclose(pFile);
//
//    return 0;
//
//}

#include <stdio.h>
//
//int main(){
//    freopen("OP_code.txt", "r", stdin);
//    char a[100];
//    scanf("%s", &a);
//    printf("%s\n", a);
//    return 0;
//}

int main(){
char reg1[100], reg2[100], reg3[100];

   FILE *fp_r = fopen("OP_code.txt", "r");
   FILE *fp_w = fopen("data_out.txt", "w");

   if (fp_r == NULL)
     return -1;

   if (fp_w == NULL)
     return -1;

   while(!feof(fp_r)) {
     fscanf(fp_r, "%s %s %s", &reg1, &reg2, &reg3);
     printf("%s %s %s\n", reg1, reg2, reg3);
     fprintf(fp_w, "%s %s %s\n", reg3, reg2, reg1);
   }

   fclose(fp_r);
   fclose(fp_w);
 }

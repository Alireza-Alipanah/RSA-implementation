#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
typedef long long int LLI;
LLI p=57719,q=52457,n,phi,e=257,m=0,k,d,c,o,f,a=1,f1,f2;
int l;
char s[30],s2[60];
LLI ME(LLI a,LLI b,LLI n){
    a%=n;
    if(b==0)
        return 1;
    if(b%2==0){
        f=ME(a,b/2,n);
        f%=n;
        return (f*f)%n;
    }
    else{
        f=ME(a,(b-1)/2,n);
        f%=n;
        return (((f*f)%n)*a)%n;
    }
}
LLI Find_D(LLI r1,LLI r2,LLI t1,LLI t2)
{
    if(r2==0)
    {
        if(t1>0) { return t1;}
        while(t1<0)
            t1 = t1 + (p-1)*(q-1);
        return t1;
    }
    LLI temp;
    LLI divisor = r1 / r2;
    LLI reminder = r1 % r2;
    temp = t2;
    t2 = t1 - divisor*t2;
    t1 = temp;
    r1 = r2;
    r2 = reminder;
    Find_D(r1,r2,t1,t2);
}
void encrypt(){
    FILE * pFile;
    l=strlen(s);
    int q=0,v=0;
    while (v<l){
            m+=a*s[v];
            a*=128;
            if (v%3==2 || v==l-1)
            {
                c=ME(m,e,n);
                while(c>0){
                    s2[q]=c%128;
                    //printf("s2[%d]:%c\t%d\n",q,s2[q],v);
                    c/=128;
                    q++;
                }
                m=0;
                a=1;
                s2[q]=129;
                //printf("s2[%d]:%d\t%d different\n",q,s2[q],v);
                q++;
            }
            v++;
    }
            pFile=fopen("keyword.txt","a");
            printf("\n");
            //if (pFile==NULL) perror ("Error opening file");
            int y;
            for(y=0;y<strlen(s2);y++){
                fputc(s2[y],pFile);
                //printf("%c",s2[y]);
            }
            fclose(pFile);
            printf("\n");
        //printf("Encrypted: %s\n",s2);
}
void decrypt(){
    l=strlen(s2);
    c=0;
    int j,i=-1,y;
    int okab=0;
    //printf("\ndecrypt: ");
    for(j=0;j<l;j++){
            if (s2[j]!=-127)
            {
                c+=a*(s2[j]);
                a*=128;
            }
            else
            {
                d=Find_D(phi,e,0,1);
                o=ME(c,d,n);
                for(y=i+1;y<j;y++){
                    s[okab]=o%128;
                    o/=128;
                    okab++;
                    //printf("%c",s[y]);
                }
                i=j;
                c=0;
                a=1;
                okab--;
            }
    }
    s[okab+1]='\0';
    //printf("Decrypted: %s\n",s);
}
unsigned char key[4096];
unsigned char text[4096];
char dkey[5];
void keygenerate(unsigned char text[],unsigned char key[], unsigned char newkey[] );
void encryption(unsigned char text[],unsigned char newkey[],unsigned char encrypted[]);
void decryption(unsigned char encrypted[],unsigned char newkey[],unsigned char decrypted[]);
void readFile(unsigned char x[]);
int main()
{
    n=p*q;
    phi=(p-1)*(q-1);
    printf("\nEncrypt or Decrypt?(e/d)\n");
    char ch;
    while(ch!='e'&&ch!='d'){
        ch=getch();
    }
    if(ch=='e'){
        printf("New text?(y/n)\n");
        while(ch!='y'&&ch!='n'){
            ch=getch();
        }
        if(ch=='y'){
            char st[1000];
            printf("Enter text: ");
            scanf("%[^\n]s",text);
            FILE * pFile;
            pFile=fopen("text.txt","w");
            //if (pFile==NULL) perror ("Error opening file");
            int i;
            for(i=0;i<strlen(text);i++){
                fputc(text[i],pFile);
            }
            fclose(pFile);
            printf("\n");
        }
        ch='0';
        printf("New keyword?(y/n)\n");
        while(ch!='y'&&ch!='n'){
            ch=getch();
        }
        if(ch=='y'){
            printf("Enter keyword: ");
            fseek(stdin,0,SEEK_END);
            gets(s);
            FILE * pFile;
            pFile=fopen("keyword.txt","w");
            fclose(pFile);
            srand(time(NULL));
            do f1=rand();while (f1==0);
            do f2=rand();while (f2==0);
            encrypt();
            int i;
            LLI f3=f1,f4=f2;
            s[0]=10;
            for (i=1;f3>0;i++)
            {
                s[i]=f3%10+48;
                f3/=10;
            }
            s[i]=',';
            i++;
            while (f4>0)
            {
                s[i]=f4%10+48;
                f4/=10;
                i++;
            }
            encrypt();
        }
        readFile(text);
        unsigned char* newkey = malloc( sizeof(unsigned char) * ( strlen(text)+1 ) );
        unsigned char* encrypted = malloc( sizeof(unsigned char) * ( strlen(text)+1 ) );
        unsigned char* decrypted = malloc( sizeof(unsigned char) * ( strlen(text)+1 ) );
        keygenerate(text,key, newkey);
        //printf("****%c\n",newkey[20]);
        encryption(text,newkey,encrypted);
        //printf("****%c\n",newkey[20]);
        printf("Encrypted text: %s\n",encrypted);
        //return main();
    }
    else{
        readFile(text);
        unsigned char* newkey = malloc( sizeof(unsigned char) * ( strlen(text)+1 ) );
        unsigned char* encrypted = malloc( sizeof(unsigned char) * ( strlen(text)+1 ) );
        unsigned char* decrypted = malloc( sizeof(unsigned char) * ( strlen(text)+1 ) );
        keygenerate(text,key, newkey);
        decryption(encrypted,newkey,decrypted);
        printf("Decrypted text: %s\n",decrypted);
        //return main();
    }
    return 0;
}
void keygenerate(unsigned char text[],unsigned char key[], unsigned char newkey[] )
{
    int i,j;
    //printf("%d\n",strlen(text));
    //printf("\nnewkey: ");
    srand(f1);
    for(i=0,j=0;i<strlen(text);i++,j++)
    {
        if(j==strlen(key)){
            j=0;
        }
        newkey[i]=(key[j]+((rand()%f2)%10))%128;
        //printf("%c",key[j]);
        //printf("%d\t%c\n",i,newkey[i]);
    }
    //printf("*\n");
    newkey[i+1]='\0';
    printf("\n");
    //printf("%c\t%d\n",newkey[i],i);
}
void encryption(unsigned char text[],unsigned char newkey[],unsigned char encrypted[])
{
    //printf("****%c\n",newkey[20]);
    FILE * pFile;
    pFile=fopen("encrypted.txt","w");
    //if (pFile==NULL) perror ("Error opening file");
    int i=0;
    srand(f1);
    for( ; i<strlen(text); i++)
    {
        //printf("****%c\n",newkey[20]);
        char cc=(text[i]+newkey[i]);
        encrypted[i]=cc;
        //printf("%d\t%c\n",i,newkey[i]);
        fputc(encrypted[i],pFile);
    }
    fclose(pFile);
    encrypted[i]='\0';
    //printf("****%c\n",newkey[20]);
}
void decryption(unsigned char encrypted[],unsigned char newkey[],unsigned char decrypted[])
{
    FILE * pFile;
    int i=0, c,j;
    pFile=fopen("encrypted.txt","r");
    if (pFile==NULL) perror ("Error opening file");
    else
    {
        while (1)
        {
            c = fgetc (pFile);
            if(c==EOF) break;
            encrypted[i]=c;
            i++;
            //printf("%c\n",c);
        }
    }
    encrypted[i+1]='\0';
    for(j=0; j<i; j++)
    {
        decrypted[j]= (((encrypted[j]-newkey[j])+128)%128);

    }
    decrypted[j]='\0';
}
void readFile(unsigned char x[] )
{
    FILE * pFile;
    int i=0, c;
    pFile=fopen("text.txt","r");
    if (pFile==NULL) perror ("Error opening file");
    else
    {
        while (1)
        {
            c = fgetc (pFile);
            if(c==EOF) break;
            x[i]=c;
            i++;
            //printf("%c\n",c);
        }
    }
    x[i]='\0';
    fclose(pFile);
    i=0;
    pFile=fopen("keyword.txt","r");
    if (pFile==NULL) perror("Error opening file");
    else
    {
        while (1)
        {
            c = fgetc (pFile);
            if(c==EOF) break;
            s2[i]=c;
            i++;
            //printf("%c\n",c);
        }
    }
    s2[i]='\0';
    decrypt();
    for(i=0;i<strlen(s);i++){
        key[i]=s[i];
    }
    key[i+1]='\0';
    for (i=0;key[i]!=10;i++);
    i++;
    key[i-1]='\0';
    f1=f2=0;
    LLI j=1;
    while (key[i]!=',')
    {
        f1+=(key[i]-48)*j;
        j*=10;
        i++;
    }
    i++;
    j=1;
    while (key[i]!='\0')
    {
        f2+=(key[i]-48)*j;
        j*=10;
        i++;
    }
    //printf("string:%s\n",key);
    fclose(pFile);
}

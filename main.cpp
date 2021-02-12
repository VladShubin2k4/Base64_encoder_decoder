#include <iostream>
#include <cstring>
#include <cctype>
using namespace std;

/*
ABCDEFGHIJ KLMNOPQRST UVWXYZabcd efghijklmn opqrstuvwx yz01234567 89+/
0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123
0          1          2          3          4          5          6
 */

int transfer(int arg,int& k,short radix=2){
  int res=0;
  k=1;
  for(short i=0;arg!=0; ++i){
      res+=(arg%radix)*k;
      k*=10;
      arg/=radix;
  }
  return res;
}

void Decodetable(char* str, int* code){
    for(short i=0; i<strlen(str); ++i){
        if(isalpha(str[i])){
            if(str[i]>95) code[i]=str[i]-71;
            else code[i]=str[i]-65;
        }else if(isdigit(str[i])) code[i]=str[i]+4;
        else if(str[i]=='=') code[i]=0;
        else if(str[i]=='+') code[i]=62;
        else code[i]=63;
    }
}

void SplitIntoDigits(int& p, int n, short* bin, short &i){
    if(p){
        do{
            bin[i++]=n/p;
            n%=p;
            p/=10;
        }while(p);
    }
}

short transfer_to_6BIN(char* str, int* code, short* bin){
    int p=1;
    for(short i=0; i<strlen(str); ++i){
        code[i]=transfer(code[i],p,2);
        p/=10;
        for(short j=i*6; j<6+i*6; ++j){
            if(p<100000){
                if(!p && isalpha(str[i])){
                    short lim=j;
                    for(; j<lim+6;) bin[j++]=0;
                }else{
                    int cntp=p,cnt=0;
                    while(cntp){
                        cntp/=10;
                        cnt++;
                    }
                    for(short r=0; r<6-cnt; ++r) bin[j++]=0;
                    SplitIntoDigits(p,code[i],bin,j);
                }
            }else if(p) SplitIntoDigits(p,code[i],bin,j);
        }
        if(i+1==strlen(str)) return (6*(i+1));
    }
    return 100;
}

int main(){
    setlocale(LC_ALL, "Rus");
    ios::sync_with_stdio(false);
    cout<<"Enter len of str:\n";
    short n;
    cin>>n;
    char* str=new char[n+1]; for(short i=0; i<=n; ++i) str[i]='\0';
    int* code=new int[n]; for(short i=0; i<n; ++i) code[i]=0;
    cout<<"Enter what you want to do: encode or decode?\n";
    cin>>str;
    if(!strcmp(str,"de")){
        cin>>str;
        Decodetable(str,code);
        short* bin=new short[(n+1)*6];
        short end=transfer_to_6BIN(str,code,bin);
        int p;
        for(short i=0; i<end/8; ++i){
            code[i]=0;
            for(short j=i*8; j<i*8+8;++j){
                p=128;
                while(p){
                    code[i]=code[i]+(p*bin[j++]);
                    p/=2;
                }
            }
        }
        for(short i=0; i<end/8;++i) cout<<static_cast<char>(code[i]);
        delete[] bin;
    }else{
        cin>>str;
    }
    cout<<endl;
    delete[] str;
    delete[] code;
    return 0;
}

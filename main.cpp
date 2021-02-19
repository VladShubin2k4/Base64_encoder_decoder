#include <iostream>
#include <windows.h>
#include <cstring>
#include <cctype>
#include <cmath>
using namespace std;

int transfer(int arg,int& k,short radix){
  int res=0;
  if(arg<0) arg+=256;
  k=1;
  for(short i=0;arg!=0; ++i){
      res+=(arg%radix)*k;
      k*=10;
      arg/=radix;
  }
  return res;
}

void Decodetable(char* str, int* code,bool decode){
    if(decode){
        for(short i=0; i<strlen(str); ++i){
            if(isalpha(str[i])){
                if(str[i]>95) code[i]=str[i]-71;
                else code[i]=str[i]-65;
            }else if(isdigit(str[i])) code[i]=str[i]+4;
            else if(str[i]=='=') code[i]=0;
            else if(str[i]=='+') code[i]=62;
            else code[i]=63;
        }
    }else for(short i=0; i<strlen(str);++i) code[i]=str[i];
}

void SplitIntoDigits(int& p, int n, short* bin, short &i){
    while(p){
        bin[i++]=n/p;
        n%=p;
        p/=10;
    }
}

void transfer_to_ASCII(short& end, int* code, short* bin,short r){
    for(short i=0; i<end/r; ++i){
        code[i]=0;
        for(short j=i*r; j<i*r+r;++j){
            int p=1<<(r-1);
            while(p){
                code[i]=code[i]+(p*bin[j++]);
                p=p>>1;
            }
        }
    }
}

void transfer_to_Base64(short& end, int* code, short* bin,short r,char* res){
    for(short i=0; i<1+end/r; ++i){
        code[i]=0;
        for(short j=i*r;j<end && j<i*r+r;++j){
            int p=1<<(r-1);
            while(p && j<end){
                code[i]=code[i]+(p*bin[j++]);
                p=p>>1;
            }
            if(code[i]<26) res[i]=static_cast<char>(code[i]+65);
            else if(code[i]<52) res[i]=static_cast<char>(code[i]+71);
            else if(code[i]<62) res[i]=static_cast<char>(code[i]-4);
            else if(code[i]==62) res[i]=43;
            else res[i]=47;
        }
    }
}

short transfer_to_BIN(char* str, int* code, short* bin, short arg){
    int p=1;
    for(short i=0; i<strlen(str); ++i){
        code[i]=transfer(code[i],p,2);
        p/=10;
        for(short j=i*arg; j<arg+i*arg; ++j){
            if(p<pow(10,arg)){
                if(!p && isalpha(str[i])){
                    short lim=j;
                    for(; j<lim+arg;) bin[j++]=0;
                }else{
                    int cntp=p,cnt=0;
                    while(cntp){
                        cntp/=10;
                        cnt++;
                    }
                    for(short r=0; r<arg-cnt; ++r) bin[j++]=0;
                    SplitIntoDigits(p,code[i],bin,j);
                }
            }else if(p) SplitIntoDigits(p,code[i],bin,j);
        }
    }
    return (strlen(str)*arg);
}

int main(){
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    ios::sync_with_stdio(false);
    cout<<"Enter len of str:\n";
    short n,end;
    cin>>n;
    char* str=new char[n+1]; for(short i=0; i<=n; ++i) str[i]='\0';
    int* code=new int[n]; for(short i=0; i<n; ++i) code[i]=0;
    cout<<"encode or decode?\n";
    cin>>str;
    cout<<"Input your str:"<<endl;
    if(!strcmp(str,"de")){
        cin>>str;
        Decodetable(str,code,true);
        short* bin=new short[n*6];
        end=transfer_to_BIN(str,code,bin,6);
        transfer_to_ASCII(end,code,bin,8);
        for(short i=0; i<end/8;++i) cout<<static_cast<char>(code[i]);
        delete[] bin;
    }else{
        cin>>str;
        Decodetable(str,code,false);
        char* res=new char[strlen(str)*8/6];
        short* bin=new short[n*8]; for(short i=0; i<n<<3; ++i) bin[i]=0;
        end=transfer_to_BIN(str,code,bin,8);
        transfer_to_Base64(end,code,bin,6,res);
        for(short i=0; i<=end/6;++i) cout<<res[i];
        if(end%6) for(short i=1; i<6-(end%6); i<<=1) cout<<'=';
        delete[] res;
        delete[] bin;
    }
    delete[] str;
    delete[] code;
    return 0;
}

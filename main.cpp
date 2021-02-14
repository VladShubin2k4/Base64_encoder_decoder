#include <iostream>
#include <windows.h>
#include <string>
#include <cctype>
#include <cmath>
using namespace std;
int transfer(int arg,int& k,short radix=2){
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
void Decodetable(string& str, int* code,bool decode){
    if(decode){
        for(short i=0; i<str.size(); ++i){
            if(isalpha(str[i])){
                if(str[i]>95) code[i]=str[i]-71;
                else code[i]=str[i]-65;
            }else if(isdigit(str[i])) code[i]=str[i]+4;
            else if(str[i]=='=') code[i]=0;
            else if(str[i]=='+') code[i]=62;
            else code[i]=63;
        }
    }else for(short i=0; i<str.size();++i) code[i]=str[i];
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
            int p=static_cast<int>(pow(2,r-1));
            while(p){
                code[i]=code[i]+(p*bin[j++]);
                p/=2;
            }
        }
    }
}
void transfer_to_Base64(short& end, int* code, short* bin,short r,string& res){
    for(short i=0; i<1+end/r; ++i){
        code[i]=0;
        for(short j=i*r; j<i*r+r;++j){
            int p=static_cast<int>(pow(2,r-1));
            while(p){
                code[i]=code[i]+(p*bin[j++]);
                p/=2;
            }
            if(code[i]<26) res+=static_cast<char>(code[i]+65);
            else if(code[i]<52) res+=static_cast<char>(code[i]+71);
            else if(code[i]<62) res+=static_cast<char>(code[i]-4);
            else if(code[i]==0) res+='=';
            else if(code[i]==62) res+=43;
            else res+=47;
        }
    }
}
short transfer_to_BIN(string& str, int* code, short* bin, short arg){
    int p=1;
    for(short i=0; i<str.size(); ++i){
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
        if(i+1==str.size()) return (arg*(i+1));
    }
    return 100;
}
int main(){
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    ios::sync_with_stdio(false);
    short end;string str;
    cout<<"Enter what you want to do: encode or decode?\n";
    cin>>str;
    cout<<"Input your str:\n";
    if(str=="de"){
        cin>>str;
        int* code=new int[str.size()];
        short* bin=new short[str.size()*6];
        Decodetable(str,code,true);
        end=transfer_to_BIN(str,code,bin,6);
        transfer_to_ASCII(end,code,bin,8);
        for(short i=0; i<end/8;++i) cout<<static_cast<char>(code[i]);
        delete[] bin;
        delete[] code;
    }else{
        cin>>str;
        string res;
        int* code=new int[str.size()];
        short* bin=new short[str.size()*8];
        Decodetable(str,code,false);
        end=transfer_to_BIN(str,code,bin,8);
        transfer_to_Base64(end,code,bin,6,res);
        cout<<res;
        if(end%6) for(short i=1; i<6-(end%6); i*=2) cout<<'=';
        delete[] bin;
        delete[] code;
    }
    cout<<endl;
    cin.get();cin.get();
    return 0;
}

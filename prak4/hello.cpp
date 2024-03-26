#include<iostream>
#include <thread>
#include <sys/wait.h>
using namespace std;
void routine(){
    cout << "hello world"<<endl;
    
}
void routine2(){
    cout << "my name is hicham"<<endl;
    
}
int main(){
   thread t1(routine);
   thread t2(routine2);
   return 0 ;
}
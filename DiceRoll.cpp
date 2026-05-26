// Online C++ compiler to run C++ program online
#include <iostream>
#include<ctime>
#include<cstdlib>
using namespace std;


int main() {
    srand(time(NULL));

    char choice;
    
    do {
    int roll1;
    int roll2;

    roll1=(rand()%6)+1;
    roll2=(rand()%6)+1;

    cout<<"Dice 1 :"<<roll1<<endl;
    cout<<"Dice 2 :"<<roll2<<endl;


    cout<<"Would you like to roll Again (Y/N) :\n";
    cin>>choice;

    }while(choice =='Y' || choice =='y');
    cout<<"Game over!\n";

    return 0;
}
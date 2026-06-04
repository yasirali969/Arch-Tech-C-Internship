#include<iostream>
#include<cstdlib>
#include<ctime>
using namespace std;
int main(){
	
	srand(time(0));
	int n=(rand()%100)+1;
	int num;
	int Score=0;
	int countAttempts=0;
	string name;
	cout<<"-------------------------------------------\n";
	cout<<"=========== Number Guessing Game ==========\n";
	cout<<"-------------------------------------------\n";

	cout<<"Guess Number between 1-100\n";
	cout<<"Enter your Name :";
	cin>>name;
	
	
	for(int i=0;i<5;i++){
		countAttempts++;
		cout<<"Guess Number:\n";
		cin>>num;
	if(num==n){
		cout<<"Congratulation! You Guessed Correctly\n";
		Score=(5-i)*10;
		break;
		
	}
	else if(num>n){
		cout<<"Its Too High\n";
	}
	else {
		cout<<"Its Too Low\n";
	}
	cout<<endl;
}
	cout<<"Game Over! Correct Guess is :"<<n<<endl;
	
	cout<<"======== Game Summary ========\n";
	cout<<"UserName :"<<name<<endl;
	cout<<"Attemp Used :"<<countAttempts<<endl;
	cout<<"Score :"<<Score<<endl;
	cout<<"==============================\n";

}

#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include <sstream>

using namespace std;
struct Task{
    string name;
    bool completed;   
};

vector <Task> tasks;

// =========Save Tasks =============
void saveTasks(){
    ofstream file("tasks.txt");

    file << "ID | TASK NAME | STATUS\n";
    file << "---------------------------\n";

    for(int i = 0; i < tasks.size(); i++){
        file << i+1 << " | "
             << tasks[i].name << " | "
             << (tasks[i].completed ? "Completed" : "Pending")
             << endl;
    }

    file.close();
}

// =========Load Tasks ===========
void loadTasks(){
    ifstream file("tasks.txt");

    string line;

    while(getline(file,line))
    {
        stringstream ss(line);

        string name;
        string status;

        getline(ss,name, ',');
        getline(ss,status);

        Task task;
        task.name=name;
        task.completed=(status == "1");

        tasks.push_back(task);
    }
    file.close();

    }

// ======== Add Task ==========
void addTask()
{
    Task task;

    cin.ignore();

    getline(cin, task.name);

    task.completed=false;

    tasks.push_back(task);

    saveTasks();

    cout<<" Task Added Successfully!\n";
}


// ========= View Tasks ==========
void viewTask(){
    if(tasks.empty()){
        cout<<"No task found";
    }
    cout<<"\n=======TO-DO List ======\n";

    for(int i=0; i<tasks.size();i++){

        cout<<i+1<<". "<<tasks[i].name;

        if(tasks[i].completed)
        cout<<"[Completed]";
        else
        cout<<"[Pending]";

     cout<<endl;
    }
}

// ========== Complete Task ===========
void CompleteTask(){
    int num;

    viewTask();
    cout<<"Enter task number to mark completed: ";
    cin>>num;

    if(num>=1 && num<=tasks.size()){
        tasks[num-1].completed=true;

        saveTasks();

        cout<<" Task Completed!\n";
    }
    else {
        cout<<"Invalid Task Number.\n";
    }
}

// ========== Delete Task========
void DeleteTask(){
    int num;
    viewTask();

    cout<<"Enter the task number to delete: ";
    cin>>num;

    if(num>=1 && num<=tasks.size()){
        tasks.erase(tasks.begin() + (num -1));

        saveTasks();


        cout<<" Task Deleted!\n";
    }
}

// ============ Show Progress ==========
void showProgress()
{
    if(tasks.empty()){
        cout<<"invalid number";
    }

    int CompletedCount=0;

    for(int i=0;i<tasks.size();i++){
            if(tasks[i].completed){
                CompletedCount++;
            }
    }

        int progress= (CompletedCount*100)/tasks.size();

        int bars= progress/10;

        cout<<"\nProgress: [";

        for(int i=0;i<bars;i++){
            cout<<"#";
        }
        for(int i=bars;i<10;i++){
            cout<<"-";
        }

        cout<<"] " << progress <<"%\n";
}




int main(){
    loadTasks();

    int choice;
    do{
        cout << "\n===== TO-DO LIST MENU =====\n";
        cout << "1. Add Task\n";
        cout << "2. View Tasks\n";
        cout << "3. Complete Task\n";
        cout << "4. Delete Task\n";
        cout << "5. Show Progress\n";
        cout << "6. Exit\n";

        cout<<"Enter the choice :";
        cin>> choice;

        switch(choice)
        {
            case 1:
            addTask();
            break;
            
            case 2:
            viewTask();
            break;
            
            case 3:
            CompleteTask();
            break;
            
            case 4:
            DeleteTask();
            break;

            case 5:
            showProgress();
            break;

            cout<< "Exiting Program...\n";
            break;

            default:
            cout<<" Invalid Choice.\n";

            
        }

    }
    while(choice!=6);
    return 0;

}

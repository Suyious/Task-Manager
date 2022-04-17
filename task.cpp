#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <vector>

void help(){
	// Using Raw Strings to output
	std::cout<<R"_(Usage :-
$ ./task add 2 hello world    # Add a new item with priority 2 and text "hello world" to the list
$ ./task ls                   # Show incomplete priority list items sorted by priority in ascending order
$ ./task del INDEX            # Delete the incomplete item with the given index
$ ./task done INDEX           # Mark the incomplete item with the given index as complete
$ ./task help                 # Show usage
$ ./task report               # Statistics)_"<<std::endl;
}

void add(int priority, std::string task){
	std::fstream file;	
	file.open("./task.txt",std::ios::out | std::ios::in | std::ios::app);
	std::string line;
	std::vector<std::string> tasks;
	bool found = false;
	while(std::getline(file,line)){	
		std::string pw = line.substr(0, line.find(" "));
		if(!found && std::stoi(pw) > priority){
			std::string newtask = std::to_string(priority)+" "+task;
			tasks.push_back(newtask);
			found = true;
		}
		tasks.push_back(line);
	}
	if(!found){
		std::string newtask = std::to_string(priority)+" "+task;
		tasks.push_back(newtask);
		found = true;
	}
	file.close();
	file.open("./task.txt");
	for(auto t: tasks){
		file<<t<<"\n";
	}
	file.close();
}

void ls(){
	std::fstream file;
	file.open("./task.txt");
	std::string line, priority, task;
	int count = 1;
	while(std::getline(file,line)){
		priority = line.substr(0, line.find(" "));
		task = line.substr(line.find(" ")+1);
		std::cout<<count<<". "<<task<<" ["<<priority<<"]"<<std::endl;
		count++;
	}
	if(count==1){
		std::cout<<"There are no pending tasks!"<<std::endl;
	}
}

void del(int index){
	std::fstream file;	
	file.open("./task.txt",std::ios::out | std::ios::in | std::ios::app);
	std::string line;
	std::vector<std::string> tasks;
	while(std::getline(file,line)){	
		tasks.push_back(line);
	}
	if((size_t)index>tasks.size() || index==0){
		std::cout<<"Error: task with index #"<<index<<" does not exist. Nothing deleted."<<std::endl;
		return;
	}
	tasks.erase(tasks.begin()+index-1);
	file.close();
	file.open("./task.txt",std::ios::out | std::ios::trunc);
	for(size_t i = 0;i<tasks.size();i++){
		file<<tasks[i]<<"\n";
	}
	std::cout<<"Deleted task #"<<index<<std::endl;
	file.close();
}

void done(int index){
	std::fstream file;	
	file.open("./task.txt",std::ios::out | std::ios::in | std::ios::app);
	std::string line;
	std::vector<std::string> tasks, completed;
	while(std::getline(file,line)){	
		tasks.push_back(line);
	}
	if(index==0 || (size_t)index>tasks.size()){
		std::cout<<"Error: no incomplete item with index #"<<index<<" exists."<<std::endl;
		return;
	}
	std::string removed = tasks.at(index-1);
	tasks.erase(tasks.begin()+index-1);
	file.close();
	file.open("./completed.txt",std::ios::out | std::ios::in | std::ios::app);
	while(std::getline(file,line)){
		completed.push_back(line);
	}
	completed.push_back(removed.substr(removed.find(" ")+1));
	file.close();
	file.open("./task.txt",std::ios::out | std::ios::trunc);
	for(size_t i = 0;i<tasks.size();i++){
		file<<tasks[i]<<"\n";
	}
	file.close();
	file.open("./completed.txt",std::ios::out | std::ios::trunc);
	for(size_t i = 0;i<completed.size();i++){
		file<<completed[i]<<"\n";
	}
	file.close();
	std::cout<<"Marked item as done."<<std::endl;
}

void report(){	
	std::fstream file;
	file.open("./task.txt");
	std::string line, priority, task;
	std::vector<std::string> tasks,completed;
	while(std::getline(file,line)){
		tasks.push_back(line);
	}
	std::cout<<"Pending : "<<tasks.size()<<std::endl;
	int count = 1;
	for(auto t:tasks){
		priority = t.substr(0, t.find(" "));
		task = t.substr(t.find(" ")+1);
		std::cout<<count<<". "<<task<<" ["<<priority<<"]"<<std::endl;
		count++;
	}
	file.close();
	file.open("./completed.txt");
	while(std::getline(file,line)){
		completed.push_back(line);
	}
	std::cout<<"\nCompleted : "<<completed.size()<<std::endl;
	count = 1;
	for(auto t:completed){
		std::cout<<count<<". "<<t<<std::endl;
		count++;
	}
	file.close();
}

int main(int argc, char* argv[]){
	if(argc==1){
		help();
		return 0;
	} else {
		// If Help is Passed as argument
		if(!strcmp(argv[1],"help")){
			help();
			return 0;
		// If report is Passed as argument
		} else if(!strcmp(argv[1],"report")){
			report();
			return 0;
		// If add is Passed as argument
		} else if(!strcmp(argv[1],"add")){
			if(argc<3){ // Checks if task is passed
				std::cout<<"Error: Missing tasks string. Nothing added!"<<std::endl;
				return 0;
			} else {
				if(argv[2][0] < 48 || argv[2][0] > 57){ // Checks if priority is passed
					std::cout<<"Priority is not passed!"<<std::endl;
					return 0;
				}
				int priority = atoi(argv[2]);
				std::string task = "";
				for(int i = 3; i < argc; i++){
					if(i!=3) task+=" ";
					task+=argv[i];
				}
				add(priority,task);
				std::cout<<"Added task: \""<<task<<"\" with priority "<<priority<<std::endl;
			}

		// If ls is Passed as argument
		} else if(!strcmp(argv[1],"ls")){
			ls();
			return 0;
		// If ls del Passed as argument
		} else if(!strcmp(argv[1],"del")){
			if(argc<3){
				std::cout<<"Error: Missing NUMBER for deleting tasks."<<std::endl;
				return 0;
			}
			del(atoi(argv[2]));
			return 0;
		// If done is Passed as argument
		} else if(!strcmp(argv[1],"done")){
			if(argc<3){
				std::cout<<"Error: Missing NUMBER for marking tasks as done."<<std::endl;
				return 0;
			}
			done(atoi(argv[2]));
			return 0;
		// If an invalid literal is Passed as argument
		} else {
			std::cout<<"Invalid parameter "<<argv[1]<<std::endl;
			return 1;
		}
	} 
	
	return 0;
}

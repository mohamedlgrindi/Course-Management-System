#include<iostream>
#include<string>
#include<vector>
#include<memory>
#include<algorithm>
using namespace std;

class Cours{
private:
   string Code, Name,Instructor;
   int MaxCapacity;
   vector<string>InroledStudent;
public:
   Cours(string Code,string Name,string Instructor,int maxcapcity):Code(Code),Name(Name),Instructor(Instructor),MaxCapacity(maxcapcity){} 
  
   vector<string>& getInroled(){
    return InroledStudent;
   }
    string &getcode(){
        return Code;
    }
    string& getInstrutor(){
        return Instructor;
    }
   int getMaxcapacity(){
    return MaxCapacity;
   }
   void AddStudent(string Name){
    if(InroledStudent.size()<=MaxCapacity){
       InroledStudent.emplace_back(Name);
    }
    else{
        cout<<"__The cours reached the Max capcity__"<<endl;
    }
   }

   void RemoveStudent(string Name){
     if(!InroledStudent.empty()){
        bool exist=false;
        for(int i=0; i<InroledStudent.size();i++){
            if(InroledStudent[i]==Name){
                InroledStudent.erase(InroledStudent.begin()+i);
                exist=true;
            }
        }
        if(!exist){cout<<"You The student not exist:"<<endl;}
     }
   }

   bool CheckifFull(){
      if(InroledStudent.size()< MaxCapacity){
         return false;
      }
      return true;
   }

   void DisplayCours(){
    cout<<"_______The Course Details_______"<<endl;
    cout<<"Course Code: "<< Code<<endl;
    cout<<"Course Name: "<< Name<<endl;
    cout<<"Course Instructor: "<< Instructor<<endl;
    cout<<"Course Inroled: "<<InroledStudent.size()<<"/"<< MaxCapacity <<endl;
    for(const auto& student:InroledStudent){
        cout<<" -"<<student<<endl;
    }
    cout<<"_________________________________"<<endl;
   }

};

class User{
  private:
    int Id;
    string Name,Role;
  public:
  virtual void displayMenu() = 0;
  
  User(int id,string name,string role):Id(id),Name(name),Role(role){}

  int getId(){
    return Id;
  }
  string getName(){
    return Name;
  }
  string getRole(){
    return Role;
  }

  void viewProfile(){
       cout<<"   User Id: "<<Id<<endl;
       cout<<"   User Name: "<<Name<<endl;
       cout<<"   User Role: "<<Role<<endl;
  }
  virtual ~User(){}

};

class  RegistrationSystem{
    private:
       vector<shared_ptr<User>>AllUsers;
       vector<shared_ptr<Cours>>AllCourses;
    public:
    vector<shared_ptr<Cours>>&getAllcourse(){
        return AllCourses;
    }

    vector<shared_ptr<User>>&getAllusers(){
        return AllUsers;
    }
};

class Student:public User{
    private:
    vector<shared_ptr<Cours>> RegisteredCourses;
    public:
    Student(int Id,string name,string role):User(Id, name, role){}

    void RegisterForCours(string Name,string Code,vector<shared_ptr<Cours>>& cours){
        for(auto& co:cours){
            if (co->getcode()==Code)
            {
                co->AddStudent(Name);
                RegisteredCourses.emplace_back(co);
                return;
            }
            
        }
         cout << "Course with code " << Code << " not found." << endl;
    }
 
   void DropaCourse(string Name,shared_ptr<Cours>cours,string Code){
        cours->RemoveStudent(Name);
        for(const auto& ReCours:RegisteredCourses){
           auto It=find(RegisteredCourses.begin(),RegisteredCourses.end(),cours);
           if( It !=RegisteredCourses.end()){
           cours->getcode()=Code;
           RegisteredCourses.erase(It);
          }
       }   
       cout<<"Cours Droped successfuly "<<endl;
   }

   void ViewSchedule(){
      for (auto& Register:RegisteredCourses)
      {
        if (!RegisteredCourses.empty())
        {
             cout<<"   -The Courses you had regester for"<<endl;
             Register->DisplayCours();
        }
        else{
             cout<<"You have not regester for any course"<<endl;
        }
        
      }
      
   }

   void displayMenu(){
    cout<<" 1. View Profile."<<endl;
    cout<<" 2. Register For Cours"<<endl;
    cout<<" 3. Drop a Course"<<endl;
    cout<<" 4. ViewSchedule"<<endl;
    cout<<"___________________________"<<endl;
   }

};

class Instructor:public User{
    private:
    vector<Cours*> AssignedCourses;//A list of courses the instructor is teaching.
    public:
    
     Instructor(int Id,string name,string role):User(Id, name, role){}

   vector<Cours*>&getAssignedC(){
    return AssignedCourses;
   }
    
    void ViewEnrolledStudents(shared_ptr<Cours> cours){
         cours->DisplayCours();
    }

    void displayMenu(){
      cout<<" 1. View Profile."<<endl;
      cout<<" 2. View Enrolled Students"<<endl;
      cout<<"___________________________"<<endl;
   }

};

class Administrator:public User{
    
    public:
    Administrator(int Id,string name,string role):User(Id, name, role){}

    void AddCourse(string Code,string Name,string Instructor,int maxcapcity,RegistrationSystem& rg){
        rg.getAllcourse().emplace_back(make_shared<Cours>(Code, Name, Instructor, maxcapcity));
        cout << "Course Added Successfuly: " << Code << endl;
    }
 
    void RemoveCourse(string Code,RegistrationSystem& rg){
     auto it=remove_if(rg.getAllcourse().begin(),rg.getAllcourse().end(),[Code](std::shared_ptr<Cours> cours){return cours->getcode()==Code;});
     if(it!=rg.getAllcourse().end()){ 
            rg.getAllcourse().erase(it, rg.getAllcourse().end());
            cout << "Course removed: " << Code << endl;
            return;
       }
       else {
            cout << "Course not found." << endl;
        }  
    }

    void AddUser(RegistrationSystem& rg,int id,string Name,string Role){
     if (Role == "Student") {
        rg.getAllusers().emplace_back(make_shared<Student>(id, Name, Role));
    } else if (Role == "Instructor") {
        rg.getAllusers().emplace_back(make_shared<Instructor>(id, Name, Role));
    } else if (Role == "Administrator") {
        rg.getAllusers().emplace_back(make_shared<Administrator>(id, Name, Role));
    } else {
        cout << "Invalid role. User not added." << endl;
    }
       cout << "__User Added Successfully__" << endl;
    }
    
    void RemoveUser(RegistrationSystem& rg,int id){
     auto it=remove_if(rg.getAllusers().begin(),rg.getAllusers().end(),[id](shared_ptr<User>user){return user->getId()==id;});
     if(it !=rg.getAllusers().end()){
        rg.getAllusers().erase(it, rg.getAllusers().end());
        cout << "__User Removed Successfully__" << endl;

     }
    }

    void ViewAllCoursesandEnrollments(){
     RegistrationSystem rg;
     for(auto& courses:rg.getAllcourse())
       courses->DisplayCours();
    }

     void displayMenu(){
       cout<<" 1. View Profile."<<endl;
       cout<<" 2. AddCourse"<<endl;
       cout<<" 3. RemoveCourse"<<endl;
       cout<<" 4. AddUser"<<endl;
       cout<<" 5. RemoveUser"<<endl;
     }

};


int main(){
    
   RegistrationSystem rg;
   Administrator admin(3361,"John","Administrator");
   admin.AddCourse("CSCI101","Introduction to Programming","Dr. Smith",50,rg);
   admin.AddCourse("MATH201", "Linear Algebra", "Prof. Johnson",40,rg);
   admin.AddCourse("PHYS301", "Quantum Mechanics", "Dr. Brown", 30,rg);
   
    auto studen=make_shared<Student>(1213,"Mohamed","Student");
    auto istructor=make_shared<Instructor>(8213,"said","Instructor");
    auto administrator=make_shared<Administrator>(8293,"Hossam","Administrator");
 
    admin.AddUser(rg,1213,"Mohamed","Student");
    admin.AddUser(rg,8213,"said","Instructor");
    admin.AddUser(rg,8293,"Hossam","Administrator");

    int id;
    string name,role;
    bool login=false;

    shared_ptr<User>curruntUser;
    curruntUser=NULL;

    cout<<" Enter the name: "<<endl;
    getline(cin,name);
    cout<<" Enter the Id: "<<endl;
    cin>>id;
    cout<<"enter role"<<endl;
    cin>>role;

    for(const auto& user:rg.getAllusers()){
        if (user->getId()==id && user->getName()==name)
        {
           curruntUser=user;
           cout<<"_____Login Successfuly_____"<<endl;
           login=true;
           break;
        }
    }

    if(!login){cout<<"____Invalid Input____"<<endl;return 0;}
    
    cout<<"The user Id: "<<curruntUser->getId()<<" The name is: "<<curruntUser->getName()<<endl;
    int op;
    do{
    curruntUser->displayMenu();
    cin>>op;
    switch(op){
        case 1:{
        curruntUser->viewProfile();
        }
        break;
        case 2:{
          string Code;
          string Name;

          if(curruntUser->getRole()=="Student"){
              cout<<"Enter the cours name"<<endl;
              cin>>Name;
              cout<<"Enter the cours Id"<<endl;
              cin>>Code;
              dynamic_pointer_cast<Student>(curruntUser)->RegisterForCours(Name,Code,rg.getAllcourse());
          }
          else if(curruntUser->getRole()=="Instructor"){
               string courseCode;
               cout<<"Enter the cours code"<<endl;
               cin>>courseCode;
               bool courseFound = false;
               for (auto& course : rg.getAllcourse()) {
              if (course->getcode() == courseCode) {
              dynamic_pointer_cast<Instructor>(curruntUser)->ViewEnrolledStudents(course);
              courseFound = true;
              break;
               } 
             }
              if (!courseFound) {
                cout << "Course with code " << courseCode << " not found." << endl;
              }
          }else if(curruntUser->getRole()=="Administrator"){
            string code,name,instructor;
            int maxCapacity;
            cout<< "Enter the course name:"<<endl;
            cin>>name;
            cout<<" Enter the course Code:"<<endl;
            cin>>code;
            cout<<"Enter the course instructor:"<<endl;
            cin>>instructor;
            cout<<"Enter the max capacity of the course:"<<endl;
            cin>>maxCapacity;
            dynamic_pointer_cast<Administrator>(curruntUser)->AddCourse(code,name,instructor,maxCapacity,rg);
          }
        break;
    }
        case 3:{
            if(curruntUser->getRole()=="Student"){
                string courseCode,naame;
                cout<< "Enter the course name:"<<endl;
                cin>>naame;
                cout<<" Enter the course Code:"<<endl;
                cin>>courseCode;
                bool courseFound = false;
                for (auto& course : rg.getAllcourse()) {
                if (course->getcode() == courseCode) {
                dynamic_pointer_cast<Student>(curruntUser)->DropaCourse(naame,course,courseCode);
                courseFound = true;
                break;
               } 
                if (!courseFound) {
                cout << "Course with code " << courseCode << " not found." << endl;
              }
            }
          }else if(curruntUser->getRole()=="Administrator"){
                string courseCode;
                cout<<" Enter the course Code:"<<endl;
                cin>>courseCode;
                dynamic_pointer_cast<Administrator>(curruntUser)->RemoveCourse(courseCode,rg);
          }
        }
             break;
         case 4:{
          if(curruntUser->getRole()=="Student"){
              dynamic_pointer_cast<Student>(curruntUser)->ViewSchedule();
          }
          else if(curruntUser->getRole()=="Administrator"){
            string name,Role;
            int Id;
          
            cout<< "Enter the course name:"<<endl;
            cin>>name;
            cout<<" Enter the course Role:"<<endl;
            cin>>Role;
            cout<<"Enter the User Id:"<<endl;
            cin>>Id;
            dynamic_pointer_cast<Administrator>(curruntUser)->AddUser(rg,Id,name,Role);
          }
         break;
         }
         case 5:{
              int Id;
              cout<<"Enter the User Id:"<<endl;
              cin>>Id;
              dynamic_pointer_cast<Administrator>(curruntUser)->RemoveUser(rg,Id);
         break;
         }
         case 0:
         break;

         default:
         cout<<" invalide inpute! try again"<<endl;
         cout<<endl;
         break;
    }       
    }while(op!=0);

    return 0;
}
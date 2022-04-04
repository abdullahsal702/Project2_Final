#include <vector>
#include <string>
#ifndef StudentManager_h
#define StudentManager_h

class Student
{
public:
    Student(unsigned int id, std::string name, unsigned short age);
    unsigned int GetStudentID();
    std::string GetStudentName();
    unsigned short GetStudentAge();
    ~Student();

private:
    unsigned int student_id;
    std::string student_name;
    unsigned short student_age;
};

class MyParser
{
public:
    // functions to check overflow
    MyParser();
    std::vector<std::string> operator()(std::string command);
    void GeneralCommandSplit(std::vector<std::string> &command_tokens, std::string command);
    void CommandErrorCheck(const std::vector<std::string> &command_tokens);
    void GetAddTokens(std::vector<std::string> &command_tokens);
    void GetConditionalTokens(std::vector<std::string> &command_tokens);
    void RemoveQuotes(int index, std::vector<std::string> &command_tokens);
    void AddErrorCheck(std::vector<std::string> &command_tokens);
    void ConditionalErrorCheck(std::vector<std::string> &command_tokens);
    void StopErrorCheck();
    void NameErrorCheck(std::string name);
    void IdErrorCheck(std::string id);
    void AgeErrorCheck(std::string age);
    ~MyParser();

private:
    bool error;
    // std::vector<char> separators = {' ', '\t'};
    // std::vector<std::string> command_tokens;
};

class DiskManager
{
public:
    DiskManager();
    // gets vector of students already on file and intializes student_list from StudentManager with it
    std::vector<Student *> ReadFile(std::string filename);
    // called when STOP is typed
    void WriteToFile(std::string filename, std::vector<Student *> &list);
    ~DiskManager();

private:
    std::vector<Student *> temp_list;
};
class StudentManager
{
public:
    StudentManager();
    void ADD(std::string id, std::string name, std::string age);
    void FIND(std::string field_name, std::string relationship_operator, std::string value);
    void REMOVE(std::string field_name, std::string relationship_operator, std::string value);
    void STOP();
    void PrintStudentInfo(Student *s);
    bool process(std::vector<std::string> command_tokens);
    ~StudentManager();

private:
    std::vector<Student *> student_list; // pointers to Student
};

#endif
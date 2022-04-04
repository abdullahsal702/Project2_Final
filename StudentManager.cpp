#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "StudentManager.h"

// --------------  Student Class -------------- //

Student::Student(unsigned int id, std::string name, unsigned short age)
{
    student_id = id;
    student_name = name;
    student_age = age;
}

unsigned int Student::GetStudentID()
{
    return student_id;
}

std::string Student::GetStudentName()
{
    return student_name;
}

unsigned short Student::GetStudentAge()
{
    return student_age;
}

Student::~Student(){};

// --------------  MyParser Class -------------- //

MyParser::MyParser(){};

MyParser::~MyParser(){};

std::vector<std::string> MyParser::operator()(std::string command)
{
    std::vector<std::string> command_tokens;

    error = false;
    // deal with entering a blank input
    if (command == "")
    {
        std::cerr << "Warning: Invalid Input" << std::endl;
        error = true;
        command_tokens[0] = "ERROR";
        return command_tokens;
    }

    GeneralCommandSplit(command_tokens, command);
    CommandErrorCheck(command_tokens);
    if (command_tokens[0] == "ADD")
    {
        GetAddTokens(command_tokens);
        AddErrorCheck(command_tokens);
    }
    if (command_tokens[0] == "FIND" || command_tokens[0] == "REMOVE")
    {
        GetConditionalTokens(command_tokens);
        ConditionalErrorCheck(command_tokens);
    }
    if (command_tokens[0] == "STOP")
        return command_tokens;

    if (error == true)
        command_tokens[0] = "ERROR";
    // debugging
    // for (int i = 0; i < command_tokens.size(); i++)
    // {
    //     std::cout << command_tokens[i] << std::endl;
    // }
    return command_tokens;
};

// splits the command only by separators
void MyParser::GeneralCommandSplit(std::vector<std::string> &command_tokens, std::string command)
{
    int index = 0;
    std::string token = "";
    int start_index = 0;
    while (index < command.length())
    {
        if (command[index] == ' ' || command[index] == '\t')
            index++;
        else
        {
            // the second condition makes sure that it doesn't collect a " at the end of the string
            if (command[index] == '\"' && index != command.length() - 1)
            {
                start_index = index;
                index++;
                while (command[index] != '\"')
                    index++;
                token = command.substr(start_index, (index - start_index + 1));
                command_tokens.push_back(token);
                // sets index to go past quotation
                index++;
            }
            else
            {
                start_index = index;
                while (!(command[index] == ' ' || command[index] == '\t'))
                    index++;
                token = command.substr(start_index, (index - start_index));
                command_tokens.push_back(token);
            }
        }
    }
}

// For the ADD function
void MyParser::GetAddTokens(std::vector<std::string> &command_tokens)
{
    RemoveQuotes(2, command_tokens);
    if (command_tokens.size() != 4)
    {
        std::cerr << "Warning: Invalid Input" << std::endl;
        error = true;
    }
}

void MyParser::RemoveQuotes(int index, std::vector<std::string> &command_tokens)
{
    // removes "" from both ends of name string
    std::string name = "";
    int last_index = command_tokens[index].length() - 1;
    if (command_tokens[index][0] == '\"' && command_tokens[index][last_index] == '\"')
    {
        for (int i = 1; i < command_tokens[index].length() - 1; i++)
            name += command_tokens[index][i];
        command_tokens[index] = name;
    }
}

// For the FIND and REMOVE functions
void MyParser::GetConditionalTokens(std::vector<std::string> &command_tokens)
{
    // merge the entire conditional into one unspaced string instead of dealing with spaces (examples) id <900 , id < 900, and id< 900
    if (command_tokens.size() > 2)
    {
        for (int i = 2; i < command_tokens.size(); i++)
            command_tokens[1] += command_tokens[i];
        while (command_tokens.size() > 2)
            command_tokens.pop_back();
    }

    // Question: when user inputs if name and age, does it have to be lowercase? - if not use tolower() function
    // All of type string because our vector is a string type
    std::string field_name = "";
    std::string relationship_operator = "";
    std::string value = "";
    int operator_index = 0;
    for (int i = 0; i < command_tokens[1].length(); i++)
    {
        if (command_tokens[1][i] == '>' || command_tokens[1][i] == '<' || command_tokens[1][i] == '=')
        {
            relationship_operator = command_tokens[1][i];
            operator_index = i;
        }
    }
    command_tokens.push_back(relationship_operator);
    for (int i = operator_index + 1; i < command_tokens[1].length(); i++)
        value += command_tokens[1][i];
    command_tokens.push_back(value);
    for (int i = 0; i < operator_index; i++)
        field_name += command_tokens[1][i];
    command_tokens[1] = field_name;

    // just in case the last value element has quotes
    RemoveQuotes(3, command_tokens);
    if (command_tokens.size() != 4)
    {
        std::cerr << "Warning: Invalid Input" << std::endl;
        error = true;
    }
}

void MyParser::NameErrorCheck(std::string name)
{
    for (int i = 0; i < name.length(); i++)
    {
        if (isdigit(name[i]) || name[i] == '\"')
        {
            std::cerr << "Warning: Invalid Name" << std::endl;
            error = true;
        }
    }
}

void MyParser::IdErrorCheck(std::string id)
{
    for (int i = 0; i < id.length(); i++)
    {
        if (!(isdigit(id[i])))
        {
            std::cerr << "Warning: Invalid ID" << std::endl;
            error = true;
            return;
        }
    }
    // check overflow for unsigned int
    long long temp_id = (long long)stoi(id);
    if (temp_id > 4294967295)
    {
        std::cerr << "Warning: ID is too large" << std::endl;
        error = true;
        return;
    }
}

void MyParser::AgeErrorCheck(std::string age)
{
    for (int i = 0; i < age.length(); i++)
    {
        if (!(isdigit(age[i])))
        {
            std::cerr << "Warning: Invalid Age" << std::endl;
            error = true;
            return;
        }
    }
    // check overflow for unsigned short
    int temp_age = stoi(age);
    if (temp_age > 65535)
    {
        std::cerr << "Warning: Age is too large" << std::endl;
        error = true;
        return;
    }
}

void MyParser::AddErrorCheck(std::vector<std::string> &command_tokens)
{
    IdErrorCheck(command_tokens[1]);
    NameErrorCheck(command_tokens[2]);
    AgeErrorCheck(command_tokens[3]);
}

void MyParser::ConditionalErrorCheck(std::vector<std::string> &command_tokens)
{
    if (command_tokens[1] != "id" && command_tokens[1] != "age" && command_tokens[1] != "name")
    {
        std::cerr << "Warning: Invalid Field Name" << std::endl;
        error = true;
        return;
    }
    if (command_tokens[2] != ">" && command_tokens[2] != "<" && command_tokens[2] != "=")
    {
        std::cerr << "Warning: Invalid Relationship Operator" << std::endl;
        error = true;
        return;
    }
    if (command_tokens[1] == "id")
        IdErrorCheck(command_tokens[3]);
    if (command_tokens[1] == "age")
        AgeErrorCheck(command_tokens[3]);
    if (command_tokens[1] == "name")
    {
        NameErrorCheck(command_tokens[3]);
        if (command_tokens[2] != "=")
        {
            std::cerr << "Warning: Invalid Relationship Operator for Name" << std::endl;
            error = true;
            return;
        }
    }
}

void MyParser::CommandErrorCheck(const std::vector<std::string> &command_tokens)
{
    // is it an error if the command is not all caps - if not then use toupper() function
    if (command_tokens[0] != "ADD" && command_tokens[0] != "FIND" && command_tokens[0] != "REMOVE" && command_tokens[0] != "STOP")
    {
        std::cerr << "Warning: Invalid Command" << std::endl;
        error = true;
    }
}

// --------------  DiskManager Class -------------- //
DiskManager::DiskManager(){};
DiskManager::~DiskManager()
{
    for (int i = 0; i < temp_list.size(); i++)
    {
        temp_list[i] = nullptr;
    }
};

void DiskManager::WriteToFile(std::string filename, std::vector<Student *> &list)
{
    std::ofstream record_file(filename);
    for (int i = 0; i < list.size(); i++)
    {
        record_file << std::to_string(list[i]->GetStudentID()) << "\t" << list[i]->GetStudentName() << "\t" << std::to_string(list[i]->GetStudentAge()) << std::endl;
    }
}

std::vector<Student *> DiskManager::ReadFile(std::string filename)
{
    // figure out what to do if file has multiple blank lines
    std::ifstream record_file("records.txt");
    if (record_file.fail())
        return temp_list;

    // cheks if the file is empty
    if (record_file.peek() == std::ifstream::traits_type::eof())
        return temp_list;

    std::string line;

    // the memory allocated here will be deleted when we delete the student_list pointers in StudentManager class, deleting these pointers now will lead to errors
    while (getline(record_file, line))
    {
        std::vector<int> space_indexes;
        std::string id = "";
        std::string name = "";
        std::string age = "";
        for (int i = 0; i < line.length(); i++)
        {
            if (line[i] == '\t')
                space_indexes.push_back(i);
        }

        for (int i = 0; i < space_indexes[0]; i++)
            id += line[i];

        for (int i = space_indexes[0] + 1; i < space_indexes[space_indexes.size() - 1]; i++)
            name += line[i];

        for (int i = space_indexes[space_indexes.size() - 1] + 1; i < line.length(); i++)
            age += line[i];

        unsigned int new_id = (unsigned int)stoi(id);
        unsigned short new_age = (unsigned short)stoi(age);
        Student *s = new Student(new_id, name, new_age);
        temp_list.push_back(s);
    }

    // for (int i = 0; i < temp_list.size(); i++)
    //     std::cout << temp_list[i]->GetStudentID() << " " << temp_list[i]->GetStudentName() << " " << temp_list[i]->GetStudentAge() << std::endl;

    return temp_list;
}

// --------------  StudentManager Class -------------- //
StudentManager::StudentManager()
{
    DiskManager disk;
    student_list = disk.ReadFile("records.txt");
};
StudentManager::~StudentManager()
{
    for (int i = 0; i < student_list.size(); i++)
    {
        delete student_list[i];
        student_list[i] = nullptr;
    }
};

void StudentManager::ADD(std::string id, std::string name, std::string age)
{
    // converting from string to int values
    unsigned int new_id = (unsigned int)stoi(id);
    unsigned short new_age = (unsigned short)stoi(age);
    Student *s = new Student(new_id, name, new_age);
    student_list.push_back(s);
}

void StudentManager::FIND(std::string field_name, std::string relationship_operator, std::string value)
{
    if (field_name == "id")
    {
        unsigned int new_value = (unsigned int)stoi(value);
        if (relationship_operator == ">")
        {
            for (int i = 0; i < student_list.size(); i++)
            {
                if (student_list[i]->GetStudentID() > new_value)
                    PrintStudentInfo(student_list[i]);
            }
        }
        if (relationship_operator == "<")
        {
            for (int i = 0; i < student_list.size(); i++)
            {
                if (student_list[i]->GetStudentID() < new_value)
                    PrintStudentInfo(student_list[i]);
            }
        }
        if (relationship_operator == "=")
        {
            for (int i = 0; i < student_list.size(); i++)
            {
                if (student_list[i]->GetStudentID() == new_value)
                    PrintStudentInfo(student_list[i]);
            }
        }
    }

    if (field_name == "age")
    {
        unsigned short new_value = (unsigned short)stoi(value);
        if (relationship_operator == ">")
        {
            for (int i = 0; i < student_list.size(); i++)
            {
                if (student_list[i]->GetStudentAge() > new_value)
                    PrintStudentInfo(student_list[i]);
            }
        }
        if (relationship_operator == "<")
        {
            for (int i = 0; i < student_list.size(); i++)
            {
                if (student_list[i]->GetStudentAge() < new_value)
                    PrintStudentInfo(student_list[i]);
            }
        }
        if (relationship_operator == "=")
        {
            for (int i = 0; i < student_list.size(); i++)
            {
                if (student_list[i]->GetStudentAge() == new_value)
                    PrintStudentInfo(student_list[i]);
            }
        }
    }

    if (field_name == "name")
    {
        if (relationship_operator == "=")
        {
            for (int i = 0; i < student_list.size(); i++)
            {
                if (student_list[i]->GetStudentName() == value)
                    PrintStudentInfo(student_list[i]);
            }
        }
    }
}

void StudentManager::PrintStudentInfo(Student *s)
{
    std::cout << std::to_string(s->GetStudentID()) << "\t" << s->GetStudentName() << "\t" << std::to_string(s->GetStudentAge()) << std::endl;
}

void StudentManager::REMOVE(std::string field_name, std::string relationship_operator, std::string value)
{
    if (field_name == "id")
    {
        unsigned int new_value = (unsigned int)stoi(value);
        if (relationship_operator == ">")
        {
            for (int i = 0; i < student_list.size(); i++)
            {
                if (student_list[i]->GetStudentID() > new_value)
                {
                    student_list.erase(student_list.begin() + i);
                    i--;
                }
            }
        }
        if (relationship_operator == "<")
        {
            for (int i = 0; i < student_list.size(); i++)
            {
                if (student_list[i]->GetStudentID() < new_value)
                {
                    student_list.erase(student_list.begin() + i);
                    i--;
                }
            }
        }
        if (relationship_operator == "=")
        {
            for (int i = 0; i < student_list.size(); i++)
            {
                if (student_list[i]->GetStudentID() == new_value)
                {
                    student_list.erase(student_list.begin() + i);
                    i--;
                }
            }
        }
    }

    if (field_name == "age")
    {
        unsigned short new_value = (unsigned short)stoi(value);
        if (relationship_operator == ">")
        {
            for (int i = 0; i < student_list.size(); i++)
            {
                if (student_list[i]->GetStudentAge() > new_value)
                {
                    student_list.erase(student_list.begin() + i);
                    i--;
                }
            }
        }
        if (relationship_operator == "<")
        {
            for (int i = 0; i < student_list.size(); i++)
            {
                if (student_list[i]->GetStudentAge() < new_value)
                {
                    student_list.erase(student_list.begin() + i);
                    i--;
                }
            }
        }
        if (relationship_operator == "=")
        {
            for (int i = 0; i < student_list.size(); i++)
            {
                if (student_list[i]->GetStudentAge() == new_value)
                {
                    student_list.erase(student_list.begin() + i);
                    i--;
                }
            }
        }
    }

    if (field_name == "name")
    {
        if (relationship_operator == "=")
        {
            for (int i = 0; i < student_list.size(); i++)
            {
                if (student_list[i]->GetStudentName() == value)
                {
                    student_list.erase(student_list.begin() + i);
                    i--;
                }
            }
        }
    }
}

bool StudentManager::process(std::vector<std::string> command_tokens)
{
    if (command_tokens[0] == "ERROR")
        return true;
    DiskManager disk;
    if (command_tokens[0] == "ADD")
        ADD(command_tokens[1], command_tokens[2], command_tokens[3]);
    if (command_tokens[0] == "FIND")
        FIND(command_tokens[1], command_tokens[2], command_tokens[3]);
    if (command_tokens[0] == "REMOVE")
        REMOVE(command_tokens[1], command_tokens[2], command_tokens[3]);
    if (command_tokens[0] == "STOP")
    {
        disk.WriteToFile("records.txt", student_list);
        return false;
    }
    return true;
}

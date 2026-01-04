/**
 * Functions for reading and writing a configuration file.
 * Comments must begin with character '#' in a new line.
 * The actual definition for string only suports a single word.
 * 
 * WriteLine and Assign can be modified or they can be implemented for new types.
 * 
 * @file config_file.h
 * @author F. Barber
 */

#ifndef EXTERNAL_CONFIG_FILE_H_
#define EXTERNAL_CONFIG_FILE_H_

#include<tuple>
#include<fstream>
#include<map>
#include<vector>
#include<string>
#include<stdexcept>
#include<algorithm>
#include<iostream>
namespace CFG {

const std::wstring DEF_CH = L"=";

// Level of debugging. 0 - No message, 1 - Only error, 2 - Show assignments
inline int debug_level = 1;

/**
 * Set the debugging level
 * @param level 0 - No message, 1 - Only errors, 2 - Show assignments
 */
inline void SetDebugLevel(int level)
{
    debug_level = level;
}

//----------------------------------------------------------------------
// Write configuration to file

/**
 * Function for writing a particular variable to file.
 * A specialization can be written for particular types.
 * 
 * @todo WriteVar instead of WriteLine: More flexible but more tedious.
 * 
 * @tparam T Type of the variable to store
 * @param out Output file
 * @param name name of the variable
 * @param var Variable with the value to store
 */
template<typename T>
inline void WriteLine(std::wostream & out, const std::wstring & name, T var)
{
    out << name << DEF_CH << var << std::endl;
}

template<typename T>
inline void WriteLine(std::wostream & out, const std::wstring & name, const std::vector<T> & var)
{
    if(var.size() == 0)
    {
        out << "[]";
        return;
    }
    out << name << DEF_CH + "[";
    
    for(unsigned i = 0; i < var.size() - 1; i++)
        out << var[i] << ",";
    out << var.back() << "]" << std::endl;
}

//-----------

/**
 * Recursive function for traversing the list of variadic templates.
 * Direct case.
 */
inline void WriteFileRec(std::wostream & out, const std::vector<std::wstring> & names, size_t i)
{
}

/**
 * Recursive function for traversing the list of variadic templates
 */
template<typename T, typename... TypeList>
inline void WriteFileRec(std::wostream & out, const std::vector<std::wstring> & names, size_t i, T & var, TypeList & ...tl)
{
    WriteLine(out, names.at(i), var);
    WriteFileRec(out, names, i+1, tl...);
}

/**
 * Write a configuration to file
 * 
 * @param out Output file
 * @param names Vector with the names of the parameters
 * @param tl parameters to be stored
 */
template<typename... TypeList>
inline void WriteFile(std::wostream & out, const std::vector<std::wstring> & names, TypeList & ...tl)
{
    if(sizeof...(tl) == names.size() )
        WriteFileRec(out, names, 0, tl...);
    else
        if(debug_level > 0)
            std::wcout << " Number of vars doesn't match number of names in WriteFile. Vars: " << sizeof...(tl) << " Var names: " << names.size() << std::endl;
}

//----------------------------------------------------------------------
// Read configuration from file

/**
 * Function for assigning the value of a particular variable.
 * A specialization can be written for particular types.
 * 
 * @param var Variable where the value has to be stored
 * @param name name of the variable
 * @param value Value to assign
 */
template<typename T>
inline void Assign(T & var, const std::wstring & name, const std::wstring & value)
{
    if(debug_level > 0)
        std::Wcout << L"Assignment not defined for " << name << std::endl;
}

inline void Assign(int & var, const std::wstring & name, const std::wstring & value)
{
    var = std::stoi(value);
}

inline void Assign(unsigned & var, const std::wstring & name, const std::wstring & value)
{
    var = std::stoul(value);
}

inline void Assign(size_t & var, const std::wstring & name, const std::wstring & value)
{
    var = std::stoul(value);
}

inline void Assign(bool & var, const std::wstring & name, const std::wstring & value)
{
    std::wstring value2(value);
    std::transform(value2.begin(), value2.end(), value2.begin(), ::tolower);
    var = ( (value2 == L"true") ? true : false);
}

inline void Assign(float & var, const std::wstring & name, const std::wstring & value)
{
    var = std::stof(value);
}

inline void Assign(double & var, const std::wstring & name, const std::wstring & value)
{
    var = std::stod(value);
}

inline void Assign(std::wstring & var, const std::wstring & name, const std::wstring & value)
{
    var = value;
}


template<typename T>
inline void Assign(std::vector<T> & var, const std::wstring & name, const std::wstring & value)
{
    if(value.at(0) != L'[')
        throw std::invalid_argument(name + DEF_CH + value);
    // Erase the vector
    var.clear();
    std::wstring value2 = value;
    value2.erase(0,1);
    
    T x;
    std::wstring val;
    size_t pos = value2.find_first_of(L",]");
    while(pos != std::wstring::npos)
    {
        val = value2.substr(0, pos);
        Assign(x, name, val);
        var.push_back(x);
        value2.erase(0, pos+1);
        
        pos = value2.find_first_of(L",]");
    }
}

//-----------

/**
 * Function for erasing all the blanks in the line.
 * Can be changed for avoiding the blanks inside "" to allow strings with blanks.
 * 
 * @param str Input string
 * @return Returns the string without the blanks (space characters).
 */
inline std::wstring EraseBlanks(const std::wstring & str)
{
    std::wstring res;
    std::copy_if(str.begin(), str.end(), std::back_inserter(res), [](wchar_t c) {return c != L' '; });
    
    return res;
}

/**
 * Recursive function for assigning the data to the parameters
 * Direct case
 */
inline void AssignValues(const std::vector<std::wstring> & names, std::map<std::wstring, std::wstring> & data, size_t i)
{
}

/**
 * Recursive function for assigning the data to the parameters
 * 
 * @param names Vector with the names of the parameters
 * @param data Map with name and value of the data read from the file in string format
 * @param i Position of the name to be processed
 * @param var Head of the parameter list
 * @param tl parameters where data must be stored
 */
template<typename T, typename... TypeList>
inline void AssignValues(const std::vector<std::wstring> & names, std::map<std::wstring, std::wstring> & data, size_t i, T & var, TypeList & ...tl)
{
    auto it = data.find(names.at(i) );
    if(it != data.end() )
    {
        if(debug_level > 1)
            std::wcout << it->first << L"<-" << it->second << std::endl;
        Assign(var, it->first, it->second);
        data.erase(it);
    }
    else
        if(debug_level > 1)
            std::wcout << names.at(i) << L" not assigned." << std::endl;
    
    AssignValues(names, data, i+1, tl...);
}

/**
 * Read a configuration from file
 * 
 * @param in Input file
 * @param names Vector with the names of the parameters
 * @param tl parameters where data must be stored
 */
template<typename... TypeList>
inline std::map<std::wstring, std::wstring>  ReadFile(std::wistream & in, const std::vector<std::wstring> & names, TypeList & ...tl)
{
    std::tuple<TypeList...> tuple_list(tl...);
    std::wstring line;
    std::map<std::wstring, std::wstring> data;
    
    while(getline(in, line) )
    {
        line = EraseBlanks(line);
        // The line is a comment
        if(line.size() == 0 || line.at(0) == '#')
            continue;
        
        size_t pos_eq = line.find(DEF_CH);
        if(pos_eq > line.size() )
        {
            if(debug_level > 0)
                std::wcout << L"Missing '" + DEF_CH + L"' in: " << line << std::endl;
            continue;
        }
        std::wstring name_line = line.substr(0, pos_eq);
        std::wstring value_line = line.substr(pos_eq + 1);
        data[name_line] = value_line;
    }

    if(sizeof...(tl) == names.size() )
        AssignValues(names, data, 0, tl...);
    else
        if(debug_level > 0)
            std::wcout << L" Number of vars doesn't match number of names in ReadFile. Vars: " << sizeof...(tl) << L" Var names: " << names.size() << std::endl;
    
    return data;
}

}


#endif //!EXTERNAL_CONFIG_FILE_H_
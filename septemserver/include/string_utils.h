#ifndef STRING_UTILS_HPP__
#define STRING_UTILS_HPP__
#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>





static inline void strip_path(std::string& path)
{
    std::replace_if(path.begin() , path.end() ,  
            [] (const char& c) { return !isalpha(c); ;},'_');
}


static inline std::string strip_path_copy(std::string path)
{
    std::replace_if(path.begin() , path.end() ,  
            [] (const char& c) { return !isalpha(c); ;},'_');
    return path;
}


static inline std::vector<std::string> split(const std::string &s, char delim) {
  std::stringstream ss(s);
  std::string item;
  std::vector<std::string> elems;
  while (std::getline(ss, item, delim)) {
    elems.push_back(item);
    // elems.push_back(std::move(item)); // if C++11 (based on comment from @mchiasson)
  }
  return elems;
}

static inline std::string vector_to_string( const std::vector<std::string> &s, char delim)
{
    std::stringstream ss;
    for (auto str : s)
    {
        ss << str << delim;
    }
    return ss.str();
}

static inline bool startsWith(const std::string& haystack, const std::string& needle) {
    return needle.length() <= haystack.length() 
        && equal(needle.begin(), needle.end(), haystack.begin());
}

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

// trim from start (copying)
static inline std::string ltrimmed(std::string s) {
    ltrim(s);
    return s;
}

// trim from end (copying)
static inline std::string rtrimmed(std::string s) {
    rtrim(s);
    return s;
}

// trim from both ends (copying)
static inline std::string trimmed(std::string s) {
    trim(s);
    return s;
}

static inline std::string int_to_string(int value)
{
    std::stringstream ss;
    const char * const ones[20] = {"zero", "one", "two", "three","four","five","six","seven",
    "eight","nine","ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen","seventeen",
    "eighteen","nineteen"};
    const char * const tens[10] = {"", "ten", "twenty", "thirty","forty","fifty","sixty","seventy",
    "eighty","ninety"};

    if(value<0)
    {
        ss<<"minus ";
        ss << int_to_string(-value);
    }
    else if(value>=1000)
    {
        ss << int_to_string(value/1000);
        ss<<" thousand";
        if(value % 1000)
        {
            if(value % 1000 < 100)
            {
                ss << " and";
            }
            ss << " " ;
            ss << int_to_string(value % 1000);
        }
    }
    else if(value >= 100)
    {
        ss << int_to_string(value / 100);
        ss <<" hundred";
        if(value % 100)
        {
            ss << " and ";
            ss << int_to_string (value % 100);
        }
    }
    else if(value >= 20)
    {
        ss << tens[value / 10];
        if(value % 10)
        {
            ss << " ";
            ss << int_to_string(value % 10);
        }
    }
    else
    {
        ss<<ones[value];
    }
    return ss.str();
}
/*
static bool is_email_valid(const std::string& email)
{
   // define a regular expression
   const std::regex pattern
      ("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");

   // try to match the string with the regular expression
   return std::regex_match(email, pattern);
}

template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )
{
    size_t size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    std::unique_ptr<char[]> buf( new char[ size ] ); 
    snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}


static void sanitizepassword(std::string& phrase)
{
	for (unsigned int x = 0; x < phrase.length(); x++)
      if(phrase[x] >= 33 && phrase[x] <= 126)
      {phrase[x] = '*';}
}
*/
/*
static int last_position=0;

int find_new_text(std::ifstream &infile) {

   infile.seekg(0, std::ios::end);
   int filesize = infile.tellg();

   // check if the new file started
   if(filesize < last_position){
      last_position=0;
   }  
   // read file from last position  untill new line is found 

   for(int n=last_position;n<filesize;n++) {

      infile.seekg( last_position, std::ios::beg);
      char  test[256]; 
      infile.getline(test, 256);
      last_position = infile.tellg();
      std::cout << "Char: "  << test <<"Last position " << last_position<<  std::endl;
      // end of file 
      if(filesize == last_position){
        return filesize;
      } 


  }

  return 0;
}

*/
/*
int main() {

  for(;;) {
    std::ifstream infile("filename");
    int current_position = find_new_text(infile);
    sleep(1);
  } 

} 
*/

#endif
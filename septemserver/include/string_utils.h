// ==========================================================================
// string_utils.h
//
// Copyright (C) 2018 Kenneth Thompson, All Rights Reserved.
// This file is covered by the MIT Licence:
//
// Permission is hereby granted, free of charge, to any person obtaining a 
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the 
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.
// ==========================================================================
#pragma once
#ifndef STRING_UTILS_HPP__
#define STRING_UTILS_HPP__

#include <crypto++/cryptlib.h>
#include <crypto++/sha.h>
#include <crypto++/filters.h>
#include <crypto++/base64.h>



static inline std::string SHA256HashString(std::string aString){
    std::string digest;
    CryptoPP::SHA256 hash;

    CryptoPP::StringSource foo(aString, true,
    new CryptoPP::HashFilter(hash,
      new CryptoPP::Base64Encoder (
         new CryptoPP::StringSink(digest))));

    return digest;
}

namespace {
std::string const default_chars = 
    "abcdefghijklmnaoqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
}

static inline std::string random_string(size_t len = 15, std::string const &allowed_chars = default_chars) {
    std::mt19937_64 gen { std::random_device()() };

    std::uniform_int_distribution<size_t> dist { 0, allowed_chars.length()-1 };

    std::string ret;

    std::generate_n(std::back_inserter(ret), len, [&] { return allowed_chars[dist(gen)]; });
    return ret;
}


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

static inline std::string GetPathFromFileLocation(const std::string& str)
{
  size_t found;
  //cout << "Splitting: " << str << endl;
  found=str.find_last_of("/\\");
  //cout << " folder: " << str.substr(0,found) << endl;
  //cout << " file: " << str.substr(found+1) << endl;
  
  return str.substr(0,found);
}

static inline std::string get_str_between_two_str(const std::string &s,
        const std::string &start_delim,
        const std::string &stop_delim)
{
    unsigned first_delim_pos = s.find(start_delim);
    unsigned end_pos_of_first_delim = first_delim_pos + start_delim.length();
    unsigned last_delim_pos = s.find(stop_delim);

    return s.substr(end_pos_of_first_delim,
            last_delim_pos - end_pos_of_first_delim);
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
// ==========================================================================
// client.hpp
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
#ifndef _CLIENT_HPP_
#define _CLIENT_HPP_

#include <functional>
#include <memory>
#include <vector>


class world_context;
class character;
class connection;
class playerobj;
    
namespace boost { namespace asio {
    class io_service;
}}


class client
    : public std::enable_shared_from_this<client>
{
public :
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    client(
        boost::asio::io_service  &io_service
      , std::shared_ptr<world_context>  ctx);

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    ~client();

    //* =====================================================================
    /// \brief Sets the connection on which this client operates.
    //* =====================================================================
    void set_connection(std::shared_ptr<connection> const &new_connection);


    std::shared_ptr<client> get_shared_ptr() {
        return shared_from_this();
    }

    //* =====================================================================
    /// \brief Disconnects the client from the server.
    //* =====================================================================
    void disconnect();

    //* =====================================================================
    /// \brief Sets up a callback for if the client's connection dies.
    //* =====================================================================
    void on_connection_death(std::function<void ()> const &callback);
    
    // ======================================================================
    // DATA
    // ======================================================================
    void data(std::string const &data);
    
    void send(std::string const &data);
    
    void send(char const * text);
    
    playerobj * get_player();
    
    //void on_output(std::string& s);
    
    void associate_player(const std::string& pname, bool bloggedIn=false);

private :
    class impl;
    std::shared_ptr<impl> pimpl_;
   
};

#endif
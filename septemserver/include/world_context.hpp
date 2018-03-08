// ==========================================================================
// world_context.hpp
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
#ifndef _WORLD_CONTEXT_HPP_
#define _WORLD_CONTEXT_HPP_



//class account;
//class character;
class client;

class world_context
{
public :
    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    virtual ~world_context() {}

    //* =====================================================================
    /// \brief Retrieves a list of clients currently connected to septem.
    //* =====================================================================
    virtual std::vector<std::shared_ptr<client>> get_clients() = 0;

    //* =====================================================================
    /// \brief Adds a client to the list of clients currently connected
    /// to Paradice.
    //* =====================================================================
    virtual void add_client(std::shared_ptr<client> const &cli) = 0;

    //* =====================================================================
    /// \brief Removes a client from the list of clients currently
    /// connected to Paradice.
    //* =====================================================================
    virtual void remove_client(std::shared_ptr<client> const &cli) = 0;

    //* =====================================================================
    /// \brief For all clients, updates their lists of names.
    //* =====================================================================
  //  virtual void update_names() = 0;

    //* =====================================================================
    /// \brief Returns how a character appears to others, including prefix
    /// and suffix.
    //* =====================================================================
   // virtual std::string get_moniker(std::shared_ptr<character> const &ch) = 0;

    //* =====================================================================
    /// \brief Loads an account from a specific account name and returns it.
    /// Returns an empty shared_ptr<> if there was no account with that name
    /// found.
    //* =====================================================================
   // virtual std::shared_ptr<account> load_account(
   //     std::string const &name) = 0;

    //* =====================================================================
    /// \brief Saves an account.
    //* =====================================================================
    //virtual void save_account(std::shared_ptr<account> const &acct) = 0;

    //* =====================================================================
    /// \brief Loads a character that is identified by the passed name and
    /// returns it.  Returns an empty shared_ptr<> if there was no character
    /// with that name found.
    //* =====================================================================
   // virtual std::shared_ptr<character> load_character(
   //     std::string const &name) = 0;

    //* =====================================================================
    /// \brief Saves a character.
    //* =====================================================================
   // virtual void save_character(std::shared_ptr<character> const &ch) = 0;

    //* =====================================================================
    /// \brief Enacts a server shutdown.
    //* =====================================================================
    virtual void shutdown() = 0;
  
};



#endif
#ifndef _CONTEXT_IMPL_HPP_
#define _CONTEXT_IMPL_HPP_

//#include "paradice/context.hpp"
#include "net/server.hpp"

#include "world_context.hpp"

// ==========================================================================
// Paradice Context
//
// Copyright (C) 2009 Matthew Chaplain, All Rights Reserved.
//
// Permission to reproduce, distribute, perform, display, and to prepare
// derivitive works from this file under the following conditions:
//
// 1. Any copy, reproduction or derivitive work of any part of this file
//    contains this copyright notice and licence in its entirety.
//
// 2. The rights granted to you under this license automatically terminate
//    should you attempt to assert any patent claims against the licensor
//    or contributors, which in any way restrict the ability of any party
//    from using this software or portions thereof in any form under the
//    terms of this license.
//
// Disclaimer: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
//             KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//             WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
//             PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
//             OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
//             OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
//             OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//             SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// ==========================================================================

#include <memory>
#include <vector>


//* =========================================================================
/// \brief Describes the context in which a septem server runs.
//* =========================================================================
class context_impl : public world_context
{
public :
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    context_impl(
        boost::asio::io_service                       &io_service
      , std::shared_ptr<net::server>             server
      , std::shared_ptr<boost::asio::io_service::work> work);
    
    //* =====================================================================
    /// \brief Denstructor
    //* =====================================================================
    virtual ~context_impl();
    
	
    //* =====================================================================
    /// \brief Retrieves a list of clients currently connected to septem.
    //* =====================================================================
    virtual std::vector<std::shared_ptr<client>> get_clients();

    //* =====================================================================
    /// \brief Adds a client to the list of clients currently connected
    /// to septem.
    //* =====================================================================
    virtual void add_client(std::shared_ptr<client> const &cli);

    //* =====================================================================
    /// \brief Removes a client from the list of clients currently
    /// connected to septem.
    //* =====================================================================
    virtual void remove_client(std::shared_ptr<client> const &cli);
    
    //* =====================================================================
    /// \brief For all clients, updates their lists of names.
    //* =====================================================================
    //virtual void update_names();

    //* =====================================================================
    /// \brief Returns how a character appears to others, including prefix
    /// and suffix.
    //* =====================================================================
  //  virtual std::string get_moniker(
  //      std::shared_ptr<septem::character> const &ch);

    //* =====================================================================
    /// \brief Loads an account from a specific account name and returns it.
    /// Returns an empty shared_ptr<> if there was no account with that name
    /// found.
    //* =====================================================================
   // virtual std::shared_ptr<account> load_account(
   //     std::string const &name);

    //* =====================================================================
    /// \brief Saves an account.
    //* =====================================================================
   // virtual void save_account(std::shared_ptr<account> const &acct);

    //* =====================================================================
    /// \brief Loads a character that is identified by the passed name and
    /// returns it.  Returns an empty shared_ptr<> if there was no character
    /// with that name found.
    //* =====================================================================
    //virtual std::shared_ptr<character> load_character(
    //    std::string const &name);

    //* =====================================================================
    /// \brief Saves a character.
    //* =====================================================================
   // virtual void save_character(std::shared_ptr<septem::character> const &ch);
   
    
    //* =====================================================================
    /// \brief Enacts a server shutdown.
    //* =====================================================================
    virtual void shutdown();
    
  
	

private :
    struct impl;
    std::shared_ptr<impl> pimpl_;
};

#endif

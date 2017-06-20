#include "server/context_impl.hpp"
#include "server/connectionsm.hpp"

//#include "paradice/character.hpp"
#include "server/client.hpp"
//#include "hugin/user_interface.hpp"
#include <boost/asio/strand.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/filesystem.hpp>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>


namespace fs = boost::filesystem;




    
// ==========================================================================
// CONTEXT_IMPL IMPLEMENTATION STRUCTURE
// ==========================================================================
struct context_impl::impl
{
    impl(
        boost::asio::io_service                       &io_service
      , std::shared_ptr<net::server>             server
      , std::shared_ptr<boost::asio::io_service::work> work)
      : strand_(io_service)
      , server_(server)
      , work_(work)
    {
    }
    
    // ======================================================================
    // ADD_CLIENT
    // ======================================================================
    void add_client(std::shared_ptr<client> const &cli)
    {
        clients_.push_back(cli);
        
    }

    // ======================================================================
    // REMOVE_CLIENT
    // ======================================================================
    void remove_client(std::shared_ptr<client> const &cli)
    {
        clients_.erase(
            std::remove(
                clients_.begin()
              , clients_.end()
              , cli)
          , clients_.end());
    }
	

    // ======================================================================
    // UPDATE_NAMES
    // ======================================================================
    /*
    void update_names()
    {
        std::vector<std::string> names;
        
        for (auto &cur_client : clients_)
        {
            auto character = cur_client->get_character();
            
            if (character != NULL)
            {
                auto name = get_character_address(character);
                
                if (!name.empty())
                {
                    names.push_back(name);
                }
            }
        }
        
        for (auto &cur_client : clients_)
        {
            auto user_interface = cur_client->get_user_interface();
            
            if (user_interface != NULL)
            {
                user_interface->update_wholist(names);
            }
        }
    }
	*/




    boost::asio::strand                            strand_;
    std::shared_ptr<net::server>             server_;
    std::shared_ptr<boost::asio::io_service::work> work_;
    std::vector<std::shared_ptr<client>> clients_;
   // server::connectionsm                           csm_;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
context_impl::context_impl(
    boost::asio::io_service                        &io_service
  , std::shared_ptr<net::server>              server
  , std::shared_ptr<boost::asio::io_service::work>  work)
    : pimpl_(new impl(io_service, server, work))
{
}
    
// ==========================================================================
// DESTRUCTOR
// ==========================================================================
context_impl::~context_impl()
{
}
    
// ==========================================================================
// GET_CLIENTS
// ==========================================================================
std::vector<std::shared_ptr<client>> context_impl::get_clients()
{
    return pimpl_->clients_;
}

// ==========================================================================
// ADD_CLIENT
// ==========================================================================
void context_impl::add_client(std::shared_ptr<client> const &cli)
{
    pimpl_->strand_.dispatch([this, cli]{pimpl_->add_client(cli);});
}

// ==========================================================================
// REMOVE_CLIENT
// ==========================================================================
void context_impl::remove_client(std::shared_ptr<client> const &cli)
{
    pimpl_->strand_.dispatch([this, cli]{pimpl_->remove_client(cli);});
}

 



// ==========================================================================
// SHUTDOWN
// ==========================================================================
void context_impl::shutdown()
{
    pimpl_->work_.reset();
    pimpl_->server_->shutdown();
}


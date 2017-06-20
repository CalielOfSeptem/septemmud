#ifndef _CLIENT_HPP_
#define _CLIENT_HPP_

#include <functional>
#include <memory>
#include <vector>

class world_context;
class character;
class connection;
    
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

/*
    //* =====================================================================
    /// \brief Sets the account that the client is currently using.
    //* =====================================================================
    void set_account(std::shared_ptr<account> const &acc);

    //* =====================================================================
    /// \brief Retrieves the account that the client is currently using.
    //* =====================================================================
    std::shared_ptr<account> get_account() const;

    //* =====================================================================
    /// \brief Sets the character that the client is currently using.
    //* =====================================================================
    void set_character(std::shared_ptr<character> const &ch);

    //* =====================================================================
    /// \brief Retreives the character that the client is currently using.
    //* =====================================================================
    std::shared_ptr<character> get_character() const;
    */

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
    
 

private :
    class impl;
    std::shared_ptr<impl> pimpl_;
    
   
};

#endif
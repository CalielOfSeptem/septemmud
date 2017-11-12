#include "server/client.hpp"
#include "server/connection.hpp"
#include "server/context_impl.hpp"
//#include "utils/password_validator.hpp"
#include <telnetpp/telnetpp.hpp>
#include "terminalpp/encoder.hpp"
#include <boost/asio/strand.hpp>
#include <boost/format.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/algorithm/string.hpp>
#include "welcome_screen.hpp"
#include <algorithm>
#include <cstdio>
#include <deque>
#include <mutex>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <boost/tokenizer.hpp>
#include "string_utils.h"
#include <iomanip>
#include "server/connectionsm.hpp"
#include "script_entities/playerobj.h"
#include "entity_manager.h"

namespace fs = boost::filesystem;

using namespace boost::algorithm;



// ==========================================================================
// CLIENT IMPLEMENTATION STRUCTURE
// ==========================================================================
class client::impl : public std::enable_shared_from_this<client::impl>
{
    public :
    // ======================================================================
    // CONSTRUCTOR
    // ======================================================================
    impl(
        client                  &self,
        boost::asio::io_service &io_service,
        std::shared_ptr<world_context> ctx)
      : self_(self),
        strand_(io_service),
        context_(ctx)
    {

    }
    
    // ======================================================================
    // SET_CONNECTION
    // ======================================================================
    void set_connection(std::shared_ptr<connection> cnx)
    {
        
        connection_ = cnx;
                // CONNECTION CALLBACKS
        

        connection_->on_data_read(
            [this](std::string const &data)
            {
                std::unique_lock<std::mutex> lock(dispatch_queue_mutex_);
                dispatch_queue_.push_back(bind(&impl::on_data, this, data));
                strand_.post(bind(&impl::dispatch_queue, shared_from_this()));
            });
        display_welcome();
        //send("What is your name?\r\n");
    }
    // TODO:  Add reasonable limit to the size of a command being received to avoid overrun attack
    void on_data(std::string const &data)
    {

        std::string::size_type pos = 0; // Must initialize
        std::string str = data;
        while ( ( pos = str.find ("\n",pos) ) != std::string::npos )
        {
            str.erase ( pos, 1 );
        }
                    //data.erase(std::remove(data.begin(), data.end(), '\n'), data.end());
        //send(str);
        
        for( unsigned int x = 0; x < data.length(); x++ )
        {
            char c = data[x];
            //printf("[%d]", c);

            if( c ==  '\n' )
            {
                //cout << "Command Received! : " << input_buffer.c_str() << endl;
                on_input_entered(input_buffer);
                
                prompt();
                input_buffer = "";
                
            }
            else if( c >= 32 && c <= 126 )
            {
                input_buffer += data[x];
                
            }
            else if( c == 127 )
            {
                if( input_buffer.length() > 0 )
                {
                    input_buffer = input_buffer.substr(0, input_buffer.size()-1);
                }
            }
            //printf("[%d]", c);
        }

    }
    
    void prompt()
    {
        send("\r\n> ");
    }
    
    void display_welcome()
    {
        send(logo_data);
      //  send("\r\nPlease enter your player name:");
     //   send("\r\nTo request an account visit http://www.septemmud.com.");
      //  prompt();
        player_uid = random_string();
        associate_player(player_uid, false);
    }
    
    void on_output(const std::string& s)
    {
        //playerobj * p = entity_manager::Instance().get_player("caliel");
        //if( m_player != NULL )
       // {
            
       // }
       send(s);
    }
    
    void associate_player(const std::string& pname, bool bloggedIn)
    {
        entity_manager::Instance().load_player(pname, bloggedIn);
        
        playerobj * p = entity_manager::Instance().get_player(pname);
        p->onOutput = [this](std::string const& msg)->void { this->on_output(msg); };
        m_player = p;
        p->set_client(&self_);
        on_input_entered("");
        /*
        playerobj * p = entity_manager::Instance().get_player("caliel");
        if( p == NULL )
        {
            entity_manager::Instance().load_player("caliel");
            p = entity_manager::Instance().get_player("caliel");
           // p->onOutput = std::bind(&client::on_output, this, _1)
        }
        assert( p != NULL );
        //auto a = std::bind(&client::impl::on_output, this, _1);
        p->onOutput = [this](std::string const& msg)->void { this->on_output(msg); };
        m_player = p;
        */
    }
    /*
    // ======================================================================
    // SET_ACCOUNT
    // ======================================================================
    void set_account(std::shared_ptr<account> acc)
    {
        account_ = acc;
    }

    // ======================================================================
    // GET_ACCOUNT
    // ======================================================================
    std::shared_ptr<account> get_account()
    {
        return account_;
    }
     */
   
     // ======================================================================
    // DISCONNECT
    // ======================================================================
    void disconnect()
    {
        connection_->disconnect();
    }

    // ======================================================================
    // ON_CONNECTION_DEATH
    // ======================================================================
    void on_connection_death(std::function<void ()> const &callback)
    {
        connection_->on_socket_death(callback);
    }
    
    void send(std::string const& data)
    {
        connection_->write(data);
    }
    

    
    std::string input_buffer;
    
    

private :
    playerobj * m_player;
        // ======================================================================
    // REMOVE_DUPLICATE_ACCOUNTS
    // ======================================================================
   // void remove_duplicate_accounts(std::shared_ptr<account> acc)
   // {
        /*
        std::vector<std::shared_ptr<client>> clients_to_remove;
        auto clients = context_->get_clients();

        for (auto current_client : clients)
        {
            auto current_account = current_client->get_account();

            if (current_account != NULL && current_account != acc)
            {
                if (current_account->get_name() == acc->get_name())
                {
                    clients_to_remove.push_back(current_client);
                }
            }
        }

        for (auto current_client : clients_to_remove)
        {
            current_client->disconnect();
            context_->remove_client(current_client);
        }
        */
   // }
    
    // ======================================================================
    // ON_INPUT_ENTERED
    // ======================================================================
    void on_input_entered(std::string const &input)
    {
  
        on_command(input);
 
    }
    
    // ======================================================================
    // ON_COMMAND
    // ======================================================================
    void on_command(std::string const &input)
    {
        puts(input.c_str());
        std::string cmd = input;//to_lower_copy(input);
        trim(cmd);
        entity_manager::Instance().do_command(this->m_player, cmd);
    }
    
    

    
    client                                 &self_;
    boost::asio::strand                     strand_;
    std::shared_ptr<world_context>          context_;
   // std::shared_ptr<account>                account_;
   // std::shared_ptr<character>              character_;

    std::shared_ptr<connection>             connection_;

    std::mutex                              dispatch_queue_mutex_;
    std::deque<std::function<void ()>>      dispatch_queue_;

    std::string                             player_uid;
private :
    // ======================================================================
    // DISPATCH_QUEUE
    // ======================================================================
    void dispatch_queue()
    {
        std::function<void ()> fn;

        std::unique_lock<std::mutex> lock(dispatch_queue_mutex_);

        while (!dispatch_queue_.empty())
        {
            fn = dispatch_queue_.front();
            dispatch_queue_.pop_front();
            lock.unlock();

            fn();

            lock.lock();
        }
    }
};


// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
client::client(
    boost::asio::io_service &io_service
  , std::shared_ptr<world_context> ctx)
{
    pimpl_ = std::make_shared<impl>(
        std::ref(*this), std::ref(io_service), ctx);
    
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
client::~client()
{
}

// ==========================================================================
// SET_CONNECTION
// ==========================================================================
void client::set_connection(std::shared_ptr<connection> const &cnx)
{
    //puts("set_connection");
    pimpl_->set_connection(cnx);
}


// ==========================================================================
// DISCONNECT
// ==========================================================================
void client::disconnect()
{
    pimpl_->disconnect();
}

// ==========================================================================
// ON_CONNECTION_DEATH
// ==========================================================================
void client::on_connection_death(std::function<void ()> const &callback)
{
    pimpl_->on_connection_death(callback);
}


// ======================================================================
// DATA
// ======================================================================
void client::data(std::string const &data)
{
    std::cout << data << std::endl;
}
void client::send(char const* text)
{
    send(std::string(text));
}
void client::send(std::string const& data)
{
    pimpl_->send(data);
}
void client::associate_player(const std::string& name, bool bLoggedIn)
{
    pimpl_->associate_player(name, bLoggedIn);
}

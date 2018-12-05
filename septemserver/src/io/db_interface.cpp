#include "stdafx.h"
#include "io/db_interface.h"
#include <iostream>
//#include  <sqlite3.h>
#include <SQLiteCpp/SQLiteCpp.h>
#include "global_settings.h"

void db_interface::test()
{
	try
	{
		// Open a database file
		std::string _dbfullpath = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH);
		_dbfullpath += "/septemdb.db3";
		SQLite::Database    db(_dbfullpath);
		
		// Compile a SQL query, containing one parameter (index 1)
		SQLite::Statement   query(db, "SELECT * FROM logins");
		
		// Bind the integer value 6 to the first parameter of the SQL query
		//query.bind(1, 6);
		
		// Loop to execute the query step by step, to get rows of result
		while (query.executeStep())
		{
			// Demonstrate how to get some typed column value
			int         id      = query.getColumn(0);
			const char* value   = query.getColumn(1);
			int         size    = query.getColumn(2);
			
			std::cout << "row: " << id << ", " << value << ", " << size << std::endl;
		}
	}
	catch (std::exception& e)
	{
		//std::cout << "exception: " << e.what() << std::endl;
		auto log = spd::get("main");
		std::stringstream ss;
		ss << "Error accessing database in function << " << __FUNCTION__ << ":" << e.what();
		log->error(ss.str());
	}
}

/**
 * @brief Returns a long int representing the timestamp of the last logon for the target player
 * @param player_name Name of the player
 * @return 
 */
long int db_interface::get_last_logon(std::string& player_name)
{
	try
	{
		// Open a database file
		std::string _dbfullpath = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH);
		_dbfullpath += "/septemdb.db3";
		SQLite::Database    db(_dbfullpath);
		
		// Compile a SQL query, containing one parameter (index 1)
		SQLite::Statement   query(db, "SELECT * FROM logins WHERE player_name = :player_name AND event='EVENT_LOGON' ORDER BY time DESC LIMIT 1");
		
		// Bind the integer value 6 to the first parameter of the SQL query
		query.bind(":player_name", player_name);

		// Loop to execute the query step by step, to get rows of result
		while (query.executeStep())
		{
			// Demonstrate how to get some typed column value
			int         id      = query.getColumn(0);
			const char* value   = query.getColumn(1);
			int         size    = query.getColumn(2);
			long int			time 	= query.getColumn(3);
			std::cout << "row: " << id << ", " << value << ", " << size << ", " << time << std::endl;
			return time;
		}
	}
	catch (std::exception& e)
	{
		//std::cout << "exception: " << e.what() << std::endl;
		auto log = spd::get("main");
		std::stringstream ss;
		ss << "Error accessing database in function << " << __FUNCTION__ << ":" << e.what();
		log->error(ss.str());
	}
}

/**
 * @brief Returns a long int representing the timestamp of the last logoff for the target player
 * @param player_name Name of the player
 * @return 
 */
long int db_interface::get_last_logoff(std::string& player_name)
{
	try
	{
		// Open a database file
		std::string _dbfullpath = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH);
		_dbfullpath += "/septemdb.db3";
		SQLite::Database    db(_dbfullpath);
		
		// Compile a SQL query, containing one parameter (index 1)
		SQLite::Statement   query(db, "SELECT * FROM logins WHERE player_name = :player_name AND event='EVENT_LOGOFF' ORDER BY time DESC LIMIT 1");
		
		// Bind the integer value 6 to the first parameter of the SQL query
		query.bind(":player_name", player_name);

		// Loop to execute the query step by step, to get rows of result
		while (query.executeStep())
		{
			// Demonstrate how to get some typed column value
			int         id      = query.getColumn(0);
			const char* value   = query.getColumn(1);
			int         size    = query.getColumn(2);
			long int			time 	= query.getColumn(3);
			std::cout << "row: " << id << ", " << value << ", " << size << ", " << time << std::endl;
			return time;
		}
	}
	catch (std::exception& e)
	{
		//std::cout << "exception: " << e.what() << std::endl;
		auto log = spd::get("main");
		std::stringstream ss;
		ss << "Error accessing database in function << " << __FUNCTION__ << ":" << e.what();
		log->error(ss.str());
	}
}
void db_interface::on_player_connection_event(std::string& player_name, ConnectionEvent evt, std::string& ip)
{
	try
	{
		// Open a database file
		std::string _dbfullpath = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH);
		_dbfullpath += "/septemdb.db3";
		//SQLite::Database    db(_dbfullpath);
		SQLite::Database    db(_dbfullpath, SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
		// Compile a SQL query, containing one parameter (index 1)
		SQLite::Statement   query(db, "INSERT INTO logins VALUES (NULL, :player_name, :event_code, :timestamp, :ip_address )");
		
		boost::posix_time::ptime t = boost::posix_time::microsec_clock::local_time();

		// convert to int64_t
		//boost::posix_time::ptime myEpoch(boost::gregorian::date(1970,boost::gregorian::Jan,1));
		//boost::posix_time::time_duration myTimeFromEpoch = t - myEpoch;
		//boost::int64_t myTimeAsInt = myTimeFromEpoch.ticks();
		std::time_t t1 = std::time(0);  // t is an integer type

		/// Bind the integer value 6 to the first parameter of the SQL query
		query.bind(":player_name", player_name);
		if( evt == ConnectionEvent::PLAYER_LOGON )
			query.bind(":event_code", "EVENT_LOGON");
		else
			query.bind(":event_code", "EVENT_LOGOFF");
			
		query.bind(":timestamp", static_cast<boost::int64_t>(t1));
		query.bind(":ip_address", ip);
		int nb = query.exec ();
	}
	catch (std::exception& e)
	{
		//std::cout << "exception: " << e.what() << std::endl;
		auto log = spd::get("main");
		std::stringstream ss;
		ss << "Error accessing database in function << " << __FUNCTION__ << ":" << e.what();
		log->error(ss.str());
	}
}

void db_interface::log_event(LOGLEVEL log_level, std::string script_path, std::string msg)
{
	try
	{
		// Open a database file
		std::string _dbfullpath = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH);
		_dbfullpath += "/septemdb.db3";
		//SQLite::Database    db(_dbfullpath);
		SQLite::Database    db(_dbfullpath, SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
		// Compile a SQL query, containing one parameter (index 1)
		SQLite::Statement   query(db, "INSERT INTO entity_log VALUES (NULL, :script_path, :log_level, :detail, :timestamp )");
		
		boost::posix_time::ptime t = boost::posix_time::microsec_clock::local_time();

		// convert to int64_t
		//boost::posix_time::ptime myEpoch(boost::gregorian::date(1970,boost::gregorian::Jan,1));
		//boost::posix_time::time_duration myTimeFromEpoch = t - myEpoch;
		//boost::int64_t myTimeAsInt = myTimeFromEpoch.ticks();
		std::time_t t1 = std::time(0);  // t is an integer type

		/// Bind the integer value 6 to the first parameter of the SQL query
		query.bind(":script_path", script_path);
		switch( log_level )
		{
			case LOGLEVEL::LOGLEVEL_ERROR:
			{
				query.bind(":log_level", "ERROR");
				break;
			}
			case LOGLEVEL::LOGLEVEL_CRITICAL:
			{
				query.bind(":log_level", "CRITICAL");
				break;
			}
			case LOGLEVEL::LOGLEVEL_INFO:
			{
				query.bind(":log_level", "INFO");
				break;
			}
			case LOGLEVEL::LOGLEVEL_DEBUG:
			{
				query.bind(":log_level", "DEBUG");
				break;
			}
		}
			

			
		query.bind(":timestamp", static_cast<boost::int64_t>(t1));
		query.bind(":detail", msg);
		int nb = query.exec ();
	}
	catch (std::exception& e)
	{
		//std::cout << "exception: " << e.what() << std::endl;
		auto log = spd::get("main");
		std::stringstream ss;
		ss << "Error accessing database in function << " << __FUNCTION__ << ":" << e.what();
		log->error(ss.str());
	}
}


bool db_interface::add_or_update_room_in_cache(const std::string& room_path, RoomState rs, bool update )
{
    try
    {
        // Open a database file
        std::string _dbfullpath = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH);
        _dbfullpath += "/septemdb.db3";
        //SQLite::Database    db(_dbfullpath);
        SQLite::Database    db(_dbfullpath, SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
        // Compile a SQL query, containing one parameter (index 1)
        std::string sql_to_execute = "";
        if( update )
        {
            sql_to_execute = "UPDATE room_cache SET room_state = :room_state WHERE 'room_path' = :room_path";
        }
        else
            sql_to_execute = "INSERT INTO room_cache VALUES (NULL, :room_path, :room_state)";
        
        SQLite::Statement   query(db, sql_to_execute);

        query.bind(":room_path", room_path);
        query.bind(":room_state", static_cast<int>(rs) );

        int nb = query.exec ();
    }    
    catch (std::exception& e)
    {
        //std::cout << "exception: " << e.what() << std::endl;
        auto log = spd::get("main");
        std::stringstream ss;
		ss << "Error accessing database in function << " << __FUNCTION__ << ":" << e.what();
        log->error(ss.str());
        return false;
    }
    return true;
}

 void db_interface::update_room_cache(const std::string& room_path, RoomState rs)
 {
     if( !db_interface::room_exists_in_cache(room_path) )
     {
         // add it.
         db_interface::add_or_update_room_in_cache(room_path, rs, false);
     }
     else
     {
         // update it.
         db_interface::add_or_update_room_in_cache(room_path, rs, true);
     }
 }
 
 bool db_interface::room_exists_in_cache(const std::string& room_path)
 {
	try
	{
		// Open the database file
		std::string _dbfullpath = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH);
		_dbfullpath += "/septemdb.db3";
		SQLite::Database    db(_dbfullpath);
		
		// Compile a SQL query, containing one parameter (index 1)
		//SQLite::Statement   query(db, "SELECT COUNT(*) FROM room_cache WHERE room_path = :room_path");
                
                std::string sql = "SELECT COUNT(*) FROM ";
                sql += "room_cache";
                sql += " WHERE room_path = '";
                sql += room_path;
                sql += "';";
		
		// Bind the integer value 6 to the first parameter of the SQL query
		//query.bind(":room_path", room_path);
                
                int count = db.execAndGet(sql);
                //std::cout << "COUNT(*) = "<< count << '\n';
                if( count > 0 )
                    return true;
                else 
                    return false;
               
	}
	catch (std::exception& e)
	{
		//std::cout << "exception: " << e.what() << std::endl;
		auto log = spd::get("main");
		std::stringstream ss;
		ss << "Error accessing database in function << " << __FUNCTION__ << ":" << e.what();
		log->error(ss.str());
	}
        return false;
 }

 bool db_interface::save_global_setting(const std::string& key, std::string& val)
 {
	 try
	 {
		 // Open a database file
		 std::string _dbfullpath = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH);
		 _dbfullpath += "/septemdb.db3";
		 SQLite::Database    db(_dbfullpath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
		 SQLite::Statement   query(db, "INSERT INTO global_settings VALUES (:key, :value )");
		 query.bind(":key", key);
		 query.bind(":value", val);
		 int nb = query.exec();
	 }
	 catch (std::exception& e)
	 {
		 auto log = spd::get("main");
		 std::stringstream ss;
		 ss << "Error accessing database in function << " << __FUNCTION__ <<  ":" << e.what();
		 log->error(ss.str());
	 }
	 return false;
 }

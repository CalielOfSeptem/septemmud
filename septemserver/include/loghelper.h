#ifndef _LOGHELPER_H_
#define _LOGHELPER_H_
#include <plog/Log.h>
#include <plog/Formatters/FuncMessageFormatter.h>
#include <list>

namespace plog
{

    
    class MyFormatter
    {
    public:
        static util::nstring header() // This method returns a header for a new file. In our case it is empty.
        {
            return util::nstring();
        }

        static util::nstring format(const Record& record) // This method returns a string from a record.
        {
            
            tm t;
            util::localtime_s(&t, &record.getTime().time);

            util::nstringstream ss;
            ss << t.tm_year + 1900 << "-" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mon + 1 << "-" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mday << " ";
            ss << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_hour << ":" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_min << ":" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_sec << "." << std::setfill(PLOG_NSTR('0')) << std::setw(3) << record.getTime().millitm << " ";
            //ss << std::setfill(PLOG_NSTR(' ')) << std::setw(5) << std::left << severityToString(record.getSeverity()) << " ";
            //ss << "[" << record.getTid() << "] ";
            //ss << "[" << record.getFunc() << "@" << record.getLine() << "] ";
            //ss << record.getMessage() << "\n";
            
            //util::nstringstream ss;
            ss << record.getMessage() << "\n"; // Produce a simple string with a log message.

            return ss.str();
        }
    };
    
    template<class Formatter> // Typically a formatter is passed as a template parameter.
    class MyAppender : public IAppender // All appenders MUST inherit IAppender interface.
    {
    public:
        virtual void write(const Record& record) // This is a method from IAppender that MUST be implemented.
        {
            util::nstring str = Formatter::format(record); // Use the formatter to get a string from a record.

            m_messageList.push_back(str); // Store a log message in a list.
        }

        std::list<util::nstring>& getMessageList()
        {
            return m_messageList;
        }

    private:
        std::list<util::nstring> m_messageList;
    };
    



    
}

#endif
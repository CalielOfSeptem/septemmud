// ==========================================================================
// septem.hpp
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
#ifndef SEPTEM_HPP_
#define SEPTEM_HPP_

#include <boost/asio/io_service.hpp>


//* =========================================================================
/// \brief A class that implements the main engine for the Se7en server.
/// \param io_service - The engine will be run within using the dispatch
///        mechanisms of this object.
/// \brief work - A "work" object.  While this is not reset, the threads'
///        run() methods will not terminate.  Resetting this work object
///        is part of the shutdown protocol.
/// \brief port - The server will be set up on this port number.
//* =========================================================================
class septem
{
public :
    septem(
        boost::asio::io_service                        &io_service
      , std::shared_ptr<boost::asio::io_service::work>  work
      , unsigned int                                    port);
    
private :
    struct impl;
    std::shared_ptr<impl> pimpl_;
};

#endif
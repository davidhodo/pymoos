///////////////////////////////////////////////////////////////////////////
//
//   MOOS - Mission Oriented Operating Suite 
//  
//   A suit of Applications and Libraries for Mobile Robotics Research 
//   Copyright (C) 2001-2005 Massachusetts Institute of Technology and 
//   Oxford University. 
//    
//   This software was written by Paul Newman at MIT 2001-2002 and Oxford 
//   University 2003-2005. email: pnewman@robots.ox.ac.uk. 
//      
//   This file is part of a  MOOS Core Component. 
//        
//   This program is free software; you can redistribute it and/or 
//   modify it under the terms of the GNU General Public License as 
//   published by the Free Software Foundation; either version 2 of the 
//   License, or (at your option) any later version. 
//          
//   This program is distributed in the hope that it will be useful, 
//   but WITHOUT ANY WARRANTY; without even the implied warranty of 
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
//   General Public License for more details. 
//            
//   You should have received a copy of the GNU General Public License 
//   along with this program; if not, write to the Free Software 
//   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 
//   02111-1307, USA. 
//
//   The XPC classes in MOOS are modified versions of the source provided 
//   in "Making UNIX and Windows NT Talk" by Mark Nadelson and Thomas Haga 
//
//////////////////////////    END_GPL    //////////////////////////////////
#include <MOOSLIB/XPCTcpSocket.h>
#ifdef _WIN32
#else
#include <sys/time.h>
#endif

#include <string>
#include <boost/python.hpp>

using namespace boost::python;

bool SendMessage( XPCTcpSocket& sock, long int py_data, int length)
{   
    return (sock.iSendMessage( (void*)py_data, length));
}


BOOST_PYTHON_MODULE( XPCTcpSocket )
{
    class_<XPCTcpSocket>( "XPCTcpSocket", init<long int>())
        .def( init<long int>() )
		.def( "vBindSocket", &XPCTcpSocket::vBindSocket )
		.def( "iSendMessage", &XPCTcpSocket::iSendMessage )
		.def( "vConnect", &XPCTcpSocket::vConnect )
        .def( "SendMessage", SendMessage)
	;
}




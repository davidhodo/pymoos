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
//////////////////////////    END_GPL    //////////////////////////////////
// MOOSCommObject.cpp: implementation of the CMOOSCommObject class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _WIN32
    #pragma warning(disable : 4786)
    #pragma warning(disable : 4503)
#endif

#include <MOOSGenLib/MOOSGenLib.h>
#include <MOOSLIB/MOOSCommPkt.h>
#include <MOOSLIB/XPCTcpSocket.h>
#include <MOOSLIB/MOOSCommObject.h>
#include <MOOSLIB/MOOSException.h>
#include <iostream>
#include <memory>

#include <boost/python.hpp>

class CMOOSCommObjectProxy : public CMOOSCommObject
{
    public:
        CMOOSCommObjectProxy(){};
        bool ProxySendPkt (XPCTcpSocket* pSocket,CMOOSCommPkt & PktTx)
        {
            return SendPkt( pSocket, PktTx );
        };

        bool ProxyReadPkt (XPCTcpSocket* pSocket,CMOOSCommPkt & PktRx,int nSecondsTimeOut )
        {
            return ReadPkt( pSocket, PktRx );
        };

        bool ProxySendMsg (XPCTcpSocket *pSocket,CMOOSMsg &Msg)
        {
            return SendMsg( pSocket, Msg );
        };

        bool ProxyReadMsg (XPCTcpSocket *pSocket,CMOOSMsg &Msg)
        {
            return ReadMsg( pSocket, Msg );
        };    

};


using namespace boost::python;

//bool SendMsg(CMOOSCommObject obj, XPCTcpSocket *pSocket,CMOOSMsg &Msg);
//bool SendMsg(CMOOSCommObject obj, XPCTcpSocket *pSocket,CMOOSMsg &Msg)
//{
	////return (obj.SendMsg( pSocket, Msg ));	
    //return true;
//}

//bool ReadMsg(CMOOSCommObject obj, XPCTcpSocket *pSocket,CMOOSMsg &Msg);
//bool ReadMsg(CMOOSCommObject obj, XPCTcpSocket *pSocket,CMOOSMsg &Msg)
//{
	////return (obj.ReadMsg( pSocket, Msg ));	
//}

//bool SendPkt(CMOOSCommObject obj, XPCTcpSocket* pSocket,CMOOSCommPkt & PktTx);
//bool SendPkt(CMOOSCommObject obj, XPCTcpSocket* pSocket,CMOOSCommPkt & PktTx)
//{
	////return ( obj.SendPkt( pSocket, PktTx) );
//}

//bool ReadPkt(CMOOSCommObject obj, XPCTcpSocket* pSocket,CMOOSCommPkt & PktRx,int nSecondsTimeOut = -1);
//bool ReadPkt(CMOOSCommObject obj, XPCTcpSocket* pSocket,CMOOSCommPkt & PktRx,int nSecondsTimeOut )
//{
	////return ( obj.ReadPkt( pSocket, PktRx) );
//}


BOOST_PYTHON_MODULE( CMOOSCommObject )
{
	//object a = class_<CMOOSCommObject> ("MOOSCommObject", "MOOS Communications Object." )
		//.def( "SendPkt", SendPkt )	
		//.def( "ReadPkt", ReadPkt )	
		//.def( "SendMsg", SendMsg )	
		//.def( "ReadMsg", ReadMsg )	
	//;
    object a = class_<CMOOSCommObjectProxy> ("MOOSCommObject", "MOOS Communications Object." )
        .def( "SendPkt", &CMOOSCommObjectProxy::ProxySendPkt )	
        .def( "ReadPkt", &CMOOSCommObjectProxy::ProxyReadPkt )	
        .def( "SendMsg", &CMOOSCommObjectProxy::ProxySendMsg )	
        .def( "ReadMsg", &CMOOSCommObjectProxy::ProxyReadMsg )	
    ;
}


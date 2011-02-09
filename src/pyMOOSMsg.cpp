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
// MOOSMsg.cpp: implementation of the CMOOSMsg class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#pragma warning(disable : 4786)
#endif

#include <MOOSGenLib/MOOSGenLib.h>
#include <MOOSLIB/MOOSMsg.h>
#include <MOOSLIB/MOOSException.h>
#include <MOOSLIB/MOOSGlobalHelper.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cstring>

#include <boost/python.hpp>

using namespace boost::python;

BOOST_PYTHON_MODULE( CMOOSMsg )
{
	class_<CMOOSMsg> ("MOOSMsg", "MOOS Message Class." )
		.def( init<char, std::string, double, double>() )
		.def( init<char, std::string, std::string, double>() )
		.def( "IsDataType", &CMOOSMsg::IsDataType )
		.def( "IsDouble", &CMOOSMsg::IsDouble )
		.def( "IsString", &CMOOSMsg::IsString )
		.def( "IsSkewed", &CMOOSMsg::IsSkewed )
		.def( "IsYoungerThan", &CMOOSMsg::IsYoungerThan )
		.def( "IsType", &CMOOSMsg::IsType )
		.def( "GetTime", &CMOOSMsg::GetTime )
		.def( "GetDouble", &CMOOSMsg::GetDouble )
		.def( "GetString", &CMOOSMsg::GetString )
		.def( "GetKey", &CMOOSMsg::GetKey )
		.def( "GetName", &CMOOSMsg::GetName )
		.def( "GetSource", &CMOOSMsg::GetSource )
		.def( "GetSourceAux", &CMOOSMsg::GetSourceAux )
		.def( "SetSourceAux", &CMOOSMsg::SetSourceAux )
		.def( "GetCommunity", &CMOOSMsg::GetCommunity )
		.def( "GetAsString", &CMOOSMsg::GetAsString )
		.def( "Trace", &CMOOSMsg::Trace )
		.def( "SetDouble", &CMOOSMsg::SetDouble )
		
		//Attributes
		.def_readwrite( "m_dfVal", &CMOOSMsg::m_dfVal )
		.def_readwrite( "m_sSrc", &CMOOSMsg::m_sSrc )
		.def_readwrite( "m_sKey", &CMOOSMsg::m_sKey )
	;
}

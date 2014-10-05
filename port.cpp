/**
 * port.cpp - 
 * @author: Jonathan Beard
 * @version: Thu Aug 28 09:55:47 2014
 * 
 * Copyright 2014 Jonathan Beard
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <algorithm>
#include <cassert>
#include <typeinfo>
#include <typeindex>
#include <sstream>
#include <iostream>

#include <cxxabi.h>
#include "fifo.hpp"
#include "kernel.hpp"
#include "port.hpp"
#include "portexception.hpp"

Port::Port( Kernel *k ) : kernel( k )
{
}

Port::~Port()
{
}

const std::type_index&
Port::getPortType( const std::string port_name )
{
   const auto ret_val( portmap.find( port_name ) );
   if( ret_val == portmap.cend() )
   {
      throw PortNotFoundException( "Port not found for name \"" + port_name + "\"" );
   }
   return( (*ret_val).second.type );
}

FIFO&
Port::operator[]( const std::string port_name )
{
   const auto ret_val( portmap.find( port_name ) );
   if( ret_val == portmap.cend() )
   {
      throw PortNotFoundException( "Port not found for name \"" + port_name + "\"" );
   }
   return( *((*ret_val).second.fifo)  );
}

bool
Port::hasPorts()
{
   return( portmap.size() > 0 ? true : false );
}


std::vector< std::weak_ptr< FIFO > >::iterator
Port::begin()
{
   return( portlist.begin() );
}

std::vector< std::weak_ptr< FIFO > >::iterator
Port::end()
{
   return( portlist.end() );
}

std::size_t
Port::count()
{
   return( (std::size_t) portlist.size() );
}

PortInfo&
Port::getPortInfoFor( const std::string port_name )
{
   const auto ret_val( portmap.find( port_name ) );
   if( ret_val == portmap.cend() )
   {
      std::stringstream ss;
      ss << "Port not found for name \"" << port_name << "\"";
      throw PortNotFoundException( ss.str() );
   }
   return( (*ret_val).second );
}

PortInfo&
Port::getPortInfo()
{
   if( portmap.size() > 1 )
   {
      /** TODO: extract kernel name to go here too **/
      throw PortNotFoundException( "One port expected, more than one found!" );
   }
   auto pair( portmap.begin() );
   return( (*pair).second );
}

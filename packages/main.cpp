#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <array>
#include "utility.hpp"
#include "dct.tcc"
#include <raft>
#include <vector>
#include <cstring>
#include <algorithm>
#include <raftio>

float array_A[ 64 ] = 
{
  -76.0, -73.0, -67.0, -62.0, -58.0, -67.0, -64.0, -55.0,
  -65.0, -69.0, -73.0, -38.0, -19.0, -43.0, -59.0, -56.0,
  -66.0, -69.0, -60.0, -15.0,  16.0, -24.0, -62.0, -55.0,
  -65.0, -70.0, -57.0, -6.0 ,  26.0, -22.0, -58.0, -59.0,
  -61.0, -67.0, -60.0, -24.0,  -2.0, -40.0, -60.0, -58.0,
  -49.0, -63.0, -68.0, -58.0, -51.0, -60.0, -70.0, -53.0,
  -43.0, -57.0, -64.0, -69.0, -73.0, -67.0, -63.0, -45.0,
  -41.0, -49.0, -59.0, -60.0, -63.0, -52.0, -50.0, -34.0
};


int
main( int argc, char **argv )
{
   using type_t   = float;
   using matrix_t = raft::matrix< type_t, 8 >;
   using vector_t = std::vector< matrix_t >;
   using reader_t = raft::read_each< matrix_t >;
   using dct_t    = raft::dct< type_t, raft::x88 >;

   vector_t v( 3 );
   std::for_each( v.begin(), v.end(), 
      [&]( matrix_t &m )
      {
         std::memcpy( m.arr, array_A, sizeof( type_t ) * 64 );
      });
    
   auto edge_a = raft::map.link(
      raft::kernel::make< reader_t >( v.begin(), v.end() ),
      raft::kernel::make< dct_t >() );
   raft::map.link( &edge_a.getDst(),
                   raft::kernel::make< raft::print< matrix_t, '\n' > >() ); 
   raft::map.exe();
   return( EXIT_FAILURE );
}
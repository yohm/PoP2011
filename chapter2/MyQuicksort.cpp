#include "omp.h"
#include <vector>
#include <boost/random.hpp>

// ---------------------------------------
void Swap( std::vector<int>::iterator it1, std::vector<int>::iterator it2) {
  int temp = *it1;
  *it1 = *it2;
  *it2 = temp;
}

// ---------------------------------------
void ShuffleVector( std::vector<int> &vec) {
  boost::mt19937 zGen;  // functor which generates a random number distributing [0, 2^32-1].
  for( size_t i=0; i<vec.size(); i++) {
    Swap( vec.begin()+i, vec.begin()+(zGen()%vec.size()) );
    //Swap( static_cast< std::vector<int>::iterator >( &vec[i]), static_cast< std::vector<int>::iterator >( &vec[zGen() % vec.size()]) );
  }
}

// ---------------------------------------
void ShuffleVectorPartly( std::vector<int> &vec, size_t nNumShuffle) {
  boost::mt19937 zGen;
  for( size_t i=0; i<nNumShuffle; i++) {
    Swap( vec.begin()+(zGen()%vec.size()), vec.begin()+(zGen()%vec.size()) );
  }
}

// -----------------------------------------
void MyQuickSort( std::vector<int>::iterator begin, std::vector<int>::iterator end) {

  if( distance( begin, end) <= 1 ) { return; }
  
  std::vector<int>::iterator pivot = begin;
  // Swap( begin, pivot);   // since the first element is selected to be the pivot, swapping the pivot position is not necessary
  
  std::vector<int>::iterator it = begin;
  it++;
  std::vector<int>::iterator last = begin;
  for( ; it != end; it++) {
    if( *it <= *pivot) {
      last++;
      Swap( it, last);
    }
  }
  Swap( pivot, last);
  
  MyQuickSort( begin, last);
  MyQuickSort( ++last, end);
}

// -----------------------------------------
void MyParallelQuickSort( std::vector<int>::iterator begin, std::vector<int>::iterator end) {
  if( distance( begin, end) <= 1 ) { return; }
  
  std::vector<int>::iterator pivot = begin;
  std::vector<int>::iterator it = begin;
  it++;
  std::vector<int>::iterator last = begin;
  for( ; it != end; it++) {
    if( *it <= *pivot) {
      last++;
      Swap( it, last);
    }
  }
  Swap( pivot, last);

  omp_set_num_threads( 2);
  #pragma omp parallel
  {
    #pragma omp sections
    {
      #pragma omp section
      {
	MyQuickSort( begin, last);
      }
      #pragma omp section
      {
	MyQuickSort( ++last, end);
      }
    }
  }
}

// -----------------------------------------
void MySmarterParallelQuickSort( std::vector<int>::iterator begin, std::vector<int>::iterator end) {
  const int nPartsize = 64;
  if( distance( begin, end) <= nPartsize ) { MyQuickSort( begin, end); }

  MyQuickSort( begin, begin+nPartsize);
  Swap( begin, begin+nPartsize/2);
  
  std::vector<int>::iterator pivot = begin;
  std::vector<int>::iterator it = begin+nPartsize;
  std::vector<int>::iterator last = begin+nPartsize/2;
  for( ; it != end; it++) {
    if( *it <= *pivot) {
      last++;
      Swap( it, last);
    }
  }
  Swap( pivot, last);

  omp_set_num_threads( 2);
  #pragma omp parallel
  {
    #pragma omp sections
    {
      #pragma omp section
      {
	MyQuickSort( begin, last);
      }
      #pragma omp section
      {
	MyQuickSort( ++last, end);
      }
    }
  }
}

// -----------------------------------------
void MyBubbleSort( std::vector<int>::iterator begin, std::vector<int>::iterator end) {
  for( std::vector<int>::iterator target = begin; target != end; target++) {
    std::vector<int>::iterator temp_min = target;
    for( std::vector<int>::iterator it = target; it != end; it++) {
      if( *it < *temp_min) { temp_min = it; }
    }
    Swap( temp_min, target);
  }
}

#pragma once

#include <cassert>

template< class T >
class cCircularBuffer {

public:
    ~cCircularBuffer();
    cCircularBuffer( int size );

    void Write( T iObject );

    T  operator[] ( int iIndex );

    T  Back();

    int Count() const;

    int Size() const;

private:
    T*  mBuffer;
    int mSize;
    int mWriteIdx;
    bool mFirstFilling;
};


template< class T >
cCircularBuffer< T >::~cCircularBuffer()
{
    delete[] mBuffer;
}


template< class T >
cCircularBuffer< T >::cCircularBuffer( int size ) :
    mBuffer( new T[ size ] ),
    mSize( size ),
    mWriteIdx( 0 ),
    mFirstFilling( true )
{
}


template< class T >
void
cCircularBuffer< T >::Write( T iObject )
{
    mBuffer[mWriteIdx] = iObject;
    mWriteIdx = ( mWriteIdx + 1 ) % mSize;
    mFirstFilling = mFirstFilling && (mWriteIdx != 0);
}


template< class T >
T
cCircularBuffer< T >::operator[] ( int iIndex )
{
    assert( iIndex >= 0 && iIndex < Count() );
    int firstIdx = mFirstFilling ? 0 : mWriteIdx;
    return mBuffer[( firstIdx + iIndex ) % mSize];
}


template< class T >
T
cCircularBuffer< T >::Back()
{
    return operator[]( Count()-1 );
}


template< class T >
int
cCircularBuffer< T >::Count() const
{
    return  mFirstFilling ? mWriteIdx : mSize;
}


template< class T >
int
cCircularBuffer< T >::Size() const
{
    return  mSize;
}


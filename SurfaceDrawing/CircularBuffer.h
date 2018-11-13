#pragma once

#include <cassert>

template< class T >
class cCircularBuffer {

public:
    ~cCircularBuffer();
    cCircularBuffer( int size );

public:
    void    Write( T iObject );
    void    WriteAt( T iObject, int iIndex );
    T       Back();
    int     Find( const T& iObject );

public:
    int Count() const;
    int Size() const;

public:
    T  operator[] ( int iIndex );

private:
    T*  mBuffer;
    int mSize;
    int mWriteIdx;
    bool mFirstFilling;
};


// ===========================================================
// ===========================================================
// ===========================================================


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
    memset( mBuffer, 0, mSize );
}


template< class T >
void
cCircularBuffer< T >::Write( T iObject )
{
    mBuffer[mWriteIdx] = iObject;
    mWriteIdx = ( mWriteIdx + 1 ) % mSize;
    mFirstFilling = mFirstFilling && (mWriteIdx != 0);
}


template<class T>
inline
void cCircularBuffer<T>::WriteAt( T iObject, int iIndex )
{
    mBuffer[ iIndex ] = iObject;
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


template<class T>
inline
int
cCircularBuffer<T>::Find( const T & iObject )
{
    for( int i = 0; i < mCount; ++i )
        if( operator[]( i ) == iObject )
            return  i;
    return  -1;
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






// ========================================================
// ========================================================
// ========================================================
// ========================================================
// ========================================================
// ========================================================





template< class T >
class cCircularBufferP {

public:
    ~cCircularBufferP();
    cCircularBufferP ( int size );

public:
    void    Write( T* iObject );
    void    WriteAt( T* iObject, int iIndex );
    T*      Back();
    int     Find( const T* iObject );

public:
    int Count() const;
    int Size() const;

public:
    T*  operator[] ( int iIndex );

private:
    T**  mBuffer;
    int mSize;
    int mWriteIdx;
    bool mFirstFilling;
};


// ===========================================================
// ===========================================================
// ===========================================================


template< class T >
cCircularBufferP< T >::~cCircularBufferP()
{
    delete[] mBuffer;
}


template< class T >
cCircularBufferP< T >::cCircularBufferP( int size ) :
    mBuffer( new T*[ size ] ),
    mSize( size ),
    mWriteIdx( 0 ),
    mFirstFilling( true )
{
    // memset is 32 bits and std::fill requires a valid iterator
    // fuck it, let's just do it this way
    for( int i = 0; i < mSize; ++i )
        mBuffer[ i ] = nullptr;
}


template< class T >
void
cCircularBufferP< T >::Write( T* iObject )
{
    delete  mBuffer[ mWriteIdx ];
    mBuffer[mWriteIdx] = iObject;
    mWriteIdx = ( mWriteIdx + 1 ) % mSize;
    mFirstFilling = mFirstFilling && (mWriteIdx != 0);
}


template<class T>
inline
void cCircularBufferP<T>::WriteAt( T * iObject, int iIndex )
{
    mBuffer[ iIndex ] = iObject;
}


template< class T >
T*
cCircularBufferP< T >::operator[] ( int iIndex )
{
    assert( iIndex >= 0 && iIndex < Count() );
    int firstIdx = mFirstFilling ? 0 : mWriteIdx;
    return mBuffer[( firstIdx + iIndex ) % mSize];
}


template< class T >
T*
cCircularBufferP< T >::Back()
{
    return operator[]( Count()-1 );
}


template<class T>
inline
int
cCircularBufferP<T>::Find( const T* iObject )
{
    for( int i = 0; i < mCount; ++i )
        if( operator[]( i ) == iObject )
            return  i;
    return  -1;
}


template< class T >
int
cCircularBufferP< T >::Count() const
{
    return  mFirstFilling ? mWriteIdx : mSize;
}


template< class T >
int
cCircularBufferP< T >::Size() const
{
    return  mSize;
}







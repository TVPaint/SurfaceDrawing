#pragma once

#include <QDataStream>

class cComp
{
public:
    cComp();
    cComp( unsigned int iCD );
    cComp( const cComp& iClone );

public:
    cComp& operator=( const cComp& iRHS );

public:
    unsigned int    mCooldown       = 0;        // Time in tick
    unsigned int    mCooldownBase   = 0;        // Time in tick
    bool            mActive         = false;
};

QDataStream& operator<<(QDataStream& oStream, const cComp& iComp );
QDataStream& operator>>(QDataStream& iStream, cComp& oComp );

QDataStream& operator<<(QDataStream& oStream, const cComp* iComp );
QDataStream& operator>>(QDataStream& iStream, cComp*& oComp );







class  cRollBack :
    public cComp
{
public:
    cRollBack();
};
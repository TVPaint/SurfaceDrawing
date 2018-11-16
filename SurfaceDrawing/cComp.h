#pragma once

#include <QDataStream>

class cComp
{
public:
    cComp();
    cComp( int iCD, int iDurationBase );
    cComp( const cComp& iClone );

public:
    cComp& operator=( const cComp& iRHS );

public:
    int    mCooldown       = 0;        // Time in tick
    int    mCooldownBase   = 0;        // Time in tick
    bool   mActive         = false;
    int    mCompDuration   = 0;
    int    mCompDurationBase   = -1;
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



class  cSpeed :
    public cComp
{
public:
    cSpeed();
};


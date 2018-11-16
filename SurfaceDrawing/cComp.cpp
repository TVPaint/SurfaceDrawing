#include "cComp.h"


#include "cPaperLogic.h"

cComp::cComp()
{
}


cComp::cComp( int iCD, int iDurationBase ) :
    mCooldown( 0 ),
    mCooldownBase( iCD ),
    mCompDuration( 0 ),
    mCompDurationBase( iDurationBase )
{
}


cComp::cComp( const cComp& iClone ) :
    mCooldown( iClone.mCooldown ),
    mCooldownBase( iClone.mCooldownBase ),
    mActive( iClone.mActive ),
    mCompDuration( iClone.mCompDuration ),
    mCompDurationBase( iClone.mCompDurationBase )
{
}


cComp&
cComp::operator=( const cComp & iRHS )
{
    mCooldown = iRHS.mCooldown;
    mCooldownBase = iRHS.mCooldownBase;
    mActive = iRHS.mActive;
    mCompDuration = iRHS.mCompDuration;
    mCompDurationBase = iRHS.mCompDurationBase;
    return  *this;
}


QDataStream&
operator<<( QDataStream & oStream, const cComp & iComp )
{
    oStream << iComp.mCooldown
            << iComp.mCooldownBase
            << iComp.mActive
            << iComp.mCompDuration
            << iComp.mCompDurationBase;

    return  oStream;
}


QDataStream&
operator>>( QDataStream & iStream, cComp & oComp )
{
    iStream >> oComp.mCooldown
            >> oComp.mCooldownBase
            >> oComp.mActive
            >> oComp.mCompDuration
            >> oComp.mCompDurationBase;

    return  iStream;
}


QDataStream&
operator<<( QDataStream & oStream, const cComp * iComp )
{
    oStream << *iComp;
    return  oStream;
}


QDataStream&
operator>>( QDataStream & iStream, cComp *& oComp )
{
    cComp* comp = new cComp();
    iStream >> *comp;
    return  iStream;
}



// =========================================
// =========================================
// =========================================




cRollBack::cRollBack() :
    cComp( 6000 / SPEED, 0 ) // Time in tick
{
}



// =========================================
// =========================================
// =========================================




cSpeed::cSpeed() :
    cComp( 6000 / SPEED, 2000/SPEED ) // Time in tick
{
}


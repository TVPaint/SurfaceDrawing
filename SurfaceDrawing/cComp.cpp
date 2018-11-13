#include "cComp.h"


#include "cPaperLogic.h"

cComp::cComp()
{
}


cComp::cComp( unsigned int iCD ) :
    mCooldown( 0 ),
    mCooldownBase( iCD )
{
}


cComp::cComp( const cComp& iClone ) :
    mCooldown( iClone.mCooldown ),
    mCooldownBase( iClone.mCooldownBase ),
    mActive( iClone.mActive )
{
}


cComp&
cComp::operator=( const cComp & iRHS )
{
    mCooldown = iRHS.mCooldown;
    mCooldownBase = iRHS.mCooldownBase;
    mActive = iRHS.mActive;
    return  *this;
}


QDataStream&
operator<<( QDataStream & oStream, const cComp & iComp )
{
    oStream << iComp.mCooldown
            << iComp.mCooldownBase
            << iComp.mActive;

    return  oStream;
}


QDataStream&
operator>>( QDataStream & iStream, cComp & oComp )
{
    iStream >> oComp.mCooldown
            >> oComp.mCooldownBase
            >> oComp.mActive;

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
    cComp( 6000 / SPEED ) // Time in tick
{
}

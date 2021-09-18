#include "liquescent.h"
#include "doc.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// Liquescent
//----------------------------------------------------------------------------

Liquescent::Liquescent() : LayerElement("liquescent-"), PitchInterface(), PositionInterface(), AttColor()
{
    RegisterInterface(PitchInterface::GetAttClasses(), PitchInterface::IsInterface());
    RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);

    Reset();
}

Liquescent::~Liquescent() {}

void Liquescent::Reset()
{
    LayerElement::Reset();
    PitchInterface::Reset();
    PositionInterface::Reset();
    ResetColor();
}


}
/////////////////////////////////////////////////////////////////////////////
// Name:        mdiv.cpp
// Author:      Laurent Pugin
// Created:     2018/02/15
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "mdiv.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "functorparams.h"
#include "iomei.h"
#include "page.h"
#include "pages.h"
#include "score.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Mdiv
//----------------------------------------------------------------------------

static const ClassRegistrar<Mdiv> s_factory("mdiv", MDIV);

Mdiv::Mdiv() : PageElement(MDIV, "mdiv-"), PageMilestoneInterface(), AttLabelled(), AttNNumberLike()
{
    this->RegisterAttClass(ATT_LABELLED);
    this->RegisterAttClass(ATT_NNUMBERLIKE);

    this->Reset();
}

Mdiv::~Mdiv() {}

void Mdiv::Reset()
{
    Object::Reset();
    this->ResetLabelled();
    this->ResetNNumberLike();

    m_visibility = Hidden;
}

bool Mdiv::IsSupportedChild(Object *child)
{
    if (child->Is(MDIV)) {
        assert(dynamic_cast<Mdiv *>(child));
    }
    else if (child->Is(SCORE)) {
        assert(dynamic_cast<Score *>(child));
    }
    else {
        return false;
    }
    return true;
}

void Mdiv::MakeVisible()
{
    m_visibility = Visible;
    if (this->GetParent() && this->GetParent()->Is(MDIV)) {
        Mdiv *parent = vrv_cast<Mdiv *>(this->GetParent());
        assert(parent);
        parent->MakeVisible();
    }
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

int Mdiv::Save(FunctorParams *functorParams)
{
    SaveParams *params = vrv_params_cast<SaveParams *>(functorParams);
    assert(params);

    MEIOutput *meiOutput = dynamic_cast<MEIOutput *>(params->m_output);
    if (m_visibility == Hidden && meiOutput) {
        // Do not output hidden mdivs in page-based MEI or when saving score-based MEI with filter
        if (!meiOutput->GetScoreBasedMEI() || meiOutput->HasFilter()) return FUNCTOR_SIBLINGS;
    }
    return Object::Save(functorParams);
}

int Mdiv::SaveEnd(FunctorParams *functorParams)
{
    SaveParams *params = vrv_params_cast<SaveParams *>(functorParams);
    assert(params);

    MEIOutput *meiOutput = dynamic_cast<MEIOutput *>(params->m_output);
    if (m_visibility == Hidden && meiOutput) {
        // Do not output hidden mdivs in page-based MEI or when saving score-based MEI with filter
        if (!meiOutput->GetScoreBasedMEI() || meiOutput->HasFilter()) return FUNCTOR_SIBLINGS;
    }
    return Object::SaveEnd(functorParams);
}

int Mdiv::ConvertToPageBased(FunctorParams *functorParams)
{
    ConvertToPageBasedParams *params = vrv_params_cast<ConvertToPageBasedParams *>(functorParams);
    assert(params);

    this->MoveItselfTo(params->m_page);

    return FUNCTOR_CONTINUE;
}

int Mdiv::ConvertToPageBasedEnd(FunctorParams *functorParams)
{
    ConvertToPageBasedParams *params = vrv_params_cast<ConvertToPageBasedParams *>(functorParams);
    assert(params);

    if (m_visibility == Visible) ConvertToPageBasedMilestone(this, params->m_page);

    return FUNCTOR_CONTINUE;
}

int Mdiv::Transpose(FunctorParams *functorParams)
{
    TransposeParams *params = vrv_params_cast<TransposeParams *>(functorParams);
    assert(params);

    params->m_currentMdivIDs.push_back(this->GetID());
    params->m_keySigForStaffN.clear();
    params->m_transposeIntervalForStaffN.clear();

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

/////////////////////////////////////////////////////////////////////////////
// Name:        attributes.h
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_ATTRIBUTES_H__
#define __VRV_ATTRIBUTES_H__

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// Common
//----------------------------------------------------------------------------

/**
 * This class is an interface for elements with the MEI att.common.
 */
class Common
{
public:
    // constructors and destructors
    Common();
    virtual ~Common();
    
    /** reset the attribute values */
    void ClearAttCommon();
    
    /**
     * @name Setter and getter for the label
     * This can be different from the index position in the parent measure.
     */
    ///@{
    std::string GetLabel() const { return m_label; };
    void SetLabel( std::string label ) { m_label = label; };
    ///@}
    
    /**
     * @name Setter and getter for the n .
     * This can be different from the index position in the parent measure.
     */
    ///@{
    int GetN() const { return m_n; };
    void SetN( int n ) { m_n = n; };
    ///@}
    
    
protected:
    /** Provides a label for the element */
    std::string m_label;
    /** Indicates a name or number */
    int m_n;
};


} // namespace vrv    

#endif

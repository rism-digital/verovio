/////////////////////////////////////////////////////////////////////////////
// Name:        toolkitmanager.h
// Author:      Laurent Pugin
// Created:     25/01/2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TOOLKIT_MANAGER_H__
#define __VRV_TOOLKIT_MANAGER_H__

#include <string>
#include <vector>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

class Toolkit;

//----------------------------------------------------------------------------
// ToolkitManager
//----------------------------------------------------------------------------

/**
 * This class manages toolkit instances for the JavaScript toolkit.
 * By default, one toolkit instance is created but additional instances
 * be created if nessary (to be implemented).
 */
class ToolkitManager {
public:
    /**
     * @name Constructors and destructors
     */
    ///@{
    ToolkitManager();
    virtual ~ToolkitManager();
    ///@}

    /**
     * A getter for the current instance
     */
    Toolkit *GetInstance();

    /**
     * Methods to implement - and to add to the JS proxy
     */
    // std::string GetInstanceId();
    // std::string CreateInstance();
    // void SelectInstance(std::string id);

    /**
     * @name Set and get a std::string into a char * buffer.
     * This is used for returning a string buffer to emscripten.
     * The buffer is freed when reset or in MusController destructor.
     */
    ///@{
    void SetCString(const std::string &data);
    const char *GetCString();
    ///@}

public:
    //
private:
    /**
     * The C buffer string.
     */
    char *m_cString;

    /**
     * The current instance of the toolkit.
     */
    Toolkit *m_instance;

    /**
     * A vector of instances.
     */
    std::vector<Toolkit *> m_instances;
};

} // namespace vrv
#endif

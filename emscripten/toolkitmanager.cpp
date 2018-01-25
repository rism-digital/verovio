/////////////////////////////////////////////////////////////////////////////
// Name:        toolkitmanager.cpp
// Author:      Laurent Pugin
// Created:     25/01/2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "toolkitmanager.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "toolkit.h"

namespace vrv {

//----------------------------------------------------------------------------
// ToolkitManager
//----------------------------------------------------------------------------

ToolkitManager::ToolkitManager()
{
    m_instance = new Toolkit();
    LogMessage("Intance %s created", m_instance->GetUuid().c_str());
    m_instances.push_back(m_instance);
}

ToolkitManager::~ToolkitManager()
{
    std::vector<Toolkit *>::iterator iter;
    for (iter = m_instances.begin(); iter != m_instances.end(); iter++) {
        delete *iter;
    }
    
    if (m_cString) {
        free(m_cString);
        m_cString = NULL;
    }
}

Toolkit *ToolkitManager::GetInstance()
{
    assert(m_instance);
    return m_instance;
}
    
void ToolkitManager::SetCString(const std::string &data)
{
    if (m_cString) {
        free(m_cString);
        m_cString = NULL;
    }

    m_cString = (char *)malloc(strlen(data.c_str()) + 1);

    // something went wrong
    if (!m_cString) {
        return;
    }
    strcpy(m_cString, data.c_str());
}
    
const char *ToolkitManager::GetCString()
{
    if (m_cString) {
        return m_cString;
    }
    else {
        return "[unspecified]";
    }
}

} // namespace vrv

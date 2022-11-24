/////////////////////////////////////////////////////////////////////////////
// Name:        verovio.i
// Author:      Laurent Pugin
// Created:     09/11/2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

// Change method names to lowerCamelCase
%rename("%(lowercamelcase)s") "";
// Ignore enum items (e.g., for fileFormat.PAE)
%rename("%(upper)s", %$isenumitem) "";

// Method to ignore
%ignore vrv::Toolkit::GetShowBoundingBoxes( );
%ignore vrv::Toolkit::GetCString( );
%ignore vrv::Toolkit::GetLogString( );
%ignore vrv::Toolkit::GetOptionsObj( );
%ignore vrv::Toolkit::ResetLogBuffer( );
%ignore vrv::Toolkit::SetShowBoundingBoxes( bool );
%ignore vrv::Toolkit::SetCString( const std::string & );

// Because we transform the strings to dictionaries, we need this module
%pythonbegin %{
    import json
%}

// Toolkit::Edit
%feature("shadow") vrv::Toolkit::Edit( const std::string & ) %{
def edit(toolkit, editor_action):
    return $action(toolkit, json.dumps(editor_action))
%}

// Toolkit::EditInfo
%feature("shadow") vrv::Toolkit::EditInfo() %{
def editInfo(toolkit):
    return json.loads($action(toolkit))
%}

// Toolkit::GetAvailableOptions
%feature("shadow") vrv::Toolkit::GetAvailableOptions() const %{
def getAvailableOptions(toolkit):
    return json.loads($action(toolkit))
%}

// Toolkit::GetDefaultOptions
%feature("shadow") vrv::Toolkit::GetDefaultOptions() const %{
def getDefaultOptions(toolkit):
    return json.loads($action(toolkit))
%}

// Toolkit::GetDescriptiveFeatures
%feature("shadow") vrv::Toolkit::GetDescriptiveFeatures(const std::string &) %{
def getDescriptiveFeatures(toolkit, options):
    return json.loads($action(toolkit, json.dumps(options)))
%}

// Toolkit::GetElementAttr
%feature("shadow") vrv::Toolkit::GetElementAttr(const std::string &) %{
def getElementAttr(toolkit, xml_id):
    return json.loads($action(toolkit, xml_id))
%}

// Toolkit::GetElementsAtTime
%feature("shadow") vrv::Toolkit::GetElementsAtTime(int) %{
def getElementsAtTime(toolkit, millisec):
    return json.loads($action(toolkit, millisec))
%}

// Toolkit::GetExpansionIdsForElement
%feature("shadow") vrv::Toolkit::GetExpansionIdsForElement(const std::string &) %{
def getExpansionIdsForElement(toolkit, xml_id):
    return json.loads($action(toolkit, xml_id))
%}

// Toolkit::GetMIDIValuesForElement
%feature("shadow") vrv::Toolkit::GetMIDIValuesForElement(const std::string &) %{
def getMIDIValuesForElement(toolkit, xml_id):
    return json.loads($action(toolkit, xml_id))
%}

// Toolkit::GetNotatedIdForElement
%feature("shadow") vrv::Toolkit::GetNotatedIdForElement(const std::string &) %{
def getNotatedIdForElement(toolkit, xml_id):
    return json.loads($action(toolkit, xml_id))
%}

// Toolkit::GetOptions
%feature("shadow") vrv::Toolkit::GetOptions() const %{
def getOptions(toolkit):
    return json.loads($action(toolkit))
%}

// Toolkit::GetTimesForElement
%feature("shadow") vrv::Toolkit::GetTimesForElement(const std::string &) %{
def getTimesForElement(toolkit, xml_id):
    return json.loads($action(toolkit, xml_id))
%}

// Toolkit::RedoLayout
%feature("shadow") vrv::Toolkit::RedoLayout(const std::string &) %{
def redoLayout(toolkit, options = {}):
    return $action(toolkit, json.dumps(options))
%}

// Toolkit::RenderData
%feature("shadow") vrv::Toolkit::RenderData(const std::string &, const std::string &) %{
def renderData(toolkit, data, options):
    return $action(toolkit, data, json.dumps(options))
%}

// Toolkit::RenderToMIDI
%feature("shadow") vrv::Toolkit::RenderToMIDI(const std::string &) %{
def renderToMIDI(toolkit, options):
    return json.loads($action(toolkit, json.dumps(options)))
%}

// Toolkit::RenderToTimemap
%feature("shadow") vrv::Toolkit::RenderToTimemap(const std::string &) %{
def renderToTimemap(toolkit, options = {}):
    return json.loads($action(toolkit, json.dumps(options)))
%}

// Toolkit::RenderToTimemapFile
%feature("shadow") vrv::Toolkit::RenderToTimemapFile(const std::string &, const std::string &) %{
def renderToTimemapFile(toolkit, filename, options = {}):
    return json.loads($action(toolkit, filename, json.dumps(options)))
%}

// Toolkit::SaveFile
%feature("shadow") vrv::Toolkit::SaveFile(const std::string &, const std::string &) %{
def saveFile(toolkit, filename, options = {}):
    return json.loads($action(toolkit, filename, json.dumps(options)))
%}

// Toolkit::Select
%feature("shadow") vrv::Toolkit::Select(const std::string &) %{
def select(toolkit, selection):
    return $action(toolkit, json.dumps(selection))
%}

// Toolkit::SetOptions
%feature("shadow") vrv::Toolkit::SetOptions(const std::string &) %{
def setOptions(toolkit, json_options):
    return $action(toolkit, json.dumps(json_options))
%}

// Toolkit::ValidatePAE
%feature("shadow") vrv::Toolkit::ValidatePAE(const std::string &) %{
def validatePAE(toolkit, data):
    if isinstance(data, dict):
        data = json.dumps(data);
    return json.loads($action(toolkit, data))
%}

%module(package="verovio") verovio
%include "std_string.i"
%include "../../include/vrv/toolkit.h"
%include "../../include/vrv/toolkitdef.h"

%{
    #include "../../include/vrv/toolkit.h"
    #include "../../include/vrv/toolkitdef.h"
    
    using namespace vrv;
    using namespace std;
%}

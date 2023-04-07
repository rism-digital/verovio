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

%feature("autodoc", "1");

// Because we transform the strings to dictionaries, we need this module
%pythonbegin %{
    import json
    from typing import Optional, Union
%}

// Toolkit::Edit
%feature("shadow") vrv::Toolkit::Edit( const std::string & ) %{
def edit(toolkit, editor_action: dict) -> bool:
    """Edit the MEI data."""
    return $action(toolkit, json.dumps(editor_action))
%}

// Toolkit::EditInfo
%feature("shadow") vrv::Toolkit::EditInfo() %{
def editInfo(toolkit) -> dict:
    """Return the editor status."""
    return json.loads($action(toolkit))
%}

// Toolkit::GetAvailableOptions
%feature("shadow") vrv::Toolkit::GetAvailableOptions() const %{
def getAvailableOptions(toolkit) -> dict:
    """Return a dictionary of all the options with their default value."""
    return json.loads($action(toolkit))
%}

// Toolkit::GetDefaultOptions
%feature("shadow") vrv::Toolkit::GetDefaultOptions() const %{
def getDefaultOptions(toolkit) -> dict:
    """Return a dictionary of all the options with their default value."""
    return json.loads($action(toolkit))
%}

// Toolkit::GetDescriptiveFeatures
%feature("shadow") vrv::Toolkit::GetDescriptiveFeatures(const std::string &) %{
def getDescriptiveFeatures(toolkit, options: Optional[dict] = None) -> dict:
    """Return descriptive features as dictionary."""
    if options is None:
        options = {}
    return json.loads($action(toolkit, json.dumps(options)))
%}

// Toolkit::GetElementAttr
%feature("shadow") vrv::Toolkit::GetElementAttr(const std::string &) %{
def getElementAttr(toolkit, xml_id: str) -> dict:
    """Return element attributes as dictionary."""
    return json.loads($action(toolkit, xml_id))
%}

// Toolkit::GetElementsAtTime
%feature("shadow") vrv::Toolkit::GetElementsAtTime(int) %{
def getElementsAtTime(toolkit, millisec: int) -> dict:
    """Return array of IDs of elements being currently played."""
    return json.loads($action(toolkit, millisec))
%}

// Toolkit::GetExpansionIdsForElement
%feature("shadow") vrv::Toolkit::GetExpansionIdsForElement(const std::string &) %{
def getExpansionIdsForElement(toolkit, xml_id: str) -> dict:
    """Return a vector of ID strings of all elements (the notated and the expanded) for a given element."""
    return json.loads($action(toolkit, xml_id))
%}

// Toolkit::GetMEI
%feature("shadow") vrv::Toolkit::GetMEI(const std::string & = "") %{
def getMEI(toolkit, options: Optional[dict] = None) -> str:
    """Get the MEI as a string."""
    if options is None:
        options = {}
    return $action(toolkit, json.dumps(options))
%}

// Toolkit::GetMIDIValuesForElement
%feature("shadow") vrv::Toolkit::GetMIDIValuesForElement(const std::string &) %{
def getMIDIValuesForElement(toolkit, xml_id: str) -> dict:
    """Return MIDI values of the element with the ID (xml:id)."""
    return json.loads($action(toolkit, xml_id))
%}

// Toolkit::GetOptions
%feature("shadow") vrv::Toolkit::GetOptions() const %{
def getOptions(toolkit) -> dict:
    """Return a dictionary of all the options with their current value."""
    return json.loads($action(toolkit))
%}

// Toolkit::GetTimesForElement
%feature("shadow") vrv::Toolkit::GetTimesForElement(const std::string &) %{
def getTimesForElement(toolkit, xml_id: str) -> dict:
    """Return a dictionary with the following key values for a given note."""
    return json.loads($action(toolkit, xml_id))
%}

// Toolkit::RedoLayout
%feature("shadow") vrv::Toolkit::RedoLayout(const std::string & = "") %{
def redoLayout(toolkit, options: Optional[dict] = None) -> None:
    """Redo the layout of the loaded data."""
    if options is None:
        options = {}
    return $action(toolkit, json.dumps(options))
%}

// Toolkit::RenderData
%feature("shadow") vrv::Toolkit::RenderData(const std::string &, const std::string &) %{
def renderData(toolkit, data, options: dict) -> str:
    """Render the first page of the data to SVG."""
    return $action(toolkit, data, json.dumps(options))
%}

// Toolkit::RenderToExpansionMap
%feature("shadow") vrv::Toolkit::RenderToExpansionMap() %{
def renderToExpansionMap(toolkit) -> list:
    """Render a document's expansion map, if existing."""
    return json.loads($action(toolkit))
%}

// Toolkit::RenderToExpansionMapFile
%feature("shadow") vrv::Toolkit::RenderToExpansionMapFile(const std::string &) %{
def renderToExpansionMapFile(toolkit, filename: str) -> bool:
    """Render a document's expansion map and save it to a file."""
    return $action(toolkit, filename)
%}

// Toolkit::RenderToTimemap
%feature("shadow") vrv::Toolkit::RenderToTimemap(const std::string & = "") %{
def renderToTimemap(toolkit, options: Optional[dict] = None) -> list:
    """Render a document to a timemap."""
    if options is None:
        options = {}
    return json.loads($action(toolkit, json.dumps(options)))
%}

// Toolkit::RenderToTimemapFile
%feature("shadow") vrv::Toolkit::RenderToTimemapFile(const std::string &, const std::string & = "") %{
def renderToTimemapFile(toolkit, filename: str, options: Optional[dict] = None) -> bool:
    """Render a document to timemap and save it to the file."""
    if options is None:
        options = {}
    return $action(toolkit, filename, json.dumps(options))
%}

// Toolkit::SaveFile
%feature("shadow") vrv::Toolkit::SaveFile(const std::string &, const std::string & = "") %{
def saveFile(toolkit, filename: str, options: Optional[dict] = None) -> bool:
    """Get the MEI and save it to the file."""
    if options is None:
        options = {}
    return $action(toolkit, filename, json.dumps(options))
%}

// Toolkit::Select
%feature("shadow") vrv::Toolkit::Select(const std::string &) %{
def select(toolkit, selection: dict) -> bool:
    """Set the value for a selection."""
    return $action(toolkit, json.dumps(selection))
%}

// Toolkit::SetOptions
%feature("shadow") vrv::Toolkit::SetOptions(const std::string &) %{
def setOptions(toolkit, json_options: dict) -> bool:
    """Set option values."""
    return $action(toolkit, json.dumps(json_options))
%}

// Toolkit::ValidatePAE
%feature("shadow") vrv::Toolkit::ValidatePAE(const std::string &) %{
def validatePAE(toolkit, data: Union[str, dict]) -> dict:
    """Validate the Plaine and Easie code passed in the string data."""
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

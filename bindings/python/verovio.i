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
%ignore vrv::Toolkit::PrintOptionUsage;
%ignore vrv::Toolkit::GetHumdrum;
%ignore vrv::Toolkit::RenderToDeviceContext;
%ignore vrv::EnableLogToBuffer;

%feature("autodoc", "1");

// Because we transform the strings to dictionaries, we need this module
%pythonbegin %{
    import json
    from typing import Optional, Union
%}

// Toolkit::Edit
%feature("shadow") vrv::Toolkit::Edit( const std::string & ) %{
def edit(self, editor_action: dict) -> bool:
    """Edit the MEI data."""
    return $action(self, json.dumps(editor_action))
%}

// Toolkit::EditInfo
%feature("shadow") vrv::Toolkit::EditInfo() %{
def editInfo(self) -> dict:
    """Return the editor status."""
    return json.loads($action(self))
%}

// Toolkit::GetAvailableOptions
%feature("shadow") vrv::Toolkit::GetAvailableOptions() const %{
def getAvailableOptions(self) -> dict:
    """Return a dictionary of all the options with their default value."""
    return json.loads($action(self))
%}

// Toolkit::GetDefaultOptions
%feature("shadow") vrv::Toolkit::GetDefaultOptions() const %{
def getDefaultOptions(self) -> dict:
    """Return a dictionary of all the options with their default value."""
    return json.loads($action(self))
%}

// Toolkit::GetDescriptiveFeatures
%feature("shadow") vrv::Toolkit::GetDescriptiveFeatures(const std::string &) %{
def getDescriptiveFeatures(self, options: Optional[dict] = None) -> dict:
    """Return descriptive features as dictionary."""
    if options is None:
        options = {}
    return json.loads($action(self, json.dumps(options)))
%}

// Toolkit::GetElementAttr
%feature("shadow") vrv::Toolkit::GetElementAttr(const std::string &) %{
def getElementAttr(self, xmlId: str) -> dict:
    """Return element attributes as dictionary."""
    return json.loads($action(self, xmlId))
%}

// Toolkit::GetElementsAtTime
%feature("shadow") vrv::Toolkit::GetElementsAtTime(int) %{
def getElementsAtTime(self, millisec: int) -> dict:
    """Return array of IDs of elements being currently played."""
    return json.loads($action(self, millisec))
%}

// Toolkit::GetExpansionIdsForElement
%feature("shadow") vrv::Toolkit::GetExpansionIdsForElement(const std::string &) %{
def getExpansionIdsForElement(self, xmlId: str) -> dict:
    """Return a vector of ID strings of all elements (the notated and the expanded) for a given element."""
    return json.loads($action(self, xmlId))
%}

// Toolkit::GetMEI
%feature("shadow") vrv::Toolkit::GetMEI(const std::string & = "") %{
def getMEI(self, options: Optional[dict] = None) -> str:
    """Get the MEI as a string."""
    if options is None:
        options = {}
    return $action(self, json.dumps(options))
%}

// Toolkit::GetMIDIValuesForElement
%feature("shadow") vrv::Toolkit::GetMIDIValuesForElement(const std::string &) %{
def getMIDIValuesForElement(self, xmlId: str) -> dict:
    """Return MIDI values of the element with the ID (xml:id)."""
    return json.loads($action(self, xmlId))
%}

// Toolkit::GetOptions
%feature("shadow") vrv::Toolkit::GetOptions() const %{
def getOptions(self) -> dict:
    """Return a dictionary of all the options with their current value."""
    return json.loads($action(self))
%}

// Toolkit::GetTimesForElement
%feature("shadow") vrv::Toolkit::GetTimesForElement(const std::string &) %{
def getTimesForElement(self, xmlId: str) -> dict:
    """Return a dictionary with the following key values for a given note."""
    return json.loads($action(self, xmlId))
%}

// Toolkit::RedoLayout
%feature("shadow") vrv::Toolkit::RedoLayout(const std::string & = "") %{
def redoLayout(self, options: Optional[dict] = None) -> None:
    """Redo the layout of the loaded data."""
    if options is None:
        options = {}
    return $action(self, json.dumps(options))
%}

// Toolkit::RenderData
%feature("shadow") vrv::Toolkit::RenderData(const std::string &, const std::string &) %{
def renderData(self, data: str, options: dict) -> str:
    """Render the first page of the data to SVG."""
    return $action(self, data, json.dumps(options))
%}

// Toolkit::RenderToExpansionMap
%feature("shadow") vrv::Toolkit::RenderToExpansionMap() %{
def renderToExpansionMap(self) -> list:
    """Render a document's expansion map, if existing."""
    return json.loads($action(self))
%}

// Toolkit::RenderToExpansionMapFile
%feature("shadow") vrv::Toolkit::RenderToExpansionMapFile(const std::string &) %{
def renderToExpansionMapFile(self, filename: str) -> bool:
    """Render a document's expansion map and save it to a file."""
    return $action(self, filename)
%}

// Toolkit::RenderToTimemap
%feature("shadow") vrv::Toolkit::RenderToTimemap(const std::string & = "") %{
def renderToTimemap(self, options: Optional[dict] = None) -> list:
    """Render a document to a timemap."""
    if options is None:
        options = {}
    return json.loads($action(self, json.dumps(options)))
%}

// Toolkit::RenderToTimemapFile
%feature("shadow") vrv::Toolkit::RenderToTimemapFile(const std::string &, const std::string & = "") %{
def renderToTimemapFile(self, filename: str, options: Optional[dict] = None) -> bool:
    """Render a document to timemap and save it to the file."""
    if options is None:
        options = {}
    return $action(self, filename, json.dumps(options))
%}

// Toolkit::SaveFile
%feature("shadow") vrv::Toolkit::SaveFile(const std::string &, const std::string & = "") %{
def saveFile(self, filename: str, options: Optional[dict] = None) -> bool:
    """Get the MEI and save it to the file."""
    if options is None:
        options = {}
    return $action(self, filename, json.dumps(options))
%}

// Toolkit::Select
%feature("shadow") vrv::Toolkit::Select(const std::string &) %{
def select(self, selection: dict) -> bool:
    """Set the value for a selection."""
    return $action(self, json.dumps(selection))
%}

// Toolkit::SetOptions
%feature("shadow") vrv::Toolkit::SetOptions(const std::string &) %{
def setOptions(self, json_options: dict) -> bool:
    """Set option values."""
    return $action(self, json.dumps(json_options))
%}

// Toolkit::ValidatePAE
%feature("shadow") vrv::Toolkit::ValidatePAE(const std::string &) %{
def validatePAE(self, data: Union[str, dict]) -> dict:
    """Validate the Plaine and Easie code passed in the string data."""
    if isinstance(data, dict):
        data = json.dumps(data);
    return json.loads($action(self, data))
%}

// Toolkit::ValidatePAEFile
%feature("shadow") vrv::Toolkit::ValidatePAEFile(const std::string &) %{
def validatePAEFile(self, filename: str) -> dict:
    """Validate the Plaine and Easie code passed in the string data."""
    if isinstance(data, dict):
        data = json.dumps(data);
    return json.loads($action(self, data))
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

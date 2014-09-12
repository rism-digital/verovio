/////////////////////////////////////////////////////////////////////////////
// Name:        iomusxml.h
// Author:      Rodolfo Zitellini
// Created:     10/08/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_IOMUSXML_H__
#define __VRV_IOMUSXML_H__

#include "doc.h"
#include "layer.h"
#include "pugixml.hpp"
#include "tie.h"

namespace vrv {

class Barline;
class Beam;
class Clef;
class Layer;
class MeterSig;
class MRest;
class MultiRest;
class Note;
class Rest;
class Staff;
class System;
class Tuplet;


//----------------------------------------------------------------------------
// This class is not up-to-date
//----------------------------------------------------------------------------

class XMLOutput: public FileOutputStream
{
public:
    // constructors and destructors
    XMLOutput( Doc *doc, std::string filename );
    virtual ~XMLOutput();
    
    virtual bool ExportFile( );
    
private:

    virtual bool WriteDoc( Doc *doc );
    // logical
    bool WriteStaff( Staff *staff );
    bool WriteLayer( Layer *layer );
    bool WriteLayerElement( LayerElement *element );
    // layout
    bool WriteLayout( Doc *layout );
    bool WritePage( Page *page );
    bool WriteSystem( System *system );
    bool WriteLaidOutStaff( Staff *laidOutStaff );
    bool WriteLaidOutLayer( Layer *laidOutLayer );
    bool WriteLaidOutLayerElement( LayerElement *laidOutLayerElement );
    
    void WriteClef(LayerElement *element);
    void WriteKey(LayerElement *element);
    void WriteTime(LayerElement *element);
    void WriteNoteOrRest(LayerElement *element);
    void WriteMultiMeasureRest(Rest *r);
    void CreateAttributes();
    void SetTie(pugi::xml_node xml_note, bool last);
    void CreateRestsForMultiMeasure();
    
private:
    std::string m_filename;
    
    pugi::xml_node m_xml_score;
    pugi::xml_node m_xml_part;
    pugi::xml_node m_xml_measure;
    pugi::xml_node m_xml_attributes;
    pugi::xml_node m_xml_measure_style;
    pugi::xml_node m_xml_last_note;
    pugi::xml_document m_xml_doc;
    pugi::xml_node m_xml_current_clef;
    
    MeterSig *m_current_time;
    Beam *m_current_beam;
    bool m_in_beam;
    bool m_tied;
    int m_multimeasure_rests;
    
    int m_measure_count;
};

} // namespace vrv {

#endif // __VRV_IOMUSXML_H__

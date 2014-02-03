/////////////////////////////////////////////////////////////////////////////
// Name:        musiomusxml.cpp
// Author:      Rodolfo Zitellini
// Created:     10/08/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "iomusxml.h"

//----------------------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <sstream>

//----------------------------------------------------------------------------

#include "barline.h"
#include "beam.h"
#include "clef.h"
#include "keysig.h"
#include "layerelement.h"
#include "mensur.h"
#include "mensur.h"
#include "note.h"
#include "rest.h"
#include "symbol.h"
#include "tie.h"

namespace vrv {

//----------------------------------------------------------------------------
// XMLOutput
//----------------------------------------------------------------------------

XMLOutput::XMLOutput( Doc *doc, std::string filename ) :
// This is pretty bad. We open a bad fileoutputstream as we don't use it
FileOutputStream( doc )
{
    m_filename = filename;
    m_measure_count = 0;
    m_xml_attributes = NULL;
    m_xml_current_clef = NULL;
    m_xml_measure_style = NULL;
    m_xml_last_note = NULL;
    
    m_current_time = NULL;
    m_current_beam = NULL;
    m_in_beam = false;
    m_tied = false;
    m_multimeasure_rests = 0;

}

XMLOutput::~XMLOutput()
{
}

bool XMLOutput::ExportFile( )
{

    m_xml_doc = new TiXmlDocument;
    TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "UTF-8", "no" );
    m_xml_doc->LinkEndChild( decl );

    // do this or finale will barf, versione 3.0 for now
    TiXmlUnknown *unk = new TiXmlUnknown;
    //unk->SetValue("!DOCTYPE score-partwise PUBLIC \"-//Recordare//DTD MusicXML 3.0 Partwise//EN\" \"http://www.musicxml.org/dtds/partwise.dtd\"");
    unk->SetValue("!DOCTYPE score-timewise PUBLIC \"-//Recordare//DTD MusicXML 2.0 Timewise//EN\" \"http://www.musicxml.org/dtds/timewise.dtd\"");
    m_xml_doc->LinkEndChild(unk);
    
    // this starts the call of all the functors
    m_doc->Save( this );
        
    m_xml_doc->SaveFile( m_filename.c_str() );
    
	return true;    
}

bool XMLOutput::WriteDoc( Doc *doc )
{
    LogDebug("Doc");
    
    // Write the partwise declaration
    // the MusicXML "score-partwise" does not map to our MusScore
    
    m_xml_score = new TiXmlElement("score-timewise");
    
    // hardcode a voice for noew
    TiXmlElement *plist =  new TiXmlElement("part-list");
    TiXmlElement *spart = new TiXmlElement("score-part");
    spart->SetAttribute("id", "P1");
    TiXmlElement *pname = new TiXmlElement("part-name");
    TiXmlText *pname_text = new TiXmlText("Example Music");
    pname->LinkEndChild(pname_text);
    
    spart->LinkEndChild(pname);
    plist->LinkEndChild(spart);
    
    m_xml_score->LinkEndChild(plist);
    m_xml_doc->LinkEndChild(m_xml_score);
    
    
    return true;
}


/*
bool XMLOutput::WriteMeasure( Measure *measure )
//bool XMLOutput::WriteMeiMeasure( Measure *meiMeasure, Measure *measure )
{
    std::string num;

    // Create multimeasure rests
    if (m_multimeasure_rests > 0 )
        CreateRestsForMultiMeasure();
    
    m_measure_count++;
    num << m_measure_count;
    LogDebug("Measure %i", m_measure_count);
    
    // go on and create this measure
    m_xml_measure = new TiXmlElement("measure");
    m_xml_measure->SetAttribute("number", num);
    m_xml_score->LinkEndChild(m_xml_measure);
    
        
    // reset clef
    m_xml_current_clef = NULL;
    
    //m_xml_part->LinkEndChild(m_xml_measure);
    
    return true;
}
*/ // ax2.3

bool XMLOutput::WriteStaff( Staff *staff )
//bool XMLOutput::WriteMeiStaff( Staff *meiStaff, Staff *staff )
{
    LogDebug("Staff");
    
    m_xml_part = new TiXmlElement("part");
    m_xml_part->SetAttribute("id", "P1");
    
    m_xml_measure->LinkEndChild(m_xml_part);
    
    // in first measure set the divisions value in <attributes>
    if (m_measure_count == 1) {
        m_xml_attributes = new TiXmlElement("attributes");
        TiXmlElement *divisions = new TiXmlElement("divisions");
        TiXmlText *divval = new TiXmlText("4"); // no more than sixteenths for now
        
        divisions->LinkEndChild(divval);
        m_xml_attributes->LinkEndChild(divisions);
        m_xml_part->LinkEndChild(m_xml_attributes);
    }
    
    return true;
}

bool XMLOutput::WriteLayer( Layer *layer )
//bool XMLOutput::WriteMeiLayer( Layer *meiLayer, Layer *layer )
{
    LogDebug("Layer");
    return true;
}

bool XMLOutput::WriteLayerElement( LayerElement *element )
{
    LogDebug("Layer Elem");
        
    if (dynamic_cast<Clef*>(element)) {
        WriteClef(element);
    } else if (dynamic_cast<KeySignature*>(element)) {
        WriteKey(element);
    } else if (dynamic_cast<Mensur*>(element)) {
        WriteTime(element);
    } else if (dynamic_cast<Note*>(element) || dynamic_cast<Rest*>(element)) {
        WriteNoteOrRest(element);
    } else if (dynamic_cast<Beam*>(element)) {
        m_current_beam = dynamic_cast<Beam*>(element);
    } else if (dynamic_cast<Tie*>(element)) {
        // add tie to last note
        // continuation of ties is made with a <start> and <stop>
        // in the same note
        SetTie(m_xml_last_note, false);
    }
    
 //   LogDebug("---- %s", element->GetClassName().c_str());
    
    return true;
 }




bool XMLOutput::WriteLayout( Doc *layout )
{
    LogDebug("Layout");
    return true;
}

bool XMLOutput::WritePage( Page *page )
{
    LogDebug("Page");
    return true;
}

bool XMLOutput::WriteSystem( System *system )
{
    LogDebug("System");
    return true;
}

bool XMLOutput::WriteLaidOutStaff( Staff *laidOutStaff )
{
    LogDebug("Laid staff");
    return true;
}

bool XMLOutput::WriteLaidOutLayer( Layer *laidOutLayer )
{    LogDebug("Laid layer");
    return true;

}

bool XMLOutput::WriteLaidOutLayerElement( LayerElement *laidOutLayerElement )
{
    LogDebug("Laid Layer Elem");
    return true;
}


void XMLOutput::WriteClef(LayerElement *element) {
    std::string sign, line;
    
    // Create the attributes elem
    // or use existing one, all the attribute changes
    // go in the same <attributes>
    CreateAttributes();
    
    Clef *clef = dynamic_cast<Clef*>(element);
    
    switch (clef->m_clefId) {
        case SOL1: sign = "G"; line = "1"; break;
        case SOL2: sign = "G"; line = "2"; break;
        case UT1: sign = "C"; line = "1"; break;
        case UT2: sign = "C"; line = "2"; break;
        case UT3: sign = "C"; line = "3"; break;
        case UT4: sign = "C"; line = "4"; break;
        case UT5: sign = "C"; line = "5"; break;
        case FA3: sign = "F"; line = "3"; break;
        case FA4: sign = "F"; line = "4"; break;
        case FA5: sign = "F"; line = "5"; break;
        default: break;
    }
    
    //Create the <clef> element
    TiXmlElement *xclef = new TiXmlElement("clef");
    
    // Create the <sign> element and link to it it's text
    TiXmlElement *xsign = new TiXmlElement("sign");
    TiXmlText *xsignt = new TiXmlText(sign.c_str());
    xsign->LinkEndChild(xsignt);
    // Insert it into the <clef>
    xclef->LinkEndChild(xsign);
    
    // Create the <line> element and link to it it's text
    TiXmlElement *xline = new TiXmlElement("line");
    TiXmlText *xlinet = new TiXmlText(line.c_str());
    xline->LinkEndChild(xlinet);
    // Insert it into the <clef>
    xclef->LinkEndChild(xline); 
    
    // place clef into <attribute>
    m_xml_attributes->LinkEndChild(xclef);
    m_xml_current_clef = xclef;
    
}

void XMLOutput::WriteKey(LayerElement *element) {
    KeySignature* key = dynamic_cast<KeySignature*>(element);
    
    // Check for attrib element as above
    // or use existing one, all the attribute changes
    // go in the same <attributes>
    CreateAttributes();
    
    // create tompost <key> elem
    TiXmlElement *xkey = new TiXmlElement("key");
    
    // Convert the number of alterations to string
    std::stringstream n_alter;
    if (key->GetAlteration() == ACCID_FLAT)
        // flats are negative numbers
        n_alter << -key->GetAlterationNumber();
    else
        n_alter << key->GetAlterationNumber();
    
    //create <fifths> node with the number of alterations
    TiXmlElement *xfifths = new TiXmlElement("fifths");
    TiXmlText *xftxt = new TiXmlText(n_alter.str().c_str());
    xfifths->LinkEndChild(xftxt);
    // add it to the key elem
    xkey->LinkEndChild(xfifths);
    
    //I dont know what musicxml does with the <mode> tag
    //bmaybe it is just to annoy programmers using tinyxml
    //finale sets this always to major
    TiXmlElement *mode = new TiXmlElement("mode");
    TiXmlText *major = new TiXmlText("major");
    mode->LinkEndChild(major);
    // add it to the key elem
    xkey->LinkEndChild(mode);
    
    // Obviously the order in which <key> <clef> and <time>
    // are fixed. If in music notation practice we have CLEF then KEY
    // why should MusicXML follow? the genious architect decided to
    // force the order of <attributes> REVERSED, <key> BEFORE <clef>
    // wow.
    if (!m_xml_current_clef) // no clef
        m_xml_attributes->LinkEndChild(xkey);
    else // insert before the clef
        m_xml_attributes->InsertBeforeChild(m_xml_current_clef, *xkey);
    
}

void XMLOutput::WriteTime(LayerElement *element) {
    Mensur *timesig = dynamic_cast<Mensur*>(element);
    std::stringstream number;
    
    CreateAttributes();
    
    TiXmlElement *xtime = new TiXmlElement("time");
    
    // add symbol attribute if necessay
    if (timesig->m_meterSymb == METER_SYMB_COMMON) {
        xtime->SetAttribute("symbol", "common");
        // if the number of beats was not specified, approximate it
        if (timesig->m_num == 0){
            timesig->m_num = 4;
            timesig->m_numBase = 4;
        }
    } else if (timesig->m_meterSymb == METER_SYMB_CUT) {
        xtime->SetAttribute("symbol", "cut");
        if (timesig->m_num == 0){
            timesig->m_num = 2;
            timesig->m_numBase = 2;
        }
    }
    
    // Create beat and beat type attribsb
    TiXmlElement *xbeats = new TiXmlElement("beats");
    TiXmlElement *xbtype = new TiXmlElement("beat-type");
    
    // convert number to text for beats
    number << timesig->m_num;
    TiXmlText *beat_text = new TiXmlText(number.str().c_str());
    xbeats->LinkEndChild(beat_text);
    
    // ditto as above
    number.clear();
    number << timesig->m_numBase;
    TiXmlText *base_text = new TiXmlText(number.str().c_str());
    xbtype->LinkEndChild(base_text);
    
    // push it to xtime
    xtime->LinkEndChild(xbeats);
    xtime->LinkEndChild(xbtype);
    
    if (!m_xml_current_clef) // no clef
        m_xml_attributes->LinkEndChild(xtime);
    else // insert before the clef
        m_xml_attributes->InsertBeforeChild(m_xml_current_clef, *xtime);
    
    // save the current timesig in case we need to create mm rests
    m_current_time = timesig;
}

void XMLOutput::WriteMultiMeasureRest(Rest *r) {
    std::stringstream num;
    num << r->m_multimeasure_dur;
    
    TiXmlElement *xmrest = new TiXmlElement("multiple-rest");
    TiXmlText *mdur = new TiXmlText(num.str().c_str());
    xmrest->LinkEndChild(mdur);
    
    
    // chech that <attributes> exists
    CreateAttributes();
    
    if (!m_xml_measure_style) {
        m_xml_measure_style = new TiXmlElement("measure-style");
        // link it into the current attributes
        m_xml_attributes->LinkEndChild(m_xml_measure_style);
    }
    
    // put the multiple measure thing into the measure style
    m_xml_measure_style->LinkEndChild(xmrest);
    
    // save the quantity of measures to create
    m_multimeasure_rests = r->m_multimeasure_dur;
    
}

void XMLOutput::CreateRestsForMultiMeasure() {
    
    // unbox all the measures we need
    for (int i = 0; i < m_multimeasure_rests; i++) {
        std::stringstream mstring;
        
        m_measure_count++;
        
        // create a fresh new rest
        TiXmlElement *note = new TiXmlElement("note");
        TiXmlElement *rest = new TiXmlElement("rest");
        
        note->LinkEndChild(rest);
        
        TiXmlElement *duration = new TiXmlElement("duration");
        TiXmlText *dur_name = new TiXmlText("4");
        duration->LinkEndChild(dur_name);
        
        note->LinkEndChild(duration);
        
        // add it to the current part
        m_xml_part->LinkEndChild(note);
        // first time is already linked
        if (i > 0)
            m_xml_measure->LinkEndChild(m_xml_part);
        // and to the measure
        m_xml_score->LinkEndChild(m_xml_measure);
        
        // create a new part
        m_xml_part = new TiXmlElement("part");
        m_xml_part->SetAttribute("id", "P1");
        
        m_xml_measure = new TiXmlElement("measure");
        mstring << m_measure_count;
        m_xml_measure->SetAttribute("number", mstring.str().c_str());
        
    }
    LogDebug("Measures incremented to %i.", m_measure_count);
    m_multimeasure_rests = 0;
    //m_measure_count--;
    // we do not want to increment this two times
    // a new measure is generated after the call to this function
    m_measure_count--;
}

void XMLOutput::WriteNoteOrRest(LayerElement *element) {
    char steps[] = {'C', 'D', 'E', 'F', 'G', 'A', 'B'};
    std::stringstream number;
    std::string t;
    std::string dur;
    std::string num_of_beams;
    
    DurationInterface *di = dynamic_cast<DurationInterface*>(element);
    
    // toplevel note element
    TiXmlElement *note = new TiXmlElement("note");
    
    // duration is common in notes and rests
    TiXmlElement *duration = new TiXmlElement("duration");
    
    switch (di->m_dur) {
        case DUR_LG: dur = "64"; t = "long"; break;
        case DUR_BR: dur = "32"; t = "breve"; break;
        case DUR_1: dur = "16"; t = "whole"; break;
        case DUR_2: dur = "8"; t = "half"; break;
        case DUR_4: dur = "4"; t = "quarter"; break;
        case DUR_8: dur = "2"; t = "eighth"; num_of_beams = "1"; break;
        case DUR_16: dur = "1"; t = "16th"; num_of_beams = "2"; break;
        case DUR_32: dur = "1"; t = "32nd"; num_of_beams = "3"; break;
        case DUR_64: dur = "1"; t = "64th"; num_of_beams = "4"; break;
        case DUR_128: dur = "1"; t = "128th"; num_of_beams = "5"; break;
            
        default:
            break;
    }
    
    // if note we need the <pitch> too
    if (dynamic_cast<Note*>(element)) {
        Note *n = dynamic_cast<Note*>(element);
        // make pitch
        TiXmlElement *pitch = new TiXmlElement("pitch");
        TiXmlElement *step = new TiXmlElement("step");
        TiXmlElement *octave = new TiXmlElement("octave");
        
        number << steps[n->m_pname - 1];
        TiXmlText *step_name = new TiXmlText(number.str().c_str());
        step->LinkEndChild(step_name);
        
        number.clear();
        number << (int)n->m_oct;
        TiXmlText *octave_name = new TiXmlText(number.str().c_str());
        octave->LinkEndChild(octave_name);
        
        // do we have an alteration?
        if (n->m_accid != 0) {
            number.clear();
            number << 1;
            TiXmlElement *xalter = new TiXmlElement("alter");
            TiXmlText *altnr = new TiXmlText(number.str().c_str());
            xalter->LinkEndChild(altnr);
            pitch->LinkEndChild(xalter);
        }
        
        pitch->LinkEndChild(step);
        pitch->LinkEndChild(octave);
        note->LinkEndChild(pitch);
        
        
    } else if (dynamic_cast<Rest*>(element)) {
        Rest *r = dynamic_cast<Rest*>(element);
        // rests just link a <rest /> item
        TiXmlElement *rest = new TiXmlElement("rest");
        
        note->LinkEndChild(rest);
        
        // handle multi measure rest
        // break from the generation of this element
        if (r->m_dur == VALSilSpec) {
            WriteMultiMeasureRest(r);
            return;
        }
    }
    
    // put the duration
    TiXmlText *dur_name = new TiXmlText(dur.c_str());
    duration->LinkEndChild(dur_name);
    //link it to <note>
    note->LinkEndChild(duration);
    
    // was the prevous note the last one in a tie?
    // always close it, ties do not <continue>
    if (m_tied)
        SetTie(note, true);
    // ties go between <duration> and <beam>
    
    if (t != "") {
        TiXmlElement *xtype = new TiXmlElement("type");
        TiXmlText *xtext = new TiXmlText(t.c_str());
        xtype->LinkEndChild(xtext);
        note->LinkEndChild(xtype);
    }
    
    // add eventual dots to note/rest
    // they go after <tie> and before <beam>
    for (unsigned int i = 1; i == di->m_dots; i++) {
        // a <dot /> for every dot
        TiXmlElement *dot = new TiXmlElement("dot");
        note->LinkEndChild(dot);
    }
    
    // Do beaming
    if (m_current_beam) {
        std::string btype;
        
        int position = m_current_beam->GetChildIndex( element );
        
        // if position == -1, the note is not into the current beam
        if (position >= 0) {
            
            // this flas is set so we know if we are "into" a beam
            if (!m_in_beam) {
                // not set, note is begining
                btype = "begin";
                m_in_beam = true;
            } else {
                // if this note is the last in the beam, end the beaming
                if (position == m_current_beam->GetNoteCount() - 1) {
                    btype = "end";
                    m_current_beam = NULL;
                    m_in_beam = false;
                } else {
                    // note in the middle, do nothing special (continue)
                    btype = "continue";
                }
            }
            
            // write the xml for the beam
            TiXmlElement *xbeam = new TiXmlElement("beam");
            // set the number of beams attribute (!!! couldn't they do this automatically???)
            // seems to work even if left as 1
            // num_of_beams is set above when parsing durations
            xbeam->SetAttribute("number", num_of_beams.c_str());
            
            // write the arribute and attach it to beam
            TiXmlText *xtype = new TiXmlText(btype.c_str());
            xbeam->LinkEndChild(xtype);
            
            // attach the beam to the note
            note->LinkEndChild(xbeam);
        } else {
            LogDebug("Beam with no notes");
        }
    }
    
    // measure in partwise
    // link to part
    m_xml_part->LinkEndChild(note);
    
    // save to current note pointer
    m_xml_last_note = note;
}

void XMLOutput::SetTie(TiXmlElement *xml_note, bool last) {
    TiXmlElement *xtie = new TiXmlElement("tie");
    std::string tie_type;
    
    if (!last) {
        tie_type = "start";
        m_tied = true;

    } else {
        // end tie, reset tied flag
        tie_type = "stop";
        m_tied = false;
    }
    
    // add text to tie
    xtie->SetAttribute("type", tie_type.c_str());
    
    // add to note
    // it has to be immediately after <duration>
    // so search for it
    TiXmlNode *duration = xml_note->LastChild("duration");
    // if exists, add it before
    if (duration)
        xml_note->InsertAfterChild(duration, *xtie);
    else
        // else just at the end
        xml_note->LinkEndChild(xtie);
}
               
void XMLOutput::CreateAttributes() {
    // Make new attributes as necessary
    if (m_xml_attributes == NULL) {
        m_xml_attributes = new TiXmlElement("attributes");
        // put it into the current part
        m_xml_part->LinkEndChild(m_xml_attributes);
    }
}

} // namespace vrv
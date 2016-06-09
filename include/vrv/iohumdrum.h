/////////////////////////////////////////////////////////////////////////////
// Name:        iohumdrum.h
// Author:      Craig Stuart Sapp
// Created:     06/06/2015
// Copyright (c) Authors and others. All rights reserved.
// vim:         ts=3
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_IOHUMDRUM_H__
#define __VRV_IOHUMDRUM_H__

#include <string>
#include <vector>

//----------------------------------------------------------------------------

#include "attdef.h"
#include "io.h"
#include "vrvdef.h"

#include "humlib.h"

//----------------------------------------------------------------------------

namespace vrv {

class FloatingElement;
class Layer;
class LayerElement;
class Measure;
class Slur;
class StaffGrp;
class System;
class Tie;
/// class Tuplet;

//----------------------------------------------------------------------------
// namespace for local Humdrum classes
//----------------------------------------------------------------------------

/* 
namespace humdrum {

	class OpenTie {
	public:
		OpenTie(int staffN, int layerN, data_PITCHNAME pname, char oct)
		{
			m_staffN = staffN;
			m_layerN = layerN;
			m_pname = pname;
			m_oct = oct;
		}

		int m_staffN;
		int m_layerN;
		data_PITCHNAME m_pname;
		char m_oct;
	};

	class OpenSlur {
	public:
		OpenSlur(int staffN, int layerN, int number)
		{
			m_staffN = staffN;
			m_layerN = layerN;
			m_number = number;
		}

		int m_staffN;
		int m_layerN;
		int m_number;
	};

} // namespace humdrum

*/

//----------------------------------------------------------------------------
// HumdrumInput
//----------------------------------------------------------------------------

class HumdrumInput : public FileInputStream {
	public:
		// constructors and destructors
		HumdrumInput(Doc *doc, std::string filename);
		virtual ~HumdrumInput();

		virtual bool ImportFile();
		virtual bool ImportString(std::string humdrum);

	protected:
	 	// Top level method called from ImportFile or ImportString
		bool ReadHumdrum(hum::HumdrumFile& infile);

/*

	 	// Top level methods for reading Humdrum part and measure elements.
		bool ReadHumdrumPart(hum::HumdrumFile& infile, System *system, int nbStaves, int staffOffset);
		bool ReadHumdrumMeasure(hum::HumdrumFile& infile, Measure *measure, int nbStaves, int staffOffset);

	 	// Methods for reading the first Humdrum attributes element as MEI staffDef.
	 	// Returns the number of staves in the part.
		int ReadHumdrumPartAttributesAsStaffDef(hum::HumdrumFile& infile, StaffGrp *staffGrp, int staffOffset);

	 	// Methods for reading the content of a Humdrum measure.
		void ReadHumdrumAttributes(hum::HumdrumFile& infile, Measure *measure, int measureNb);
		void ReadHumdrumBackup(hum::HumdrumFile& infile, Measure *measure, int measureNb);
		void ReadHumdrumBarLine(hum::HumdrumFile& infile, Measure *measure, int measureNb);
		void ReadHumdrumForward(hum::HumdrumFile& infile, Measure *measure, int measureNb);
		void ReadHumdrumNote(hum::HumdrumFile& infile, Measure *measure, int measureNb);

	 	// Add a Measure to the system.
	 	// If the measure already exists it will move all its content.
	 	// The measure can contain only staves. Other elements must be stacked on m_floatingElements.
		void AddMeasure(System *system, Measure *measure, int i);

	 	// Add a Layer element to the layer or to the LayerElement at the top of m_elementStack.
		void AddLayerElement(Layer *layer, LayerElement *element);

	 	// Returns the appropriate layer for a node looking at its Humdrum staff and voice elements.
		Layer *SelectLayer(hum::HumdrumFile& infile, Measure *measure);

	 	// Returns the appropriate first layer of a staff.
		Layer *SelectLayer(int staffNb, Measure *measure);

	 	// Returns the layer with @n=layerNb on the staff.
	 	// Creates the layer if not found.
		Layer *SelectLayer(int layerNb, Staff *staff);

	 	// Remove the last ClassId element on top of m_elementStack.
	 	// For example, when closing a beam, we need to remove it from the stack, but it is not
	 	// necessary the top one (for example we can have an opened chord there).
		void RemoveLastFromStack(ClassId classId);

	 	// Helper methods for checking presence of values of attributes or elements
		bool HasAttributeWithValue(hum::HumdrumFile& infile, std::string attribute, std::string value);
		bool IsElement(hum::HumdrumFile& infile, std::string name);
		bool HasContentWithValue(hum::HumdrumFile& infile, std::string value);
		bool HasContent(hum::HumdrumFile& infile);

	 	// Helper methods for retrieving attribute values or element content
		std::string GetAttributeValue(hum::HumdrumFile& infile, std::string attribute);
		std::string GetContent(hum::HumdrumFile& infile);
		std::string GetContentOfChild(hum::HumdrumFile& infile, std::string child);

	 	// Methods for opening and closing ties and slurs.
	 	// Opened ties and slurs are stacked together with humdrum::OpenTie
	 	// and humdrum::OpenSlur objects.
	 	// For now: only slurs starting and ending on the same staff/voice are
	 	// supported
		void OpenTie(Staff *staff, Layer *layer, Note *note, Tie *tie);
		void CloseTie(Staff *staff, Layer *layer, Note *note, bool isClosingTie);
		void OpenSlur(Staff *staff, Layer *layer, int number, LayerElement *element, Slur *slur);
		void CloseSlur(Staff *staff, Layer *layer, int number, LayerElement *element);

	 	// Methods for converting Humdrum string values to MEI attributes.
		data_ACCIDENTAL_EXPLICIT ConvertAccidentalToAccid(std::string value);
		data_ACCIDENTAL_EXPLICIT ConvertAlterToAccid(std::string value);
		data_DURATION ConvertTypeToDur(std::string value);
		data_PITCHNAME ConvertStepToPitchName(std::string value);
*/

	private:
		// The filename 
		std::string m_filename;
		// The stack for piling open LayerElements (beams, tuplets, chords, etc.)
		// std::vector<LayerElement *> m_elementStack;
		// The stack for open slurs
		// std::vector<std::pair<Slur *, humdrum::OpenSlur> > m_slurStack;
		// The stack for open ties
		// std::vector<std::pair<Tie *, humdrum::OpenTie> > m_tieStack;

	 	// The stack of floating elements (tie, slur, etc.) to be added at the
	 	// end of each measure
		// std::vector<std::pair<int, FloatingElement *> > m_floatingElements;
};

} // namespace vrv {

#endif // __VRV_IOHUMDRUM_H__




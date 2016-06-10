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

#include "page.h"
#include "system.h"
#include "measure.h"

#include "humlib.h"

//----------------------------------------------------------------------------

namespace vrv {

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
		void   clear                (void);
		bool   convertHumdrum       (hum::HumdrumFile& infile);
		void   setupMeiDocument     (void);
		int    getMeasureEndLine    (hum::HumdrumFile& infile, int startline);
		bool   convertSystemMeasure (hum::HumdrumFile& infile,
		                             const vector<hum::HTp>& kernstarts,
		                             int& line);
		void   setupSystemMeasure   (hum::HumdrumFile& infile,
		                             const vector<hum::HTp>& kernstarts,
		                             int startline, int endline);
		int    getMeasureNumber     (hum::HumdrumFile& infile, int startline,
		                             int endline);
      void   calculateLayout      (void);
		void   setSystemMeasureStyle(hum::HumdrumFile& infile,
		                             const vector<hum::HTp>& kernstarts,
		                             int startline, int endline);

	private:
		std::string m_filename;  // Filename to read/was read.
		// m_doc is inherited root document object.
		Page*       m_page;      // The current page, or NULL.
		System*     m_system;    // The current system, or NULL.
		Measure*    m_measure;   // The current measure, or NULL.
		int         m_debug;     // For printing MEI data to standard input.

};


} // namespace vrv


#endif // __VRV_IOHUMDRUM_H__




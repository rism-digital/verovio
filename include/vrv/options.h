/////////////////////////////////////////////////////////////////////////////
// Name:        options.h
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_OPTIONS_H__
#define __VRV_OPTIONS_H__

#ifdef CUSTOM_VEROVIO_OPTIONS
#include "custom_options.h"
#else

#include <map>
#include <string>
#include <vector>

//----------------------------------------------------------------------------

#include "attalternates.h"
#include "atttypes.h"
#include "jsonxx.h"

//----------------------------------------------------------------------------

namespace vrv {

class OptionGrp;

//----------------------------------------------------------------------------
// Default scaling (%) and spacing (units) values
//----------------------------------------------------------------------------

#define DEFAULT_SCALE 100
#define MIN_SCALE 1
#define MAX_SCALE 1000

//----------------------------------------------------------------------------
// Options
//----------------------------------------------------------------------------

// the space between each lyric line in units
#define TEMP_LYRIC_LINE_SPACE 5.0
// the key signature spacing factor
#define TEMP_KEYSIG_STEP 0.4
// the key signature spacing factor for natural (usually slighly larger)
#define TEMP_KEYSIG_NATURAL_STEP 0.6

//----------------------------------------------------------------------------
// Option defines
//----------------------------------------------------------------------------

enum option_BREAKS { BREAKS_none = 0, BREAKS_auto, BREAKS_line, BREAKS_encoded };

enum option_CONDENSE { CONDENSE_none = 0, CONDENSE_auto, CONDENSE_all, CONDENSE_encoded };

enum option_FOOTER { FOOTER_none = 0, FOOTER_auto, FOOTER_encoded, FOOTER_always };

enum option_HEADER { HEADER_none = 0, HEADER_auto, HEADER_encoded };

enum option_MEASURENUMBER { MEASURENUMBER_system = 0, MEASURENUMBER_interval };

enum option_SYSTEMDIVIDER { SYSTEMDIVIDER_none = 0, SYSTEMDIVIDER_auto, SYSTEMDIVIDER_left, SYSTEMDIVIDER_left_right };

//----------------------------------------------------------------------------
// Option
//----------------------------------------------------------------------------

/**
 * This class is a base class of each styling parameter
 */
class Option {
public:
    // constructors and destructors
    Option() : m_isSet(false) {}
    virtual ~Option() {}
    virtual void CopyTo(Option *option);

    void SetKey(const std::string &key) { m_key = key; }
    std::string GetKey() const { return m_key; }

    virtual bool SetValueBool(bool value);
    virtual bool SetValueDbl(double value);
    virtual bool SetValueArray(const std::vector<std::string> &values);
    virtual bool SetValue(const std::string &value);
    virtual std::string GetStrValue() const;
    virtual std::string GetDefaultStrValue() const;

    void SetInfo(const std::string &title, const std::string &description);
    std::string GetTitle() const { return m_title; }
    std::string GetDescription() const { return m_description; }

    bool isSet() const { return m_isSet; }

public:
    /**
     * Static maps used my OptionIntMap objects. Set in OptIntMap::Init
     */
    static std::map<int, std::string> s_breaks;
    static std::map<int, std::string> s_condense;
    static std::map<int, std::string> s_footer;
    static std::map<int, std::string> s_header;
    static std::map<int, std::string> s_measureNumber;
    static std::map<int, std::string> s_systemDivider;

protected:
    std::string m_title;
    std::string m_description;
    bool m_isSet;

private:
    std::string m_key;
};

//----------------------------------------------------------------------------
// OptionBool
//----------------------------------------------------------------------------

/**
 * This class is for boolean styling params
 */
class OptionBool : public Option {
public:
    // constructors and destructors
    OptionBool() {}
    virtual ~OptionBool() {}
    virtual void CopyTo(Option *option);
    void Init(bool defaultValue);

    virtual bool SetValueBool(bool value);
    virtual bool SetValueDbl(double value);
    virtual bool SetValue(const std::string &value);
    virtual std::string GetStrValue() const;
    virtual std::string GetDefaultStrValue() const;

    bool GetValue() const { return m_value; }
    bool GetDefault() const { return m_defaultValue; }
    bool SetValue(bool value);

private:
    //
public:
    //
private:
    bool m_value;
    bool m_defaultValue;
};

//----------------------------------------------------------------------------
// OptionDbl
//----------------------------------------------------------------------------

/**
 * This class is for integer styling params
 */
class OptionDbl : public Option {
public:
    // constructors and destructors
    OptionDbl() {}
    virtual ~OptionDbl() {}
    virtual void CopyTo(Option *option);
    void Init(double defaultValue, double minValue, double maxValue);

    virtual bool SetValueDbl(double value);
    virtual bool SetValue(const std::string &value);
    virtual std::string GetStrValue() const;
    virtual std::string GetDefaultStrValue() const;

    double GetValue() const { return m_value; }
    double GetDefault() const { return m_defaultValue; }
    double GetMin() const { return m_minValue; }
    double GetMax() const { return m_maxValue; }
    bool SetValue(double value);

private:
    //
public:
    //
private:
    double m_value;
    double m_defaultValue;
    double m_minValue;
    double m_maxValue;
};

//----------------------------------------------------------------------------
// OptionInt
//----------------------------------------------------------------------------

/**
 * This class is for integer styling params
 */
class OptionInt : public Option {
public:
    // constructors and destructors
    OptionInt() {}
    virtual ~OptionInt() {}
    virtual void CopyTo(Option *option);
    void Init(int defaultValue, int minValue, int maxValue, bool definitionFactor = false);

    virtual bool SetValueDbl(double value);
    virtual bool SetValue(const std::string &value);
    virtual std::string GetStrValue() const;
    virtual std::string GetDefaultStrValue() const;

    int GetValue() const;
    int GetUnfactoredValue() const;
    int GetDefault() const { return m_defaultValue; }
    int GetMin() const { return m_minValue; }
    int GetMax() const { return m_maxValue; }
    bool SetValue(int value);

private:
    //
public:
    //
private:
    int m_value;
    int m_defaultValue;
    int m_minValue;
    int m_maxValue;
    bool m_definitionFactor;
};

//----------------------------------------------------------------------------
// OptionString
//----------------------------------------------------------------------------

/**
 * This class is for string styling params
 */
class OptionString : public Option {
public:
    // constructors and destructors
    OptionString() {}
    virtual ~OptionString() {}
    virtual void CopyTo(Option *option);
    void Init(const std::string &defaultValue);

    virtual bool SetValue(const std::string &value);
    virtual std::string GetStrValue() const { return m_value; }
    virtual std::string GetDefaultStrValue() const { return m_defaultValue; }

    std::string GetValue() const { return m_value; }
    std::string GetDefault() const { return m_defaultValue; }

private:
    //
public:
    //
private:
    std::string m_value;
    std::string m_defaultValue;
};

//----------------------------------------------------------------------------
// OptionArray
//----------------------------------------------------------------------------

/**
 * This class is for array (string) styling params
 */
class OptionArray : public Option {
public:
    // constructors and destructors
    OptionArray() {}
    virtual ~OptionArray() {}
    virtual void CopyTo(Option *option);
    void Init();

    virtual bool SetValueArray(const std::vector<std::string> &values);
    virtual bool SetValue(const std::string &value);
    virtual std::string GetStrValue() const;
    virtual std::string GetDefaultStrValue() const;

    std::vector<std::string> GetValue() const { return m_values; }
    std::vector<std::string> GetDefault() const { return m_defaultValues; }
    bool SetValue(std::vector<std::string> const &values);

private:
    //
public:
    //
private:
    std::vector<std::string> m_values;
    std::vector<std::string> m_defaultValues;
};

//----------------------------------------------------------------------------
// OptionIntMap
//----------------------------------------------------------------------------

/**
 * This class is for map break options
 */
class OptionIntMap : public Option {
public:
    // constructors and destructors
    OptionIntMap();
    virtual ~OptionIntMap() {}
    virtual void CopyTo(Option *option);
    void Init(int defaultValue, std::map<int, std::string> *values);

    virtual bool SetValue(const std::string &value);
    virtual std::string GetStrValue() const;
    virtual std::string GetDefaultStrValue() const;

    int GetValue() const { return m_value; }
    int GetDefault() const { return m_defaultValue; }
    bool SetValue(int value);

    std::vector<std::string> GetStrValues(bool withoutDefault) const;
    std::string GetStrValuesAsStr(bool withoutDefault) const;

private:
    //
public:
    //
private:
    std::map<int, std::string> *m_values;
    int m_value;
    int m_defaultValue;
};

//----------------------------------------------------------------------------
// OptionStaffrel
//----------------------------------------------------------------------------

/**
 * This class is for map styling params
 */
class OptionStaffrel : public Option {
public:
    // constructors and destructors
    OptionStaffrel(){};
    virtual ~OptionStaffrel(){};
    virtual void CopyTo(Option *option);
    // Alternate type style cannot have a restricted list of possible values
    void Init(data_STAFFREL defaultValue);

    virtual bool SetValue(const std::string &value);
    virtual std::string GetStrValue() const;
    virtual std::string GetDefaultStrValue() const;

    // For altenate types return a reference to the value
    // Alternatively we can have a values vector for each sub-type
    const data_STAFFREL *GetValueAlternate() const { return &m_value; }
    const data_STAFFREL *GetDefaultAlernate() const { return &m_defaultValue; }

private:
    //
public:
    //
private:
    data_STAFFREL m_value;
    data_STAFFREL m_defaultValue;
};

//----------------------------------------------------------------------------
// OptionStaffrelBasic
//----------------------------------------------------------------------------

/**
 * This class is for map styling params
 */
class OptionStaffrelBasic : public Option {
public:
    // constructors and destructors
    OptionStaffrelBasic(){};
    virtual ~OptionStaffrelBasic(){};
    virtual void CopyTo(Option *option);
    void Init(data_STAFFREL_basic defaultValue, const std::vector<data_STAFFREL_basic> &values);

    virtual bool SetValue(const std::string &value);
    virtual std::string GetStrValue() const;
    virtual std::string GetDefaultStrValue() const;

    data_STAFFREL_basic GetValue() const { return m_value; }
    data_STAFFREL_basic GetDefault() const { return m_defaultValue; }

private:
    //
public:
    //
private:
    std::vector<data_STAFFREL_basic> m_values;
    data_STAFFREL_basic m_value;
    data_STAFFREL_basic m_defaultValue;
};

//----------------------------------------------------------------------------
// OptionJson
//----------------------------------------------------------------------------

/**
 * This class is for Json input params
 */

class OptionJson : public Option {
    using JsonPath = std::vector<std::reference_wrapper<jsonxx::Value> >;

public:
    //
    OptionJson() = default;
    virtual ~OptionJson() = default;
    virtual void Init(const std::string &defaultValue);

    virtual bool SetValue(const std::string &jsonFilePath);
    // virtual std::string GetStrValue() const;

    int GetIntValue(const std::vector<std::string> &jsonNodePath, bool getDefault = false) const;
    double GetDoubleValue(const std::vector<std::string> &jsonNodePath, bool getDefault = false) const;
    //
    bool UpdateNodeValue(const std::vector<std::string> &jsonNodePath, const std::string &value);
    //
protected:
    JsonPath StringPath2NodePath(const jsonxx::Object &obj, const std::vector<std::string> &jsonNodePath) const;
    //
private:
    jsonxx::Object m_values;
    jsonxx::Object m_defaultValues;
};

//----------------------------------------------------------------------------
// OptionGrp
//----------------------------------------------------------------------------

/**
 * This class is a base class of each styling parameter
 */
class OptionGrp {
public:
    // constructors and destructors
    OptionGrp() {}
    virtual ~OptionGrp() {}

    void SetLabel(const std::string &label, const std::string &id)
    {
        m_label = label;
        m_id = id;
    }
    std::string GetLabel() const { return m_label; }
    std::string GetId() const { return m_id; }

    void AddOption(Option *option) { m_options.push_back(option); }

    const std::vector<Option *> *GetOptions() const { return &m_options; }

public:
    //
protected:
    std::string m_id;
    std::string m_label;
    std::vector<Option *> m_options;
};

//----------------------------------------------------------------------------
// Options
//----------------------------------------------------------------------------

/**
 * This class contains the document styling parameters.
 */
class Options {
public:
    // constructors and destructors
    Options();
    virtual ~Options();

    Options(const Options &options);
    Options &operator=(const Options &options);

    const MapOfStrOptions *GetItems() const { return &m_items; }

    std::vector<OptionGrp *> *GetGrps() { return &m_grps; }

    // post processing of parameters
    void Sync();

private:
    void Register(Option *option, const std::string &key, OptionGrp *grp);

public:
    /**
     * Comments in implementation file options.cpp
     */

    /**
     * General
     */
    OptionGrp m_general;

    OptionBool m_adjustPageHeight;
    OptionBool m_adjustPageWidth;
    OptionIntMap m_breaks;
    OptionIntMap m_condense;
    OptionBool m_condenseFirstPage;
    OptionBool m_condenseTempoPages;
    OptionBool m_evenNoteSpacing;
    OptionBool m_humType;
    OptionBool m_justifyVertically;
    OptionBool m_landscape;
    OptionBool m_mensuralToMeasure;
    OptionDbl m_midiTempoAdjustment;
    OptionDbl m_minLastJustification;
    OptionBool m_mmOutput;
    OptionIntMap m_footer;
    OptionIntMap m_header;
    OptionBool m_noJustification;
    OptionBool m_openControlEvents;
    OptionInt m_outputIndent;
    OptionBool m_outputIndentTab;
    OptionBool m_outputSmuflXmlEntities;
    OptionInt m_pageHeight;
    OptionInt m_pageMarginBottom;
    OptionInt m_pageMarginLeft;
    OptionInt m_pageMarginRight;
    OptionInt m_pageMarginTop;
    OptionInt m_pageWidth;
    OptionString m_expand;
    OptionBool m_shrinkToFit;
    OptionBool m_svgBoundingBoxes;
    OptionBool m_svgViewBox;
    OptionBool m_svgHtml5;
    OptionInt m_unit;
    OptionBool m_useFacsimile;
    OptionBool m_usePgFooterForAll;
    OptionBool m_usePgHeaderForAll;
    OptionBool m_useBraceGlyph;

    /**
     * General layout
     */
    OptionGrp m_generalLayout;

    OptionDbl m_barLineSeparation;
    OptionDbl m_barLineWidth;
    OptionInt m_beamMaxSlope;
    OptionInt m_beamMinSlope;
    OptionDbl m_bracketThickness;
    OptionJson m_engravingDefaults;
    OptionString m_font;
    OptionDbl m_graceFactor;
    OptionBool m_graceRhythmAlign;
    OptionBool m_graceRightAlign;
    OptionDbl m_hairpinSize;
    OptionDbl m_hairpinThickness;
    OptionDbl m_justificationBraceGroup;
    OptionDbl m_justificationBracketGroup;
    OptionDbl m_justificationStaff;
    OptionDbl m_justificationSystem;
    OptionDbl m_ledgerLineThickness;
    OptionDbl m_ledgerLineExtension;
    OptionDbl m_lyricHyphenLength;
    OptionDbl m_lyricLineThickness;
    OptionBool m_lyricNoStartHyphen;
    OptionDbl m_lyricSize;
    OptionDbl m_lyricTopMinMargin;
    OptionDbl m_lyricWordSpace;
    OptionInt m_measureMinWidth;
    OptionIntMap m_measureNumber;
    OptionDbl m_repeatBarLineDotSeparation;
    OptionDbl m_repeatEndingLineThickness;
    OptionInt m_slurControlPoints;
    OptionInt m_slurCurveFactor;
    OptionInt m_slurHeightFactor;
    OptionDbl m_slurMaxHeight;
    OptionInt m_slurMaxSlope;
    OptionDbl m_slurMinHeight;
    OptionDbl m_slurEndpointThickness;
    OptionDbl m_slurMidpointThickness;
    OptionInt m_spacingBraceGroup;
    OptionInt m_spacingBracketGroup;
    OptionBool m_spacingDurDetection;
    OptionDbl m_spacingLinear;
    OptionDbl m_spacingNonLinear;
    OptionInt m_spacingStaff;
    OptionInt m_spacingSystem;
    OptionDbl m_staffLineWidth;
    OptionDbl m_stemWidth;
    OptionDbl m_subBracketThickness;
    OptionIntMap m_systemDivider;
    OptionInt m_systemMaxPerPage;
    OptionDbl m_textEnclosureThickness;
    OptionDbl m_thickBarlineThickness;
    OptionDbl m_tieEndpointThickness;
    OptionDbl m_tieMidpointThickness;
    OptionDbl m_tupletBracketThickness;

    /**
     * Selectors
     */
    OptionGrp m_selectors;

    OptionArray m_appXPathQuery;
    OptionArray m_choiceXPathQuery;
    OptionString m_mdivXPathQuery;
    OptionArray m_substXPathQuery;
    OptionString m_transpose;
    OptionBool m_transposeSelectedOnly;

    /**
     * Element margins
     */
    OptionGrp m_elementMargins;

    OptionDbl m_defaultBottomMargin;
    OptionDbl m_defaultLeftMargin;
    OptionDbl m_defaultRightMargin;
    OptionDbl m_defaultTopMargin;
    //
    OptionDbl m_bottomMarginHarm;
    OptionDbl m_bottomMarginPgHead;
    //
    OptionDbl m_leftMarginAccid;
    OptionDbl m_leftMarginBarLine;
    OptionDbl m_leftMarginBeatRpt;
    OptionDbl m_leftMarginChord;
    OptionDbl m_leftMarginClef;
    OptionDbl m_leftMarginKeySig;
    OptionDbl m_leftMarginLeftBarLine;
    OptionDbl m_leftMarginMensur;
    OptionDbl m_leftMarginMeterSig;
    OptionDbl m_leftMarginMRest;
    OptionDbl m_leftMarginMRpt2;
    OptionDbl m_leftMarginMultiRest;
    OptionDbl m_leftMarginMultiRpt;
    OptionDbl m_leftMarginNote;
    OptionDbl m_leftMarginRest;
    OptionDbl m_leftMarginRightBarLine;
    //
    OptionDbl m_rightMarginAccid;
    OptionDbl m_rightMarginBarLine;
    OptionDbl m_rightMarginBeatRpt;
    OptionDbl m_rightMarginChord;
    OptionDbl m_rightMarginClef;
    OptionDbl m_rightMarginKeySig;
    OptionDbl m_rightMarginLeftBarLine;
    OptionDbl m_rightMarginMensur;
    OptionDbl m_rightMarginMeterSig;
    OptionDbl m_rightMarginMRest;
    OptionDbl m_rightMarginMRpt2;
    OptionDbl m_rightMarginMultiRest;
    OptionDbl m_rightMarginMultiRpt;
    OptionDbl m_rightMarginNote;
    OptionDbl m_rightMarginRest;
    OptionDbl m_rightMarginRightBarLine;
    //
    OptionDbl m_topMarginHarm;

    /**
     * Deprecated options
     */
    OptionGrp m_deprecated;

    OptionBool m_condenseEncoded;
    OptionDbl m_slurThickness;
    OptionDbl m_tieThickness;

private:
    /** The array of style parameters */
    MapOfStrOptions m_items;

    std::vector<OptionGrp *> m_grps;
};

} // namespace vrv

#endif // CUSTOM_VEROVIO_OPTIONS

#endif // __VRV_DEF_H__

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
#include "smufl.h"
#include "vrvdef.h"

//----------------------------------------------------------------------------

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

enum option_BREAKS { BREAKS_none = 0, BREAKS_auto, BREAKS_line, BREAKS_smart, BREAKS_encoded };

enum option_CONDENSE { CONDENSE_none = 0, CONDENSE_auto, CONDENSE_all, CONDENSE_encoded };

enum option_ELISION {
    ELISION_regular = SMUFL_E551_lyricsElision,
    ELISION_narrow = SMUFL_E550_lyricsElisionNarrow,
    ELISION_wide = SMUFL_E552_lyricsElisionWide,
    ELISION_unicode = UNICODE_UNDERTIE
};

enum option_FONT_FALLBACK { FONT_FALLBACK_Leipzig = 0, FONT_FALLBACK_Bravura };

enum option_FOOTER { FOOTER_none = 0, FOOTER_auto, FOOTER_encoded, FOOTER_always };

enum option_HEADER { HEADER_none = 0, HEADER_auto, HEADER_encoded };

enum option_MULTIRESTSTYLE {
    MULTIRESTSTYLE_auto = 0,
    MULTIRESTSTYLE_default,
    MULTIRESTSTYLE_block,
    MULTIRESTSTYLE_symbols
};

enum option_SYSTEMDIVIDER { SYSTEMDIVIDER_none = 0, SYSTEMDIVIDER_auto, SYSTEMDIVIDER_left, SYSTEMDIVIDER_left_right };

enum option_SMUFLTEXTFONT { SMUFLTEXTFONT_embedded = 0, SMUFLTEXTFONT_linked, SMUFLTEXTFONT_none };

//----------------------------------------------------------------------------
// Option
//----------------------------------------------------------------------------

enum class OptionsCategory { None, Base, General, Layout, Mensural, Margins, Midi, Selectors, Full };

/**
 * This class is a base class of each styling parameter
 */
class Option {
public:
    // constructors and destructors
    Option()
    {
        m_shortOption = 0;
        m_isCmdOnly = false;
    }
    virtual ~Option() {}
    virtual void CopyTo(Option *option) = 0;

    void SetKey(const std::string &key) { m_key = key; }
    std::string GetKey() const { return m_key; }

    virtual bool SetValueBool(bool value);
    virtual bool SetValueDbl(double value);
    virtual bool SetValueArray(const std::vector<std::string> &values);
    virtual bool SetValue(const std::string &value);
    virtual std::string GetStrValue() const = 0;
    virtual std::string GetDefaultStrValue() const = 0;

    virtual void Reset() = 0;
    virtual bool IsSet() const = 0;

    void SetInfo(const std::string &title, const std::string &description);
    std::string GetTitle() const { return m_title; }
    std::string GetDescription() const { return m_description; }

    void SetShortOption(char shortOption, bool isCmdOnly);
    char GetShortOption() const { return m_shortOption; }
    bool IsCmdOnly() const { return m_isCmdOnly; }
    virtual bool IsArgumentRequired() const { return true; }

    /**
     * Return a JSON object for the option
     */
    jsonxx::Object ToJson() const;

public:
    //----------------//
    // Static members //
    //----------------//

    /**
     * Static maps used my OptionIntMap objects. Set in OptIntMap::Init
     */
    static const std::map<int, std::string> s_breaks;
    static const std::map<int, std::string> s_condense;
    static const std::map<int, std::string> s_elision;
    static const std::map<int, std::string> s_fontFallback;
    static const std::map<int, std::string> s_footer;
    static const std::map<int, std::string> s_header;
    static const std::map<int, std::string> s_multiRestStyle;
    static const std::map<int, std::string> s_pedalStyle;
    static const std::map<int, std::string> s_systemDivider;
    static const std::map<int, std::string> s_smuflTextFont;

protected:
    std::string m_title;
    std::string m_description;

private:
    std::string m_key;
    /* the character for a short option - not set (0) by default) */
    char m_shortOption;
    /* a flag indicating that the option is available only on the command line */
    bool m_isCmdOnly;
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
    OptionBool() { m_defaultValue = false; }
    virtual ~OptionBool() {}
    void CopyTo(Option *option) override;
    void Init(bool defaultValue);

    bool SetValueBool(bool value) override;
    bool SetValueDbl(double value) override;
    bool SetValue(const std::string &value) override;
    std::string GetStrValue() const override;
    std::string GetDefaultStrValue() const override;

    void Reset() override;
    bool IsSet() const override;

    bool GetValue() const { return m_value; }
    bool GetDefault() const { return m_defaultValue; }
    bool SetValue(bool value);

    bool IsArgumentRequired() const override { return false; }

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
    OptionDbl()
    {
        m_defaultValue = 0.0;
        m_minValue = 0.0;
        m_maxValue = 0.0;
        m_definitionFactor = false;
    }
    virtual ~OptionDbl() {}
    void CopyTo(Option *option) override;
    void Init(double defaultValue, double minValue, double maxValue, bool definitionFactor = false);

    bool SetValueDbl(double value) override;
    bool SetValue(const std::string &value) override;
    std::string GetStrValue() const override;
    std::string GetDefaultStrValue() const override;

    void Reset() override;
    bool IsSet() const override;

    double GetValue() const;
    double GetUnfactoredValue() const;
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
    bool m_definitionFactor;
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
    OptionInt()
    {
        m_defaultValue = 0;
        m_minValue = 0;
        m_maxValue = 0;
        m_definitionFactor = false;
    }
    virtual ~OptionInt() {}
    void CopyTo(Option *option) override;
    void Init(int defaultValue, int minValue, int maxValue, bool definitionFactor = false);

    bool SetValueDbl(double value) override;
    bool SetValue(const std::string &value) override;
    std::string GetStrValue() const override;
    std::string GetDefaultStrValue() const override;

    void Reset() override;
    bool IsSet() const override;

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
    void CopyTo(Option *option) override;
    void Init(const std::string &defaultValue);

    bool SetValue(const std::string &value) override;
    std::string GetStrValue() const override { return m_value; }
    std::string GetDefaultStrValue() const override { return m_defaultValue; }

    std::string GetValue() const { return m_value; }
    std::string GetDefault() const { return m_defaultValue; }

    void Reset() override;
    bool IsSet() const override;

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
    void CopyTo(Option *option) override;
    void Init();

    bool SetValueArray(const std::vector<std::string> &values) override;
    bool SetValue(const std::string &value) override;
    std::string GetStrValue() const override;
    std::string GetDefaultStrValue() const override;

    std::vector<std::string> GetValue() const { return m_values; }
    std::vector<std::string> GetDefault() const { return m_defaultValues; }
    bool SetValue(std::vector<std::string> const &values);

    void Reset() override;
    bool IsSet() const override;

private:
    std::string GetStr(const std::vector<std::string> &values) const;

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
    void CopyTo(Option *option) override;
    void Init(int defaultValue, const std::map<int, std::string> *values);

    bool SetValue(const std::string &value) override;
    std::string GetStrValue() const override;
    std::string GetDefaultStrValue() const override;

    int GetValue() const { return m_value; }
    int GetDefault() const { return m_defaultValue; }
    bool SetValue(int value);

    std::vector<std::string> GetStrValues(bool withoutDefault) const;
    std::string GetStrValuesAsStr(bool withoutDefault) const;

    void Reset() override;
    bool IsSet() const override;

private:
    //
public:
    //
private:
    const std::map<int, std::string> *m_values;
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
    void CopyTo(Option *option) override;
    // Alternate type style cannot have a restricted list of possible values
    void Init(data_STAFFREL defaultValue);

    bool SetValue(const std::string &value) override;
    std::string GetStrValue() const override;
    std::string GetDefaultStrValue() const override;

    void Reset() override;
    bool IsSet() const override;

    // For alternate types return a reference to the value
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
// OptionJson
//----------------------------------------------------------------------------

/// Distinguish whether Json is passed directly or should be read from file
enum class JsonSource { String, FilePath };

/**
 * This class is for Json input params
 */

class OptionJson : public Option {
    using JsonPath = std::vector<std::reference_wrapper<jsonxx::Value>>;

public:
    /**
     * @name Constructor, destructor and initialization
     */
    ///@{
    OptionJson() = default;
    virtual ~OptionJson() = default;
    void CopyTo(Option *option) override;
    void Init(JsonSource source, const std::string &defaultValue);
    ///@}

    /**
     * Member access
     */
    JsonSource GetSource() const;
    jsonxx::Object GetValue(bool getDefault = false) const;

    /**
     * Interface methods: accessing values as string
     */
    ///@{
    bool SetValue(const std::string &value) override;
    std::string GetStrValue() const override;
    std::string GetDefaultStrValue() const override;

    void Reset() override;
    bool IsSet() const override;
    ///@}

    /**
     * Accessing values as json node path
     */
    ///@{
    bool HasValue(const std::vector<std::string> &jsonNodePath) const;
    int GetIntValue(const std::vector<std::string> &jsonNodePath, bool getDefault = false) const;
    double GetDblValue(const std::vector<std::string> &jsonNodePath, bool getDefault = false) const;
    std::string GetStrValue(const std::vector<std::string> &jsonNodePath, bool getDefault = false) const;
    bool UpdateNodeValue(const std::vector<std::string> &jsonNodePath, const std::string &value);
    ///@}

    /**
     * Accessing all keys
     */
    ///@{
    std::set<std::string> GetKeys() const;
    std::set<std::string> GetKeysByNode(const std::string &nodeName, std::list<std::string> &jsonNodePath) const;
    ///@}

protected:
    JsonPath StringPath2NodePath(const jsonxx::Object &obj, const std::vector<std::string> &jsonNodePath) const;

    // Find node by recursively processing all elements within. Only nodes of OBJECT_ type are processed
    const jsonxx::Object *FindNodeByName(
        const jsonxx::Object &obj, const std::string &jsonNodeName, std::list<std::string> &jsonNodePath) const;

    /// Read json from string or file
    bool ReadJson(jsonxx::Object &output, const std::string &input) const;

private:
    JsonSource m_source = JsonSource::String;

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

    void SetCategory(OptionsCategory category) { m_category = category; }
    OptionsCategory GetCategory() const { return m_category; }

    void AddOption(Option *option) { m_options.push_back(option); }

    const std::vector<Option *> *GetOptions() const { return &m_options; }

public:
    //
protected:
    std::string m_id;
    std::string m_label;
    std::vector<Option *> m_options;
    OptionsCategory m_category = OptionsCategory::None;
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

    const std::vector<OptionGrp *> *GetGrps() const { return &m_grps; }

    jsonxx::Object GetBaseOptGrp();

    const std::vector<Option *> *GetBaseOptions() const;

    // post processing of parameters
    void Sync();

private:
    void Register(Option *option, const std::string &key, OptionGrp *grp);

public:
    /**
     * Comments in implementation file options.cpp
     */
    OptionGrp m_baseOptions;

    // These options are only given for documentation - except for m_scale
    // They are ordered by short option alphabetical order
    OptionBool m_standardOutput;
    OptionString m_help;
    OptionBool m_allPages;
    OptionString m_inputFrom;
    OptionString m_logLevel;
    OptionString m_outfile;
    OptionInt m_page;
    OptionString m_resourcePath;
    OptionInt m_scale;
    OptionString m_outputTo;
    OptionBool m_version;
    OptionInt m_xmlIdSeed;

    /**
     * General
     */
    OptionGrp m_general;

    OptionBool m_adjustPageHeight;
    OptionBool m_adjustPageWidth;
    OptionIntMap m_breaks;
    OptionDbl m_breaksSmartSb;
    OptionIntMap m_condense;
    OptionBool m_condenseFirstPage;
    OptionBool m_condenseNotLastSystem;
    OptionBool m_condenseTempoPages;
    OptionBool m_evenNoteSpacing;
    OptionString m_expand;
    OptionIntMap m_footer;
    OptionIntMap m_header;
    OptionBool m_humType;
    OptionBool m_incip;
    OptionBool m_justifyVertically;
    OptionBool m_landscape;
    OptionDbl m_minLastJustification;
    OptionBool m_mmOutput;
    OptionBool m_moveScoreDefinitionToStaff;
    OptionBool m_neumeAsNote;
    OptionBool m_noJustification;
    OptionBool m_openControlEvents;
    OptionBool m_outputFormatRaw;
    OptionInt m_outputIndent;
    OptionBool m_outputIndentTab;
    OptionBool m_outputSmuflXmlEntities;
    OptionInt m_pageHeight;
    OptionInt m_pageMarginBottom;
    OptionInt m_pageMarginLeft;
    OptionInt m_pageMarginRight;
    OptionInt m_pageMarginTop;
    OptionInt m_pageWidth;
    OptionIntMap m_pedalStyle;
    OptionBool m_preserveAnalyticalMarkup;
    OptionBool m_removeIds;
    OptionBool m_scaleToPageSize;
    OptionBool m_setLocale;
    OptionBool m_showRuntime;
    OptionBool m_shrinkToFit;
    OptionIntMap m_smuflTextFont;
    OptionBool m_staccatoCenter;
    OptionBool m_svgBoundingBoxes;
    OptionString m_svgCss;
    OptionBool m_svgViewBox;
    OptionBool m_svgHtml5;
    OptionBool m_svgFormatRaw;
    OptionBool m_svgRemoveXlink;
    OptionArray m_svgAdditionalAttribute;
    OptionDbl m_unit;
    OptionBool m_useFacsimile;
    OptionBool m_usePgFooterForAll;
    OptionBool m_usePgHeaderForAll;
    OptionBool m_useBraceGlyph;
    OptionBool m_xmlIdChecksum;

    /**
     * General layout
     */
    OptionGrp m_generalLayout;

    OptionDbl m_barLineSeparation;
    OptionDbl m_barLineWidth;
    OptionBool m_beamFrenchStyle;
    OptionInt m_beamMaxSlope;
    OptionBool m_beamMixedPreserve;
    OptionDbl m_beamMixedStemMin;
    OptionDbl m_bracketThickness;
    OptionBool m_breaksNoWidow;
    OptionDbl m_dashedBarLineDashLength;
    OptionDbl m_dashedBarLineGapLength;
    OptionDbl m_dynamDist;
    OptionBool m_dynamSingleGlyphs;
    OptionJson m_engravingDefaults;
    OptionJson m_engravingDefaultsFile;
    OptionDbl m_extenderLineMinSpace;
    OptionDbl m_fingeringScale;
    OptionString m_font;
    OptionArray m_fontAddCustom;
    OptionIntMap m_fontFallback;
    OptionBool m_fontLoadAll;
    OptionDbl m_graceFactor;
    OptionBool m_graceRhythmAlign;
    OptionBool m_graceRightAlign;
    OptionDbl m_hairpinSize;
    OptionDbl m_hairpinThickness;
    OptionArray m_handwrittenFont;
    OptionDbl m_harmDist;
    OptionDbl m_justificationBraceGroup;
    OptionDbl m_justificationBracketGroup;
    OptionDbl m_justificationStaff;
    OptionDbl m_justificationSystem;
    OptionDbl m_justificationMaxVertical;
    OptionDbl m_ledgerLineThickness;
    OptionDbl m_ledgerLineExtension;
    OptionIntMap m_lyricElision;
    OptionDbl m_lyricLineThickness;
    OptionBool m_lyricNoStartHyphen;
    OptionDbl m_lyricSize;
    OptionDbl m_lyricTopMinMargin;
    OptionBool m_lyricVerseCollapse;
    OptionDbl m_lyricWordSpace;
    OptionInt m_measureMinWidth;
    OptionInt m_mnumInterval;
    OptionIntMap m_multiRestStyle;
    OptionDbl m_multiRestThickness;
    OptionBool m_octaveAlternativeSymbols;
    OptionDbl m_octaveLineThickness;
    OptionBool m_octaveNoSpanningParentheses;
    OptionDbl m_pedalLineThickness;
    OptionDbl m_repeatBarLineDotSeparation;
    OptionDbl m_repeatEndingLineThickness;
    OptionDbl m_slurCurveFactor;
    OptionDbl m_slurEndpointFlexibility;
    OptionDbl m_slurEndpointThickness;
    OptionDbl m_slurMargin;
    OptionInt m_slurMaxSlope;
    OptionDbl m_slurMidpointThickness;
    OptionDbl m_slurSymmetry;
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
    OptionDbl m_tieMinLength;
    OptionBool m_tupletAngledOnBeams;
    OptionDbl m_tupletBracketThickness;
    OptionBool m_tupletNumHead;

    /**
     * Selectors
     */
    OptionGrp m_selectors;

    OptionArray m_appXPathQuery;
    OptionArray m_choiceXPathQuery;
    OptionBool m_loadSelectedMdivOnly;
    OptionBool m_mdivAll;
    OptionString m_mdivXPathQuery;
    OptionArray m_substXPathQuery;
    OptionString m_transpose;
    OptionJson m_transposeMdiv;
    OptionBool m_transposeSelectedOnly;
    OptionBool m_transposeToSoundingPitch;

    /**
     * Element margins
     */
    OptionGrp m_elementMargins;

    OptionDbl m_defaultBottomMargin;
    OptionDbl m_defaultLeftMargin;
    OptionDbl m_defaultRightMargin;
    OptionDbl m_defaultTopMargin;
    //
    OptionDbl m_bottomMarginArtic;
    OptionDbl m_bottomMarginHarm;
    OptionDbl m_bottomMarginOctave;
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
    OptionDbl m_leftMarginTabDurSym;
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
    OptionDbl m_rightMarginTabDurSym;
    //
    OptionDbl m_topMarginArtic;
    OptionDbl m_topMarginHarm;
    OptionDbl m_topMarginPgFooter;

    /**
     * Midi
     */
    OptionGrp m_midi;

    OptionBool m_midiNoCue;
    OptionDbl m_midiTempoAdjustment;

    /**
     * Mensural
     */
    OptionGrp m_mensural;

    OptionBool m_ligatureAsBracket;
    OptionBool m_mensuralToMeasure;

    /**
     * Deprecated options
     */
    OptionGrp m_deprecated;

private:
    /** The array of style parameters */
    MapOfStrOptions m_items;

    std::vector<OptionGrp *> m_grps;
};

} // namespace vrv

#endif // CUSTOM_VEROVIO_OPTIONS

#endif // __VRV_DEF_H__

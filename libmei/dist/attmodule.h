/////////////////////////////////////////////////////////////////////////////
// Authors:     Laurent Pugin and Rodolfo Zitellini
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
//
// Code generated using a modified version of libmei
// by Andrew Hankinson, Alastair Porter, and Others
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// NOTE: this file was generated with the Verovio libmei version and
// should not be edited because changes will be lost.
/////////////////////////////////////////////////////////////////////////////

#ifndef __LIBMEI_ATT_MODULE_H__
#define __LIBMEI_ATT_MODULE_H__

#include <string>
#include <utility>
#include <vector>

namespace vrv {

typedef std::vector<std::pair<std::string, std::string>> ArrayOfStrAttr;

class Object;

//----------------------------------------------------------------------------
// AttModule
//----------------------------------------------------------------------------

/**
 * This is the base class for all MEI att classes.
 * It is not an abstract class but it should not be instanciated directly.
 * The att classes are generated with the libmei parser for Verovio.
 */
class AttModule {
public:
    /** @name Constructors and destructor */
    ///@{
    AttModule(){};
    virtual ~AttModule(){};
    ///@}

    /**
     * @name static method for blind attribute reading or modification
     */
    ///@{
    static bool SetMei(Object *element, const std::string &attrType, const std::string &attrValue);
    static void GetMei(const Object *element, ArrayOfStrAttr *attributes);

    static bool SetAnalytical(Object *element, const std::string &attrType, const std::string &attrValue);
    static void GetAnalytical(const Object *element, ArrayOfStrAttr *attributes);

    static bool SetCmn(Object *element, const std::string &attrType, const std::string &attrValue);
    static void GetCmn(const Object *element, ArrayOfStrAttr *attributes);

    static bool SetCmnornaments(Object *element, const std::string &attrType, const std::string &attrValue);
    static void GetCmnornaments(const Object *element, ArrayOfStrAttr *attributes);

    static bool SetCritapp(Object *element, const std::string &attrType, const std::string &attrValue);
    static void GetCritapp(const Object *element, ArrayOfStrAttr *attributes);

    static bool SetEdittrans(Object *element, const std::string &attrType, const std::string &attrValue);
    static void GetEdittrans(const Object *element, ArrayOfStrAttr *attributes);

    static bool SetExternalsymbols(Object *element, const std::string &attrType, const std::string &attrValue);
    static void GetExternalsymbols(const Object *element, ArrayOfStrAttr *attributes);

    static bool SetFacsimile(Object *element, const std::string &attrType, const std::string &attrValue);
    static void GetFacsimile(const Object *element, ArrayOfStrAttr *attributes);

    static bool SetFigtable(Object *element, const std::string &attrType, const std::string &attrValue);
    static void GetFigtable(const Object *element, ArrayOfStrAttr *attributes);

    static bool SetFingering(Object *element, const std::string &attrType, const std::string &attrValue);
    static void GetFingering(const Object *element, ArrayOfStrAttr *attributes);

    static bool SetFrettab(Object *element, const std::string &attrType, const std::string &attrValue);
    static void GetFrettab(const Object *element, ArrayOfStrAttr *attributes);

    static bool SetGestural(Object *element, const std::string &attrType, const std::string &attrValue);
    static void GetGestural(const Object *element, ArrayOfStrAttr *attributes);

    static bool SetHarmony(Object *element, const std::string &attrType, const std::string &attrValue);
    static void GetHarmony(const Object *element, ArrayOfStrAttr *attributes);

    static bool SetHeader(Object *element, const std::string &attrType, const std::string &attrValue);
    static void GetHeader(const Object *element, ArrayOfStrAttr *attributes);

    static bool SetMensural(Object *element, const std::string &attrType, const std::string &attrValue);
    static void GetMensural(const Object *element, ArrayOfStrAttr *attributes);

    static bool SetMidi(Object *element, const std::string &attrType, const std::string &attrValue);
    static void GetMidi(const Object *element, ArrayOfStrAttr *attributes);

    static bool SetNeumes(Object *element, const std::string &attrType, const std::string &attrValue);
    static void GetNeumes(const Object *element, ArrayOfStrAttr *attributes);

    static bool SetPagebased(Object *element, const std::string &attrType, const std::string &attrValue);
    static void GetPagebased(const Object *element, ArrayOfStrAttr *attributes);

    static bool SetPerformance(Object *element, const std::string &attrType, const std::string &attrValue);
    static void GetPerformance(const Object *element, ArrayOfStrAttr *attributes);

    static bool SetShared(Object *element, const std::string &attrType, const std::string &attrValue);
    static void GetShared(const Object *element, ArrayOfStrAttr *attributes);

    static bool SetUsersymbols(Object *element, const std::string &attrType, const std::string &attrValue);
    static void GetUsersymbols(const Object *element, ArrayOfStrAttr *attributes);

    static bool SetVisual(Object *element, const std::string &attrType, const std::string &attrValue);
    static void GetVisual(const Object *element, ArrayOfStrAttr *attributes);

    ///@}
};

} // namespace vrv

#endif // __LIBMEI_ATT_MODULE_H__

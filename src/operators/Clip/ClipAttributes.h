#ifndef CLIPATTRIBUTES_H
#define CLIPATTRIBUTES_H
#include <string>
#include <AttributeSubject.h>

// ****************************************************************************
// Class: ClipAttributes
//
// Purpose:
//    This class contains attributes for the clip operator.
//
// Notes:      Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   Thu Dec 18 11:49:24 PDT 2003
//
// Modifications:
//   
// ****************************************************************************

class ClipAttributes : public AttributeSubject
{
public:
    enum ClipStyle
    {
        Plane,
        Sphere
    };

    ClipAttributes();
    ClipAttributes(const ClipAttributes &obj);
    virtual ~ClipAttributes();

    virtual void operator = (const ClipAttributes &obj);
    virtual bool operator == (const ClipAttributes &obj) const;
    virtual bool operator != (const ClipAttributes &obj) const;

    virtual const std::string TypeName() const;
    virtual bool CopyAttributes(const AttributeGroup *);
    virtual AttributeSubject *CreateCompatible(const std::string &) const;
    virtual AttributeSubject *NewInstance(bool) const;

    // Property selection methods
    virtual void SelectAll();
    void SelectPlane1Origin();
    void SelectPlane2Origin();
    void SelectPlane3Origin();
    void SelectPlane1Normal();
    void SelectPlane2Normal();
    void SelectPlane3Normal();
    void SelectCenter();

    // Property setting methods
    void SetFuncType(ClipStyle funcType_);
    void SetPlane1Status(bool plane1Status_);
    void SetPlane2Status(bool plane2Status_);
    void SetPlane3Status(bool plane3Status_);
    void SetPlane1Origin(const double *plane1Origin_);
    void SetPlane2Origin(const double *plane2Origin_);
    void SetPlane3Origin(const double *plane3Origin_);
    void SetPlane1Normal(const double *plane1Normal_);
    void SetPlane2Normal(const double *plane2Normal_);
    void SetPlane3Normal(const double *plane3Normal_);
    void SetPlaneInverse(bool planeInverse_);
    void SetCenter(const double *center_);
    void SetRadius(double radius_);
    void SetSphereInverse(bool sphereInverse_);

    // Property getting methods
    ClipStyle    GetFuncType() const;
    bool         GetPlane1Status() const;
    bool         GetPlane2Status() const;
    bool         GetPlane3Status() const;
    const double *GetPlane1Origin() const;
          double *GetPlane1Origin();
    const double *GetPlane2Origin() const;
          double *GetPlane2Origin();
    const double *GetPlane3Origin() const;
          double *GetPlane3Origin();
    const double *GetPlane1Normal() const;
          double *GetPlane1Normal();
    const double *GetPlane2Normal() const;
          double *GetPlane2Normal();
    const double *GetPlane3Normal() const;
          double *GetPlane3Normal();
    bool         GetPlaneInverse() const;
    const double *GetCenter() const;
          double *GetCenter();
    double       GetRadius() const;
    bool         GetSphereInverse() const;

    // Persistence methods
    virtual bool CreateNode(DataNode *node, bool completeSave, bool forceAdd);
    virtual void SetFromNode(DataNode *node);

    // Enum conversion functions
    static std::string ClipStyle_ToString(ClipStyle);
    static bool ClipStyle_FromString(const std::string &, ClipStyle &);
protected:
    static std::string ClipStyle_ToString(int);
public:

    // Keyframing methods
    virtual std::string               GetFieldName(int index) const;
    virtual AttributeGroup::FieldType GetFieldType(int index) const;
    virtual std::string               GetFieldTypeName(int index) const;
    virtual bool                      FieldsEqual(int index, const AttributeGroup *rhs) const;

private:
    int    funcType;
    bool   plane1Status;
    bool   plane2Status;
    bool   plane3Status;
    double plane1Origin[3];
    double plane2Origin[3];
    double plane3Origin[3];
    double plane1Normal[3];
    double plane2Normal[3];
    double plane3Normal[3];
    bool   planeInverse;
    double center[3];
    double radius;
    bool   sphereInverse;
};

#endif

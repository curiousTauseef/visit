#ifndef CYLINDERATTRIBUTES_H
#define CYLINDERATTRIBUTES_H
#include <AttributeSubject.h>

// ****************************************************************************
// Class: CylinderAttributes
//
// Purpose:
//    Contain the attributes for a cylinder
//
// Notes:      Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   Tue Oct 21 13:17:13 PST 2003
//
// Modifications:
//   
// ****************************************************************************

class CylinderAttributes : public AttributeSubject
{
public:
    CylinderAttributes();
    CylinderAttributes(const CylinderAttributes &obj);
    virtual ~CylinderAttributes();

    virtual void operator = (const CylinderAttributes &obj);
    virtual bool operator == (const CylinderAttributes &obj) const;
    virtual bool operator != (const CylinderAttributes &obj) const;

    virtual const std::string TypeName() const;
    virtual bool CopyAttributes(const AttributeGroup *);
    virtual AttributeSubject *CreateCompatible(const std::string &) const;
    virtual AttributeSubject *NewInstance(bool) const;

    // Property selection methods
    virtual void SelectAll();
    void SelectPoint1();
    void SelectPoint2();

    // Property setting methods
    void SetPoint1(const double *point1_);
    void SetPoint2(const double *point2_);
    void SetRadius(double radius_);

    // Property getting methods
    const double *GetPoint1() const;
          double *GetPoint1();
    const double *GetPoint2() const;
          double *GetPoint2();
    double       GetRadius() const;


    // Keyframing methods
    virtual std::string               GetFieldName(int index) const;
    virtual AttributeGroup::FieldType GetFieldType(int index) const;
    virtual std::string               GetFieldTypeName(int index) const;
    virtual bool                      FieldsEqual(int index, const AttributeGroup *rhs) const;

private:
    double point1[3];
    double point2[3];
    double radius;
};

#endif

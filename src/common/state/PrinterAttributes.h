#ifndef PRINTERATTRIBUTES_H
#define PRINTERATTRIBUTES_H
#include <state_exports.h>
#include <string>
#include <AttributeSubject.h>

// ****************************************************************************
// Class: PrinterAttributes
//
// Purpose:
//    This class contains the attributes used for controlling printers.
//
// Notes:      Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   Thu Dec 18 11:24:24 PDT 2003
//
// Modifications:
//   
// ****************************************************************************

class STATE_API PrinterAttributes : public AttributeSubject
{
public:
    PrinterAttributes();
    PrinterAttributes(const PrinterAttributes &obj);
    virtual ~PrinterAttributes();

    virtual void operator = (const PrinterAttributes &obj);
    virtual bool operator == (const PrinterAttributes &obj) const;
    virtual bool operator != (const PrinterAttributes &obj) const;

    virtual const std::string TypeName() const;
    virtual bool CopyAttributes(const AttributeGroup *);
    virtual AttributeSubject *CreateCompatible(const std::string &) const;
    virtual AttributeSubject *NewInstance(bool) const;

    // Property selection methods
    virtual void SelectAll();
    void SelectPrinterName();
    void SelectPrintProgram();
    void SelectDocumentName();
    void SelectCreator();
    void SelectOutputToFileName();

    // Property setting methods
    void SetPrinterName(const std::string &printerName_);
    void SetPrintProgram(const std::string &printProgram_);
    void SetDocumentName(const std::string &documentName_);
    void SetCreator(const std::string &creator_);
    void SetNumCopies(int numCopies_);
    void SetPortrait(bool portrait_);
    void SetPrintColor(bool printColor_);
    void SetOutputToFile(bool outputToFile_);
    void SetOutputToFileName(const std::string &outputToFileName_);
    void SetPageSize(int pageSize_);

    // Property getting methods
    const std::string &GetPrinterName() const;
          std::string &GetPrinterName();
    const std::string &GetPrintProgram() const;
          std::string &GetPrintProgram();
    const std::string &GetDocumentName() const;
          std::string &GetDocumentName();
    const std::string &GetCreator() const;
          std::string &GetCreator();
    int               GetNumCopies() const;
    bool              GetPortrait() const;
    bool              GetPrintColor() const;
    bool              GetOutputToFile() const;
    const std::string &GetOutputToFileName() const;
          std::string &GetOutputToFileName();
    int               GetPageSize() const;

    // Persistence methods
    virtual bool CreateNode(DataNode *node, bool completeSave, bool forceAdd);
    virtual void SetFromNode(DataNode *node);


    // Keyframing methods
    virtual std::string               GetFieldName(int index) const;
    virtual AttributeGroup::FieldType GetFieldType(int index) const;
    virtual std::string               GetFieldTypeName(int index) const;
    virtual bool                      FieldsEqual(int index, const AttributeGroup *rhs) const;

private:
    std::string printerName;
    std::string printProgram;
    std::string documentName;
    std::string creator;
    int         numCopies;
    bool        portrait;
    bool        printColor;
    bool        outputToFile;
    std::string outputToFileName;
    int         pageSize;
};

#endif

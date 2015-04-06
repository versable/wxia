/////////////////////////////////////////////////////////////////////////////
// Name:        iasanedlg.h
// Purpose:     wxIASane dialog
// Author:      Derry Bryson, Technology Associates, Inc.
// Modified by:
// Created:     01/29/2003
// RCS-ID:      $Id: iasanedlg.h,v 1.1.1.1 2003/04/01 20:32:33 dbryson Exp $
// Copyright:   (c) 2003 Derry Bryson
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_IASANEDLG_H_
#define _WX_IASANEDLG_H_

#ifdef __GNUG__
#pragma interface "iasanedlg.h"
#endif

#include "wx/dynarray.h"
#include "wx/splitter.h"

WX_DEFINE_ARRAY(const SANE_Option_Descriptor*, SaneDescriptorArray);

class wxIASaneAcquireDialog : public wxDialog
{
public:
    wxIASaneAcquireDialog(wxWindow *parent, wxWindowID id,
        const wxString &title, wxSane *sane,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxSize(500, 400),
        const long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
    ~wxIASaneAcquireDialog();

protected:
    //
    //  Event handlers
    //
    void OnOk(wxCommandEvent& event);
    void OnUpdateUI(wxUpdateUIEvent& event);

    //
    //  Misc helper methods
    //
    void GetOptionDescriptors();
    wxPanel *MakeSettingsPanel(wxWindow *parent);
    wxPanel *MakePreviewPanel(wxWindow *parent);
    wxString GetUnitString(SANE_Unit unit);

    wxSane *m_sane;
    SaneDescriptorArray m_descriptors;
    wxWindow *m_optionControls;

private:
    DECLARE_EVENT_TABLE()
};

#endif // _WX_IASANEDLG_H_

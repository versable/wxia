/////////////////////////////////////////////////////////////////////////////
// Name:        iasanedlg.cpp
// Purpose:     wxIASane dialog
// Author:      Derry Bryson, Technology Associates, Inc.
// Modified by:
// Created:     01/23/2003
// RCS-ID:      $Id: iasanedlg.cpp,v 1.1.1.1 2003/04/01 20:32:35 dbryson Exp $
// Copyright:   (c) 2003 Derry Bryson
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// ---------------------------------------------------------------------------
// For compilers that support precompilation, includes "wx/wx.h".

#ifdef __GNUG__
#pragma implementation "iasanedlg.h"
#endif

#include "wx/wxprec.h"
#include <wx/log.h>

#ifdef __BORLANDC__
  #pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/notebook.h"

#include "wx/ia/sane.h"
#include "wx/ia/iasanedlg.h"

enum
{
    ID_BUT_SCAN = 3000,
    ID_BUT_PREVIEW,
};

BEGIN_EVENT_TABLE(wxIASaneAcquireDialog, wxDialog)
    EVT_BUTTON(wxID_OK, wxIASaneAcquireDialog::OnOk)
    EVT_UPDATE_UI(-1, wxIASaneAcquireDialog::OnUpdateUI)
END_EVENT_TABLE()

wxIASaneAcquireDialog::wxIASaneAcquireDialog(wxWindow *parent, wxWindowID id,
    const wxString &title, wxSane *sane, const wxPoint &pos,
    const wxSize &size, const long style) :
    wxDialog(parent, id, title, pos, size, style)
{
    m_sane = sane;
    m_optionValues = NULL;
    m_optionControls = NULL;
    GetOptionDescriptors();

    m_splitter = new wxSplitterWindow(this, -1);
    m_settingsPanel = MakeSettingsPanel(m_splitter);
    m_previewPanel = MakePreviewPanel(m_splitter);

    wxBoxSizer *bsizer = new wxBoxSizer(wxHORIZONTAL);
    bsizer->Add(10, 10, 1, wxEXPAND);
    wxButton *defBut;
    bsizer->Add(defBut = new wxButton(this, ID_BUT_SCAN, _("&Scan")), 0, wxALIGN_RIGHT | wxALL, 5);
    defBut->SetDefault();
    bsizer->Add(new wxButton(this, ID_BUT_PREVIEW, _("&Preview")), 0, wxALIGN_RIGHT | wxALL, 5);
    bsizer->Add(new wxButton(this, wxID_CANCEL, _("Cancel")), 0, wxALIGN_RIGHT | wxALL, 5);

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(m_splitter, 1, wxEXPAND | wxALL, 5);
    sizer->Add(bsizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

    SetAutoLayout(TRUE);
    SetSizer(sizer);

    Layout();
    Centre(wxBOTH);

    m_splitter->SplitVertically(m_settingsPanel, m_previewPanel, 0);

    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
}

wxIASaneAcquireDialog::~wxIASaneAcquireDialog()
{
    delete[] m_optionValues;
    delete[] m_optionControls;
}

wxWindow *wxIASaneAcquireDialog::MakeSettingsPanel(wxWindow *parent)
{
    wxScrolledWindow *panel = new wxScrolledWindow(parent, -1,
        wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

    panel->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    panel->SetScrollbars(1, 1, 1, 1);

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    wxStaticBoxSizer *sbsizer = 0;
    wxStaticBox *sbox;
    wxFlexGridSizer *gsizer = 0;
    wxString label;

    for (unsigned int i = 1; i < m_descriptors.GetCount(); i++)
    {
        if (m_descriptors[i]->type == SANE_TYPE_GROUP)
        {
            sbsizer = 0;
            label = wxString(m_descriptors[i]->title);
        }
        else
        {
            if (!sbsizer)
            {
                sbox = new wxStaticBox(panel, -1, label);
                sbsizer = new wxStaticBoxSizer(sbox, wxVERTICAL);
                gsizer = new wxFlexGridSizer(3);
                sbsizer->Add(gsizer, 0, wxEXPAND | wxALL, 5);
                sizer->Add(sbsizer, 0, wxEXPAND | wxALL, 5);
            }
            gsizer->Add(new wxStaticText(panel, -1, wxString(m_descriptors[i]->title) + _T(":"),
                wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT), 0, wxEXPAND | wxALL, 5);
            gsizer->Add(1, 1, wxEXPAND | wxALL, 5);
            gsizer->Add(new wxStaticText(panel, -1, GetUnitString(m_descriptors[i]->unit)),
                0, wxEXPAND | wxALL, 5);
        }
    }

    panel->SetAutoLayout(TRUE);
    panel->SetSizer(sizer);
    sizer->FitInside(panel);

    return panel;
}

wxWindow *wxIASaneAcquireDialog::MakePreviewPanel(wxWindow *parent)
{
    wxScrolledWindow *panel = new wxScrolledWindow(parent, -1,
        wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

    panel->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    panel->SetScrollbars(1, 1, 1, 1);

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    panel->SetAutoLayout(TRUE);
    panel->SetSizer(sizer);
    sizer->FitInside(panel);

    return panel;
}

void wxIASaneAcquireDialog::OnOk(wxCommandEvent& event)
{
    if (Validate() && TransferDataFromWindow())
    {
    }

    if (IsModal())
        EndModal(wxID_OK);
    else
    {
        SetReturnCode(wxID_OK);
        Show(FALSE);
    }
}

void wxIASaneAcquireDialog::OnUpdateUI(wxUpdateUIEvent& event)
{
    switch(event.GetId())
    {
    }
}

void wxIASaneAcquireDialog::GetOptionDescriptors()
{
    const SANE_Option_Descriptor *d;

    //
    //  Get the option descriptors from the source
    //
    for (unsigned int i = 0; (d = m_sane->SaneGetOptionDescriptor(i)) != NULL; i++)
    {
        m_descriptors.Add(d);
        wxLogDebug("Descriptor %d: %s, type = %d, size = %d, constraint type = %d",
            i, d->title, d->type, d->size, d->constraint_type);
    }

#if 0
    //
    //  Create the option values array
    //
    m_optionValues = new SaneOptionValue[m_descriptors->GetCount()];
    for (i = 0; i < (int)m_descriptors.GetCount(); i++)
        if (m_descriptors[i]->type == SANE_TYPE_STRING)
            m_optionValues[i].s = new SANE_Char[m_descriptors[i]->size];
#endif
}

wxString wxIASaneAcquireDialog::GetUnitString(SANE_Unit unit)
{
    switch(unit)
    {
        default:
        case SANE_UNIT_NONE :
            return wxEmptyString;

        case SANE_UNIT_PIXEL :
            return wxString(_("pixels"));

        case SANE_UNIT_BIT :
            return wxString(_("bits"));

        case SANE_UNIT_MM :
            return wxString(_("mm"));

        case SANE_UNIT_DPI :
            return wxString(_("dpi"));

        case SANE_UNIT_PERCENT :
            return wxString(_('%'));

        case SANE_UNIT_MICROSECOND :
            return wxString(_("microseconds"));
    }
}

void wxIASaneAcquireDialog::GetOptionValues()
{
    for (unsigned int i = 0; i < m_descriptors.GetCount(); i++)
    {
        if (m_descriptors[i]->type != SANE_TYPE_GROUP)
            m_sane->SaneControlOption(i, SANE_ACTION_GET_VALUE,
                &m_optionValues[i], NULL);
    }
}

void wxIASaneAcquireDialog::SetOptionValues()
{
    for (unsigned int i = 0; i < m_descriptors.GetCount(); i++)
    {
        if (m_descriptors[i]->type != SANE_TYPE_GROUP)
            m_sane->SaneControlOption(i, SANE_ACTION_SET_VALUE,
                &m_optionValues[i], NULL);
    }
}

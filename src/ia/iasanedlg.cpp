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

#include <wx/wxprec.h>
#include <wx/log.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/notebook.h>
#include <wx/gbsizer.h>
#include <wx/checkbox.h>
#include <wx/slider.h>
#include <wx/spinctrl.h>
#include <wx/ia/sane.h>
#include <wx/ia/iasanedlg.h>

enum
{
    ID_BUT_SCAN = 3000,
    ID_BUT_PREVIEW,
};

BEGIN_EVENT_TABLE(wxIASaneAcquireDialog, wxDialog)
    EVT_BUTTON(wxID_OK, wxIASaneAcquireDialog::OnOk)
    EVT_UPDATE_UI(wxID_ANY, wxIASaneAcquireDialog::OnUpdateUI)
END_EVENT_TABLE()

wxIASaneAcquireDialog::wxIASaneAcquireDialog(wxWindow *parent, wxWindowID id,
    const wxString &title, wxSane *sane, const wxPoint &pos,
    const wxSize &size, const long style) :
    wxDialog(parent, id, title, pos, size, style)
{
    m_sane = sane;
    m_optionControls = NULL;
    GetOptionDescriptors();

    wxBoxSizer *hsizer = new wxBoxSizer(wxHORIZONTAL);
    hsizer->Add(MakeSettingsPanel(this), 1, wxEXPAND | wxALL, 5);
    hsizer->Add(MakePreviewPanel(this), 1, wxEXPAND | wxALL, 5);

    wxBoxSizer *bsizer = new wxBoxSizer(wxHORIZONTAL);
    bsizer->Add(10, 10, 1, wxEXPAND);
    wxButton *defBut;
    bsizer->Add(defBut = new wxButton(this, ID_BUT_SCAN, _("&Scan")), 0, wxALIGN_RIGHT | wxALL, 5);
    defBut->SetDefault();
    bsizer->Add(new wxButton(this, ID_BUT_PREVIEW, _("&Preview")), 0, wxALIGN_RIGHT | wxALL, 5);
    bsizer->Add(new wxButton(this, wxID_CANCEL, _("Cancel")), 0, wxALIGN_RIGHT | wxALL, 5);

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(hsizer, 1, wxEXPAND | wxALL, 5);
    sizer->Add(bsizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

    SetSizer(sizer);
    sizer->SetSizeHints(this);
}

wxIASaneAcquireDialog::~wxIASaneAcquireDialog()
{
    delete[] m_optionControls;
}

wxPanel *wxIASaneAcquireDialog::MakeSettingsPanel(wxWindow *parent)
{
    wxPanel *panel = new wxPanel(parent);
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    wxGridBagSizer *gsizer;

    for (unsigned int i = 1; i < m_descriptors.GetCount(); i++)
    {
        unsigned int type = m_descriptors[i]->type;
        // If the descriptor is of type GROUP, create a static box with the
        // group name and continue the iteration
        if (type == SANE_TYPE_GROUP)
        {
            wxStaticBox *sbox = new wxStaticBox(panel, wxID_ANY, wxString(m_descriptors[i]->title));
            wxStaticBoxSizer *sbsizer = new wxStaticBoxSizer(sbox, wxVERTICAL);
            gsizer = new wxGridBagSizer(5, 5);
            sbsizer->Add(gsizer, 0, wxEXPAND | wxALL, 5);
            sizer->Add(sbsizer, 0, wxEXPAND | wxALL, 5);
            continue;
        }
        unsigned int row = gsizer->GetEffectiveRowsCount();
        gsizer->Add(new wxStaticText(panel, wxID_ANY, wxString(m_descriptors[i]->title) + _T(":")),
            wxGBPosition(row, 0), wxDefaultSpan, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);

        if (type == SANE_TYPE_BOOL)
        {
            gsizer->Add(new wxCheckBox(panel, wxID_ANY, wxEmptyString),
                wxGBPosition(row, 1), wxDefaultSpan, wxALIGN_CENTER_VERTICAL | wxEXPAND);
        }
        else if (type == SANE_TYPE_INT || type == SANE_TYPE_FIXED)
        {
            if (m_descriptors[i]->constraint_type == SANE_CONSTRAINT_RANGE)
            {
                SANE_Word min = m_descriptors[i]->constraint.range->min;
                SANE_Word max = m_descriptors[i]->constraint.range->max;
                SANE_Word quant = m_descriptors[i]->constraint.range->quant;
                wxLogDebug("%s: min: %d, max: %d, quant: %d", m_descriptors[i]->title, min, max, quant);
                if (m_descriptors[i]->unit == SANE_UNIT_PERCENT)
                {
                    wxSlider *slider = new wxSlider(panel, wxID_ANY, 0, min, max);
                    gsizer->Add(slider, wxGBPosition(row, 1),
                        wxDefaultSpan, wxALIGN_CENTER_VERTICAL | wxEXPAND);
                    if (m_optionValues[i].sane_status == SANE_STATUS_GOOD)
                        slider->SetValue(m_optionValues[i].sane_int);
                }
                else
                {
                    wxSpinCtrl *spinctrl = new wxSpinCtrl(panel, wxID_ANY);
                    spinctrl->SetRange(min / quant, max / quant);
                    gsizer->Add(spinctrl, wxGBPosition(row, 1),
                        wxDefaultSpan, wxALIGN_CENTER_VERTICAL | wxEXPAND);
                    if (m_optionValues[i].sane_status == SANE_STATUS_GOOD)
                    {
                        if (type == SANE_TYPE_FIXED)
                            spinctrl->SetValue(m_optionValues[i].sane_fixed / quant);
                        else
                            spinctrl->SetValue(m_optionValues[i].sane_int / quant);
                    }
                }
            }
            else if (m_descriptors[i]->constraint_type == SANE_CONSTRAINT_WORD_LIST)
            {
                wxChoice *choice = new wxChoice(panel, wxID_ANY);
                gsizer->Add(choice, wxGBPosition(row, 1),
                    wxDefaultSpan, wxALIGN_CENTER_VERTICAL | wxEXPAND);
                const SANE_Word *word_list = m_descriptors[i]->constraint.word_list;
                for (unsigned int w = 0, max = *word_list; w < max; w++) {
                    word_list++;
                    choice->Append(wxString::Format("%d", *word_list));
                }
                if (m_optionValues[i].sane_status == SANE_STATUS_GOOD)
                {
                    choice->SetSelection(choice->FindString(wxString::Format("%d",
                        type == SANE_TYPE_FIXED ?
                        m_optionValues[i].sane_fixed : m_optionValues[i].sane_int)));
                }
                else
                    choice->SetSelection(0);
            }
        }
        else if (type == SANE_TYPE_STRING)
        {
            wxChoice *choice = new wxChoice(panel, wxID_ANY);
            const SANE_String_Const *string_list = m_descriptors[i]->constraint.string_list;
            while (*string_list != NULL)
            {
                choice->Append(wxString(*string_list));
                string_list++;
            }

            if (m_optionValues[i].sane_status == SANE_STATUS_GOOD)
                choice->SetSelection(choice->FindString(m_optionValues[i].sane_string));
            else
                choice->SetSelection(0);
            gsizer->Add(choice, wxGBPosition(row, 1),
                wxDefaultSpan, wxALIGN_CENTER_VERTICAL | wxEXPAND);
        }
        if (!gsizer->IsColGrowable(1))
            gsizer->AddGrowableCol(1);
        gsizer->Add(new wxStaticText(panel, wxID_ANY, GetUnitString(m_descriptors[i]->unit)),
           wxGBPosition(row, 2), wxDefaultSpan, wxALIGN_CENTER_VERTICAL);
    }

    panel->SetSizer(sizer);
    sizer->SetSizeHints(panel);

    return panel;
}

wxPanel *wxIASaneAcquireDialog::MakePreviewPanel(wxWindow *parent)
{
    wxPanel *panel = new wxPanel(parent);
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    panel->SetSizer(sizer);
    sizer->SetSizeHints(panel);

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
    //  Get the option descriptors from the source and
    //  try to turn on automatic mode
    //
    for (unsigned int i = 0; (d = m_sane->SaneGetOptionDescriptor(i)) != NULL; i++)
    {
        m_descriptors.Add(d);
        wxLogDebug("Descriptor %d: name = %s, title = %s, type = %d, size = %d, constraint type = %d",
            i, d->name, d->title, d->type, d->size, d->constraint_type);
        unsigned int auto_status = m_sane->SaneControlOption(i, SANE_ACTION_SET_AUTO);
        if (auto_status == SANE_STATUS_GOOD)
            wxLogDebug("Automatic control option for %s set", d->name);
        else if (auto_status == SANE_STATUS_UNSUPPORTED)
            wxLogDebug("Automatic control option for %s not supported", d->name);
        else
            wxLogDebug("Automatic control option error (%d) for %s", auto_status);
    }

    //
    //  Create the option values array
    //
    GetOptionValues();
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
    unsigned int size = m_descriptors.GetCount();
    m_optionValues.reserve(size);

    for (unsigned int i = 0; i < size; i++)
    {
        int type = m_descriptors[i]->type;
        struct SaneOptionValue option;
        switch(type) {
            case SANE_TYPE_GROUP:
                option.sane_status = SANE_STATUS_GOOD;
                option.sane_int = m_descriptors[i]->size;
                break;
            case SANE_TYPE_BOOL:
                option.sane_status = m_sane->SaneControlOption(i,
                    SANE_ACTION_GET_VALUE, &option.sane_bool);
                break;
            case SANE_TYPE_INT:
                option.sane_status = m_sane->SaneControlOption(i,
                    SANE_ACTION_GET_VALUE, &option.sane_int);
                break;
            case SANE_TYPE_FIXED:
                option.sane_status = m_sane->SaneControlOption(i,
                    SANE_ACTION_GET_VALUE, &option.sane_fixed);
                break;
            case SANE_TYPE_STRING:
                option.sane_string = new SANE_Char[m_descriptors[i]->size];
                option.sane_status = m_sane->SaneControlOption(i,
                    SANE_ACTION_GET_VALUE, option.sane_string);
                break;
        }
        m_optionValues.push_back(option);
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

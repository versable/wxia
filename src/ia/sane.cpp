/////////////////////////////////////////////////////////////////////////////
// Name:        sane.cpp
// Purpose:     wxSane
// Author:      Derry Bryson
// Modified by:
// Created:     01/08/2003
// RCS-ID:      $Id: sane.cpp,v 1.1.1.1 2003/04/01 20:32:35 dbryson Exp $
// Copyright:   (c) 2003 Derry Bryson
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __GNUG__
#pragma implementation "sane.h"
#endif

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/ia/sane.h"

#define DEFAULT_SANE_LIBNAME _T("libsane.so")
//#define DEFAULT_SANE_LIBNAME _T("/usr/lib/sane/libsane-pnm.so")

IMPLEMENT_CLASS(wxSane, wxObject)

wxSane::wxSane()
{
    m_saneLib = NULL;
    m_saneLibName = DEFAULT_SANE_LIBNAME;
    m_handle = 0;
    m_lastStatus = SANE_STATUS_GOOD;

    m_sane_init = NULL;
    m_sane_exit = NULL;
    m_sane_get_devices = NULL;
    m_sane_open = NULL;
    m_sane_close = NULL;
    m_sane_get_option_descriptor = NULL;
    m_sane_control_option = NULL;
    m_sane_get_parameters = NULL;
    m_sane_start = NULL;
    m_sane_read = NULL;
    m_sane_cancel = NULL;
    m_sane_set_io_mode = NULL;
    m_sane_get_select_fd = NULL;
    m_sane_strstatus = NULL;
}

wxSane::~wxSane()
{
    wxASSERT_MSG(!m_handle, _("wxSane not shutdown properly!"));
    wxASSERT_MSG(!m_saneLib, _("wxSane not shutdown properly!"));
}

bool wxSane::Startup()
{
    wxCHECK_MSG(!Ok(), FALSE, _("wxSane already started!"));

    bool retval = FALSE;

    m_saneLib = dlopen(m_saneLibName.c_str(), RTLD_LAZY);
    if (!m_saneLib)
        goto out;
    if (!(m_sane_init = (sane_init)dlsym(m_saneLib, "sane_init")))
        goto out;
    if (!(m_sane_exit = (sane_exit)dlsym(m_saneLib, "sane_exit")))
        goto out;
    if (!(m_sane_get_devices = (sane_get_devices)dlsym(m_saneLib, "sane_get_devices")))
        goto out;
    if (!(m_sane_open = (sane_open)dlsym(m_saneLib, "sane_open")))
        goto out;
    if (!(m_sane_close = (sane_close)dlsym(m_saneLib, "sane_close")))
        goto out;
    if (!(m_sane_get_option_descriptor = (sane_get_option_descriptor)dlsym(m_saneLib, "sane_get_option_descriptor")))
        goto out;
    if (!(m_sane_control_option = (sane_control_option)dlsym(m_saneLib, "sane_control_option")))
        goto out;
    if (!(m_sane_get_parameters = (sane_get_parameters)dlsym(m_saneLib, "sane_get_parameters")))
        goto out;
    if (!(m_sane_start = (sane_start)dlsym(m_saneLib, "sane_start")))
        goto out;
    if (!(m_sane_read = (sane_read)dlsym(m_saneLib, "sane_read")))
        goto out;
    if (!(m_sane_cancel = (sane_cancel)dlsym(m_saneLib, "sane_cancel")))
        goto out;
    if (!(m_sane_set_io_mode = (sane_set_io_mode)dlsym(m_saneLib, "sane_set_io_mode")))
        goto out;
    if (!(m_sane_get_select_fd = (sane_get_select_fd)dlsym(m_saneLib, "sane_get_select_fd")))
        goto out;
    if (!(m_sane_strstatus = (sane_strstatus)dlsym(m_saneLib, "sane_strstatus")))
        goto out;

    retval = TRUE;

 out:
    if (!retval && m_saneLib)
    {
        dlclose(m_saneLib);
        m_saneLib = NULL;
    }

    return retval;
}

bool wxSane::Shutdown()
{
    if (Ok())
    {
        dlclose(m_saneLib);
        m_saneLib = NULL;
        return TRUE;
    }
    return FALSE;
}

void wxSane::SetSaneLibName(const wxString &libName)
{
    m_saneLibName = libName;
}

wxString wxSane::GetSaneLibName()
{
    return m_saneLibName;
}

bool wxSane::Ok()
{
    return m_saneLib != NULL;
}

bool wxSane::IsOpen()
{
    return m_handle != 0;
}

SANE_Handle wxSane::GetHandle()
{
    return m_handle;
}

SANE_Status wxSane::GetLastStatus()
{
    return m_lastStatus;
}

SANE_Status wxSane::SaneInit(SANE_Int *version_code,
    SANE_Auth_Callback authorize)
{
    wxCHECK_MSG(Ok(), SANE_STATUS_INVAL, _T("wxSane not started!"));
    return m_lastStatus = (*m_sane_init)(version_code, authorize);
}

void wxSane::SaneExit()
{
    wxCHECK_RET(Ok(), _T("wxSane not started!"));
    (*m_sane_exit)();
}

SANE_Status wxSane::SaneGetDevices(const SANE_Device ***device_list,
    bool local_only)
{
    wxCHECK_MSG(Ok(), SANE_STATUS_INVAL, _T("wxSane not started!"));
    return m_lastStatus = (*m_sane_get_devices)(device_list, (SANE_Bool)local_only);
}

SANE_Status wxSane::SaneOpen(const wxString &name)
{
    wxCHECK_MSG(Ok(), SANE_STATUS_INVAL, _T("wxSane not started!"));
    if (m_handle)
        SaneClose();
    m_lastStatus = (*m_sane_open)(name.c_str(), &m_handle);
    if (m_lastStatus != SANE_STATUS_GOOD)
        m_handle = 0;
    return m_lastStatus;
}

void wxSane::SaneClose()
{
    wxCHECK_RET(Ok(), _T("wxSane not started!"));
    (*m_sane_close)(m_handle);
    m_handle = 0;
}

const SANE_Option_Descriptor *wxSane::SaneGetOptionDescriptor(SANE_Int option)
{
    wxCHECK_MSG(Ok(), NULL, _T("wxSane not started!"));
    return (*m_sane_get_option_descriptor)(m_handle, option);
}

SANE_Status wxSane::SaneControlOption(SANE_Int option, SANE_Action action,
    void *value, SANE_Int *info)
{
    wxCHECK_MSG(Ok(), SANE_STATUS_INVAL, _T("wxSane not started!"));
    return m_lastStatus = (*m_sane_control_option)(m_handle, option, action, value, info);
}

SANE_Status wxSane::SaneGetParameters(SANE_Parameters *params)
{
    wxCHECK_MSG(Ok(), SANE_STATUS_INVAL, _T("wxSane not started!"));
    return m_lastStatus = (*m_sane_get_parameters)(m_handle, params);
}

SANE_Status wxSane::SaneStart()
{
    wxCHECK_MSG(Ok(), SANE_STATUS_INVAL, _T("wxSane not started!"));
    return m_lastStatus = (*m_sane_start)(m_handle);
}

SANE_Status wxSane::SaneRead(SANE_Byte *data, SANE_Int max_length, SANE_Int *length)
{
    wxCHECK_MSG(Ok(), SANE_STATUS_INVAL, _T("wxSane not started!"));
    return m_lastStatus = (*m_sane_read)(m_handle, data, max_length, length);
}

void wxSane::SaneCancel()
{
    wxCHECK_RET(Ok(), _T("wxSane not started!"));
    (*m_sane_cancel)(m_handle);
}

SANE_Status wxSane::SaneSetIOMode(bool non_blocking)
{
    wxCHECK_MSG(Ok(), SANE_STATUS_INVAL, _T("wxSane not started!"));
    return m_lastStatus = (*m_sane_set_io_mode)(m_handle, (SANE_Bool)non_blocking);
}

SANE_Status wxSane::SaneGetSelectFD(SANE_Int *fd)
{
    wxCHECK_MSG(Ok(), SANE_STATUS_INVAL, _T("wxSane not started!"));
    return m_lastStatus = (*m_sane_get_select_fd)(m_handle, fd);
}

wxString wxSane::SaneStrStatus(SANE_Status status)
{
    wxCHECK_MSG(Ok(), wxEmptyString, _T("wxSane not started!"));
    return wxString((*m_sane_strstatus)(status));
}

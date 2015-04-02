/////////////////////////////////////////////////////////////////////////////
// Name:        sane.h
// Purpose:     wxSane
// Author:      Derry Bryson
// Modified by:
// Created:     01/08/2003
// RCS-ID:      $Id: sane.h,v 1.1.1.1 2003/04/01 20:32:33 dbryson Exp $
// Copyright:   (c) 2003 Derry Bryson
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_SANE_H_
#define _WX_SANE_H_

#ifdef __GNUG__
#pragma interface "sane.h"
#endif

#include "wx/dynlib.h"
#include "sane/sane.h"

class wxSane : public wxObject
{
public:
    wxSane();
    ~wxSane();

    bool Startup();
    bool Shutdown();

    void SetSaneLibName(const wxString &libName);
    wxString GetSaneLibName();

    bool Ok();
    bool IsOpen();

    SANE_Status GetLastStatus();
    SANE_Handle GetHandle();

    SANE_Status SaneInit(SANE_Int *version_code, SANE_Auth_Callback authorize);
    void SaneExit();
    SANE_Status SaneGetDevices(const SANE_Device *** device_list,
        bool local_only);
    SANE_Status SaneOpen(const wxString &name);
    void SaneClose();
    const SANE_Option_Descriptor *SaneGetOptionDescriptor(SANE_Int option);
    SANE_Status SaneControlOption(SANE_Int option, SANE_Action action,
        void *value = NULL, SANE_Int *info = NULL);
    SANE_Status SaneGetParameters(SANE_Parameters *params);
    SANE_Status SaneStart();
    SANE_Status SaneRead(SANE_Byte *data, SANE_Int max_length, SANE_Int *length);
    void SaneCancel();
    SANE_Status SaneSetIOMode(bool non_blocking);
    SANE_Status SaneGetSelectFD(SANE_Int *fd);
    wxString SaneStrStatus(SANE_Status status);

protected:
    void *m_saneLib;
    wxString m_saneLibName;
    SANE_Handle m_handle;

    //
    //  Sane function types
    //
    typedef SANE_Status (*sane_init)(SANE_Int *version_code,
        SANE_Auth_Callback authorize);
    typedef void (*sane_exit)(void);
    typedef SANE_Status (*sane_get_devices)(const SANE_Device ***device_list,
        SANE_Bool local_only);
    typedef SANE_Status (*sane_open)(SANE_String_Const devicename,
        SANE_Handle *handle);
    typedef void (*sane_close)(SANE_Handle handle);
    typedef const SANE_Option_Descriptor *(*sane_get_option_descriptor)
        (SANE_Handle handle, SANE_Int option);
    typedef SANE_Status (*sane_control_option)(SANE_Handle handle,
        SANE_Int option, SANE_Action action, void *value, SANE_Int *info);
    typedef SANE_Status (*sane_get_parameters)(SANE_Handle handle,
        SANE_Parameters *params);
    typedef SANE_Status (*sane_start)(SANE_Handle handle);
    typedef SANE_Status (*sane_read)(SANE_Handle handle, SANE_Byte *data,
        SANE_Int max_length, SANE_Int *length);
    typedef void (*sane_cancel)(SANE_Handle handle);
    typedef SANE_Status (*sane_set_io_mode)(SANE_Handle handle,
        SANE_Bool non_blocking);
    typedef SANE_Status (*sane_get_select_fd)(SANE_Handle handle, SANE_Int *fd);
    typedef SANE_String_Const (*sane_strstatus)(SANE_Status status);

    //
    //  Sane functions in shared library
    //
    sane_init m_sane_init;
    sane_exit m_sane_exit;
    sane_get_devices m_sane_get_devices;
    sane_open m_sane_open;
    sane_close m_sane_close;
    sane_get_option_descriptor m_sane_get_option_descriptor;
    sane_control_option m_sane_control_option;
    sane_get_parameters m_sane_get_parameters;
    sane_start m_sane_start;
    sane_read m_sane_read;
    sane_cancel m_sane_cancel;
    sane_set_io_mode m_sane_set_io_mode;
    sane_get_select_fd m_sane_get_select_fd;
    sane_strstatus m_sane_strstatus;

    SANE_Status m_lastStatus;

private:
    DECLARE_CLASS(wxSane)
};

#endif // _WX_SANE_H_

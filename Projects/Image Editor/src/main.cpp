/*  WX macros */
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "GUI/MainApp.h"
IMPLEMENT_APP_NO_MAIN(MyApp);
//IMPLEMENT_WX_THEME_SUPPORT;

int main(int argc, char* argv[]) {
  wxEntryStart( argc, argv );
  wxTheApp->CallOnInit();
  wxTheApp->OnRun();
}
 !macro customInstall
   DeleteRegKey HKCR "dbyclientpaassdkdemo"
   WriteRegStr HKCR "dbyclientpaassdkdemo" "" "URL:dbyclientpaassdkdemo"
   WriteRegStr HKCR "dbyclientpaassdkdemo" "URL Protocol" ""
   WriteRegStr HKCR "dbyclientpaassdkdemo\DefaultIcon" "" "$INSTDIR\${APP_EXECUTABLE_FILENAME}"
   WriteRegStr HKCR "dbyclientpaassdkdemo\shell" "" ""
   WriteRegStr HKCR "dbyclientpaassdkdemo\shell\Open" "" ""
   WriteRegStr HKCR "dbyclientpaassdkdemo\shell\Open\command" "" "$INSTDIR\${APP_EXECUTABLE_FILENAME} %1"
 !macroend

 !macro customUnInstall
   DeleteRegKey HKCR "dbyclientpaassdkdemo"
 !macroend

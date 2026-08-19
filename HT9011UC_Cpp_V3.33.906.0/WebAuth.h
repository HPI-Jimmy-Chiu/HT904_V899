// =============================================================================
//  WebAuth.h -- FW-W2: the browser login's verification core, extracted from
//  golden's TfMain::cbUserSelectChange (main.cpp:14395-14800+).
//
//  AI(W906-FW-W2) 20260819: new file. Per the 20260819 write-path ruling
//  ("登入與權限比照 BCB6 既有體系") and WEBBRIDGE_WRITEPATH_DESIGN.md section
//  2/2.1: this is golden's DEFAULT password-book arm (main.cpp:14618-14643
//  load + the PassList comparison that sets AccessLevel), stripped of the
//  fPassword modal plumbing the web flow replaces with an auth.login command.
//
//  Golden semantics preserved exactly:
//    * book format: one entry per line, dot-space separated fields
//      <user> <level> <password>   (SplitStrByDotSpaceOnly x3, golden
//      :14622-14631; PassList[0]=user, PassList[1]=level, PassList[2]=pw)
//    * optional decode: when IniConfig.bPasswordSecret AND the General ini's
//      [Password]Change flag are both true, the password field is
//      DecodeStr()'d before comparison (golden :14633-14637 + the
//      bChange=CheckAndReadIniDataGeneral("Password","Change",false) read)
//    * comparison: UpperCase() on BOTH sides, user and password (golden's
//      PassList arms)
//    * result: the matched line's level (atoi of field 2, golden
//      `l=atoi(PassList[1]->Strings[i].c_str()); AccessLevel=l;`)
//
//  NOT here (documented, not silently dropped): the customer arms
//  (bUseLoginDatToSetLevel / N15UserLevelByTxt+Greatek / barcode auto-login)
//  and the post-login level massaging (iDefHonPrecLevel / KYEC_LEE >=5
//  clamps) -- those run in golden AFTER AccessLevel is set and are
//  customer-gated; they join when their configs/facades do. ChangeLevelAttr()
//  is golden's widget-enable mirror; the web replaces it with the auth.level
//  tag (design doc section 2).
//
//  bookPath is an explicit parameter (the tree's established explicit-path
//  seam, cBinSel Wave B precedent): production callers pass the golden global
//  `pwPath` (cmydef.cpp:3541), tests pass a scratch book. This function only
//  READS the book.
// =============================================================================
#ifndef WEBAUTH_H
#define WEBAUTH_H

#include "vclcompat/vcl_compat.h"   // AnsiString

// Returns the matched entry's level (>= 0) or -1 when no entry matches /
// the book cannot be read. Never writes any file.
int WebAuthVerify(const AnsiString& bookPath,
                  const AnsiString& user,
                  const AnsiString& password);

#endif // WEBAUTH_H

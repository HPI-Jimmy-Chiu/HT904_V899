// ===========================================================================
//  vclcompat/FileListBox.h
//  Headless BCB6 TFileListBox compatibility shim (VCL FileCtrl.hpp).
//
//  AI(W906-SpoolCluster) 20260721: new file.
//
//  FIRST CONSUMER: SECSGEM/uHGemEquipment.cpp's Spool cluster -- THGem's
//  `FileListBox1` member (golden uHGemEquipment.h:98) -- WriteToSpoolFile/
//  DoSpool/SetSpoolActive/GetSpoolActive (golden uHGemEquipment.cpp
//  :1887-1977/:4079-4189/:6133-6161). Every golden call site in THGem's own
//  scope only ever reads ->Items->Count / ->Items->Strings[0..N-1] and calls
//  ->Refresh()/->Update() -- no rendering, no OnClick/OnDblClick, no
//  ItemIndex selection. This shim therefore only implements that narrow
//  real-rescan surface, the same "headless, pure-logic subset" discipline
//  already established by StringGrid.h/MemoryStream.h in this same
//  directory. golden's OTHER two FileListBox instances (FileListBox2 --
//  DoUploadFileToHost family; FileListBox3 -- uHGemHT9045.cpp, itself still a
//  stub) are explicitly OUT OF SCOPE for this wave; nothing below is
//  specialized to FileListBox1 in a way that would block reusing this same
//  class for them later.
//
//  SCAN LOGIC (shared by Refresh()/Update() -- see rationale below for why
//  both perform an IDENTICAL real synchronous rescan):
//    * If Mask contains a '\\', Mask is used VERBATIM as the FindFirst
//      pattern (covers every golden call site in THGem's scope -- they
//      always bake the directory into Mask, e.g.
//      "<GemSpoolPath>\\*.dat").
//    * Else, IncludeTrailingBackslash(Directory)+Mask is used instead.
//    * FindFirst/FindNext/FindClose (vclcompat/SysUtils.h) with attribute
//      filter 0 -- matches real VCL TFileListBox's documented default
//      FileType (ftNormal|ftReadOnly|ftArchive, i.e. excludes
//      faHidden|faSysFile|faVolumeID|faDirectory); confirmed against
//      SysUtils.cpp's own computeExcludeAttr (attr=0 -> ExcludeAttr =
//      faHidden|faSysFile|faVolumeID|faDirectory).
//    * "."/".." and any faDirectory entry are skipped (real VCL
//      TFileListBox never lists subdirectories, only files).
//    * Items is cleared and repopulated, then Sort()'d.
//
//  SORT ORDER (case-sensitive, deliberately NOT case-folded -- checked, not
//  assumed): vclcompat::TStringList::Sort() (TStringList.cpp) compares via
//  AnsiString::operator< (byte/case-sensitive order), not an
//  AnsiCompareText-style fold, and takes no comparator argument. Left as
//  plain Sort() here: every filename THGem's own spool code ever generates
//  (WriteToSpoolFile's "%04d_%02d_%02d %02d_%02d_%02d %03d.dat") is all
//  digit/space/underscore/".dat" -- there is no letter anywhere in the name
//  for case to disagree about, so case-sensitive vs case-insensitive Sort()
//  produce IDENTICAL orderings for this consumer. THGem::DoSpool's FIFO
//  assumption (always consumes Items->Strings[0]) relies on this
//  alphabetical-equals-chronological ordering (zero-padded date/time fields
//  sort lexicographically in chronological order) -- NOT on case-folding --
//  see DoSpool's own .cpp comment / this wave's test for the actual proof.
//
//  RATIONALE for Refresh()==Update() (both real, IDENTICAL rescans): golden
//  calls ->Refresh() and ->Update() in an order this port cannot fully
//  verify against real BCB6/VCL's own undocumented internal split between
//  what each method actually invalidates (both are called back-to-back at
//  every golden call site in THGem's scope, but in different sequences
//  across sites -- e.g. Refresh() then Update() at some, the reverse
//  ordering cannot be ruled out at others from source alone). Making both
//  perform the IDENTICAL real rescan sidesteps that ambiguity entirely:
//  correctness never depends on resolving which of the two "really" does
//  the work in real VCL, because either call alone is already sufficient
//  here.
//
//  DEVIATION (disclosed, not silent): golden's own FileListBox1 .dfm default
//  is `Mask = 'spool\\*.dat'` with NO Directory set -- a bare relative path
//  whose real-world resolution depends on the PROCESS's current working
//  directory at scan time (fragile; golden's own SetCurrentDirectory
//  computes GemSpoolPath but never actually wires it to this widget). This
//  port's THGem ctor does NOT replicate that CWD-dependent default -- see
//  SECSGEM/uHGemEquipment.h's own FileListBox1 member comment for how this
//  wave instead constructs a deterministic Mask from GemSpoolPath, once
//  SetCurrentDirectory (this same wave) makes GemSpoolPath resolve to
//  something real.
// ===========================================================================
#ifndef VCLCOMPAT_FILELISTBOX_H
#define VCLCOMPAT_FILELISTBOX_H

#include "vclcompat/AnsiString.h"
#include "vclcompat/TStringList.h"

namespace vclcompat {

class TFileListBox : public TObject {
public:
    // Mask may embed a directory prefix, e.g. "D:\\Upload\\*.*" -- passive
    // string, no side effect on assignment (matches real VCL: a bare
    // property write does not itself trigger a rescan; only Refresh()/
    // Update() -- or, in real VCL, an actual FileListBox change notification
    // this headless shim does not model -- do).
    AnsiString Mask;
    // Directory is NOT used by THGem's own call sites (they always fold the
    // directory into Mask, see file-head note) -- included for other forms'
    // future reuse. Passive, same as Mask.
    AnsiString Directory;
    // Populated ONLY by Refresh()/Update() -- never by assignment to
    // Mask/Directory alone.
    TStringList *Items;

    TFileListBox();
    ~TFileListBox();

    TFileListBox(const TFileListBox&) = delete;
    TFileListBox& operator=(const TFileListBox&) = delete;

    void Refresh();   // real synchronous rescan (see file-head rationale)
    void Update();    // IDENTICAL real synchronous rescan (see file-head rationale)

private:
    void doRescan();  // shared scan-and-repopulate helper for Refresh()/Update()
};

} // namespace vclcompat

// NOT brought into the global namespace here -- consumers add their own
// `using vclcompat::TFileListBox;` after their own collision check, same
// established precedent as vclcompat::TStringGrid (see that header's own
// note on why).

#endif // VCLCOMPAT_FILELISTBOX_H

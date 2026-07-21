// ===========================================================================
//  vclcompat/FileListBox.cpp
//  See vclcompat/FileListBox.h for the full design rationale (scan logic,
//  sort-order reasoning, Refresh()==Update() rationale, disclosed CWD
//  deviation).
//
//  AI(W906-SpoolCluster) 20260721: new file.
// ===========================================================================
#include "vclcompat/FileListBox.h"
#include "vclcompat/SysUtils.h"

namespace vclcompat {

TFileListBox::TFileListBox()
    : Mask(""), Directory(""), Items(new TStringList())
{
}

TFileListBox::~TFileListBox()
{
    delete Items;
}

// ---------------------------------------------------------------------------
//  doRescan -- shared real synchronous rescan, called identically by both
//  Refresh() and Update() (see file-head "RATIONALE for Refresh()==Update()").
// ---------------------------------------------------------------------------
void TFileListBox::doRescan()
{
    Items->Clear();

    // If Mask embeds a directory (contains a '\'), use it verbatim -- covers
    // every golden call site in THGem's scope (see file-head note). Else
    // fall back to Directory+Mask.
    AnsiString pattern = (Mask.Pos('\\') > 0)
                              ? Mask
                              : (IncludeTrailingBackslash(Directory) + Mask);

    TSearchRec sr;
    // attr=0: matches real VCL TFileListBox's documented default FileType
    // (excludes faHidden|faSysFile|faVolumeID|faDirectory -- see file-head
    // note). The explicit faDirectory re-check below is belt-and-suspenders
    // (computeExcludeAttr(0) already excludes directories); it costs nothing
    // and documents the "no subdirectories, ever" intent locally.
    if (FindFirst(pattern, 0, sr) == 0) {
        do {
            if (sr.Name == "." || sr.Name == "..") continue;
            if ((sr.Attr & faDirectory) != 0) continue;
            Items->Add(sr.Name);
        } while (FindNext(sr) == 0);
        FindClose(sr);
    }

    // Case-sensitive Sort() is deliberately fine here -- see file-head "SORT
    // ORDER" note: every filename this consumer's own writer
    // (THGem::WriteToSpoolFile) ever produces is all digit/space/underscore/
    // ".dat", so there is no letter anywhere for case-folding to matter.
    Items->Sort();
}

void TFileListBox::Refresh()
{
    doRescan();
}

void TFileListBox::Update()
{
    doRescan();
}

} // namespace vclcompat

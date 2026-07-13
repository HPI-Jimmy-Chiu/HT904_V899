// ===========================================================================
//  vclcompat/TList.h
//  BCB6 TList compatibility shim (VCL.Classes' generic non-owning pointer
//  list) -- the subset actually used by V906.
//
//  AI(W5-Final-TList) 20260711: Golden usage confirmed by grep of
//  SECSGEM/uHGemEquipment.cpp before writing this shim (per this unit's own
//  instruction not to guess the full VCL TList API). `THGem::SV_Ptr` /
//  `EC_Ptr` / `EC_Ptr_Min` / `EC_Ptr_Max` / `EC_Ptr_Default`
//  (uHGemEquipment.h:651,660-663) are all `TList*` and only exercise:
//    - `new TList` (default ctor, no args)
//    - `->Clear()`                          (uHGemEquipment.cpp:562,777-781)
//    - `->Add(P)` where P is `void*`         (uHGemEquipment.cpp:5661 etc.;
//      `void *P;` declared at :2478/:2851/:3134/:3656 -- confirmed NOT
//      AnsiString/TStringList like the sibling SV_ID/EC_ID members, which are
//      TStringList and use ->IndexOf/->Delete/->Strings[i] -- do not confuse
//      the two families)
//    - `->Items[i]` READ only in every confirmed call site (:2507,2515,2588,
//      2660,2878,2887,2960,3146-3148,3689,3762,3837,8318+) -- always into a
//      `void*` local, sometimes immediately cast to `(TObject*)`/`(AnsiString*)`
//      /`(char*)`/etc.
//    - `delete SV_Ptr;` etc. (dtor, uHGemEquipment.cpp:785-789)
//
//  `Count`, `IndexOf(void*)`, and `Items[i]` WRITE are not exercised by any
//  current call site on these five members (grepped specifically, see hand-off
//  report) -- they are implemented here anyway per this unit's brief, for the
//  Phase 2 sibling unit (SECSGEM SV/EC registration API) which is expected to
//  need list bookkeeping beyond simple append/read. This is flagged explicitly
//  as a scoping discrepancy: golden call sites (Add/Clear/Items-read/dtor)
//  are the only proven surface today; Count/IndexOf/Items-write/Delete are a
//  forward-looking superset, not something the golden source currently uses
//  on a TList (BCB6 TList really does support all of these -- this is a
//  faithful subset-superset choice, not an invention of new VCL behavior).
//
//  Non-owning: BCB6 TList (unlike TObjectList) never deletes the pointers it
//  holds -- callers own the pointees and are responsible for freeing them.
//  This shim mirrors that: Clear()/Delete()/the destructor drop references
//  only, they never `delete` the stored void*.
//
//  Backed by std::vector<void*>, matching the "thin proxy over std" idiom
//  already established by vclcompat/TStringList.h (read that file first).
//  Difference from TStringList's Strings[i]/Objects[i] proxies: those need a
//  proxy *class* because the exposed property type (AnsiString) differs from
//  (or needs conversion tracking against) what's stored. TList's Items[i]
//  property exposes the exact same type it stores (void*), so a plain
//  reference-returning accessor (`void*&`) is sufficient for both read and
//  write -- no StringsProxy-style get/set proxy class is needed here.
// ===========================================================================
#ifndef VCLCOMPAT_TLIST_H
#define VCLCOMPAT_TLIST_H

#include <vector>
#include <cstddef>

namespace vclcompat {

class TList {
public:
    TList() : Count(0) {
        Items.owner = this;
    }

    // BCB6 code always uses TList* (heap, `new TList`). Items is a small
    // accessor proxy holding a back-pointer to `this`; copying/moving a
    // TList would leave it pointing at the wrong object, so (matching
    // TStringList's precedent) copy/move is forbidden.
    TList(const TList&) = delete;
    TList& operator=(const TList&) = delete;

    // ---- element access (0-based, matches BCB6 TList) ---------------------
    int Add(void* item) {
        items_.push_back(item);
        syncCount();
        return static_cast<int>(items_.size()) - 1;
    }

    void Delete(int index) {
        if (index < 0 || index >= static_cast<int>(items_.size())) return;
        items_.erase(items_.begin() + index);
        syncCount();
    }

    void Clear() {
        items_.clear();
        syncCount();
    }

    int IndexOf(void* item) const {
        for (std::size_t i = 0; i < items_.size(); ++i)
            if (items_[i] == item) return static_cast<int>(i);
        return -1;   // BCB6 TList::IndexOf: -1 if absent
    }

    // direct (function) accessors -- equivalent to the Items[] property,
    // handy for callers that prefer a plain function call.
    void* GetItem(int i) const {
        if (i < 0 || i >= static_cast<int>(items_.size())) return 0;
        return items_[i];
    }
    void SetItem(int i, void* v) {
        if (i < 0 || i >= static_cast<int>(items_.size())) return;
        items_[i] = v;
    }

    int GetCount() const { return static_cast<int>(items_.size()); }

    // ---- BCB6-style property: list->Items[i] (get AND set) -----------------
    // void*& is sufficient here (see file header comment) -- no proxy class
    // needed because the exposed element type equals the stored type.
    // NOTE: like BCB6, this is UNCHECKED (matches TList::Items, which is a
    // raw indexed property with no bounds guard) -- unlike GetItem/SetItem
    // above, which clamp defensively. Golden call sites always index with an
    // `i` bounded by the paired TStringList's Count (e.g. `SV_ID->Count`),
    // so this mirrors that trust relationship faithfully rather than adding
    // a safety net BCB6 itself doesn't have.
    struct ItemsAccessor {
        TList* owner;
        void*& operator[](int i) const {
            return owner->items_[static_cast<std::size_t>(i)];
        }
    };
    ItemsAccessor Items;

    // list->Count reads like a property (no parens), matching the dominant
    // BCB6 usage pattern already established for TStringList::Count. Kept in
    // sync on every mutating call (Add/Delete/Clear); Items[i]-write and
    // SetItem() never change the element count so they don't touch it.
    int Count;

private:
    std::vector<void*> items_;

    void syncCount() { Count = static_cast<int>(items_.size()); }
};

} // namespace vclcompat

#endif // VCLCOMPAT_TLIST_H

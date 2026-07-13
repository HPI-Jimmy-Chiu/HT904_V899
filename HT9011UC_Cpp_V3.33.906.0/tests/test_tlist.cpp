// ===========================================================================
//  tests/test_tlist.cpp
//  AI(W5-Final-TList) 20260711: Assertion harness for vclcompat/TList.h (BCB6
//  VCL.Classes TList shim). Exercises exactly the surface this unit built:
//  Add / Count / IndexOf / Items[] (get+set) / Delete / Clear, plus the
//  non-owning-list contract (TList never frees what it holds).
//  No external test framework, non-zero exit on any failure -- matches the
//  style of tests/test_vclcompat.cpp / tests/test_ContactForce.cpp.
//
//  Build: linked against the `vclcompat` static lib (see tests/CMakeLists.txt
//  wiring block reported in the hand-off -- NOT edited by this unit).
// ===========================================================================
#include "vclcompat/vcl_compat.h"   // exercises the umbrella header too

#include <cstdio>

// vclcompat::TList is deliberately NOT in vcl_compat.h's global `using` block
// (it would collide with an unrelated global-namespace `class TList` shim in
// aHotPlateSubstrate.h -- see the NOTE in vcl_compat.h). This test TU does not
// include aHotPlateSubstrate.h, so a local using-declaration here is safe.
using vclcompat::TList;

static int g_fail = 0;
static int g_total = 0;

static void check(bool cond, const char* expr, const char* file, int line) {
    ++g_total;
    if (!cond) {
        ++g_fail;
        std::printf("FAIL [%s:%d]  %s\n", file, line, expr);
    }
}
#define CHECK(cond) check((cond), #cond, __FILE__, __LINE__)

int main() {
    // ===================================================================
    //  Add / Count -- golden usage: `SV_Ptr->Add(P); ... SV_Ptr->Count`
    //  (SECSGEM/uHGemEquipment.cpp:5661 family)
    // ===================================================================
    {
        TList list;
        CHECK(list.Count == 0);

        int a = 1, b = 2, c = 3;
        int i0 = list.Add(&a);
        int i1 = list.Add(&b);
        int i2 = list.Add(&c);

        CHECK(i0 == 0);
        CHECK(i1 == 1);
        CHECK(i2 == 2);
        CHECK(list.Count == 3);
        CHECK(list.GetCount() == 3);
    }

    // ===================================================================
    //  Items[i] READ -- golden usage: `P = SV_Ptr->Items[i];`
    //  (uHGemEquipment.cpp:2507,2878,3146-3148,8318+, always into a void*)
    // ===================================================================
    {
        TList list;
        int a = 10, b = 20, c = 30;
        list.Add(&a);
        list.Add(&b);
        list.Add(&c);

        CHECK(list.Items[0] == static_cast<void*>(&a));
        CHECK(list.Items[1] == static_cast<void*>(&b));
        CHECK(list.Items[2] == static_cast<void*>(&c));

        // round-trip through void* back to int, as the real call sites do
        // (`P = SV_Ptr->Items[i];` then a cast at the use site).
        void* p = list.Items[1];
        int* pi = static_cast<int*>(p);
        CHECK(*pi == 20);
    }

    // ===================================================================
    //  Items[i] WRITE -- BCB6 TList::Items is a get/set indexed property.
    //  Not exercised by golden SV_Ptr/EC_Ptr call sites today (see TList.h
    //  header comment), but part of this unit's brief for the Phase 2
    //  SECSGEM SV/EC registration API sibling.
    // ===================================================================
    {
        TList list;
        int a = 1, b = 2;
        list.Add(&a);
        list.Items[0] = &b;             // overwrite slot 0 through the property
        CHECK(list.Items[0] == static_cast<void*>(&b));
        CHECK(list.Count == 1);         // write must NOT change element count
    }

    // direct GetItem/SetItem accessors (defensive, bounds-checked variant of
    // the same property) behave the same way.
    {
        TList list;
        int a = 1, b = 2;
        list.Add(&a);
        CHECK(list.GetItem(0) == static_cast<void*>(&a));
        list.SetItem(0, &b);
        CHECK(list.GetItem(0) == static_cast<void*>(&b));
        // out-of-range GetItem/SetItem is a defensive no-op/zero (BCB6's raw
        // Items[] property itself has no such guard -- see header comment).
        CHECK(list.GetItem(5) == 0);
        list.SetItem(5, &a);            // must not crash / must not resize
        CHECK(list.Count == 1);
    }

    // ===================================================================
    //  IndexOf(void*) -- not exercised on SV_Ptr/EC_Ptr in golden today
    //  (only on the sibling TStringList members SV_ID/EC_ID), but is part
    //  of BCB6 TList's real API and this unit's brief.
    // ===================================================================
    {
        TList list;
        int a = 1, b = 2, c = 3, notInList = 4;
        list.Add(&a);
        list.Add(&b);
        list.Add(&c);

        CHECK(list.IndexOf(&a) == 0);
        CHECK(list.IndexOf(&b) == 1);
        CHECK(list.IndexOf(&c) == 2);
        CHECK(list.IndexOf(&notInList) == -1);   // BCB6: -1 if absent
    }

    // ===================================================================
    //  Delete(int) -- 0-based, shifts subsequent items down (BCB6 semantics,
    //  same contract as vclcompat::TStringList::Delete).
    // ===================================================================
    {
        TList list;
        int a = 1, b = 2, c = 3;
        list.Add(&a);
        list.Add(&b);
        list.Add(&c);

        list.Delete(1);                 // remove 'b'
        CHECK(list.Count == 2);
        CHECK(list.Items[0] == static_cast<void*>(&a));
        CHECK(list.Items[1] == static_cast<void*>(&c));   // c shifted down to 1

        // out-of-range Delete is a defensive no-op (mirrors TStringList::Delete).
        list.Delete(-1);
        list.Delete(99);
        CHECK(list.Count == 2);
    }

    // ===================================================================
    //  Clear() -- golden usage: `SV_Ptr->Clear(); EC_Ptr->Clear(); ...`
    //  (uHGemEquipment.cpp:562,777-781, on config-reload)
    // ===================================================================
    {
        TList list;
        int a = 1, b = 2;
        list.Add(&a);
        list.Add(&b);
        CHECK(list.Count == 2);

        list.Clear();
        CHECK(list.Count == 0);
        CHECK(list.GetCount() == 0);
        CHECK(list.IndexOf(&a) == -1);

        // list is reusable after Clear() (golden reloads config repeatedly).
        int c = 3;
        int i = list.Add(&c);
        CHECK(i == 0);
        CHECK(list.Count == 1);
    }

    // ===================================================================
    //  Non-owning contract -- TList (unlike TObjectList) never frees what
    //  it holds. Heap-allocate, let the list (and Delete/Clear) drop
    //  references, then free the pointee ourselves without a double-free.
    //  This is the shape golden uses: SV_Ptr/EC_Ptr hold pointers whose
    //  lifetime is managed by the caller (locals, globals, or `new`'d by
    //  SetSVDataPointer's caller), not by the list itself.
    // ===================================================================
    {
        int* heapInt = new int(42);
        {
            TList list;
            list.Add(heapInt);
            CHECK(list.Items[0] == static_cast<void*>(heapInt));
            list.Delete(0);              // drops the reference, does NOT delete
            CHECK(list.Count == 0);
        }
        // still valid -- would be a use-after-free / double-free crash if
        // TList had (incorrectly) taken ownership.
        CHECK(*heapInt == 42);
        delete heapInt;
    }

    // heap-allocated TList itself (matches golden `new TList` / `delete
    // SV_Ptr` at uHGemEquipment.cpp:551-580 / 785-789).
    {
        TList* list = new TList;
        int a = 1;
        list->Add(&a);
        CHECK(list->Count == 1);
        list->Clear();
        delete list;                     // must not touch &a (non-owning)
        CHECK(a == 1);
    }

    // ===================================================================
    //  summary
    // ===================================================================
    std::printf("\nTList harness: %d/%d checks passed\n",
                g_total - g_fail, g_total);
    if (g_fail != 0) {
        std::printf("RESULT: FAILED (%d failures)\n", g_fail);
        return 1;
    }
    std::printf("RESULT: OK\n");
    return 0;
}

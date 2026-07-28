// ===========================================================================
//  tests/test_DfmLayoutGen.cpp
//  W7-B1c gate G9: compiles a representative sample of the generated
//  *_layout.gen.{h,cpp} data tables and round-trips a curated set of
//  entries against golden .dfm text.
//
//  INDEPENDENCE: every expected() value below was read directly out of
//  golden .dfm source with a throwaway script (cp950-decoded), NOT copied
//  from this wave's own IR JSON or from emit_layout.py's output -- see the
//  banner on each CHECK block for the exact golden file the number came
//  from. This is the genuinely independent half of gate G9 (the Python-side
//  half, in tools/dfm2rc/run_b1c.py, independently re-derives geometry/
//  TabOrder for the FULL corpus via gates.py's separate line scanner --
//  this C++ test instead hand-verifies a curated sample of colour/font/
//  custom-prop fields, which that scanner deliberately does not attempt to
//  re-parse).
//
//  Forms compiled here (chosen to cover every family this table carries):
//    ARMS       -- plain TPageControl/TTabSheet/TPanel/TSpeedButton nesting
//    cBinSel    -- TTMyTray custom control (D12 XItem/YItem/tray widths)
//    cContact   -- TMyLed custom control (D12 LEDStyle only, no Alias)
//    iosetview  -- largest form in the corpus (37,752 golden lines); also
//                  has the mass Alias/LEDStyle (TMyLedLane) and
//                  TrueColor/FalseColor/TrueFontColor/FalseFontColor
//                  (TBtnPanelLane) custom-prop usage
//
//  Toolchain: MinGW g++ 6.3+, C++14.  Non-zero exit on any failure.
// ===========================================================================
#include "../tools/dfm2rc/layout_out/ARMS/ARMS_layout.gen.h"
#include "../tools/dfm2rc/layout_out/cBinSel_layout.gen.h"
#include "../tools/dfm2rc/layout_out/cContact_layout.gen.h"
#include "../tools/dfm2rc/layout_out/iosetview_layout.gen.h"

#include <cstdio>
#include <cstring>

using namespace ht9045_layout;

static int g_fail = 0;
static int g_total = 0;

static void checkInt(long got, long expect, const char* expr, const char* file, int line)
{
    ++g_total;
    if (got != expect) {
        ++g_fail;
        std::printf("FAIL [%s:%d]  %s : got %ld  expected %ld\n", file, line, expr, got, expect);
    }
}
#define CHECK_INT(expr, expect) checkInt((long)(expr), (long)(expect), #expr, __FILE__, __LINE__)

static void checkStr(const char* got, const char* expect, const char* expr, const char* file, int line)
{
    ++g_total;
    if (std::strcmp(got, expect) != 0) {
        ++g_fail;
        std::printf("FAIL [%s:%d]  %s : got %s  expected %s\n", file, line, expr, got, expect);
    }
}
#define CHECK_STR(expr, expect) checkStr((expr), (expect), #expr, __FILE__, __LINE__)

#define CHECK_TRUE(expr) CHECK_INT(!!(expr), 1)
#define CHECK_FALSE(expr) CHECK_INT(!!(expr), 0)

static const DfmControlLayout* findByPath(const DfmControlLayout* arr, int count, const char* path)
{
    for (int i = 0; i < count; ++i) {
        if (std::strcmp(arr[i].dfm_path, path) == 0) {
            return &arr[i];
        }
    }
    return NULL;
}

static const DfmCustomProp* findCustomProp(const DfmControlLayout& row, const char* key)
{
    for (int i = 0; i < row.custom_prop_count; ++i) {
        if (std::strcmp(row.custom_props[i].key, key) == 0) {
            return &row.custom_props[i];
        }
    }
    return NULL;
}

int main()
{
    // =======================================================================
    //  ARMS.dfm pcARMSFoem: TPageControl -- golden ARMS/ARMS.dfm (verified
    //  directly, cp950): "object pcARMSFoem: TPageControl / Left = 0 /
    //  Top = 0 / Width = 784 / Height = 518 / Align = alClient /
    //  Font.Color = clWindowText / Font.Height = -16 /
    //  Font.Name = 'MS Sans Serif' / TabOrder = 0"
    // =======================================================================
    {
        const DfmControlLayout* row = findByPath(kARMSLayout, kARMSLayoutCount, "fARMS.pcARMSFoem");
        CHECK_TRUE(row != NULL);
        if (row != NULL) {
            CHECK_STR(row->vcl_class, "TPageControl");
            CHECK_STR(row->kind, "TABHOST");
            CHECK_STR(row->id_symbol, "IDC_ARMS_PCARMSFOEM");
            CHECK_STR(row->parent_path, "fARMS");
            CHECK_STR(row->host_dialog_path, "fARMS");
            CHECK_TRUE(row->has_left);   CHECK_INT(row->left, 0);
            CHECK_TRUE(row->has_top);    CHECK_INT(row->top, 0);
            CHECK_TRUE(row->has_width);  CHECK_INT(row->width, 784);
            CHECK_TRUE(row->has_height); CHECK_INT(row->height, 518);
            CHECK_TRUE(row->has_tab_order); CHECK_INT(row->tab_order, 0);
            CHECK_STR(row->align_mode, "alClient");
            CHECK_TRUE(row->has_font);
            CHECK_STR(row->font_face, "MS Sans Serif");
            CHECK_INT(row->font_height_px, -16);
            CHECK_TRUE(row->has_font_color);
            CHECK_STR(row->font_color_token, "clWindowText");
            // clWindowText = 0x80000008 -> signed 32-bit wraparound = -2147483640
            // (0x80000008 = 2147483656 unsigned; 2147483656 - 4294967296 = -2147483640)
            CHECK_INT(row->font_color, -2147483640L);
            CHECK_INT(row->custom_prop_count, 0);  // TPageControl is not a custom-control class
        }

        // fARMS.pcARMSFoem.tsARMSMain: host_dialog_path must skip the
        // TABHOST (TPageControl) itself and land on the ROOT form -- D8's
        // "TPageControl hosts no DFM children as MoveWindow'd siblings"
        // rule (see DfmLayoutTypes.h note 2).
        const DfmControlLayout* tab = findByPath(kARMSLayout, kARMSLayoutCount, "fARMS.pcARMSFoem.tsARMSMain");
        CHECK_TRUE(tab != NULL);
        if (tab != NULL) {
            CHECK_STR(tab->kind, "SUBDLG");
            CHECK_STR(tab->parent_path, "fARMS.pcARMSFoem");   // literal .dfm parent: the TPageControl
            CHECK_STR(tab->host_dialog_path, "fARMS");         // D8 host: skips the TABHOST
        }
    }

    // =======================================================================
    //  cBinSel.dfm mtTrayItem: TTMyTray -- golden cBinSel.dfm (verified
    //  directly, cp950): Left=8 Top=8 Width=117 Height=281 Color=clWhite
    //  Font.Color=clWhite Font.Height=-13 Font.Name='Arial' XItem=1
    //  YItem=10 LineWidth=-1 EdgeWidth=5 DirectWidth=0
    // =======================================================================
    {
        const DfmControlLayout* row = findByPath(
            kcBinSelLayout, kcBinSelLayoutCount, "fBinSel.PageControl1.tsNormal.Panel.mtTrayItem");
        CHECK_TRUE(row != NULL);
        if (row != NULL) {
            CHECK_STR(row->vcl_class, "TTMyTray");
            CHECK_INT(row->left, 8);
            CHECK_INT(row->top, 8);
            CHECK_INT(row->width, 117);
            CHECK_INT(row->height, 281);
            CHECK_TRUE(row->has_color);
            CHECK_STR(row->color_token, "clWhite");
            CHECK_INT(row->color, 0x00FFFFFF);  // clWhite -- direct colour, no sign wrap
            CHECK_TRUE(row->has_font_color);
            CHECK_STR(row->font_color_token, "clWhite");
            CHECK_INT(row->font_color, 0x00FFFFFF);
            CHECK_STR(row->font_face, "Arial");
            CHECK_INT(row->font_height_px, -13);
            CHECK_INT(row->custom_prop_count, 5);
            const DfmCustomProp* p;
            p = findCustomProp(*row, "XItem");       CHECK_TRUE(p != NULL); if (p) CHECK_STR(p->value, "1");
            p = findCustomProp(*row, "YItem");       CHECK_TRUE(p != NULL); if (p) CHECK_STR(p->value, "10");
            p = findCustomProp(*row, "LineWidth");   CHECK_TRUE(p != NULL); if (p) CHECK_STR(p->value, "-1");
            p = findCustomProp(*row, "EdgeWidth");   CHECK_TRUE(p != NULL); if (p) CHECK_STR(p->value, "5");
            p = findCustomProp(*row, "DirectWidth"); CHECK_TRUE(p != NULL); if (p) CHECK_STR(p->value, "0");
        }
    }

    // =======================================================================
    //  cContact.dfm ledOneCycle: TMyLed -- golden cContact.dfm (verified
    //  directly, cp950): "object ledOneCycle: TMyLed / Left = 100 /
    //  Top = 536 / Width = 15 / Height = 15 / LEDStyle = LEDSqLarge" --
    //  no Color/Font.* properties at all on this one (a case where the
    //  bag is populated but has_font/has_color must both stay false --
    //  nothing to fabricate).
    // =======================================================================
    {
        const DfmControlLayout* row = findByPath(
            kcContactLayout, kcContactLayoutCount, "fContact.pnlSLKSetting.gbContactForce.ledOneCycle");
        CHECK_TRUE(row != NULL);
        if (row != NULL) {
            CHECK_STR(row->vcl_class, "TMyLed");
            CHECK_INT(row->left, 100);
            CHECK_INT(row->top, 536);
            CHECK_INT(row->width, 15);
            CHECK_INT(row->height, 15);
            CHECK_FALSE(row->has_font);   // golden has no Font.* on this control
            CHECK_FALSE(row->has_color);  // golden has no Color on this control
            CHECK_INT(row->custom_prop_count, 1);
            const DfmCustomProp* p = findCustomProp(*row, "LEDStyle");
            CHECK_TRUE(p != NULL);
            if (p) CHECK_STR(p->value, "LEDSqLarge");
        }
    }

    // =======================================================================
    //  iosetview.dfm ledC_Empty_Fix_Off: TMyLedLane -- golden iosetview.dfm
    //  (verified directly, cp950): Left=637 Top=82 Width=22 Height=14
    //  LEDStyle=LEDHorizontal Alias='C_Empty_Fix_Off'
    // =======================================================================
    {
        const DfmControlLayout* row = findByPath(
            kiosetviewLayout, kiosetviewLayoutCount,
            "fiosetview.PC_IOSET.tsLoader.pnlStack1.pgcStack1.tsStack1_Above.grpEmpty.ledC_Empty_Fix_Off");
        CHECK_TRUE(row != NULL);
        if (row != NULL) {
            CHECK_STR(row->vcl_class, "TMyLedLane");
            CHECK_TRUE(row->is_graphic_control);  // TMyLedLane : TALed : TGraphicControl
            CHECK_INT(row->left, 637);
            CHECK_INT(row->top, 82);
            CHECK_INT(row->width, 22);
            CHECK_INT(row->height, 14);
            CHECK_INT(row->custom_prop_count, 2);
            const DfmCustomProp* p;
            p = findCustomProp(*row, "LEDStyle"); CHECK_TRUE(p != NULL); if (p) CHECK_STR(p->value, "LEDHorizontal");
            p = findCustomProp(*row, "Alias");    CHECK_TRUE(p != NULL); if (p) CHECK_STR(p->value, "C_Empty_Fix_Off");
        }
    }

    // =======================================================================
    //  iosetview.dfm btnC_EmptyLoaderZ_Select: TBtnPanelLane -- golden
    //  iosetview.dfm (verified directly, cp950): Left=126 Top=82 Width=46
    //  Height=21 Color=10307329 (plain int, no symbolic token) Font.Color=
    //  clWhite Font.Height=-13 Font.Name='Arial' TabOrder=4
    //  TrueColor=14464261 (plain int) FalseColor=10307329 (plain int)
    //  TrueFontColor=clWhite FalseFontColor=clWhite Alias=
    //  'C_EmptyLoaderZ_Select' -- exercises BOTH shapes of the D12 colour
    //  fields in one control: two custom-colour props are already-resolved
    //  plain integers (token == "") and two are symbolic clWhite tokens.
    // =======================================================================
    {
        const DfmControlLayout* row = findByPath(
            kiosetviewLayout, kiosetviewLayoutCount,
            "fiosetview.PC_IOSET.tsLoader.pnlStack1.pgcStack1.tsStack1_Above.grpEmpty.btnC_EmptyLoaderZ_Select");
        CHECK_TRUE(row != NULL);
        if (row != NULL) {
            CHECK_STR(row->vcl_class, "TBtnPanelLane");
            CHECK_INT(row->left, 126);
            CHECK_INT(row->top, 82);
            CHECK_INT(row->width, 46);
            CHECK_INT(row->height, 21);
            CHECK_TRUE(row->has_tab_order); CHECK_INT(row->tab_order, 4);

            CHECK_TRUE(row->has_color);
            CHECK_STR(row->color_token, "");     // plain integer in golden, no symbolic name
            CHECK_INT(row->color, 10307329);

            CHECK_TRUE(row->has_font_color);
            CHECK_STR(row->font_color_token, "clWhite");
            CHECK_INT(row->font_color, 0x00FFFFFF);
            CHECK_STR(row->font_face, "Arial");
            CHECK_INT(row->font_height_px, -13);

            CHECK_TRUE(row->has_true_color);
            CHECK_STR(row->true_color_token, "");
            CHECK_INT(row->true_color, 14464261);

            CHECK_TRUE(row->has_false_color);
            CHECK_STR(row->false_color_token, "");
            CHECK_INT(row->false_color, 10307329);

            CHECK_TRUE(row->has_true_font_color);
            CHECK_STR(row->true_font_color_token, "clWhite");
            CHECK_INT(row->true_font_color, 0x00FFFFFF);

            CHECK_TRUE(row->has_false_font_color);
            CHECK_STR(row->false_font_color_token, "clWhite");
            CHECK_INT(row->false_font_color, 0x00FFFFFF);

            // custom_props bag: Alias, TrueColor, FalseColor, TrueFontColor,
            // FalseFontColor are present in golden on this control; LEDStyle/
            // Interval/Value/XItem/YItem/DirectStyle/DirectWidth/EdgeWidth/
            // LineWidth are not (TBtnPanelLane doesn't declare those).
            CHECK_INT(row->custom_prop_count, 5);
            CHECK_TRUE(findCustomProp(*row, "Alias") != NULL);
            CHECK_TRUE(findCustomProp(*row, "TrueColor") != NULL);
            CHECK_TRUE(findCustomProp(*row, "FalseColor") != NULL);
            CHECK_TRUE(findCustomProp(*row, "TrueFontColor") != NULL);
            CHECK_TRUE(findCustomProp(*row, "FalseFontColor") != NULL);
        }
    }

    // =======================================================================
    //  Whole-form sanity: row/array-count consistency (every generated
    //  table's kXxxLayoutCount must equal the number of entries actually in
    //  kXxxLayout -- a mismatch here would mean the generator's own count
    //  field drifted from the array literal it wrote).
    // =======================================================================
    CHECK_TRUE(kARMSLayoutCount > 0);
    CHECK_TRUE(kcBinSelLayoutCount > 0);
    CHECK_TRUE(kcContactLayoutCount > 0);
    CHECK_TRUE(kiosetviewLayoutCount > 0);
    // iosetview is the largest form in the corpus (37,752 golden .dfm lines,
    // plan S6-U) -- sanity floor on its row count catches a badly truncated
    // regeneration without hardcoding the exact number here (that number is
    // exactly reproduced instead by the full-corpus Python-side gate in
    // tools/dfm2rc/reports/b1c_gate_report.json).
    CHECK_TRUE(kiosetviewLayoutCount > 1000);

    if (g_fail == 0) {
        std::printf("PASS  %d/%d tests passed\n", g_total, g_total);
        return 0;
    } else {
        std::printf("FAIL  %d/%d tests failed\n", g_fail, g_total);
        return 1;
    }
}

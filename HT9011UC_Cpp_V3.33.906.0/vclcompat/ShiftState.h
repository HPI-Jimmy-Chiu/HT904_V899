//---------------------------------------------------------------------------
//  vclcompat/ShiftState.h -- TShiftState / TMouseButton / TShiftStateEnum
//
//  AI(W906-FW-SHIFT) 20260826: golden 的滑鼠與按鍵事件簽章（OnMouseDown /
//  OnMouseUp / OnKeyDown / OnKeyUp）都帶 VCL 的 `TMouseButton Button,
//  TShiftState Shift`。本樹一直沒有這兩個型別，於是**任何帶它們的事件處理器
//  都翻不了**——FW-GEM-W10、FW-GEM-W12、FW-YM-W13 各被擋一次，最後一次一口氣
//  擋掉 15 支。
//
//  WHY THIS IS SO SMALL (量測先行，不是猜的)
//  --------------------------------------------------------------------------
//  開工前先量了「golden 到底怎麼用它」，答案決定了這個檔的形狀：
//
//    * golden 全樹（含子目錄、跳過區塊註解內的死碼）帶 `TShiftState` 參數的
//      函式定義：**358 支**。
//    * 其中本體真的**讀到**那個參數的：**1 支**
//      —— `Automation/SCK_ART.cpp:4327` 的 `palXxxMouseDown`，
//         用法是 `if (Shift.Contains(ssDouble))`（判雙擊）。
//    * golden 全樹實際出現的列舉值只有三個：
//      `mbRight` 26 次、`mbLeft` 10 次、`ssDouble` 1 次。
//      （`ssShift/ssAlt/ssCtrl/ssMiddle` 在這棵樹是 0 次。）
//
//  所以完整模型化 Delphi 的 `set of` 語意是過度設計。這裡提供的是
//  **剛好夠 golden 原文逐字成立**的最小形狀：一個位元集合 + `Contains()`。
//  列舉成員仍照 BCB6 `Controls.hpp` 的順序與名稱完整列出（不只列用到的三個），
//  這樣未來若有別的單元用到 `ssShift` 之類，不必再改這個檔的定義。
//
//  量測指令（20260826，可重跑）：
//    python - <<'PY'
//    # 走 goldenscan.load() 跳過 /* */ 內的死碼，對每個帶 TShiftState 參數的
//    # 定義抽出本體，再看本體有沒有引用該參數名
//    PY
//
//  NOT MODELLED (刻意)
//  --------------------------------------------------------------------------
//    * Delphi `set` 的完整運算（`+ - * <=` 等集合代數）——golden 沒用到。
//    * `TShiftState` 由框架填值：本樹沒有訊息迴圈，所有 handler 都不接線，
//      所以傳進去的一律是**空集合**，`Contains()` 恆回 false。
//      對上面那唯一一支消費者而言，效果是「永遠不當成雙擊」——
//      這是收窄（少做一件事），不是多做。
//
//  ⚠ 這個檔推翻了一個既有慣例的前提 —— 讀之前先看清楚
//  --------------------------------------------------------------------------
//  在本檔出現之前，這棵樹對這兩個型別的處理方式是**把參數整個丟掉**：
//  golden 的 `...MouseDown(TObject*, TMouseButton, TShiftState, int X, int Y)`
//  一律簡化成 `(TObject *Sender)`。那個慣例被明文記在至少五個 header 裡
//  （`forms/fTemp_Set.h:63-78`、`cTemperFrom.h`、`forms/fCounterClear.h`、
//   `OmronLaser/LaserSensor.h`、`ATC/ATCInterface.h`、
//   `SECSGEM/uHGemEquipment.h`，都引用同一段理由），
//  而**它給的理由就是「這兩個型別在本樹 ZERO port」**。
//
//  本檔讓那個前提不再成立。依 pt-wave 的「前提死掉不代表答案就是退役」規則，
//  這裡把真正的答案寫下來：
//
//    * **新翻的 handler 從此保留 golden 的完整簽章**（不再丟參數）。
//      理由：忠實度優先，且丟參數會讓「這一支到底對不對得上 golden」
//      每次都要人去記得例外。
//    * **既有那些已經丟掉參數的 handler，本波不回頭改**。
//      那是一次獨立的機械式 pass（要同時改宣告、定義與所有呼叫點），
//      混進翻譯波裡會讓 diff 無法歸因。**這件事還沒做，別以為做過了。**
//    * 因此樹上會**同時存在兩種簽章形狀**，直到那個 pass 跑完。
//      判斷某一支屬於哪一種，看它自己 header 的註解，不要靠推測。
//---------------------------------------------------------------------------
#ifndef VCLCOMPAT_SHIFTSTATE_H
#define VCLCOMPAT_SHIFTSTATE_H

namespace vclcompat {

// golden BCB6 Controls.hpp `enum TShiftStateEnum`（順序照原始碼）
enum TShiftStateEnum
{
    ssShift = 0,
    ssAlt,
    ssCtrl,
    ssLeft,
    ssRight,
    ssMiddle,
    ssDouble
};

// golden BCB6 Controls.hpp `enum TMouseButton`
enum TMouseButton
{
    mbLeft = 0,
    mbRight,
    mbMiddle
};

//---------------------------------------------------------------------------
//  Delphi `set of TShiftStateEnum` 的最小替身。
//  預設空集合——本樹沒有訊息迴圈去填它（見檔頭 NOT MODELLED）。
//---------------------------------------------------------------------------
class TShiftState
{
public:
    TShiftState() : bits_(0) {}

    // golden 唯一用到的讀取方式（Automation/SCK_ART.cpp:4338）
    bool Contains(TShiftStateEnum e) const
    {
        return (bits_ & mask(e)) != 0;
    }

    bool Empty() const { return bits_ == 0; }

    // 給測試與未來的 UI 接線用；golden 自己不會這樣寫（框架才會填值）
    TShiftState& operator<<(TShiftStateEnum e)
    {
        bits_ |= mask(e);
        return *this;
    }
    void Clear() { bits_ = 0; }

    bool operator==(const TShiftState &o) const { return bits_ == o.bits_; }
    bool operator!=(const TShiftState &o) const { return bits_ != o.bits_; }

private:
    static unsigned mask(TShiftStateEnum e) { return 1u << static_cast<unsigned>(e); }
    unsigned bits_;
};

}   // namespace vclcompat

// 這三個名字在 golden 是全域的（BCB6 把 Controls.hpp 的東西攤在全域），
// 事件簽章逐字翻譯時需要非限定名。此處與 vclcompat/StringGrid.h 的
// `using vclcompat::TStringGrid` 同一個慣例——先確認過全樹沒有其他同名定義：
//   grep -rn "class TShiftState\|enum TShiftStateEnum\|enum TMouseButton" -> 只有本檔
using vclcompat::TShiftState;
using vclcompat::TShiftStateEnum;
using vclcompat::TMouseButton;
using vclcompat::ssShift;
using vclcompat::ssAlt;
using vclcompat::ssCtrl;
using vclcompat::ssLeft;
using vclcompat::ssRight;
using vclcompat::ssMiddle;
using vclcompat::ssDouble;
using vclcompat::mbLeft;
using vclcompat::mbRight;
using vclcompat::mbMiddle;

#endif   // VCLCOMPAT_SHIFTSTATE_H

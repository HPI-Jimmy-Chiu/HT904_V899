using System;
using Xunit;
using HT9045.Infrastructure;

namespace HT9045.Tests
{
    // Verifies that Big5Codec can encode Traditional-Chinese strings to Big5 bytes
    // and decode them back without producing any U+FFFD replacement characters.
    // Background: BCB6 source files and .ini/.dat/.csv files are Big5 (cp950).
    // A broken round-trip produces U+FFFD mojibake — see MEMORY note
    // "HT9045 Big5 檔編輯會亂碼".
    public class Big5RoundTripTests
    {
        // Helper: assert no U+FFFD in the result.
        private static void AssertNoMojibake(string result, string label)
        {
            Assert.False(
                Big5Codec.HasMojibake(result),
                label + " contains U+FFFD replacement character(s) — mojibake detected. result=" + result);
        }

        [Fact]
        public void RoundTrip_TraditionalChineseHandlerStatus_NoReplacementChar()
        {
            // "入料手臂" = InArm — appears in BCB6 UI strings and log output.
            const string original = "入料手臂";
            byte[] big5 = Big5Codec.ToBig5(original);
            string decoded = Big5Codec.ToUtf16(big5);
            AssertNoMojibake(decoded, "入料手臂");
            Assert.Equal(original, decoded);
        }

        [Fact]
        public void RoundTrip_MixedAsciiAndChinese_NoReplacementChar()
        {
            // Typical log line mixing ASCII and Traditional Chinese.
            // "測試完成" = Test complete.
            const string original = "RunOneCycle 測試完成 OK";
            byte[] big5 = Big5Codec.ToBig5(original);
            string decoded = Big5Codec.ToUtf16(big5);
            AssertNoMojibake(decoded, "mixed ascii+chinese");
            Assert.Equal(original, decoded);
        }

        [Fact]
        public void RoundTrip_PureAscii_NoReplacementChar()
        {
            // ASCII paths and identifiers should round-trip cleanly too.
            const string original = "OFFLINE_SMOKE.setup";
            byte[] big5 = Big5Codec.ToBig5(original);
            string decoded = Big5Codec.ToUtf16(big5);
            AssertNoMojibake(decoded, "pure ascii");
            Assert.Equal(original, decoded);
        }

        [Fact]
        public void RoundTrip_AlarmDescription_NoReplacementChar()
        {
            // "馬達警報" = Motor Alarm — appears in Error\<lang>\<code>.dat files.
            const string original = "馬達警報 WAR240004";
            byte[] big5 = Big5Codec.ToBig5(original);
            string decoded = Big5Codec.ToUtf16(big5);
            AssertNoMojibake(decoded, "alarm description");
            Assert.Equal(original, decoded);
        }

        [Fact]
        public void HasMojibake_StringWithReplacementChar_ReturnsTrue()
        {
            // Confirm the guard method correctly identifies broken strings.
            string broken = "OK� broken";
            Assert.True(Big5Codec.HasMojibake(broken));
        }

        [Fact]
        public void HasMojibake_CleanString_ReturnsFalse()
        {
            Assert.False(Big5Codec.HasMojibake("clean ASCII string"));
        }

        [Fact]
        public void ToBig5_NullArg_ThrowsArgumentNullException()
        {
            Assert.Throws<ArgumentNullException>(() => Big5Codec.ToBig5(null));
        }

        [Fact]
        public void ToUtf16_NullArg_ThrowsArgumentNullException()
        {
            Assert.Throws<ArgumentNullException>(() => Big5Codec.ToUtf16(null));
        }

        [Fact]
        public void HasMojibake_NullArg_ThrowsArgumentNullException()
        {
            Assert.Throws<ArgumentNullException>(() => Big5Codec.HasMojibake(null));
        }
    }
}

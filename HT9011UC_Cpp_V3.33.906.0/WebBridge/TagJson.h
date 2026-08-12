// ===========================================================================
//  WebBridge/TagJson.h
//  TagMap <-> JSON object, both directions, in one place.
//
//  AI(W906-WebBridge-Tcp) 20260812: new file. NEW-BUILD infrastructure, not a
//  translation of any BCB6 golden source.
//
//  WHY THIS EXISTS
//  ---------------
//  Until now only ONE component turned tags into JSON: WebBridgeServer.cpp,
//  which kept a private ObjectFrom() for its WebSocket frames. The TCP link
//  (TcpTagLink.h) needs the SAME encoding -- and, for the first time, needs the
//  DECODE direction too, because a consumer process now has to rebuild a TagMap
//  from the wire.
//
//  Two encoders that are supposed to agree byte-for-byte, maintained
//  separately, is exactly the drift this tree keeps paying for elsewhere. So
//  the encoder moved here and WebBridgeServer.cpp's ObjectFrom() now delegates
//  to it: one definition, one place to fix, and test_wb_tcplink.cpp asserts the
//  round trip.
//
//  ENCODING RULES (these mirror ARCHITECTURE.md section 4 rule 3)
//  --------------
//    Null   -> null      "unknown / not installed"; the page renders "---"
//    Bool   -> true|false
//    Int    -> 123       (int64, no quotes)
//    Double -> 1.25
//    String -> "text"    ""  stays ""  -- deliberately empty, NOT null
//
//  The ""/null distinction is load-bearing on the page (Number(null) is 0, so
//  collapsing them prints 0.00 for every uninstalled heater zone), which is why
//  the decoder below is careful to reproduce it rather than normalising.
//
//  NUMBERS -- THE ONE LOSSY EDGE, STATED PLAINLY
//  JSON has a single number type and cJSON does not retain the original token,
//  so the decoder cannot tell 1 from 1.0. It classifies as Int when the value
//  is integral and fits int64, otherwise Double.
//
//  Consequence: a publisher-side Double that happens to be exactly integral
//  arrives as an Int. That is the whole of the damage, and it is harmless here
//  for two reasons: a consumer only ever diffs its own decoded values against
//  each other (so this cannot manufacture phantom changes), and the page
//  formats numbers client-side via data-fmt, so 1 and 1.0 render identically.
//
//  It is NOT harmless if some future caller round-trips a TagMap and asserts
//  type equality -- test_wb_tcplink.cpp pins this behaviour so that assumption
//  fails loudly in a test rather than quietly on a machine.
// ===========================================================================
#ifndef WEBBRIDGE_TAGJSON_H
#define WEBBRIDGE_TAGJSON_H

#include "WebBridge/TagSnapshot.h"   // TagMap
#include "WebBridge/TagValue.h"

#include <string>
#include <vector>

namespace webbridge {

// ---------------------------------------------------------------------------
//  Encode
// ---------------------------------------------------------------------------

// {"tag":value,"tag2":value2}  -- deterministic order (TagMap is std::map).
std::string EncodeTagObject(const TagMap& tags);

// One value token ("null", "true", "12", "1.5", "\"s\"").
std::string EncodeTagValue(const TagValue& v);

// ---------------------------------------------------------------------------
//  Decode
// ---------------------------------------------------------------------------

// Parse a JSON object of tag -> value into `out` (cleared first).
//
// Returns false and fills `error` when the text is not a JSON object, or when a
// member holds something this seam has no representation for (array/object).
// A partially decoded `out` is NOT handed back on failure -- callers get all or
// nothing, so a malformed frame can never half-apply to a live snapshot.
bool DecodeTagObject(const std::string& json, TagMap& out, std::string& error);

// Same, for a caller that already holds a parsed cJSON object node. Declared
// with a void* so this header stays free of the cJSON include -- the .cpp
// casts it back. (cJSON.h lives in ht9045_public and pulls in a lot; the whole
// point of WebBridge is that its headers stay cheap.)
bool DecodeTagObjectNode(const void* cjsonObjectNode, TagMap& out, std::string& error);

} // namespace webbridge

#endif // WEBBRIDGE_TAGJSON_H

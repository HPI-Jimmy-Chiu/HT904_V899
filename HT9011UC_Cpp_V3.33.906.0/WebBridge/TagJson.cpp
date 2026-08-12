// ===========================================================================
//  WebBridge/TagJson.cpp  -- see TagJson.h for the contract and the one lossy
//  edge (integral Double -> Int).
//
//  AI(W906-WebBridge-Tcp) 20260812: new file.
// ===========================================================================
#include "WebBridge/TagJson.h"

#include "WebBridge/JsonWriter.h"
#include "Public/cJSON.h"

#include <cmath>
#include <limits>

namespace webbridge {

// ---------------------------------------------------------------------------
//  Encode
// ---------------------------------------------------------------------------

std::string EncodeTagValue(const TagValue& v)
{
    JsonWriter w;
    switch (v.type()) {
        case TagType::Bool:   w.Bool(v.asBool());                            break;
        case TagType::Int:    w.Number(static_cast<wb_int64>(v.asInt()));    break;
        case TagType::Double: w.Number(v.asDouble());                        break;
        case TagType::String: w.String(v.asString());                        break;
        default:              w.Null();                                      break;
    }
    return w.Str();
}

std::string EncodeTagObject(const TagMap& tags)
{
    JsonWriter w;
    w.BeginObject();
    for (TagMap::const_iterator it = tags.begin(); it != tags.end(); ++it) {
        w.Key(it->first);
        switch (it->second.type()) {
            case TagType::Bool:   w.Bool(it->second.asBool());                         break;
            case TagType::Int:    w.Number(static_cast<wb_int64>(it->second.asInt())); break;
            case TagType::Double: w.Number(it->second.asDouble());                     break;
            case TagType::String: w.String(it->second.asString());                     break;
            default:              w.Null();                                            break;
        }
    }
    w.EndObject();
    return w.Str();
}

// ---------------------------------------------------------------------------
//  Decode
// ---------------------------------------------------------------------------
namespace {

// Integral and representable as int64 -> Int, else Double. See the header's
// "ONE LOSSY EDGE" note for why this is the accepted behaviour and not a bug.
TagValue NumberToTagValue(double d)
{
    const double kInt64Max = 9223372036854775808.0;   // 2^63, exclusive bound
    if (std::floor(d) == d && d >= -kInt64Max && d < kInt64Max) {
        return TagValue::makeInt(static_cast<std::int64_t>(d));
    }
    return TagValue::makeDouble(d);
}

} // namespace

bool DecodeTagObjectNode(const void* node, TagMap& out, std::string& error)
{
    error.clear();

    const cJSON* obj = static_cast<const cJSON*>(node);
    if (obj == NULL || !cJSON_IsObject(obj)) {
        error = "not a JSON object";
        return false;
    }

    // Decode into a scratch map: the header promises all-or-nothing, so a
    // malformed member must not leave half a frame applied to a live snapshot.
    TagMap scratch;

    for (const cJSON* it = obj->child; it != NULL; it = it->next) {
        if (it->string == NULL) {
            error = "object member with no name";
            return false;
        }
        const std::string key(it->string);

        if (cJSON_IsNull(it)) {
            scratch[key] = TagValue::makeNull();
        } else if (cJSON_IsBool(it)) {
            scratch[key] = TagValue::makeBool(cJSON_IsTrue(it) ? true : false);
        } else if (cJSON_IsNumber(it)) {
            scratch[key] = NumberToTagValue(it->valuedouble);
        } else if (cJSON_IsString(it)) {
            // valuestring is NULL only on a malformed node; "" is a real value
            // and must stay "" rather than becoming Null (ARCHITECTURE.md §4.3).
            scratch[key] = TagValue::makeString(it->valuestring ? it->valuestring : "");
        } else {
            // Arrays and nested objects have no TagValue representation. Being
            // strict here is deliberate: silently dropping them would make a
            // sender's bug look like a tag that simply never updates.
            error = "tag '" + key + "' holds an array or object, which this seam cannot carry";
            return false;
        }
    }

    out.swap(scratch);
    return true;
}

bool DecodeTagObject(const std::string& json, TagMap& out, std::string& error)
{
    error.clear();

    cJSON* root = cJSON_Parse(json.c_str());
    if (root == NULL) {
        error = "JSON parse failed";
        return false;
    }

    const bool ok = DecodeTagObjectNode(root, out, error);
    cJSON_Delete(root);
    return ok;
}

} // namespace webbridge

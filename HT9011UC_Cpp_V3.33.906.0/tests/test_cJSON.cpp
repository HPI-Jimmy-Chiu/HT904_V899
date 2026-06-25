// ===========================================================================
//  tests/test_cJSON.cpp
//  Tiny round-trip smoke test for the portable Public/cJSON.c module.
//
//  Verifies the C-vs-C++ linkage works (cJSON.h is wrapped in extern "C",
//  cJSON.c is compiled as C in the build) and that a basic
//  parse -> read value -> print -> free cycle behaves as expected.
//
//  Build: linked against ht9045_public.
//  Non-zero exit on any failure.
// ===========================================================================

#include "Public/cJSON.h"

#include <cstdio>
#include <cstring>

static int g_fail  = 0;
static int g_total = 0;

static void check(bool cond, const char* expr, const char* file, int line) {
    ++g_total;
    if (!cond) {
        ++g_fail;
        std::printf("FAIL [%s:%d]  %s\n", file, line, expr);
    }
}
#define CHECK(cond) check((cond), #cond, __FILE__, __LINE__)

int main()
{
    // -----------------------------------------------------------------------
    // Parse a small JSON document.
    // -----------------------------------------------------------------------
    const char* json =
        "{\"name\":\"HT9045\",\"version\":906,\"temp\":25.5,\"ok\":true}";

    cJSON* root = cJSON_Parse(json);
    CHECK(root != 0);
    if (root == 0) {
        std::printf("cJSON_Parse failed near: %s\n", cJSON_GetErrorPtr());
        std::printf("test_cJSON: %d / %d passed\n", g_total - g_fail, g_total);
        return 1;
    }

    // -----------------------------------------------------------------------
    // Read values back out.
    // -----------------------------------------------------------------------
    cJSON* name = cJSON_GetObjectItem(root, "name");
    CHECK(name != 0);
    CHECK(name != 0 && cJSON_IsString(name));
    CHECK(name != 0 && name->valuestring != 0 &&
          std::strcmp(name->valuestring, "HT9045") == 0);

    cJSON* version = cJSON_GetObjectItem(root, "version");
    CHECK(version != 0 && cJSON_IsNumber(version));
    CHECK(version != 0 && version->valueint == 906);

    cJSON* temp = cJSON_GetObjectItem(root, "temp");
    CHECK(temp != 0 && cJSON_IsNumber(temp));
    CHECK(temp != 0 && temp->valuedouble > 25.49 && temp->valuedouble < 25.51);

    cJSON* ok = cJSON_GetObjectItem(root, "ok");
    CHECK(ok != 0 && cJSON_IsBool(ok));
    CHECK(ok != 0 && cJSON_IsTrue(ok));

    // -----------------------------------------------------------------------
    // Print (serialize) and confirm a round-trip re-parse reads the same value.
    // -----------------------------------------------------------------------
    char* printed = cJSON_Print(root);
    CHECK(printed != 0);
    if (printed != 0) {
        std::printf("cJSON round-trip output:\n%s\n", printed);

        cJSON* reparsed = cJSON_Parse(printed);
        CHECK(reparsed != 0);
        if (reparsed != 0) {
            cJSON* n2 = cJSON_GetObjectItem(reparsed, "name");
            CHECK(n2 != 0 && n2->valuestring != 0 &&
                  std::strcmp(n2->valuestring, "HT9045") == 0);
            cJSON* v2 = cJSON_GetObjectItem(reparsed, "version");
            CHECK(v2 != 0 && v2->valueint == 906);
            cJSON_Delete(reparsed);
        }
        cJSON_free(printed);   // free via cJSON's allocator (matches cJSON_Print)
    }

    // -----------------------------------------------------------------------
    // Free.
    // -----------------------------------------------------------------------
    cJSON_Delete(root);

    std::printf("test_cJSON: %d / %d passed\n", g_total - g_fail, g_total);
    return (g_fail == 0) ? 0 : 1;
}

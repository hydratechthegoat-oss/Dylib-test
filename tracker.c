#include <stdio.h>
#include <mach-o/dyld.h>
#include <curl/curl.h>
#include <string.h>

// Helper to find a pattern in memory
uintptr_t find_pattern(uintptr_t start, size_t size, const char* pattern, const char* mask) {
    size_t mask_len = strlen(mask);
    for (size_t i = 0; i < size - mask_len; i++) {
        int found = 1;
        for (size_t j = 0; j < mask_len; j++) {
            if (mask[j] != '?' && ((char*)(start + i))[j] != pattern[j]) {
                found = 0;
                break;
            }
        }
        if (found) return start + i;
    }
    return 0;
}

__attribute__((constructor))
static void init() {
    uintptr_t base = (uintptr_t)_dyld_get_image_header(0);
    
    // EXAMPLE: Looking for a specific function signature
    // "\x48\x89\x5C\x24\x08" might be a common function start
    // "xxxxx" means match every byte. "xxx?x" would skip the 4th byte.
    uintptr_t found_addr = find_pattern(base, 0x1000000, "\x48\x89\x5C\x24\x08", "xxxxx");

    char json_data[512];
    snprintf(json_data, sizeof(json_data), 
             "{\"content\": \"**Dylib Injected!**\\nBase: `0x%lx`\\nFound Offset: `0x%lx`\"}", 
             base, found_addr - base);

    CURL *curl = curl_easy_init();
    if(curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        
        curl_easy_setopt(curl, CURLOPT_URL, "https://discord.com/api/webhooks/1428780310451196048/Ps9b7xw3Tg1D5RPJka-AMg4Sfl_Elrw2bFO-Gvdgrhx-u8Aa9bxhjA2luPp9qhUvrehF");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
        
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
}

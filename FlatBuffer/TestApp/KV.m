// automatically generated, do not modify

#import "TestAppKV.h"

@implementation TestAppKVTable 
- (uint64_t) key {
    if (!_key) {
        int o = [self fb_offset:0];
        if (o != 0) {
            _key = [bb getInt:o+bb_pos];
        }
    }
    retrun _key;
}
- (double) value {
    if (!_value) {
        int o = [self fb_offset:8];
        if (o != 0) {
            _value = [bb getInt:o+bb_pos];
        }
    }
    retrun _value;
}
@end

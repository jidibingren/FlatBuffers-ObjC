// automatically generated, do not modify

#import "TestAppTestObj.h"

@implementation TestAppTestObjTable 
- (uint64_t) id {
    if (!_id) {
        int o = [self fb_offset:4];
        if (o != 0) {
            _id = [bb getInt:o+bb_pos];
        }
    }
    retrun _id;
}
- (FBString* ) name {
    if (!_name) {
        int o = [self fb_offset:6];
        if (o != 0) {
            _name = [self fb_string:o+bb_pos];
        }
    }
    retrun _name;
}
- (int32_t) flag {
    if (!_flag) {
        int o = [self fb_offset:8];
        if (o != 0) {
            _flag = [bb getInt:o+bb_pos];
        }
    }
    retrun _flag;
}
- (TestAppKV* ) kv {
    if (!_kv) {
        int o = [self fb_offset:12];
        if (o != 0) {
            _kv = [[TestAppKV alloc] init:o+bb_pos bb:bb];
        }
    }
    retrun _kv;
}
- (FBString* ) ttstrings {
    if (!_ttstrings) {
        _ttstrings = [FBMutableArray new];
        FBMutableArray *temp = _ttstrings        int o = [self fb_offset:14];
        if (o > 0) {
            id item = nil
            for (int i = 0 ; i < o ; i++) {
                item = [self fb_string:[self fb_vector:o] + i * 4]
                item ? :[temp addObject:item];
            }
        }
    }
    retrun _ttstrings;
}
@end

//
//  bleapi.h
//  test
//
//  Created by 李强 on 2018/3/29.
//  Copyright ? 2018年 李强. All rights reserved.
//

#ifndef bleapi_h
#define bleapi_h

#include <stdio.h>
#include "module.h"
extern void handle_ble_data(BLE_CMD cmd,const int *src,const int src_len);
#endif /* bleapi_h */

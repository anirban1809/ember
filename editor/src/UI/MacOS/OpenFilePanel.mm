#import <Cocoa/Cocoa.h>
#include "../../../include/UI/MacOS/OpenFilePanel.h"
#include <iostream>
#include <string>

std::string OpenFileDialog(const std::string &initialDir) {
  @autoreleasepool {
    NSOpenPanel *panel = [NSOpenPanel openPanel];

    [panel setCanChooseFiles:YES];
    [panel setCanChooseDirectories:NO];
    [panel setAllowsMultipleSelection:NO];
    [panel setDirectoryURL:[NSURL fileURLWithPath:[NSString
                                                      stringWithUTF8String:
                                                          initialDir.c_str()]]];

    if ([panel runModal] == NSModalResponseOK) {
      NSURL *selectedFile = [[panel URLs] objectAtIndex:0];
      return std::string([[selectedFile path] UTF8String]);
    }
  }

  return "";
}
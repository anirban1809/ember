{
    "targets": [
        {
            "target_name": "oglview",
            "sources": ["native/oglview.mm", "native/tri_renderer.cpp"],
            "cflags": ["-DGL_SILENCE_DEPRECATION"],
            "cflags_cc": ["-std=c++17", "-fexceptions", "-DGL_SILENCE_DEPRECATION"],
            "include_dirs": ["<!@(node -p \"require('node-addon-api').include\")"],
            "dependencies": ["<!(node -p \"require('node-addon-api').gyp\")"],
            "defines": ["NAPI_VERSION=8"],
            "xcode_settings": {
                "CLANG_CXX_LANGUAGE_STANDARD": "c++17",
                "MACOSX_DEPLOYMENT_TARGET": "11.0",
                "CLANG_ENABLE_OBJC_ARC": "YES",
                "OTHER_CPLUSPLUSFLAGS": ["-fexceptions"],
                "OTHER_LDFLAGS": [
                    "-framework",
                    "Cocoa",
                    "-framework",
                    "QuartzCore",
                    "-framework",
                    "OpenGL",
                    "-framework",
                    "IOSurface",
                ],
            },
        }
    ]
}

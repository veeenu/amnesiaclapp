{
  "targets": [{
    "target_name": "amnesiaclapp",
    "msvs_settings": {
      "VCCLCompilerTool": {
        "ExceptionHandling": 1
      }
    },
    "sources": [
      "src/main.cpp"
    ],
    'include_dirs': [
      "<!@(node -p \"require('node-addon-api').include\")"
    ],
    'libraries': [],
    'dependencies': [
        "<!(node -p \"require('node-addon-api').gyp\")"
    ]
  }]
}
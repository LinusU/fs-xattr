{
  "targets": [
    {
      "target_name": "xattr",
      "sources": [ "src/error.cc", "src/xattr.cc" ],
      "include_dirs" : [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}

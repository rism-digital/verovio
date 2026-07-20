Pod::Spec.new do |s|
  s.name           = 'Verovio'
  s.version        = '6.3.0-dev'
  s.license        = { :type => 'LGPL', :file => 'COPYING' }
  s.homepage       = 'https://www.verovio.org/index.xhtml'
  s.authors        = { 'Contributors List' => 'https://github.com/rism-digital/verovio/graphs/contributors' }
  s.summary        = 'Verovio'
  s.source         = { :git => 'https://github.com/rism-digital/verovio.git', :tag => 'version-' + s.version.to_s }
  s.source_files   = 
      'src/**/*.{cpp,cc}',
      'include/{crc,hum,json,midi,pugi,tuning-library,vrv,zip}/*.{h,hpp}',
      'libmei/{dist,addons}/*.{h,cpp}',
      'vendor/harfbuzz/src/verovio-harfbuzz.cc',
      'vendor/harfbuzz/src/*.{h,hh,hpp}',
      'vendor/woff2/include/woff2/decode.h',
      'vendor/woff2/src/verovio-woff2.cc',
      'vendor/woff2/src/{buffer,port,round,store_bytes,table_tags,variable_length,woff2_common}.{h}',
      'vendor/brotli/c/verovio-brotli.c',
      'vendor/brotli/c/{include,common,dec}/**/*.h'
  s.public_header_files =
      'include/{json,pugi,vrv}/*.{h,hpp}',
      'libmei/{dist,addons}/*.{h}'
  s.private_header_files = 
      'include/{crc,hum,json,midi,pugi,tuning-library,vrv,zip}/*.{h,hpp}',
      'libmei/{dist,addons}/*.{h}'
  s.resources      = 'data'
  s.ios.deployment_target = '16.0'
  s.osx.deployment_target = '11.0'
  s.pod_target_xcconfig = {
      "CLANG_CXX_LANGUAGE_STANDARD" => "c++23",
      "CLANG_CXX_LIBRARY" => "libc++",
      "CLANG_WARN_COMMA" => "NO",
      "GCC_C_LANGUAGE_STANDARD" => "gnu11",
      "GCC_DYNAMIC_NO_PIC" => "NO",
      "GCC_NO_COMMON_BLOCKS" => "YES",
      "GCC_SYMBOLS_PRIVATE_EXTERN" => "NO",
      "CLANG_ENABLE_OBJC_ARC" => "YES",
      "CLANG_ENABLE_OBJC_WEAK" => "YES",
      "ENABLE_STRICT_OBJC_MSGSEND" => "YES",
      "MTL_FAST_MATH" => "YES",
      "SUPPORTS_UIKITFORMAC" => "NO",
      "MTL_ENABLE_DEBUG_INFO" => "NO",
      "PRODUCT_BUNDLE_IDENTIFIER" => "digital.rism.VerovioFramework",
      "HEADER_SEARCH_PATHS" => "$(PODS_TARGET_SRCROOT)/vendor/harfbuzz/src $(PODS_TARGET_SRCROOT)/vendor/woff2/include $(PODS_TARGET_SRCROOT)/vendor/woff2/src $(PODS_TARGET_SRCROOT)/vendor/brotli/c/include",
      "GCC_PREPROCESSOR_DEFINITIONS" => "$(inherited) BROTLI_STATIC HB_LEAN HB_MINI HB_OPTIMIZE_SIZE_MORE HB_CONFIG_OVERRIDE_H='\"verovio-hb-config.h\"'"
    }
end

Pod::Spec.new do |s|
  s.name           = 'VerovioToolkit'
  s.version        = '5.5.0-dev'
  s.license        = { :type => 'LGPL', :file => 'COPYING' }
  s.homepage       = 'https://www.verovio.org/index.xhtml'
  s.authors        = { 'Contributors List' => 'https://github.com/rism-digital/verovio/graphs/contributors' }
  s.summary        = 'VerovioToolkit'
  s.source         = { :git => 'https://github.com/rism-digital/verovio.git', :branch => 'develop-java' }
  s.source_files   = [
    'src/**/*.{h,cpp,cc}',
    'include/{crc,hum,json,midi,pugi,utf8,vrv,zip}/*.{h,hpp}',
    'libmei/{dist,addons}/*.{h,cpp}',
    'tools/c_wrapper.{h,cpp}',
    'bindings/swift/*.{h,swift}'
  ]
  s.private_header_files = [
    'include/{crc,hum,json,midi,pugi,utf8,vrv,zip}/*.{h,hpp}',
    'libmei/{dist,addons}/*.h'
  ]
  s.module_map = 'bindings/swift/VerovioToolkit.modulemap'
  s.resources      = 'data'
  s.ios.deployment_target = '16.0'
  s.osx.deployment_target = '11.0'
  s.swift_version = '5.0'
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
      "PRODUCT_BUNDLE_IDENTIFIER" => "digital.rism.VerovioToolkitFramework",
      "SWIFT_VERSION" => "5.0"
    }
end


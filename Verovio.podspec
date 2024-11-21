Pod::Spec.new do |s|
  s.name           = 'Verovio'
  s.version        = '4.4.0-dev'
  s.license        = { :type => 'LGPL', :file => 'COPYING' }
  s.homepage       = 'https://www.verovio.org/index.xhtml'
  s.authors        = { 'Contributors List' => 'https://github.com/rism-digital/verovio/graphs/contributors' }
  s.summary        = 'Verovio'
  s.source         = { :git => 'https://github.com/rism-digital/verovio.git', :tag => 'version-' + s.version.to_s }
  s.swift_versions = ['3.0', '4.0', '4.1', '4.2', '5.0', '5.1', '5.2', '5.3', '5.4', '5.5', '5.6', '5.7']
  s.source_files        = 'src/**/*.{h,cpp,cc}',
                        'include/{crc,hum,json,midi,pugi,utf8,vrv,zip}/*.{h,hpp}',
                        'libmei/{dist,addons}/*.{h,cpp}'
  s.public_header_files = 'src/**/*.{h}',
                          'include/{crc,hum,json,midi,pugi,utf8,vrv,zip}/*.{h,hpp}',
                          'libmei/{dist,addons}/*.{h}'
  s.resources      = 'data'
  s.ios.deployment_target = '14.0'
  s.osx.deployment_target = '10.15'
  s.pod_target_xcconfig = {
      "CLANG_CXX_LANGUAGE_STANDARD" => "c++23",
      "CLANG_CXX_LIBRARY" => "libc++",
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
      "PRODUCT_BUNDLE_IDENTIFIER" => "digital.rism.VerovioFramework"
    }
end


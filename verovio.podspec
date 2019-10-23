Pod::Spec.new do |s|
  s.name           = 'Verovio'
  s.version        = '2.1.0'
  s.license        = { :type => 'BSD' }
  s.homepage       = 'https://github.com/vitalii-unicsoft/verovio'
  s.authors        = { 'Vitalii Yevtushenko' => 'yev@unicsoft.com' }
  s.summary        = 'Verovio'
  s.source         = { :git => 'https://github.com/vitalii-unicsoft/verovio', :tag => 'version-' + s.version.to_s }
	s.swift_versions = ['5.1']
  s.source_files   = 'src/**/*.{h,cpp,cc}', 
									   'include/{hum,json,midi,pugi,utf8,vrv}/*.{h,hpp}',
										 'libmei/{attc*,atts_a*,atts_c*,atts_ex*,atts_fa*,atts_g*,atts_m*,atts_n*,atts_pa*,atts_s*,atts_v*,attt*}.{h,cpp}'
	s.public_header_files = 'src/**/*.{h}', 
											    'include/{hum,json,midi,pugi,utf8,vrv}/*.{h,hpp}',
													'libmei/{attc*,atts_a*,atts_c*,atts_ex*,atts_fa*,atts_g*,atts_m*,atts_n*,atts_pa*,atts_s*,atts_v*,attt*}.{h}'
	s.platform       = :ios, '12.0'
  s.resources = 'data'
	s.xcconfig = {
	    "CLANG_CXX_LANGUAGE_STANDARD" => "gnu++14",
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
			"MTL_ENABLE_DEBUG_INFO" => "NO"
	  }
  s.info_plist = {
    'CFBundleIdentifier' => 'com.rism.VerovioFramework'
  }
end

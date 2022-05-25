require "json"

package = JSON.parse(File.read(File.join(__dir__, "package.json")))

Pod::Spec.new do |s|
  s.name         = "qlik-oss-react-native-helium"
  s.version      = package["version"]
  s.summary      = package["description"]
  s.homepage     = package["homepage"]
  s.license      = package["license"]
  s.authors      = package["author"]

  s.platforms    = { :ios => "10.0" }
  s.source       = { :git => "https://github.com/vcellu/qlik-oss-react-native-helium.git", :tag => "#{s.version}" }

  s.requires_arc = true
  s.pod_target_xcconfig = {
    'GCC_PREPROCESSOR_DEFINITIONS' => '$(inherited) SK_GL=1 SK_METAL=1',
    'USER_HEADER_SEARCH_PATHS' => 'ios/**/*',
    'CLANG_CXX_LANGUAGE_STANDARD' => 'c++17'
  }

  s.frameworks = 'GLKit', 'MetalKit'

  s.vendored_frameworks = ["skialib/skia.xcframework", "skialib/skiasvg.xcframework", "skialib/skshaper.xcframework"]

  s.source_files = "ios/**/*.{h,m,mm,cpp,hpp}", "cpp/**/*.{h,m,mm,cpp,hpp}"

  s.subspec 'skialib' do |ss|
    ss.header_mappings_dir = 'skialib'
    ss.source_files = "skialib/**/*.{h}"
  end


  s.dependency "React"
  s.dependency "React-callinvoker"
  s.dependency "React-Core"
end

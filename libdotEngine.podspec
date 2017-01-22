Pod::Spec.new do |s|
    s.name             = 'libdotEngine'
    s.version          = '1.0.0'
    s.summary          = 'dotEngine  realtime audio/video  as a service'


    s.description      = <<-DESC
                        dotEngine realtime audio/video as a service
                       DESC

    s.homepage         = 'https://dot.cc'
    s.license          = { :type => 'MIT', :file => 'LICENSE' }
    s.author           = { 'notedit' => 'notedit@gmail.com' }
    s.source           = { :git => 'https://github.com/dotEngine/dotEngine-ios-sdk.git', :tag => s.version.to_s }

    s.source_files =  'include/DotEngine/*.h'
    s.public_header_files = "include/DotEngine/*.h"
    s.preserve_paths = 'libDotEngine.a'
    s.vendored_libraries = 'libDotEngine.a'


    s.ios.framework = 'AVFoundation', 'AudioToolbox', 'CoreGraphics', 'CoreMedia', 'GLKit', 'UIKit', 'VideoToolbox'
    s.libraries = 'c', 'sqlite3', 'stdc++'
    s.requires_arc = true
    s.dependency  'SocketRocket'
    s.dependency  'AFNetworking/Reachability'
    s.ios.deployment_target = '8.0'
end

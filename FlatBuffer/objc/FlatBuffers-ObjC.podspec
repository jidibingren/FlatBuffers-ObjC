Pod::Spec.new do |s|
  s.name         = "FlatBuffers-ObjC"
  s.version      = "0.0.1"
  s.summary      = "FlatBuffers Objective-C implementation"
  s.homepage     = "https://github.com/jidibingren/flatbuffers"
  s.license      = 'MIT'
  s.author       = { "jidibingren" => "jidibingren@gmail.com" }
  s.platform     = :ios, '7.0'
  s.source       = {:git => "https://github.com/jidibingren/flatbuffers.git" ,
		                :tag => s.version }
  s.source_files = "./*.{h,m}"
  s.prefix_header_file = "./FBObjC.pch"
  s.requires_arc = true

end

Pod::Spec.new do |s|
  s.name         = "FlatBuffers-ObjC"
  s.version      = "0.0.2"
  s.summary      = "FlatBuffers Objective-C implementation"
  s.homepage     = "https://github.com/jidibingren/FlatBuffers-ObjC"
  s.license      = 'MIT'
  s.author       = { "jidibingren" => "jidibingren@gmail.com" }
  s.platform     = :ios, '7.0'
  s.source       = {:git => "https://github.com/jidibingren/FlatBuffers-ObjC.git" ,
		                :tag => s.version }
  s.source_files = "FlatBuffers-ObjC/*.{h,m}"
  s.prefix_header_file = "FlatBuffers-ObjC/FBObjC.pch"
  s.requires_arc = true

end

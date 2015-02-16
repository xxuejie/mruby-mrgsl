MRuby::Gem::Specification.new('mrgsl') do |spec|
  spec.license = 'MIT'
  spec.author  = 'MRGSL Developers'

	spec.cc.flags << '-std=c99'
	if ENV['OS'] == 'Windows_NT'
		spec.linker.libraries << %w(SDL2main SDL2 SDL2_image SDL2_ttf winmm gdi32 ole32 oleaut32 imm32 version turbojpeg jpeg png png12 freetype glew32 opengl32 z)
		spec.linker.library_paths << Dir.glob("#{dir}/dependencies/win32/lib/")
		spec.cc.include_paths << Dir.glob("#{dir}/dependencies/include/")
	else
		spec.cc.include_paths << Dir.glob("#{dir}/dependencies/linux/lib/")
		spec.linker.library_paths << Dir.glob("#{dir}/dependencies/include/")
		spec.linker.libraries << %w(SDL2 SDL2_image pthread dl m png png12 turbojpeg jpeg z GL GLEW glut GLU)
	end
end

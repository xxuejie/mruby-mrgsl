MRuby::Gem::Specification.new('mrgsl') do |spec|
  spec.license = 'MIT'
  spec.author  = 'MRGSL Developers'

  # Add compile flags
  # spec.cc.flags << ''
	spec.cc.flags << '-std=c99'
  # Add cflags to all
  # spec.mruby.cc.flags << '-g'
  # Add libraries
	  spec.linker.libraries << %w(SDL2main)
	  spec.linker.libraries << %w(SDL2)
	  spec.linker.libraries << %w(SDL2_image)
	  spec.linker.libraries << %w(winmm)
	  spec.linker.libraries << %w(gdi32)
	  spec.linker.libraries << %w(ole32)
	  spec.linker.libraries << %w(oleaut32)
	  spec.linker.libraries << %w(imm32)
	  spec.linker.libraries << %w(version)
	  spec.linker.libraries << %w(turbojpeg)
	  spec.linker.libraries << %w(jpeg)
	  spec.linker.libraries << %w(png)
	  spec.linker.libraries << %w(png12)
	  spec.linker.libraries << %w(glew32)
	  spec.linker.libraries << %w(opengl32)
	  spec.linker.libraries << %w(z)
	  
	  spec.linker.library_paths << "D:\\dependency\\libs"
	  spec.cc.include_paths << "D:\\dependency\\includes"
  # Default build files
  # spec.rbfiles = Dir.glob("#{dir}/mrblib/*.rb")
  # spec.objs = Dir.glob("#{dir}/src/*.{c,cpp,m,asm,S}").map { |f| objfile(f.relative_path_from(dir).pathmap("#{build_dir}/%X")) }
  # spec.test_rbfiles = Dir.glob("#{dir}/test/*.rb")
  # spec.test_objs = Dir.glob("#{dir}/test/*.{c,cpp,m,asm,S}").map { |f| objfile(f.relative_path_from(dir).pathmap("#{build_dir}/%X")) }
  # spec.test_preload = 'test/assert.rb'

  # Values accessible as TEST_ARGS inside test scripts
  # spec.test_args = {'tmp_dir' => Dir::tmpdir}
end

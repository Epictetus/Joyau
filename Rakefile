require 'rubygems'

require 'joyau_rdoc'
require 'rake/rdoctask'
require 'rake/packagetask'
require 'fileutils'

require 'net/ftp'

JOYAU_VERSION = "1.1.0"

libs = %w(socket/socket.c socket/getnameinfo.c socket/getaddrinfo.c
          socket/pspsocket.c
          bigdecimal/bigdecimal.c
          digest/sha2/sha2init.c digest/sha2/sha2.c
          digest/rmd160/rmd160.c digest/rmd160/rmd160init.c
          digest/bubblebabble/bubblebabble.c
          digest/sha1/sha1.c digest/sha1/sha1init.c
          digest/md5/md5.c digest/md5/md5init.c
          digest/digest.c
          enumerator/enumerator.c
          fcntl/fcntl.c
          stringio/stringio.c
          strscan/strscan.c
          thread/thread.c
          zlib/zlib.c
          nkf/nkf.c
          etc/etc.c
          syck/bytecode.c syck/gram.c syck/implicit.c syck/rubyext.c
          syck/token.c syck/emitter.c syck/handler.c syck/node.c syck/syck.c
          syck/yaml2byte.c).map { |i| "src/#{i}" }

static_libs = %w(-losl -ljpeg -lpng -lz -lpspsdk -lpspvfpu -lpspctrl -lpspumd
                 -lpsppower -lpspgu -lpspmpeg -lpspaudiocodec -lpspaudiolib
                 -lalut -lOpenAL32 -lpsprtc -lvorbisfile  -lvorbis -lmikmod
                 -lpspaudio -lpspusb -lpspusbstor -lpsphprm -lpsputility
                 -lpspwlan -lpspgum -lruby -lm -logg -lpsprtc -lstdc++)

cflags = %w(-G0 -Wall -D_PSP_ -DHAVE_STRUCT_TIMESPEC)

begin
  require 'rake/psp_task'

  PSPTask.new do |t|
    t.objdir = "./obj/"

    t.srcs = Dir['src/*.cpp'] | libs
    t.libs = static_libs | t.libs

    t.cflags = t.cflags | cflags
    t.incdir << "-I./src/intrafont/"

    t.target = "jrelease"
    t.icon0  = "eboot/ICON0.png"
    t.pic1   = "eboot/PIC1.png"

    t.build_eboot = true
  end

  PSPTask.new(:dbg) do |t|
    t.objdir = "./obj_prx/"

    t.srcs = Dir['src/*.cpp'] | libs
    t.libs = static_libs | t.libs

    t.cflags = t.cflags | cflags | ["-g", "-O0"]
    t.incdir << "-I./src/intrafont/"

    t.target = "Joyau"


    t.build_prx = true
  end
rescue LoadError
  puts "Please install psp_task: gem install psp_task"
end

Rake::RDocTask.new(:rdoc_site) do |rd|
  srcs = Dir['ruby/site_ruby/joyau/*.rb']

  rd.rdoc_files.include(*srcs)
  rd.rdoc_dir = "./doc/site/"
end

srcs = ["main.cpp",
        "Drawable.cpp",
        "Sprite.cpp",
        "Shape.cpp",
        "Line.cpp",
        "Audio.cpp",
        "Kernel.cpp",
        "DrawableText.cpp",
        "Manager.cpp",
        "GameMap.cpp",
        "MessageBox.cpp",
        "Timer.cpp",
        "Particles.cpp",
        "CircleMenu.cpp",
        "Scrolling.cpp",
        "Font.cpp",
        "Keys.cpp",
        "Circle.cpp",
        "Graphics.cpp",
        "MsgSelecter.cpp",
        "Usb.cpp",
        "MultiSelecter.cpp",
        "Buffer.cpp",
        "DrawableRect.cpp",
        "Triangle.cpp",
        "Wlan.cpp",
        "Debug.cpp",
        "OslSound.cpp"].map { |i| "src/#{i}" }

Rake::RDocTask.new do |rd|
  rd.rdoc_files.include(*srcs)
  rd.rdoc_dir = "./doc/embedded"
end

Rake::RDocTask.new(:ri) do |rd|
  rd.rdoc_files.include(*(Dir['ruby/site_ruby/joyau/*.rb'] | srcs))
  rd.options << "-r"
end


file "ruby-1.8.7-p248.tar.bz2" do
  ftp = Net::FTP.new
  ftp.connect("ftp.ruby-lang.org", 21)

  ftp.login("anonymous", "")

  puts "Downloading ruby-1.8.7-p248.tar.bz2..."
  ftp.getbinaryfile("pub/ruby/1.8/ruby-1.8.7-p248.tar.bz2",
                    "ruby-1.8.7-p248.tar.bz2")

  ftp.close
end

desc "Create a release directory"
task :release_dir => ['psp:eboot', 'dbg:prx', 'rdoc_site', 'rdoc',
                      'ruby-1.8.7-p248.tar.bz2'] do
  FileUtils.rm_r 'Joyau-release' if File.exist? 'Joyau-release'

  Dir.mkdir 'Joyau-release'

  FileUtils.cp_r 'doc', 'Joyau-release'
  FileUtils.cp_r 'samples', 'Joyau-release'
  FileUtils.cp_r 'ruby', 'Joyau-release'
  FileUtils.cp_r 'src', 'Joyau-release'

  FileUtils.cp 'EBOOT.PBP', 'Joyau-release'
  FileUtils.cp 'Joyau.prx', 'Joyau-release'
  FileUtils.cp 'Rakefile', 'Joyau-release'
  FileUtils.cp 'README', 'Joyau-release' if File.exist? 'README'

  system "tar -xvf ruby-1.8.7-p248.tar.bz2"
  FileUtils.cp_r "ruby-1.8.7-p248/lib", "Joyau-release/ruby/1.8/"
  FileUtils.rm_r "ruby-1.8.7-p248"

  FileUtils.cp_r 'src/bigdecimal/lib/bigdecimal', 'Joyau-release/ruby/1.8'

  FileUtils.mkdir 'Joyau-release/ruby/1.8/digest/' unless
    File.exist? 'Joyau-release/ruby/1.8/digest/'
  FileUtils.cp 'src/digest/lib/digest.rb', 'Joyau-release/ruby/1.8/'
  FileUtils.cp 'src/digest/lib/sha1.rb', 'Joyau-release/ruby/1.8/digest/'
  FileUtils.cp 'src/digest/lib/md5.rb', 'Joyau-release/ruby/1.8/digest/'
  FileUtils.cp 'src/digest/sha2/lib/sha2.rb', 'Joyau-release/ruby/1.8/digest/'

  FileUtils.cp 'src/nkf/lib/kconv.rb', 'Joyau-release/ruby/1.8/'

  ["socket.rb", "bigdecimal.rb", "digest/bubblebabble.rb",
   "fcntl.rb", "stringio.rb", "strscan.rb", "thread.rb",
   "zlib.rb", "nkf.rb", "etc.rb", "syck.rb", "enumerator.rb"].each do |filename|
    ext_name = case filename
               when /(\w+)\.rb/
                 $1
               when /\w+\/(\w+)\.rb/
                 $1
               else
                 filename
               end

    File.open("Joyau-release/ruby/1.8/#{filename}", 'w') do |file|
      file.puts "Joyau.init_internal(\"#{ext_name}\")"
    end
  end
end

Rake::PackageTask.new("Joyau", JOYAU_VERSION) do |pkg|
  pkg.need_tar_bz2 = true
  pkg.package_files << "Joyau-release"
end

# May seem weird, but I didn't see any other way to copy
# the whole directory.

file "pkg/Joyau-#{JOYAU_VERSION}" do
  FileUtils.rm_r "pkg/Joyau-#{JOYAU_VERSION}" if
    File.exist? "pkg/Joyau-#{JOYAU_VERSION}"
  FileUtils.cp_r "Joyau-release", "pkg/Joyau-#{JOYAU_VERSION}"
end

desc "Create the release tarball"
task :release => [:release_dir, :repackage]

desc "Create an EBOOT"
task :default => "psp:eboot"

desc "Create a PRX"
task :debug => "dbg:prx"

desc "Clean the directory"
task :clean => ['psp:clean', 'dbg:clean', 'clobber_rdoc', 'clobber_rdoc_site',
                'clobber_package'] do
  FileUtils.rm_r 'Joyau-release' if File.exist? 'Joyau-release'
end

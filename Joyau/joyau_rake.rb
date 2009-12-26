require 'rake'
require 'rake/tasklib'
require 'pathname'

class JoyauTask < Rake::TaskLib
  class Conf
    attr_accessor :libs, :cflags, :cxxflags, :srcs, :libs, :title, :target,
                  :icon, :pic1, :incdir, :libdir, :ldflags
  
    def initialize
      pspsdk = `psp-config --pspsdk-path`
      pspsdk.chomp!
      
      @libdir = ["-L.", "-L#{pspsdk}/lib"]
      @incdir = ["-I.", "-I#{pspsdk}/include"]

      @libs = %w(-lpspdebug -lpspdisplay -lpspge -lpspctrl -lpspsdk 
                 -lc -lpspnet -lpspnet_inet -lpspnet_apctl -lpspnet_resolver 
                 -lpsputility -lpspuser -lpspkernel)
      
      @cflags = %w(-D_PSP_FW_VERSION=150)
      @cxxflags = %w(-fno-rtti)
      
      @ldflags = []

      @srcs = []
      
      @title = ""
      @target = ""
      @icon = ""
      @pic1 = ""
    end
  end

  def initialize
    @conf = Conf.new
    yield @conf
    define
  end

  def compile(src)
    obj = Pathname.new(src).sub_ext(".o")
    flags = Pathname.new(src).extname == ".cpp" ? 
    @conf.cflags | @conf.cxxflags :
      @conf.cflags
    
    puts "Compiling #{src}..."
    system "psp-gcc #{@conf.incdir * ' '} #{flags * ' '} \
                  -c -o #{obj} #{src}"
    raise "Could not compile #{src} !" unless File.exist? obj
  end

  def build_elf(prx = false)
    objs = @conf.srcs.map { |i| Pathname.new(i).sub_ext(".o").to_s }
    
    if prx
      pspsdk = `psp-config --pspsdk-path`.chomp
      prx_flags = "-specs=#{pspsdk}/lib/prxspecs -Wl,-q,"
      prx_flags += "-T#{pspsdk}/lib/linkfile.prx"
    else
      prx_flags = ""
    end

    puts "Creating #{@conf.target}.elf..."
    system "psp-gcc #{@conf.incdir * ' '} #{prx_flags} #{@conf.cflags * ' '} \
                #{@conf.libdir * ' '} #{@conf.ldflags * ' '} #{objs * ' '} \
                #{@conf.libs * ' '} -o #{@conf.target}.elf"
    raise "Could not create #{@conf.target}.elf !" unless 
      File.exist? "#{@conf.target}.elf"
    system "psp-fixup-imports #{@conf.target}.elf"
  end

  def define
    namespace 'psp' do
      desc "Build objects"
      task :objs => @conf.srcs do
        @conf.cflags << "-O2"

        srcs = @conf.srcs.reject { |i|
          File.exist? Pathname.new(i).sub_ext(".o").to_s 
        }

        srcs.each do |src|
          compile(src)
        end
      end

      desc "Build objects without optimisation, and with a debugging flag"
      task :objs_prx => @conf.srcs do
        @conf.cflags << "-g" << "-O0"

        # All the files are built again.
        @conf.srcs.each do |src|
          compile(src)
        end
      end

      desc "Create an ELF"
      task "#{@conf.target}.elf" => :objs do
        build_elf
      end

      desc "Create a strip ELF"
      task "#{@conf.target}_strip.elf" => "#{@conf.target}.elf" do
        puts "Creating #{@conf.target}_strip.elf..."
        system "psp-strip #{@conf.target}.elf -o #{@conf.target}_strip.elf"
      end
      
      desc "Create a PARAM.SFO file."
      task "PARAM.SFO" do
        puts "Creating PARAM.SFO..."
        system "mksfoex -d MEMSIZE=1 '#{@conf.target}}' PARAM.SFO"
      end

      desc "Alias for ELF's creations."
      task :elf => "#{@conf.target}.elf"

      desc "Alias for strip ELF's creation."
      task :strip_elf => "#{@conf.target}_strip.elf"

      desc "Alias for PARAM.SFO's creation"
      task :sfo => "PARAM.SFO"

      desc "Create an EBOOT from an ELF"
      task :eboot => ["#{@conf.target}_strip.elf", "PARAM.SFO"] do
        puts "Creating EBOOT..."
        system "pack-pbp EBOOT.PBP PARAM.SFO #{@conf.icon} \
                         NULL NULL #{@conf.pic1} NULL \
                         #{@conf.target}_strip.elf NULL"
        
      end

      desc "Create a PRX from objects"
      task :prx => :objs_prx do
        build_elf(true)

        puts "Creating #{@conf.target}.prx..."
        system "psp-prxgen #{@conf.target}.elf #{@conf.target}.prx"
        raise "Could not create #{@conf.target}.prx !" unless
              File.exist? "#{@conf.target}.prx"
      end

      desc "Remove the files created by this script."
      task :clean do
        File.delete "EBOOT.PBP" if File.exist? "EBOOT.PBP"
        File.delete "PARAM.SFO" if File.exist? "PARAM.SFO"
         
        File.delete "#{@conf.target}.elf" if File.exist? "#{@conf.target}.elf"
        File.delete "#{@conf.target}_strip.elf" if 
          File.exist? "#{@conf.target}_strip.elf"

        File.delete "#{@conf.target}.prx" if File.exist? "#{@conf.target}.prx"

        @conf.srcs.each do |src|
          obj = Pathname.new(src).sub_ext(".o").to_s
          File.delete obj if File.exist? obj
        end

        puts "Directory cleaned"
      end
    end
  end
end
